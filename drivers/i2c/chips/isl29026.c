/******************************************************************
*File Name: 	ISL.c 	                                           *
*Description:	Linux device driver for ISL ambient light and    *
*			proximity sensors.                                          *                                
*******************************************************************
Geschichte:	                                                                        
Wenn               Wer          Was                                                                        Tag

******************************************************************/
// includes
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <asm/delay.h>
#include <linux/irq.h>
#include <asm/gpio.h>
#include <linux/i2c/isl29026.h>
#include <linux/input.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>

#define ISL_INT_GPIO 42
#define ISL_TAG        "[ISL]"

// device name/id/address/counts
#define ISL_DEVICE_NAME		"ISL"
#define ISL_DEVICE_ID                "isl29026"
#define ISL_ID_NAME_SIZE		10

#define ISL_MAX_NUM_DEVICES		3
#define ISL_MAX_DEVICE_REGS		10


//#define RD_CHIPID		0x0
#define ISL_CFG_REG		0x1
#define ISL_PROX_EN_Bit    0x80  
#define ISL_PROX_SLP_Bit   0x70 //011 means sleep 100ms
#define ISL_PROX_DR_Bit    0x08 // 0 is 110mA, 1 is 220mA
#define ISL_ALS_EN_Bit     0x04  
#define ISL_ALS_RANGE_Bit  0x02 //0 is low range, 1 is high range
#define ISL_ALSIR_MODE_Bit 0x01 //0 is ALS mode, 1 is IR spectrum mode
#define ISL_SENSOR_All_Off 0x00
#define ISL_ALS_On         (ISL_ALS_EN_Bit | ISL_ALS_RANGE_Bit)
#define ISL_ALS_mask       (ISL_ALS_EN_Bit | ISL_ALS_RANGE_Bit | ISL_ALSIR_MODE_Bit)
#define ISL_PROX_On        (0xB0)
#define ISL_PROX_mask      (ISL_PROX_EN_Bit | ISL_PROX_SLP_Bit | ISL_PROX_DR_Bit)

#define ISL_INT_REG		0x2
#define ISL_INT_PROX_Bit      0x80 //read value is 1 means interrupt event occurred, 0 means no interrupt or cleared
#define ISL_INT_PROX_PRST_Bit 0x60 //00 is 1 c, 01 is 4 c, 10 is 8 c, 11 is 16 c
#define ISL_INT_ALS_Bit       0x08 //read value is 1 means interrupt event occurred, 0 means no interrupt or cleared
#define ISL_INT_ALS_PRST_Bit  0x06 //00 is 1 c, 01 is 4 c, 10 is 8 c, 11 is 16 c
#define ISL_INT_CTRL_Bit      0x01 //0 is PROX or ALS, 1 is PROX and ALS
#define ISL_INT_All_Off       0x00
#define ISL_ALS_INT_On         (0x04) // 8times over
#define ISL_ALS_INT_mask       (0x0E) 
#define ISL_PROX_INT_On        (0x20)  // 4times over
#define ISL_PROX_INT_mask      (0xE0)
#define ISL_PROX_TH_L	0x3
#define ISL_PROX_TH_H	0x4
#define ISL_ALSIR_TH_L	0x5
#define ISL_ALSIR_TH_HL	0x6
#define ISL_ALSIR_TH_H	0x7

#define ISL_PROX_DATA	0x8
#define ISL_ALSIR_DATA_L	0x9
#define ISL_ALSIR_DATA_H	0xa

// lux constants
#define	ISL_MAX_LUX			65535000
#define ISL_FILTER_DEPTH		3
#define THRES_LO_TO_HI_RATIO  4/5

// forward declarations
static int ISL_probe(struct i2c_client *clientp, const struct i2c_device_id *idp);
static int ISL_remove(struct i2c_client *client);
static int ISL_open(struct inode *inode, struct file *file);
static int ISL_release(struct inode *inode, struct file *file);
static int ISL_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
static int ISL_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int ISL_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static loff_t ISL_llseek(struct file *file, loff_t offset, int orig);
static int ISL_get_lux(void);
static int ISL_lux_filter(int raw_lux);
static int ISL_device_name(unsigned char *bufp, char **device_name);
static int ISL_prox_poll(struct ISL_prox_info *prxp);

static void do_ISL_work(struct work_struct *w);
static void ISL_report_value(int mask);
static int calc_distance(int value);
	
static int light_on=0;  
static int prox_on = 0;

struct alsprox_data {
	struct input_dev *input_dev;
};

static struct alsprox_data *alsprox;

// first device number
static dev_t ISL_dev_number;

// class structure for this device
struct class *ISL_class;

// module device table
static struct i2c_device_id ISL_idtable[] = {
        {ISL_DEVICE_ID, 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, ISL_idtable);

// driver definition
static struct i2c_driver ISL_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "isl29026",
	 },
	.id_table = ISL_idtable,
	.probe = ISL_probe,
	.remove = __devexit_p(ISL_remove),
};


struct ISL_intr_data {
    int int_gpio;
    int irq;
};

// per-device data
struct ISL_data {
	struct i2c_client *client;
	struct cdev cdev;
	unsigned int addr;
	char ISL_id;
	char ISL_name[ISL_ID_NAME_SIZE];
	struct semaphore update_lock;
	char valid;
    	struct ISL_intr_data *pdata;
	struct work_struct  ISL_work;
} *ISL_datap;

static struct ISL_intr_data ISL_irq= {
    .int_gpio = ISL_INT_GPIO,
    .irq = MSM_GPIO_TO_INT(ISL_INT_GPIO),
};


// file operations
static struct file_operations ISL_fops = {
	.owner = THIS_MODULE,
	.open = ISL_open,
	.release = ISL_release,
	.read = ISL_read,
	.write = ISL_write,
	.llseek = ISL_llseek,
	.ioctl = ISL_ioctl,
};

// device configuration
struct ISL_cfg *ISL_cfgp;
static u32 calibrate_target_param = 300000;
static u16 als_time_param = 100;
static u16 scale_factor_param = 1;
static u16 gain_trim_param = 512;
static u8 filter_history_param = 3;
static u8 filter_count_param = 1;
static u8 gain_param = 1;
#if defined(CONFIG_MACH_BLADE)
static u16 prox_threshold_hi_param = 100; 
static u16 prox_threshold_lo_param = 50;
#else
static u16 prox_threshold_hi_param = 255; 
static u16 prox_threshold_lo_param = 204;
#endif

#if defined(CONFIG_MACH_MOONCAKE)
static u8 prox_pulse_cnt_param = 0x04;
#elif defined(CONFIG_MACH_RACER2)
static u8 prox_pulse_cnt_param = 0x20;
#else
static u8 prox_pulse_cnt_param = 0x08;
#endif

static u8 prox_gain_param = 0x20;

// device reg init values
u8 ISL_triton_reg_init[16] = {0x00,0xFF,0XFF,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00};

//
static u16 als_intr_threshold_hi_param = 0;
static u16 als_intr_threshold_lo_param = 0;
int ISL_g_nlux = 0;

		

// prox info
struct ISL_prox_info ISL_prox_cal_info[20];
struct ISL_prox_info ISL_prox_cur_info;
struct ISL_prox_info *ISL_prox_cur_infop = &ISL_prox_cur_info;


static int device_released = 0;
static u16 sat_als = 0;
static u16 sat_prox = 0;

// lux time scale
struct time_scale_factor  {
	u16	numerator;
	u16	denominator;
	u16	saturation;
};
struct time_scale_factor ISL_TritonTime = {1, 0, 0};
struct time_scale_factor *ISL_lux_timep = &ISL_TritonTime;

// gain table
u8 ISL_triton_gain_table[] = {1, 8, 16, 120};

// lux data
struct lux_data {
	u16	ratio;
	u16	clear;
	u16	ir;
};
struct lux_data ISL_TritonFN_lux_data[] = {
        { 9830,  8320,  15360 },
        { 12452, 10554, 22797 },
        { 14746, 6234,  11430 },
        { 17695, 3968,  6400  },
        { 0,     0,     0     }
};
struct lux_data *ISL_lux_tablep = ISL_TritonFN_lux_data;
static int lux_history[ISL_FILTER_DEPTH] = {-ENODATA, -ENODATA, -ENODATA};

//prox data
struct prox_data {
	u16	ratio;
	u16	hi;
	u16	lo;
};
struct prox_data ISL_prox_data[] = {
        { 1,  22,  20 },
        { 3, 20, 16 },
        { 6, 18, 14 },
        { 10, 16,  16 },      
        { 0,  0,   0 }
};
struct prox_data *prox_tablep = ISL_prox_data;


/* ----------------------*
* config gpio for intr utility        *
*-----------------------*/
int ISL_config_int_gpio(int int_gpio)
{
    int rc=0;
    uint32_t  gpio_config_data = GPIO_CFG(int_gpio,  0, GPIO_INPUT, GPIO_PULL_UP, GPIO_2MA);

    rc = gpio_request(int_gpio, "gpio_sensor");
    if (rc) {
        printk(ISL_TAG "%s: gpio_request(%#x)=%d\n",
                __func__, int_gpio, rc);
        return rc;
    }

    rc = gpio_tlmm_config(gpio_config_data, GPIO_ENABLE);
    if (rc) {
        printk(ISL_TAG "%s: gpio_tlmm_config(%#x)=%d\n",
                __func__, gpio_config_data, rc);
        return rc;
    }

    mdelay(1);

    rc = gpio_direction_input(int_gpio);
    if (rc) {
        printk(ISL_TAG "%s: gpio_direction_input(%#x)=%d\n",
                __func__, int_gpio, rc);
        return rc;
    }

    return 0;
}

/* ----------------------*
* ISL interrupt function         *
*-----------------------*/
static irqreturn_t ISL_interrupt(int irq, void *data)
{
    //printk(ISL_TAG "ISL_interrupt\n");	
    disable_irq_nosync(ISL_datap->pdata->irq);
    schedule_work(&ISL_datap->ISL_work);
    
    return IRQ_HANDLED;
}

static void do_ISL_work(struct work_struct *w)
{

    int ret =0;	
    //int prx_hi, prx_lo;
    u16 status = 0;	
    //u8  buf_8 = 0;
    status = i2c_smbus_read_byte_data(ISL_datap->client,ISL_INT_REG);//add error ctl
    if(status<0)
	goto read_reg_fail;

	//printk(KERN_ERR "isl do_ISL_work status =0x%x \n",status);
    //als interrupt
    if(((status & ISL_INT_ALS_Bit) != 0)&&((status & ISL_INT_PROX_Bit)==0)&&(prox_on==0))
    {
	 status = status & (~ISL_INT_ALS_Bit);
	 ISL_g_nlux = ISL_get_lux();
	//clear intr  for als intr case, as prox intr  already be cleared in prox_poll	
     if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, status))) < 0) 
	 {
   		printk(KERN_CRIT "ISL: i2c_smbus_write_byte failed in clear als interrupt\n");
	 }
     ISL_report_value(0);
     goto read_reg_fail;
    }
	
    //prox interrupt
	
	if(((status & ISL_INT_PROX_Bit)!=0)||ISL_prox_cur_infop->prox_event)
    { 
    //status = status & (~ISL_INT_PROX_Bit);
	if((ret = ISL_prox_poll(ISL_prox_cur_infop))<0)
		printk(KERN_CRIT "ISL: get prox poll  failed in  ISL interrupt()\n");  	
	
	if(ISL_prox_cur_infop->prox_data > ISL_cfgp->prox_threshold_hi)       
	 {           
	 	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_H, sat_prox))) < 0) {        	        
			pr_crit(ISL_TAG "i2c_smbus_write_byte() to ISL_TRITON_PRX_MAXTHRESHLO\n");                	
			}           
	 	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_L, ISL_cfgp->prox_threshold_lo))) < 0) {        	        
			pr_crit(ISL_TAG "i2c_smbus_write_byte() to ISL_TRITON_PRX_MAXTHRESHLO\n");                	
			}           
		ISL_prox_cur_infop->prox_event = 1;    
		//pr_crit(ISL_TAG "screen off:ISL_prox_cur_infop->prox_data=%d\n",ISL_prox_cur_infop->prox_data);            
	}        
	else if(ISL_prox_cur_infop->prox_data < ISL_cfgp->prox_threshold_lo)        
	{            
		if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_H,ISL_cfgp->prox_threshold_hi))) < 0) {    	       
			pr_crit(ISL_TAG "i2c_smbus_write_byte() to ISL_TRITON_PRX_MAXTHRESHLO\n");            	
			}             
		if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_L,0))) < 0) {    	    
			pr_crit(ISL_TAG "i2c_smbus_write_byte() to ISL_TRITON_PRX_MINTHRESHLO\n");            
			}             
		ISL_prox_cur_infop->prox_event = 0;             
		//pr_crit(ISL_TAG "screen on:ISL_prox_cur_infop->prox_data=%d\n",ISL_prox_cur_infop->prox_data);                     
	}   
   /*  if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, status))) < 0) 
	 {
   		printk(KERN_CRIT "ISL: i2c_smbus_write_byte failed in clear als interrupt\n");
	 }*/
	ISL_report_value(1);
    }
	
read_reg_fail:	
    enable_irq(ISL_datap->pdata->irq);
    return;
}
	
		
/*calc_distance, using prox value*/
static int calc_distance(int value)
{
	int temp=0;
	if(ISL_prox_cur_infop->prox_event == 1)		
		temp=4-((value-(ISL_cfgp->prox_threshold_lo))/((sat_prox-(ISL_cfgp->prox_threshold_lo))/4));
	else if(ISL_prox_cur_infop->prox_event == 0)
		temp=5;
        return temp;
}

/*report als and prox data to input sub system, 
for data poll utility in hardware\alsprox.c
*/
static void ISL_report_value(int mask)
{
	struct ISL_prox_info *val = ISL_prox_cur_infop;
	int lux_val=ISL_g_nlux;
	int  dist;
	lux_val=(lux_val<=1)? lux_val:lux_val*10;	

	if (mask==0) {
		input_report_abs(alsprox->input_dev, ABS_X, lux_val);
		printk(KERN_CRIT "ISL: als_interrupt =%d\n",  lux_val);
	}

	if (mask==1) {
		dist=calc_distance(val->prox_data);		
		input_report_abs(alsprox->input_dev, ABS_Y, dist);		
		printk(KERN_CRIT "ISL: prox_interrupt =%d, distance=%d\n",  val->prox_data,dist);
	}

	input_sync(alsprox->input_dev);		
}



/* ------------*
* driver init        *
*------------*/
static int __init ISL_init(void) {
	int ret = 0;
	printk(KERN_ERR "ISL: comes into ISL_init\n");
	if ((ret = (alloc_chrdev_region(&ISL_dev_number, 0, ISL_MAX_NUM_DEVICES, ISL_DEVICE_NAME))) < 0) {
		printk(KERN_ERR "ISL: alloc_chrdev_region() failed in ISL_init()\n");
                return (ret);
	}
        ISL_class = class_create(THIS_MODULE, ISL_DEVICE_NAME);
        ISL_datap = kmalloc(sizeof(struct ISL_data), GFP_KERNEL);
        if (!ISL_datap) {
		printk(KERN_ERR "ISL: kmalloc for struct ISL_data failed in ISL_init()\n");
                return -ENOMEM;
	}
        memset(ISL_datap, 0, sizeof(struct ISL_data));
        cdev_init(&ISL_datap->cdev, &ISL_fops);
        ISL_datap->cdev.owner = THIS_MODULE;
        if ((ret = (cdev_add(&ISL_datap->cdev, ISL_dev_number, 1))) < 0) {
		printk(KERN_ERR "ISL: cdev_add() failed in ISL_init()\n");
                return (ret);
	}
	//printk(KERN_ERR "ISL: comes before i2c_add_driver\n");
        ret = i2c_add_driver(&ISL_driver);
	if(ret){
		printk(KERN_ERR "ISL: i2c_add_driver() failed in ISL_init(),%d\n",ret);
                return (ret);
	}
    	
        return (ret);
}



// driver exit
static void __exit ISL_exit(void) {
       i2c_del_driver(&ISL_driver);
       unregister_chrdev_region(ISL_dev_number, ISL_MAX_NUM_DEVICES);
	device_destroy(ISL_class, MKDEV(MAJOR(ISL_dev_number), 0));
	cdev_del(&ISL_datap->cdev);
	class_destroy(ISL_class);
       kfree(ISL_datap);
}



// client probe
static int ISL_probe(struct i2c_client *clientp, const struct i2c_device_id *idp) {
    int ret = 0;
    int i = 0;
    unsigned char buf[ISL_MAX_DEVICE_REGS];
    char *device_name;
		//printk(KERN_ERR "isl enter ISL_probe\n");
    if (!i2c_check_functionality(clientp->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		printk(KERN_ERR "ISL: ISL_probe() - i2c smbus byte data functions unsupported\n");
		return -EOPNOTSUPP;
		}
    if (!i2c_check_functionality(clientp->adapter, I2C_FUNC_SMBUS_WORD_DATA)) {
		printk(KERN_ERR "ISL: ISL_probe() - i2c smbus word data functions unsupported\n");
        }

    ISL_datap->client = clientp;
    i2c_set_clientdata(clientp, ISL_datap);
	
    //write bytes to address control registers
    for(i = 1; i < ISL_MAX_DEVICE_REGS; i++) {
		buf[i] = i2c_smbus_read_byte_data(ISL_datap->client, i);
		if(buf[i] < 0)
		{
			printk(KERN_ERR "ISL: i2c_smbus_read_byte_data(%d) failed in ISL_probe()\n",i);
		}
		else
		{
			//printk(KERN_ERR "ISL: i2c_smbus_read_byte_data(%d) = %d in ISL_probe()\n", i, buf[i]);
		}
	}	
	//check device ID "tritonFN"
	if ((ret = ISL_device_name(buf, &device_name)) == 0) {
		printk(KERN_ERR "ISL: chip id that was read found mismatched by ISL_device_name(), in ISL_probe()\n");
 		return -ENODEV;
        }
	if (strcmp(device_name, ISL_DEVICE_ID)) {
		printk(KERN_ERR "ISL: chip id that was read does not match expected id in ISL_probe()\n");
		return -ENODEV;
        }
	else{
		printk(KERN_ERR "ISL: chip id of %s that was read matches expected id in ISL_probe()\n", device_name);		
	}
	device_create(ISL_class, NULL, MKDEV(MAJOR(ISL_dev_number), 0), &ISL_driver ,"ISL");

	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client,ISL_ALSIR_TH_L,0) < 0))) {
		printk(KERN_ERR "ISL: i2c_smbus_write_byte() to control reg failed in ISL_probe()\n");
		return(ret);
        }
	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client,ISL_ALSIR_TH_HL,0) < 0))) {
		printk(KERN_ERR "ISL: i2c_smbus_write_byte() to control reg failed in ISL_probe()\n");
		return(ret);
        }
	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client,ISL_ALSIR_TH_H,0) < 0))) {
		printk(KERN_ERR "ISL: i2c_smbus_write_byte() to control reg failed in ISL_probe()\n");
		return(ret);
        }

	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client,ISL_CFG_REG,ISL_SENSOR_All_Off) < 0))) {
		printk(KERN_ERR "ISL: i2c_smbus_write_byte() to control reg failed in ISL_probe()\n");
		return(ret);
        }

	if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client,ISL_INT_REG,ISL_INT_All_Off) < 0))) {
		printk(KERN_ERR "ISL: i2c_smbus_write_byte() to control reg failed in ISL_probe()\n");
		return(ret);
        }
		
	//printk(KERN_ERR "isl enter ISL_probe before INIT_WORK\n");
	INIT_WORK(&ISL_datap->ISL_work, do_ISL_work); 
	//init_MUTEX(&ISL_datap->proximity_calibrating);
	ISL_datap->pdata = &ISL_irq;
    	ret=ISL_config_int_gpio(ISL_datap->pdata->int_gpio);
    	if (ret) {
		printk(KERN_CRIT "ISL configure int_gpio%d failed\n",
                ISL_datap->pdata->int_gpio);
        	return ret;
    	}

    	ret = request_irq(ISL_datap->pdata->irq, ISL_interrupt, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, 
			ISL_datap->ISL_name, ISL_prox_cur_infop);
    	if (ret) {
		printk(KERN_CRIT "ISL request interrupt failed\n");
        	return ret;
    	}
	//printk(KERN_ERR "isl enter ISL_probe before alsprox\n");
	strlcpy(clientp->name, ISL_DEVICE_ID, I2C_NAME_SIZE);
	strlcpy(ISL_datap->ISL_name, ISL_DEVICE_ID, ISL_ID_NAME_SIZE);
	ISL_datap->valid = 0;
	
	init_MUTEX(&ISL_datap->update_lock);
	if (!(ISL_cfgp = kmalloc(sizeof(struct ISL_cfg), GFP_KERNEL))) {
		printk(KERN_ERR "ISL: kmalloc for struct ISL_cfg failed in ISL_probe()\n");
		return -ENOMEM;
	}
	ISL_cfgp->calibrate_target = calibrate_target_param;
	ISL_cfgp->als_time = als_time_param;
	ISL_cfgp->scale_factor = scale_factor_param;
	ISL_cfgp->gain_trim = gain_trim_param;
	ISL_cfgp->filter_history = filter_history_param;
	ISL_cfgp->filter_count = filter_count_param;
	ISL_cfgp->gain = gain_param;
	ISL_cfgp->prox_threshold_hi = prox_threshold_hi_param;
	ISL_cfgp->prox_threshold_lo = prox_threshold_lo_param;
	//ISL_cfgp->prox_int_time = prox_int_time_param;

	sat_als = 0xFFF;
	sat_prox = 0xFF;

	alsprox = kzalloc(sizeof(struct alsprox_data), GFP_KERNEL);
	if (!alsprox) {
		ret = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	alsprox->input_dev = input_allocate_device();
	if (!alsprox->input_dev) {
		ret = -ENOMEM;
		printk(KERN_ERR "ISL_probe: Failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}

	set_bit(EV_ABS, alsprox->input_dev->evbit);
	
	/* lux */
	input_set_abs_params(alsprox->input_dev,  ABS_X, 0, 65535, 0, 0);
	/* prox */
	input_set_abs_params(alsprox->input_dev, ABS_Y, 0, 65535, 0, 0);

	alsprox->input_dev->name = "alsprox";
	ret = input_register_device(alsprox->input_dev);
	if (ret) {
		printk("ISL_probe: Unable to register input device: %s\n", alsprox->input_dev->name);
		goto exit_input_register_device_failed;
	}
	
	return (ret);
//exit_ISL_device_register_failed:	
exit_input_register_device_failed:
	input_free_device(alsprox->input_dev);
	
exit_input_dev_alloc_failed:
exit_alloc_data_failed:
	kfree(alsprox);
	return (ret);		
}


// client remove
static int __devexit ISL_remove(struct i2c_client *client) {
	int ret = 0;

	return (ret);
}

// open
static int ISL_open(struct inode *inode, struct file *file) {
	struct ISL_data *ISL_datap;
	int ret = 0;

	device_released = 0;
  //printk(KERN_CRIT "[ISL]:%s\n", __func__);
	ISL_datap = container_of(inode->i_cdev, struct ISL_data, cdev);
	if (strcmp(ISL_datap->ISL_name, ISL_DEVICE_ID) != 0) {
		printk(KERN_ERR "ISL: device name incorrect during ISL_open(), get %s\n", ISL_datap->ISL_name);
		ret = -ENODEV;
	}
	return (ret);
}

// release
static int ISL_release(struct inode *inode, struct file *file) {
	struct ISL_data *ISL_datap;
	int ret = 0;

	device_released = 1;
	prox_on = 0;
	//printk(KERN_CRIT "[ISL]:%s\n", __func__);

	//prox_history_hi = 0;
	//prox_history_hi = 0;
	ISL_datap = container_of(inode->i_cdev, struct ISL_data, cdev);
	if (strcmp(ISL_datap->ISL_name, ISL_DEVICE_ID) != 0) {
		printk(KERN_ERR "ISL: device name incorrect during ISL_release(), get %s\n", ISL_datap->ISL_name);
		ret = -ENODEV;
	}
	return (ret);
}

// read
static int ISL_read(struct file *file, char *buf, size_t count, loff_t *ppos) {
	struct ISL_data *ISL_datap;
	u8 i = 0, xfrd = 0, reg = 0;
	u8 my_buf[ISL_MAX_DEVICE_REGS];
	int ret = 0;

	//if (prox_on)
		//del_timer(&prox_poll_timer);
        if ((*ppos < 0) || (*ppos >= ISL_MAX_DEVICE_REGS)  || (count > ISL_MAX_DEVICE_REGS)) {
		printk(KERN_ERR "ISL: reg limit check failed in ISL_read()\n");
		return -EINVAL;
	}
	reg = (u8)*ppos;
	ISL_datap = container_of(file->f_dentry->d_inode->i_cdev, struct ISL_data, cdev);
	while (xfrd < count) {
      		my_buf[i++] = i2c_smbus_read_byte_data(ISL_datap->client,reg);
			reg++;
			xfrd++;
        }
        if ((ret = copy_to_user(buf, my_buf, xfrd))){
		printk(KERN_ERR "ISL: copy_to_user failed in ISL_read()\n");
		return -ENODATA;
	}
	//if (prox_on)
		//ISL_prox_poll_timer_start();
        return ((int)xfrd);
}

// write
static int ISL_write(struct file *file, const char *buf, size_t count, loff_t *ppos) {
	struct ISL_data *ISL_datap;
	u8 i = 0, xfrd = 0, reg = 0;
	u8 my_buf[ISL_MAX_DEVICE_REGS];
	int ret = 0;

	//if (prox_on)
		//del_timer(&prox_poll_timer);
        if ((*ppos < 0) || (*ppos >= ISL_MAX_DEVICE_REGS) || ((*ppos + count) > ISL_MAX_DEVICE_REGS)) {
		printk(KERN_ERR "ISL: reg limit check failed in ISL_write()\n");
		return -EINVAL;
	}
	reg = (u8)*ppos;
        if ((ret =  copy_from_user(my_buf, buf, count))) {
		printk(KERN_ERR "ISL: copy_to_user failed in ISL_write()\n");
 		return -ENODATA;
	}
        ISL_datap = container_of(file->f_dentry->d_inode->i_cdev, struct ISL_data, cdev);
        while (xfrd < count) {
                if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, reg, my_buf[i++]))) < 0) {
                        printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ISL_write()\n");
                        return (ret);
                }
                reg++;
                xfrd++;
        }
        return ((int)xfrd);
}

// llseek
static loff_t ISL_llseek(struct file *file, loff_t offset, int orig) {
	int ret = 0;
	loff_t new_pos = 0;

	if ((offset >= ISL_MAX_DEVICE_REGS) || (orig < 0) || (orig > 1)) {
                printk(KERN_ERR "ISL: offset param limit or origin limit check failed in ISL_llseek()\n");
                return -EINVAL;
        }
        switch (orig) {
        	case 0:
                	new_pos = offset;
                	break;
        	case 1:
                	new_pos = file->f_pos + offset;
	                break;
		default:
			return -EINVAL;
			break;
	}
	if ((new_pos < 0) || (new_pos >= ISL_MAX_DEVICE_REGS) || (ret < 0)) {
		printk(KERN_ERR "ISL: new offset limit or origin limit check failed in ISL_llseek()\n");
		return -EINVAL;
	}
	file->f_pos = new_pos;
	return new_pos;
}

/*enable_light_and_proximity, mask values' indication*/
/*10 : light on*/
/*01 : prox on*/
/*20 : light off*/
/*02 : prox off*/
static int enable_light_and_proximity(int mask)
{
	u8 ret = 0,reg_val = 0; //itime = 0;

	if(mask==0x10) 
	{
			pr_crit(ISL_TAG "light on while prox off\n");
			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_INT_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                		return (reg_val);
			}
			reg_val = (reg_val & (~ISL_ALS_INT_mask)) | ISL_ALS_INT_On;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}

			reg_val = als_intr_threshold_lo_param & 0x00FF;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_L, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			reg_val = ((als_intr_threshold_lo_param >> 8) & 0x000F) | ((als_intr_threshold_hi_param << 4) & 0x00F0) ;			
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_HL, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			reg_val = (als_intr_threshold_hi_param >> 4) & 0x00FF;			
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_H, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_CFG_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                		return (reg_val);
			}
			reg_val = (reg_val & (~ISL_ALS_mask)) | ISL_ALS_On;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_CFG_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
	}	
	if(mask==0x01)
	{
		if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_INT_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                		return (reg_val);
			}
			reg_val = (reg_val & (~(ISL_PROX_INT_mask |ISL_ALS_INT_mask))) | ISL_PROX_INT_On;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
               		 return (ret);
			}
			reg_val = 0x00;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_L, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			reg_val = 0xF0;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_HL, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			reg_val = 0xFF;			
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_H, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
		
			reg_val = ISL_cfgp->prox_threshold_lo & 0x00FF;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_L, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
               	 	return (ret);
			}
			reg_val = ISL_cfgp->prox_threshold_hi & 0x00FF;			
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_H, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_CFG_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                		return (reg_val);
			}
			reg_val = (reg_val & (~ISL_PROX_mask)) | ISL_PROX_On;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_CFG_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}

	}        
	if(mask==0x20)
	{
			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_INT_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                return (reg_val);
			}
			reg_val = (reg_val & (~ISL_ALS_INT_mask));
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}

			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_CFG_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                return (reg_val);
			}
			reg_val = (reg_val & (~ISL_ALS_mask)) ;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_CFG_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}
			           
	}
	if(mask==0x02)
	{
			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_CFG_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                return (reg_val);
			}
			reg_val = (reg_val & (~ISL_PROX_mask));
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_CFG_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}

			if ((reg_val = (i2c_smbus_read_byte_data(ISL_datap->client,ISL_INT_REG))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_read_byte_data failed in ioctl prox_on\n");
                return (reg_val);
			}
			reg_val = (reg_val & (~ISL_PROX_INT_mask));
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_INT_REG, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}
	}
	return ret;
}


// ioctls
static int ISL_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg) {
	struct ISL_data *ISL_datap;
	int prox_sum = 0, prox_mean = 0, prox_max = 0;
	int lux_val = 0, ret = 0, i = 0;
	u8 reg_val = 0;
	u16 ratio;
	struct prox_data *prox_pt;
	
	ISL_datap = container_of(inode->i_cdev, struct ISL_data, cdev);
	switch (cmd) {
		case ISL_IOCTL_ALS_ON:			
			if(prox_on)
				break;
			ret=enable_light_and_proximity(0x10);
			if(ret>=0)
			{
				light_on=1;	
            			pr_crit(ISL_TAG "ISL_IOCTL_ALS_ON,lux=%d\n", ISL_g_nlux);
			}			
			return (ret);
			break;
            case ISL_IOCTL_ALS_OFF:	
			if(prox_on)
				break;
                        for (i = 0; i < ISL_FILTER_DEPTH; i++)
                                lux_history[i] = -ENODATA;
			ret=enable_light_and_proximity(0x20);
			if(ret>=0)
			{
				light_on=0;
            			pr_crit(ISL_TAG"ISL_IOCTL_ALS_OFF\n");
			}			
			return (ret);
                	break;
		case ISL_IOCTL_ALS_DATA:
               	reg_val = i2c_smbus_read_byte_data(ISL_datap->client,0x09);
			if(reg_val < 0)
			{
				return ret;
			}
			lux_val = reg_val;
			reg_val = i2c_smbus_read_byte_data(ISL_datap->client,0x0a);
			if(reg_val < 0)
			{
				return reg_val;
			}
			lux_val = lux_val + ((reg_val & 0xf) << 8);
			return lux_val;
			break;
		case ISL_IOCTL_ALS_CALIBRATE:

			break;
		case ISL_IOCTL_CONFIG_GET:
			ret = copy_to_user((struct ISL_cfg *)arg, ISL_cfgp, sizeof(struct ISL_cfg));
			if (ret) {
				printk(KERN_ERR "ISL: copy_to_user failed in ioctl config_get\n");
				return -ENODATA;
			}
			return (ret);
			break;
        case ISL_IOCTL_CONFIG_SET:
                        ret = copy_from_user(ISL_cfgp, (struct ISL_cfg *)arg, sizeof(struct ISL_cfg));
			if (ret) {
				printk(KERN_ERR "ISL: copy_from_user failed in ioctl config_set\n");
                                return -ENODATA;
			}
  			return (ret);
                	break;
	  case ISL_IOCTL_PROX_ON:
	  		ISL_prox_cur_infop->prox_event = 0;
            		ISL_prox_cur_infop->prox_clear = 0;
            		ISL_prox_cur_infop->prox_data = 0;
			ret=enable_light_and_proximity(0x01);
			if(ret>=0)
			{
				prox_on = 1;	
            			pr_crit(ISL_TAG "TAOS_IOCTL_PROX_ON\n");
			}	
			return ret;	
			break;
        case ISL_IOCTL_PROX_OFF:
			ret=enable_light_and_proximity(0x02);
			if(ret>=0)
			{
				prox_on = 0;	
            			pr_crit(ISL_TAG"TAOS_IOCTL_PROX_OFF\n");
			}	
			return ret;	
			break;
	  case ISL_IOCTL_PROX_DATA:
                        ret = copy_to_user((struct ISL_prox_info *)arg, ISL_prox_cur_infop, sizeof(struct ISL_prox_info));
                        if (ret) {
                                printk(KERN_ERR "ISL: copy_to_user failed in ioctl prox_data\n");
                                return -ENODATA;
                        }
                        return (ret);
                        break;
        case ISL_IOCTL_PROX_EVENT:
 			return (ISL_prox_cur_infop->prox_event);
                        break;
	  case ISL_IOCTL_PROX_CALIBRATE:
			if (!prox_on)			
			 {
				printk(KERN_ERR "ISL: ioctl prox_calibrate was called before ioctl prox_on was called\n");
				return -EPERM;
			}
			prox_sum = 0;
			prox_max = 0;
			for (i = 0; i < 20; i++) {
	                        if ((ret = ISL_prox_poll(&ISL_prox_cal_info[i])) < 0) {
        	                        printk(KERN_ERR "ISL: call to prox_poll failed in ioctl prox_calibrate\n");
                	                return (ret);
                        	}
				pr_crit(ISL_TAG "prox calibrate poll prox[%d] = %d\n",i,ISL_prox_cal_info[i].prox_data); 			
				prox_sum += ISL_prox_cal_info[i].prox_data;
				if (ISL_prox_cal_info[i].prox_data > prox_max)
					prox_max = ISL_prox_cal_info[i].prox_data;
				mdelay(100);
			}
			prox_mean = prox_sum/20;
			ratio = 10*prox_mean/sat_prox;
			for (prox_pt = prox_tablep; prox_pt->ratio && prox_pt->ratio <= ratio; prox_pt++);
        		if(!prox_pt->ratio)
                	return -1;
					 
			ISL_cfgp->prox_threshold_hi = (prox_mean*prox_pt->hi)/10;
			ISL_cfgp->prox_threshold_lo = ISL_cfgp->prox_threshold_hi*THRES_LO_TO_HI_RATIO;
				
			if (ISL_cfgp->prox_threshold_lo < ((sat_prox*15)/100)) {				
				ISL_cfgp->prox_threshold_hi = ((sat_prox*20)/100);
				ISL_cfgp->prox_threshold_lo = (ISL_cfgp->prox_threshold_hi *THRES_LO_TO_HI_RATIO);
			}
			
			//write back threshold
			reg_val = ISL_cfgp->prox_threshold_lo & 0x00FF;
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_L, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			reg_val = ISL_cfgp->prox_threshold_hi & 0x00FF;			
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_PROX_TH_H, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                		return (ret);
			}
			pr_crit(KERN_ERR "ISL prox_cal_threshold_hi=%d,prox_cal_threshold_lo=%d\n",ISL_cfgp->prox_threshold_hi,ISL_cfgp->prox_threshold_lo);
			break;
		default:
			return -EINVAL;
			break;
	}
	return (ret);
}

// read and calculate lux value
static int ISL_get_lux(void) 
{
      u16 raw_clear = 0, reg_val=0;
	u32 lux = 0;
	int ret = 0;
	u8 chdata[2];
	int i = 0;
    	//pr_crit(ISL_TAG "ISL start to calc lux value\n");

	for (i = 0; i < 2; i++) {
       	chdata[i] = i2c_smbus_read_byte_data(ISL_datap->client,ISL_ALSIR_DATA_L+i);
	}
	
	raw_clear = ((chdata[1]&0xF)<< 8)|chdata[0];
	
	als_intr_threshold_hi_param = raw_clear + raw_clear/5;
    	als_intr_threshold_lo_param = raw_clear - raw_clear/5;

	//update threshold 
    	printk(ISL_TAG "als_intr_threshold_hi_param=%x,als_intr_threshold_lo_param=%x\n",als_intr_threshold_hi_param,als_intr_threshold_lo_param);
			reg_val = als_intr_threshold_lo_param & 0x00FF;
    	//printk(ISL_TAG "ISL_ALSIR_TH_L = %x\n",reg_val);
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_L, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}
			reg_val = ((als_intr_threshold_lo_param >> 8) & 0x000F) | ((als_intr_threshold_hi_param << 4) & 0x00F0) ;			
    	//printk(ISL_TAG "ISL_ALSIR_TH_HL = %x\n",reg_val);
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_HL, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}
			reg_val = (als_intr_threshold_hi_param >> 4) & 0x00FF;			
    	//printk(ISL_TAG "ISL_ALSIR_TH_H = %x\n",reg_val);
			if ((ret = (i2c_smbus_write_byte_data(ISL_datap->client, ISL_ALSIR_TH_H, reg_val))) < 0) {
				printk(KERN_ERR "ISL: i2c_smbus_write_byte_data failed in ioctl prox_on\n");
                return (ret);
			}

	lux = raw_clear;
	return(lux);
}

static int ISL_lux_filter(int lux)
{
        static u8 middle[] = {1,0,2,0,0,2,0,1};
        int index;

        lux_history[2] = lux_history[1];
        lux_history[1] = lux_history[0];
        lux_history[0] = lux;
        if((lux_history[2] < 0) || (lux_history[1] < 0) || (lux_history[0] < 0))
		return -ENODATA;
        index = 0;
        if( lux_history[0] > lux_history[1] ) index += 4;
        if( lux_history[1] > lux_history[2] ) index += 2;
        if( lux_history[0] > lux_history[2] ) index++;
        return(lux_history[middle[index]]);
}

// verify device
static int ISL_device_name(unsigned char *bufp, char **device_name) {
/*		if( (bufp[0x12]&0xf0)!=0x00)
			return(0);
		if(bufp[0x10]|bufp[0x1a]|bufp[0x1b]|bufp[0x1c]|bufp[0x1d]|bufp[0x1e])
			return(0);
  		if(bufp[0x13]&0x0c)
			return(0);
*/			
		*device_name="isl29026";
		return(1);
}

// proximity poll
static int ISL_prox_poll(struct ISL_prox_info *prxp) {
	u8 prox_data;

	prox_data = i2c_smbus_read_byte_data(ISL_datap->client,ISL_PROX_DATA);   
	prxp->prox_data = prox_data;
	 
	return (prox_data);
}
// start prox poll timer
/*
static void ISL_prox_poll_timer_start(void) {
        init_timer(&prox_poll_timer);
        prox_poll_timer.expires = jiffies + (HZ/10);
        prox_poll_timer.function = ISL_prox_poll_timer_func;
        add_timer(&prox_poll_timer);

        return;
}*/


MODULE_AUTHOR("ZTE Software");
MODULE_DESCRIPTION("ISL ambient light and proximity sensor driver");
MODULE_LICENSE("GPL");

module_init(ISL_init);
module_exit(ISL_exit);

