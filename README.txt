高通平台android开发总结

高通android智能平台概述

高通 7230 android 智能手机解决方案的软件包括两个部分
1. 以linux 操作系统为基础的 android 系统
2. 以 L4，REX为基础的 Modem 部分

在高通7系列的架构中，一个IC内部集成有两个ARM处理器，一个ARM9（或者arm11），专门负责处理通信协议，射频以及GPIO等，软件架构采用 AMSS， 另外一个是ARM11，用来处理多媒体，上层应用，以及其他的一些任务，运行的系统是 android 系统，这两个处理器之间通过共享内存的硬件方式来进行通信。

L4是一组计算机程序，是最初由Jochen Liedtke设计的微内核构架的操作系统内核，现在已经形成一个微内核家族。L4这个微内核系统由于其出色的性能和很小的体积而开始被计算机工业所认知，被移植到了许多不同的硬件构架上。高通的L4提供了操作系统最基本的操作，是买别人的。

早期的操作系统绝大多数是 Monolithic Kernel, 意思是整个操作系统 - 包括Scheduling (调度), File system (文件系统), Networking (网络), Device driver (设备驱动程序), Memory management (存储管理), Paging(存储页面管理) - 都在内核中完成.一直到现在广泛应用的操作系统,如UNIX,Linux,和Windows还大都是monolithic kernel操作系统.但随着操作系统变得越来越复杂(现代操作系统的内核有一两百万行C程序是很常见的事情),把所有这些功能都放在内核中使设计难度迅速增加.
微内核是一个与Monolithic Kernel相反的设计理念.它的目的是使内核缩到最小,把所有可能的功能模块移出内核.理想情况下,内核中仅留下Address Space Support(地址空间支持),IPC (Inter-Process Communication,进程间通讯),和Scheduling(调度),其他功能模块做为用户进程运行。
microkernel的整体性能不如monolitic，这对于追求性价比的厂商而言，采用monolitic也是有道理的。不过 microkernel也有自己的长处，就是稳定。相对而言，microkernel比monolitic的依赖更为简单，所以它更稳定。不过第一代microkernel真的很令人失望，到了Jochen Liedtke（German），他设计了第二代microkernel，称为L4，为了追求效率，所有的L4用汇编语言实现（牛人吧，用汇编写操作系统），但是这样的结果是在追求效率的同时，使得第一代L4只能运行在486和pentium上面。但是目前L4已经变成微内核的一个设计标准，围绕这个标准，有很多的L4的实现（http://os.inf.tu-dresden.de/L4/impl.html）。最初的由Jochen设计的L4目前称之为L4/X86，现在最活跃的两个分别是Pistachio和Fiasco。前者是普通的 microkernel，后者是专门为实时系统设计的microkernel，实现了L4V2（L4 API Version 2）。由于Fiasco跟L4的这种关系，所以平时Fiasco也被称做L4/Fiasco，以强调它是L4的一个implementation。
现在，终于开讲Fiasco，Fiasco的意思是“失败“，好像是从意大利语里面借来的（不知道他们为什么去这么一个名字，不过听起来还蛮上口的）。Fiasco是Dresden Real-Time Operating Systems Project（德累斯敦大学实时操作系统项目）的核心子项目，它是一个具备实时功能的microkernel。我们知道，在一个实时系统里面，通常有两种任务，一种是real-time的，另一种是time- sharing的。real-time部分由Fiasco提供，在time-sharing部分，开发者们利用了Linux。平常Linux都是跑在某一种硬件平台（X86，ARM，PowerPC，MIPS），而在Fiasco/L4里面，开发者对Linux进行了修改，使得Linux能够运行在L4/Fiasco，这部分叫做L4Linux。目前，L4Linux的最新版本已经到了L4Linux-2.6.17,这是现行的最新的Linux- 2.6的稳定版本。
下面，我们说说L4/Fiasco中的服务模式（程序架构），这个我认为比较有意思。如果没有任何module被加载，单纯一个microkernel那是什么事情也做不了的。所以基于L4/Fiasco有好多的module被设计出来，这些module以server的形式为后面加载的module提供服务，就好似多个C/S嵌套起来一样。
最后，讲一下microkernel跟monolitic的区别，前面说Fiasco的时候，也说到module，大家在编译Linux kernel的时候，也会有module。这两个地方的module是不一样的。前面的module是彼此之间是隔离的，具有不同的address space，而后者之间共享同一个address space。

REX(Run Time Executive)是在 L4 之上封装的服务，是一个抢占式，多任务的RTOS，所有的任务都以task的形式存在，REX提供包括任务创建，同步，互斥，计时器，中断控制等功能的API，这里的task实际上就是我们的线程，每个 task对应着一个线程。REX维护一个task list(双向链表)，始终运行高优先级的task。products里面所有的服务包括3g协议栈等都是以task的形式跑在rex之上的。

而Brew的话是运行的环境，跟Java 有点儿类似，相当于是一个虚拟机。

AMSS――高级的移动用户软件(Advanced Mobile Subscriber Software)技术，是一种新的软件架构，是对原来软件架构 DMSS 的升级。 AMSS源代码实际上是QC BREW(Binary Runtime Environment For Wireless)平台的的底层部分，去掉了为应用程序提供接口的AEE(application execution environment)部分，高通在Dual Proc芯片上的其他平台基本上都是采用的这样的架构。

参考文档：
微内核操作系统及L4概述
http://wenku.baidu.com/view/90929762caaedd3383c4d311.html

MSM平台上的AMSS
http://blog.csdn.net/yili_xie/archive/2010/01/04/5129469.aspx


要编译出可供烧写使用的镜像文件需要三部分代码：
1) 获取经过高通打补丁的 android 源代码
2) 获取高通针对不同处理器的 vendor 源代码
3) 获取 modem 源代码

1　获取经过高通打补丁的android 源代码
https://www.codeaurora.org/xwiki/bin/QAEP/
2　获取高通针对不同处理器的vendor源代码
根据选择的 Android 源代码分支不同， vendor代码的选择也是不一样的，BUILD ID 必须一致
3　获取 modem 源代码
根据硬件配置情况，选择不同的 BUILD ID, 然后根据 BUILD ID 选择相近的源代码
BUILD ID 是一组字母的组合，如：AAABQOLYM
以下分别对各个字段进行说明：

第1个字母

第2个字母
F基于ffa参考设计
S基于surf的参考设计
surf和ffa的区别
generally speaking surf have FPGA and don't have battery.
and FFA don't have FPGA but have battery.
msm7627_surf and msm7627_ffa and msm7627_7x_surf means different hardware reference design. please check with your hardware engineer on which qualcomm hardware reference you use and then select the right build command.

第3个字母
A表示LPDDR2
B表示LPDDR1

第4个字母

第5个字母
J
K相对于J多了以下选项
USES_HSU_CHG_BOOT=yes
USES_HSU_FAST_CHARGE=yes
USES_CHARGER=yes
USES_EBI1_TURBO_FFA=yes

第6个字母: C/D/O/M/N/P
M表示 Multimode
C表示CDMA
D表示 JCDMA
相对于C多了
USES_UMTS=yes
USES_DSHDR_JCDMA_APIS=yes
O表示只支持 UMTS(WCDMA)
USES_IPHC=yes
USES_PDCP=yes
USES_SUPPORT_UMTS_GPS_PROTOCOLS=yes
USES_UMTS=yes
N表示支持UMTS(WCDMA) 且支持CDMA/CDMA2000
P表示只支持CDMA/CDMA2000
USES_CDMA=yes
USES_CDMA2000=yes
USES_REL_C=yes
USES_SUPPORT_CDMA_GPS_PROTOCOLS=yes
USES_EXPORT_MCCMEID=yes
USES_HDR=yes

第7/8个字母
AZ表示NADN boot
LY表示eMMC boot  #相对于AZ多了选项： USES_SDCC_BOOT=yes USES_HSU_MS_FD_BOOT=yes

最后一个字母
M表示modem
A表示app

Build commands
To create an AMSS build, run the command script with the build ID matching the build
configuration desired. The command scripts may be executed from the build/ms subdirectory or
from the AMSS root directory using the full path to the command script as in the following
examples:

./AMSS/products/<asic>/build/ms/MSNCJNLYM.cmd – For SURF multimode build
./AMSS/products/<asic>/build/ms/MSNCJOLYM.cmd – For SURF UMTS only build
./AMSS/products/<asic>/build/ms/MSNCJPLYM.cmd – For SURF 1X only build
./AMSS/products/<asic>/build/ms/MFNCKNLYM.cmd – For FFA multimode and for reference only

以高通的开发板子为例，看如何选择源代码，通过 QPST 连接高通的开发板，我们可以看到他的配置信息如下：

高通 demo 版的配置情况：SURF-MSM7630 7X30A-AAABQMAZM-1200
从配置情况来看只有 HY11-N0216-3_1.2.00  HY11-N0216-4_1.2.20 满足要求
HY11-N0216-3_1.2.00/AMSS/products/7x30/build/ms/AAABQMAZM.cmd
HY11-N0216-4_1.2.20/AMSS/products/7x30/build/ms/AAABQMAZM.cmd

后期我们要跟据实际情况来选择 BUILD ID，如我们的手机的配置情况为：
LPDDR2 ，WCDMA，eMMC 启动(NAND备选)，那么modem 的BUILD ID为：
AAABQOLYM 或者 AAABQOAZM
AAABQOLYM HY11-N0723-2_1.2.20/AMSS/products/7x30/build/ms/AAABQOLYM.cmd
AAABQOAZM HY11-N1122-1_1.2.20/AMSS/products/7x30/build/ms/AAABQOAZM.cmd (NABD boot 备选)

高通的Android第一人晓峰不建议我们第一次就用eMMC，而应该用Nand，目前只有HTC在用eMMC。高通在eMMC上面提供了参考代码，但是是未经过验证。终端厂商在使用过程中，一定会有很多问题需要自己解。

补充信息：
现在可能用到 BUILD ID 以及相应的 modem 和 vendor 源代码。
modem代码：
  BUILD ID              modem 源代码        modem镜像         说明文档
M7630AABBQMAZM1200   HY11-N0216-3.zip  HK11-N0216-3.zip  HT11-N0216-3.zip
M7630AABBQMAZM1220   HY11-N0216-4.zip  HK11-N0216-4.zip  HT11-N0216-4.zip
--

M7630AABBQMLYM1200   HY11-N0723-1.zip  HK11-N0723-1.zip  HT11-N0723-1.zip
M7630AABBQMLYM1220   HY11-N0723-2.zip  HK11-N0723-2.zip  HT11-N0723-2.zip
--
M7630AAABQCAZM1220   HY11-N1122-1.zip  HK11-N1122-1.zip  HT11-N1122-1.zip
M7630AAABQCAZM1240   HY11-N1122-2.zip  HK11-N1122-2.zip  HT11-N1122-2.zip
M7630AAABQCAZM1250   HY11-N1122-3.zip  HK11-N1122-3.zip  HT11-N1122-3.zip
M7630AAABQCAZM1260   HY11-N1122-4.zip  HK11-N1122-4.zip  HT11-N1122-4.zip
M7630AAABQCAZM1280   HY11-N1122-5.zip  HK11-N1122-5.zip  HT11-N1122-5.zip
M7630AAABQCAZM1290   HY11-N1122-6.zip  HK11-N1122-6.zip  HT11-N1122-6.zip
--
M7630AAABQMAZM1240   HY11-N1496-2.zip  HK11-N1496-2.zip  HT11-N1496-2.zip
M7630AAABQMAZM1250   HY11-N1496-3.zip  HK11-N1496-3.zip  HT11-N1496-3.zip

80-N0216-3_B_M7630AABBQMAZM1200.pdf
5.3.2.1 LPDDR1 NAND boot Multimode              AABBQMAZM.cmd
5.3.2.2 LPDDR1 eMMC boot Multimode              AABBQMLYM.cmd
5.3.2.3 LPDDR1 eMMC boot UMTS only              AABBQOLYM.cmd
5.3.2.4 LPDDR1 NAND boot JCDMA                  AABBQDAZM.cmd
5.3.2.5 LPDDR2 NAND boot Multimode              AAABQMAZM.cmd
5.3.2.6 LPDDR2 eMMC boot Multimode              AAABQMLYM.cmd

80-N0216-4_A_M7630AABBQMAZM1220.pdf
5.3.2 Build instructions and commands
5.3.2.1 LPDDR1 NAND boot Multimode              AABBQMAZM.cmd
5.3.2.2 LPDDR1 eMMC boot Multimode              AABBQMLYM.cmd
5.3.2.3 LPDDR1 eMMC boot UMTS only              AABBQOLYM.cmd
5.3.2.4 LPDDR1 NAND boot JCDMA                  AABBQDAZM.cmd
5.3.2.5 LPDDR2 NAND boot Multimode              AAABQMAZM.cmd
5.3.2.6 LPDDR2 eMMC boot Multimode              AAABQMLYM.cmd
5.3.2.7 LPDDR2 eMMC boot UMTS only              AAABQOLYM.cmd
5.3.2.8 LPDDR2 NAND boot C2K Only               AAABQCAZM.cmd
5.3.2.9 LPDDR1 eMMC boot C2K Only               AABBQCLYM.cmd
5.3.2.10 LPDDR2 NAND boot JCDMA                 AAABQDAZM.cmd
5.3.2.11 LPDDR2 NAND boot UMTS only             AAABQOAZM.cmd

80-N1665-1_B_M7630AAABQ_AZM1240.pdf
5.3.2 Build instructions and commands
5.3.2.1 LPDDR1/LPDDR2 eMMC boot Multimode      AABBQMLYM.cmd
5.3.2.2 LPDDR1/LPDDR2 eMMC boot UMTS only      AABBQOLYM.cmd
5.3.2.3 LPDDR1/LPDDR2 eMMC boot C2K only       AABBQCLYM.cmd
5.3.2.4 LPDDR2/LPDDR1 NAND boot Multimode      AAABQMAZM.cmd
5.3.2.5 LPDDR1/LPDDR2 NAND boot JCDMA          AABBQDAZM.cmd
5.3.2.6 LPDDR2/LPDDR1 NAND boot C2K only       AAABQCAZM.cmd
5.3.2.7 LPDDR2/LPDDR1 NAND boot UMTS only      AAABQOAZM.cmd

The same build ID will now work for LPDDR1 as well as LPDDR2.

从以上的发布信息，只有 LPDDR2 和 LPDDR1 不同的情况下可以使用同一个 build ID。
对于我们的硬件配置情况(LPDDR2 eMMC boot UMTS only) 可以使用的编译命令文件为：
AAABQOLYM.cmd  AABBQOLYM.cmd

选择 modem 代码 M7630AAABQMAZM1250  HK11-N1496-3.zip

如果是支持 BREW 平台，通常多下面的选项：
USES_BREW_4.0=yes
USES_BREW=yes
USES_BREW_APPMGR=yes
或者 USES_BREW_USB_HID=yes
没有 USES_DIAG_SMD_SUPPORT=yes

项目 PD1007
OEM/Target Equipment (FeaturePhone,Smartphone,Datacard): Smartphone
Anticipated Launch Date: April 1 2011
Target market (such as China Telecom): China Open market
Current Software (such as Q6270BKPRZL1505):  froyo-M76XXTSNCJNLYA7010
Bluetooth IC/Module (vendor name and module if support BT): BTS4025
WLAN IC/Module (vendor and module if support WLAN ):WCN1312
Chipset (such as QSC6270):MSM7227-1
RF chipset (such as RGR6240):RTR6285
PMIC chipset (such as PM7540):PM7540
WCMDA Supported Bands (900,1900,2100…. if support WCDMA):2100
CMDA Supported Bands (450,850,1900,2100… if support CDMA ):not suport
OS (Brew,BMP,Android,WM,ThinUI(no UI)…):android froyo
OS version (Android Donuts…): android froyo

高通 modem 源代码编译前的修正
从高通获取的源代码直接编译会存在一些问题，以下为可能的问题以及解决方法：
1)为相应的脚本加上编译选项
vim ./AMSS/products/7x30/build/ms/xxxxxx.cmd
加上： BUILD_UNIX=yes
2）如果编译过程出现 .pl 文件没有执行权限而退出，那么为工程下面所有的 pl 文件加上可执行属性，命令：
find ./ -name "*.pl" |xargs chmod  755
3) 无法找到需要的 pl 脚本解析器
vim ./AMSS/products/7x30/tools/build/depgen.pl
修改
#!/pkg/perl/5.6.1/bin/perl -w
为
#!/usr/bin/perl -w
4)如果出现错误： "AMSS/products/7x30/core/bsp/build/data/incpathsaaabqmlym.py", line 14
IndentationError: expected an indented block
修改文件：./AMSS/products/7x30/core/bsp/build/scripts/genpaths.pl 内容：
#if ($line =~ /^# makefile \(from/)
为：
if ($line =~ /^# makefile /)

原因分析：
genpaths.pl 会对编译过程的中间信息 :
...
line=# makefile (from'incpaths.min'，line 363)
line=QVPCODEC_AUDIOINC = $(QVPCODEC)/audio/inc
...

等信息进行解析，由于我们使用的是中文系统，所以以上内容变为：
line=# makefile (从'incpaths.min'，行 363)
line=QVPCODEC_AUDIOINC = $(QVPCODEC)/audio/inc
所以导致：
#if ($line =~ /^# makefile \(from/) 判断条件为为 false
无法正确生成  incpathsaaabqmlym.py，python 在解析该文件的时候认为存在语法错误。

在 modem 代码 M7630AABBQMAZM2020(HY11-N2280-2.zip)中开始使用 genincpaths.py 产生
./AMSS/products/7x30/core/bsp/build/data/incpathsaaabqoazm.py 文件，所以修改文件
amend-source-code-qualcomm-modem.sh 在脚本中需要加入以下内容：
incpathsxxx_py=$PRODUCT_DIR/core/bsp/build/scripts/genincpaths.py
if test -f $incpathsxxx_py; then
    PERL=`which /usr/bin/perl`
    sed -i "s/(from//g" $incpathsxxx_py
fi

./AMSS/products/7x30/multimedia/audio/drivers/Adie/AdieCodecDb/src/DALAdieCodecDb.c
#DalAdieCodecDbInternal.h 改为：
#include "DALAdieCodecDbInternal.h"

5) 如果出现 make: execvp: ../../../../platform/cs/bin/cifc: Permission denied 错误，执行以下命令：
chmod -R 755 AMSS/platform/cs/bin/*

配置usb

cd /etc/udev/rules.d/
sudo vi 50-android.rules
# adb single interface device
SUBSYSTEM=="usb_device", SYSFS{idVendor}=="18d1", SYSFS{idProduct}=="d00d",
MODE="0664", GROUP="plugdev"
# adb composite interface device
SUBSYSTEM=="usb_device", SYSFS{idVendor}=="18d1", SYSFS{idProduct}=="deed",
MODE="0664", GROUP="plugdev"
# Sooner low-level bootloader
BUS=="usb", SYSFS{idVendor}=="0bb4", SYSFS{idProduct}=="0a51",
MODE="0664",GROUP="plugdev"

sudo /etc/init.d/udev restart

建立 modem 开发环境
与android 源代码不同，modem 编译工具使用的是 RVDS ,实际上只用到其中的 RVCT 编译器， 高通不提供该工具，我们必须从arm公司获取相应的授权，RVDS 是 ADS 的升级版本，从 ADS 1.2 开始到 RVDS 3.1 我们公司都有其授权，按照高通的要求我们目前必须使用 RVDS 2.2 并且使用他的 349 patch。

在高通开发板上烧录文件系统
通过设置模块完成系统，应用程序等个性化设置。
1. 烧录之前必须了解获取硬件配置情况
高通 demo 板的配置情况：SURF-MSM7630 7X30A-AAABQMBZM-1090  或者 7X30A-AAABQMAZM-1200
以上信息可以通过 QPST 连接高通的demo开发板获得
2. 由硬件配置情况到高通网站选择相应的镜像文件
要给高通的开发板烧录文件系统，必须有以下的镜像文件：
adsp.mbn           dsp相关的
amss.mbn           AMSS modem binary image
dbl.mbn            设备启动的装载程序
osbl.mbn           OS boot loader binary image
partition.mbn      分区表的二进制文件
appsboot.mbn       应用程序的启动文件，如果是 eMMC 启动对应文件 emmc_appsboot.mbn
boot.img           ap 端的linux 内核
system.img         android 文件系统的 system 分区
userdata.img       android 文件系统的 data 分区

其中 adsp.mbn，amss.mbn，dbl.mbn，osbl.mbn，partition.mbn 可以从以下目录获取
unzip /home/shared/qualcommon/HK11-N0216-3.zip
unzip /home/shared/qualcommon/HK11-N0216-4.zip
HK11-N0216-3_1.2.00/AAABQMAZ/
HK11-N0216-4_1.2.20/AAABQMAZ/

appsboot.mbn，boot.img，system.img，userdata.img 必须来自eclair源代码

3. 烧录镜像文件到高通demo开发板
怎样烧写文件请参考工具使用文档
通过fastboot烧写 boot.img syste.img usrdata.img
参考文档：
80-VR192-1_D_AMSS_Linux_Software_Users_Guide.pdf
6.2 USB setup in Linux
6.2.1 Using Fastboot in Linux

4. 从高通网站下载源代码编译相应的镜像文件进行验证
高通有两个cpu，他们分别跑不同的系统，应用程序(ap)端是android系统，modem 端是高通自己的系统。
android 系统目前使用的是 eclair 版本，此版本来自标准的 android 2.1 eclair，高通在上面加了自己的补丁
appsboot.mbn，boot.img，system.img，userdata.img 位于以下目录：
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/appsboot.mbn
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/boot.img
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/system.img
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/userdata.img
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/system.img.ext3
./eclair-M7630AABBQMLZA1150/out/target/product/msm7630_surf/userdata.img.ext3
eMMC 启动使用的是 ext3 文件系统，需要使用镜像 system.img.ext3 userdata.img.ext3

modem代码从高通网站：https://support.cdmatech.com/login/上获取
其中 adsp.mbn，amss.mbn，dbl.mbn，osbl.mbn，partition.mbn 位于以下目录：
./modem-M7630AABBQMAZM1220/AMSS/products/7x30/build/ms/bin/AAABQMAZ/adsp.mbn
./modem-M7630AABBQMAZM1220/AMSS/products/7x30/build/ms/bin/AAABQMAZ/amss.mbn
./modem-M7630AABBQMAZM1220/AMSS/products/7x30/build/ms/bin/AAABQMAZ/dbl.mbn
./modem-M7630AABBQMAZM1220/AMSS/products/7x30/build/ms/bin/AAABQMAZ/osbl.mbn
./modem-M7630AABBQMAZM1220/AMSS/products/7x30/build/ms/bin/AAABQMAZ/partition.mbn
partition.mbn 文件的原始数据来自配置文件：
./AMSS/products/7x30/tools/jsdcc/partition_load_pt/partition.xml

高通平台android和 modem 编译流程分析
android代码编译流程分析
. ./setenv.sh  设置环境变量
Setting Environment ...

通常情况下 generic 用于编译模拟器版本
Options are:
     1. generic
     2. msm7627_surf
     3. msm7627_ffa
     4. tiny-system
====从以上列表中选择====
Which would you like? [1]

烧录的镜像文件appsboothd.mbn  appsboot.mbn boot.img system.img userdata.img persist.img 来自 android 源码
appsboot.mbn 的主要功能是装载 linux 内核，通过 linux 的引导来完成 android系统的加载，
appsboot.mbn 被烧写在相应的分区，它的加载由 osbl.mbn 来完成，osbl.mbn程序来自modem。

7x30 7x27 系列 appsboot 不一样，7x30的appsboot.mbn是7x27的appsboothd.mbn和appsboot.mbn两个文件的合并

appsboot.mbn 生成过程解析
make out/target/product/msm7630_surf/appsboot.mbn
make out/target/product/msm7630_surf/nandwrite
make out/target/product/msm7630_surf/emmc_appsboot.mbn

分别执行：
make -C bootable/bootloader/lk BOOTLOADER_OUT=../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ msm7630_surf
make -C bootable/bootloader/lk BOOTLOADER_OUT=../../.././out/target/product/msm7630_surf/obj/nandwrite msm7630_surf_nandwrite BUILD_NANDWRITE=1
make -C bootable/bootloader/lk BOOTLOADER_OUT=../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_EMMC_OBJ msm7630_surf EMMC_BOOT=1

生成 nandwrite
===============
#build nandwrite as a part of Android Build
TARGET_NANDWRITE := $(PRODUCT_OUT)/obj/nandwrite/build-$(TARGET_PRODUCT)_nandwrite/lk
NANDWRITE_OUT := $(TOP)/$(TARGET_OUT_INTERMEDIATES)/nandwrite

nandwrite_clean:
    $(hide) rm -f $(TARGET_NANDWRITE)
    $(hide) rm -rf $(NANDWRITE_OUT)
$(NANDWRITE_OUT):
    mkdir -p $(NANDWRITE_OUT)
$(TARGET_NANDWRITE): nandwrite_clean $(NANDWRITE_OUT)
    @echo $(TARGET_PRODUCT)_nandwrite
    $(MAKE) -C bootable/bootloader/lk BOOTLOADER_OUT=../../../$(NANDWRITE_OUT) $(TARGET_PRODUCT)_nandwrite BUILD_NANDWRITE=1
================

vim ./bootable/bootloader/lk/makefile:98:
TARGET   := msm7630_surf  #./bootable/bootloader/lk/project/msm7630_surf.mk
PLATFORM := msm7x30       #./bootable/bootloader/lk/target/msm7630_surf/rules.mk
msm7x30
msm7630_surf.mk
msm7630_surf_nandwrite
=============================

include project/$(PROJECT).mk
include target/$(TARGET)/rules.mk
include target/$(TARGET)/tools/makefile
include platform/$(PLATFORM)/rules.mk
include arch/$(ARCH)/rules.mk
include platform/rules.mk
include target/rules.mk
include kernel/rules.mk
include dev/rules.mk
include app/rules.mk
...

include make/module.mk
...
include make/build.mk
========================
vim bootable/bootloader/lk/make/module.mk
INCMODULES := $(MODULES)
$(info including $(INCMODULES))
include $(addsuffix /rules.mk,$(INCMODULES))
展开为：
./bootable/bootloader/lk/app/nandwrite/rules.mk
./bootable/bootloader/lk/dev/fbcon/rules.mk
./bootable/bootloader/lk/dev/keys/rules.mk
./bootable/bootloader/lk/lib/debug/rules.mk
./bootable/bootloader/lk/lib/heap/rules.mk
./bootable/bootloader/lk/lib/libc/rules.mk
./bootable/bootloader/lk/lib/ptable/rules.mk
========================
bootable/bootloader/lk/make/build.mk:29:
include arch/$(ARCH)/compile.mk

./bootable/bootloader/lk/project/msm7630_surf_nandwrite.mk 中有：
MODULES += app/nandwrite
./bootable/bootloader/lk/platform/msm7x30/rules.mk中有：
MODULES += dev/fbcon
./bootable/bootloader/lk/kernel/rules.mk 中有：
MODULES += lib/libc lib/debug lib/heap
./bootable/bootloader/lk/target/msm7630_surf/rules.mk 中有：
MODULES += dev/keys lib/ptable
所以：
MODULES= app/nandwrite dev/fbcon dev/keys lib/debug lib/heap lib/libc lib/ptable

因为
ifeq ($(BUILD_NANDWRITE), 1)
    APPSBOOTHDR_FILES :=
  else
out/host/linux-x86/bin/acp -fpt out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk out/target/product/msm7630_surf/nandwrite

  25780     184   34000   59964    ea3c ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk

arm-eabi-objcopy -O binary ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.bin
generating listing: ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.lst
generating listing: ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.debug.lst
generating symbols: ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.sym
generating size map: ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.size
make[2]:正在离开目录 /opt/workspace/gphone/qualcomm/eclair-M7630AABBQMLZA1150/bootable/bootloader/lk
make[1]:正在离开目录 /opt/workspace/gphone/qualcomm/eclair-M7630AABBQMLZA1150/bootable/bootloader/lk
target Prebuilt:  (out/target/product/msm7630_surf/nandwrite)

make -C bootable/bootloader/lk BOOTLOADER_OUT=./out/target/product/msm7630_surf/obj/nandwrite msm7630_surf_nandwrite BUILD_NANDWRITE=1
make[2]: 正在进入目录 `/opt/workspace/gphone/qualcomm/eclair-M7630AABBQMLZA1150/bootable/bootloader/lk`
msm7630_surf_nandwrite
make -C bootable/bootloader/lk BOOTLOADER_OUT=../../.././out/target/product/msm7630_surf/obj/nandwrite msm7630_surf_nandwrite BUILD_NANDWRITE=1

==》生成 mkheader 工具
gcc target/msm7630_surf/tools/mkheader.c -o target/msm7630_surf/tools/mkheader
$(BUILDDIR)/system-onesegment.ld: $(LOCAL_DIR)/system-onesegment.ld
    @echo generating $@
    @$(MKDIR)
    $(NOECHO)sed "s/%MEMBASE%/$(MEMBASE)/;s/%MEMSIZE%/$(MEMSIZE)/" < $< > $@
generating ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/system-onesegment.ld
compiling arch/arm/crt0.S
compiling arch/arm/crt0.S
。。。compiling lib/ptable/ptable.c
compiling lib/ptable/ptable.c

vim bootable/bootloader/lk/platform/msm7x30/rules.mk
-----
LINKER_SCRIPT += $(BUILDDIR)/system-onesegment.ld
include platform/msm_shared/rules.mk
-----

./build/core/product_config.mk:177:TARGET_DEVICE := $(PRODUCTS.$(INTERNAL_PRODUCT).PRODUCT_DEVICE)
./build/core/envsetup.mk:152:OUT_DIR := $(TOPDIR)out
./build/core/envsetup.mk:159:TARGET_OUT_ROOT_release := $(OUT_DIR)/target
./build/core/envsetup.mk:161:TARGET_OUT_ROOT := $(TARGET_OUT_ROOT_$(TARGET_BUILD_TYPE))
./build/core/envsetup.mk:178:  TARGET_PRODUCT_OUT_ROOT := $(TARGET_OUT_ROOT)/product
./build/core/envsetup.mk:184:PRODUCT_OUT := $(TARGET_PRODUCT_OUT_ROOT)/$(TARGET_DEVICE)
build/core/envsetup.mk:200:TARGET_OUT_INTERMEDIATES := $(PRODUCT_OUT)/obj
bootable/bootloader/lk/AndroidBoot.mk:4:BOOTLOADER_OUT := $(TOP)/$(TARGET_OUT_INTERMEDIATES)/BOOTLOADER_OBJ
bootable/bootloader/lk/makefile:32:BUILDDIR := $(BOOTLOADER_OUT)/build-$(PROJECT)
bootable/bootloader/lk/makefile:34:OUTELF := $(BUILDDIR)/lk

所以：
TARGET_PRODUCT_OUT_ROOT=out/target/product/
PRODUCT_OUT=out/target/product/msm7630_surf/
TARGET_OUT_INTERMEDIATES=out/target/product/msm7630_surf/obj/
BOOTLOADER_OUT=out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/

所以：
BUILDDIR=out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf

所以：
OUTELF=out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk

make -C bootable/bootloader/lk BOOTLOADER_OUT=../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ msm7630_surf

bootable/bootloader/lk/makefile
OUTELF := $(BUILDDIR)/lk
./bootable/bootloader/lk/make/build.mk
$(OUTELF): $(ALLOBJS) $(LINKER_SCRIPT)
    @echo linking $@
    $(NOECHO)$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(ALLOBJS) $(LIBGCC) -o $@
vim ./bootable/bootloader/lk/AndroidBoot.mk
TARGET_NANDWRITE := $(PRODUCT_OUT)/obj/nandwrite/build-$(TARGET_PRODUCT)_nandwrite/lk
$(TARGET_NANDWRITE): nandwrite_clean $(NANDWRITE_OUT)
    @echo $(TARGET_PRODUCT)_nandwrite
    $(MAKE) -C bootable/bootloader/lk BOOTLOADER_OUT=../../../$(NANDWRITE_OUT) $(TARGET_PRODUCT)_nandwrite BUILD_NANDWRITE=1
BOOTLOADER_EMMC_OUT := $(TOP)/$(TARGET_OUT_INTERMEDIATES)/BOOTLOADER_EMMC_OBJ

vim ./bootable/bootloader/lk/make/build.mk
$(OUTELF): $(ALLOBJS) $(LINKER_SCRIPT)
    @echo linking $@
    $(NOECHO)$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(ALLOBJS) $(LIBGCC) -o $@
bootable/bootloader/lk/makefile:33:OUTBIN := $(BUILDDIR)/lk.bin
bootable/bootloader/lk/makefile:34:OUTELF := $(BUILDDIR)/lk

./lk/platform/msm7x30/rules.mk:23:LINKER_SCRIPT += $(BUILDDIR)/system-onesegment.ld

==》链接 lk
linking ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk
linking ../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk
generating image: ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk.bin
generating image: ../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin
   text    data     bss     dec     hex filename
  29592    7388   42720   79700   13754 ../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk
   text    data     bss     dec     hex filename
  25780     184   34000   59964    ea3c ../../.././out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk

==》以 binary 格式，把文件 out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk 输出到：
out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin
arm-eabi-objcopy -O binary out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin

==》
生成... out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.lst
生成... out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.debug.lst
生成... out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.sym
生成... out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.size
make[1]:正在离开目录  /opt/workspace/gphone/qualcomm/eclair-M7630AABBQMLZA1150/bootable/bootloader/lk
==》
make -C kernel O=../out/target/product/msm7630_surf/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE=arm-eabi- msm7630-perf_defconfig
make[1]: 正在进入目录 /opt/workspace/gphone/qualcomm/eclair-M7630AABBQMLZA1150/kernel
generating symbols: ../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.sym
generating size map: ../../.././out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.size

mkheader 从 lk.bin 生成 appsboothd.mbn 语法： mkheader <bin> <hdr>
然后把 appsboothd.mbn 和 lk.bin 合并为文件 appsboot.mbn，删除 appsboothd.mbn 文件

appsboothd.mbn 大小 40K
./bootable/bootloader/lk/target/msm7630_surf/tools/mkheader out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin out/target/product/msm7630_surf/appsboothd.mbn
合并 out/target/product/msm7630_surf/appsboothd.mbn out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin 为：
out/target/product/msm7630_surf/appsboot.mbn
cat out/target/product/msm7630_surf/appsboothd.mbn out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin > out/target/product/msm7630_surf/appsboot.mbn

删除：out/target/product/msm7630_surf/appsboothd.mbn
==================================================
boot.img 生成过程解析
boot.img 包括两个部分： kernel 和 ramdisk ，通过命令：
make  out/target/product/msm7630_surf/boot.img showcommands
会自动生成 kernel 和 ramdisk，然后把两者合并制作成烧写的镜像文件：boot.img
kernel 为 out/target/product/$TARGET_PRODUCT/obj/KERNEL_OBJ/arch/arm/boot/zImage
或者 out/target/product/$TARGET_PRODUCT/kernel , kernel 是 zImage 的拷贝
编译内核之前必须获取一个默认的配置文件 .config ,此配置文件可以生成，或者来自默认配置，高通中 msm7630_surf 的
默认配置文件名为： msm7630-perf_defconfig 在文件：
vendor/qcom/msm7630_surf/AndroidBoard.mk:45 中通过：
KERNEL_DEFCONFIG := msm7630-perf_defconfig 进行了指定。
我们可以通过命令：
make -j3 ARCH=arm CROSS_COMPILE=arm-eabi- kernel showcommands
或者通过如下命令一步步完成：
1. 获取默认的内核配置并生成.config 的命令如下：
   make -C kernel O=../out/target/product/msm7630_surf/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE=arm-eabi- msm7630-perf_defconfig
   -C kernel 告诉 make 进入 kernel 工作目录，到其下面寻找 makefile 文件
   O  内核编译的中间文件以及编译结果等都存储在该目录下,此做法的好处是不会在kernel源代码目录下生成垃圾文件
   ARCH=arm 告诉系统到 ./kernel/arch/$ARCH/configs/ 目录下找文件 msm7630-perf_defconfig，生成的.config 保存在：
   out/target/product/msm7630_surf/obj/KERNEL_OBJ/.config
2. 手动编译内核
   make -C kernel O=../out/target/product/msm7630_surf/obj/KERNEL_OBJ/
3. 如果需要配置内核，使用下面的命令
  make kernelconfig
  他首先会进入 menuconfig 界面，用户配置完成以后，会把修改后的配置文件$(KERNEL_OUT)/.config 更新为默认的配置文件
cp out/target/product/msm7627_ffa/obj/KERNEL_OBJ/.config kernel/arch/arm/configs/msm7627-perf_defconfig
  kernel/arch/arm/configs/$(KERNEL_DEFCONFIG)
4. 手动生成 ramdisk 执行命令
  make  out/target/product/msm7630_surf/ramdisk.img showcommands
    会用到命令：
  out/host/linux-x86/bin/mkbootfs out/target/product/msm7630_surf/root | out/host/linux-x86/bin/minigzip > \
  out/target/product/msm7630_surf/ramdisk.img
5. 手动生成 boot.img 需要使用命令 mkbootimg ,语法如下:
   mkbootimg --kernel 内核 --ramdisk ramdisk镜像 --cmdline 命令行 --base 基地址 --pagesize 大小 --output 输出的boot.img
==========
KERNEL=out/target/product/$TARGET_PRODUCT/kernel
KERNEL=out/target/product/$TARGET_PRODUCT/obj/KERNEL_OBJ/arch/arm/boot/zImage
RAMDISK=out/target/product/$TARGET_PRODUCT/ramdisk.img
CMDLINE="console=ttyDCC0 androidboot.hardware=qcom"
BASE=0x00200000
PAGESIZE=4096
OUTPUT=out/target/product/$TARGET_PRODUCT/boot.img
out/host/linux-x86/bin/mkbootimg  --kernel $KERNEL --ramdisk $RAMDISK --cmdline "$CMDLINE"  --base $BASE --pagesize $PAGESIZE --output $OUTPUT
=========================================
编译 bootloader (appsboot.mbn) ,nandwrite 生成过程解析
./vendor/qcom/msm7630_surf/AndroidBoard.mk:25:
include bootable/bootloader/lk/AndroidBoot.mk
===================
因为有：
bootable/bootloader/lk/AndroidBoot.mk:3:
TARGET_BOOTLOADER := $(PRODUCT_OUT)/appsboot.mbn
INSTALLED_BOOTLOADER_TARGET := $(PRODUCT_OUT)/bootloader
file := $(INSTALLED_BOOTLOADER_TARGET)
ALL_PREBUILT += $(file)
$(file): $(TARGET_BOOTLOADER) | $(ACP)
        $(transform-prebuilt-to-target)
所以拷贝：
out/target/product/msm7630_surf/appsboot.mbn
为：
out/target/product/msm7630_surf/bootloader
====================
因为有：
./bootable/bootloader/lk/AndroidBoot.mk:38:
TARGET_NANDWRITE := $(PRODUCT_OUT)/obj/nandwrite/build-$(TARGET_PRODUCT)_nandwrite/lk
# Copy nandwrite utility to target out directory
INSTALLED_NANDWRITE_TARGET := $(PRODUCT_OUT)/nandwrite
file := $(INSTALLED_NANDWRITE_TARGET)
ALL_PREBUILT += $(file)
$(file) : $(TARGET_NANDWRITE) | $(ACP)
        $(transform-prebuilt-to-target)
endif
所以拷贝：
out/target/product/msm7630_surf/obj/nandwrite/build-msm7630_surf_nandwrite/lk
为：
out/target/product/msm7630_surf/nandwrite
=====================
编译内核：
#----------------------------------------------------------------------
# Compile Linux Kernel
#----------------------------------------------------------------------
ifeq ($(KERNEL_DEFCONFIG),)
    KERNEL_DEFCONFIG := msm7630-perf_defconfig
endif
include kernel/AndroidKernel.mk
======================
编译 appsboot.mbn
源代码位于： bootable/bootloader/lk/
因为：
vendor/qcom/msm7630_surf/BoardConfig.mk:60:
TARGET_USERIMAGES_USE_EXT2 := true
所以：分别编译支持nand 和 emmc启动的 appsboot.mbn
===================
bootable/bootloader/lk/AndroidBoot.mk
$(TARGET_BOOTLOADER): appsbootldr_clean emmc_appsbootldr_clean $(BOOTLOADER_OUT) $(BOOTLOADER_EMMC_OUT)
        $(MAKE) -C bootable/bootloader/lk BOOTLOADER_OUT=../../../$(BOOTLOADER_OUT) $(TARGET_PRODUCT)
        $(MAKE) -C bootable/bootloader/lk BOOTLOADER_OUT=../../../$(BOOTLOADER_EMMC_OUT) $(TARGET_PRODUCT) EMMC_BOOT=1
$(TARGET_NANDWRITE): nandwrite_clean $(NANDWRITE_OUT)
        @echo $(TARGET_PRODUCT)_nandwrite
        $(MAKE) -C bootable/bootloader/lk BOOTLOADER_OUT=../../../$(NANDWRITE_OUT) $(TARGET_PRODUCT)_nandwrite BUILD_NANDWRITE=1
====================
-C 表明到目录 bootable/bootloader/lk 下找 makfile 文件
bootable/bootloader/lk/makefile
====================
bootable/bootloader/lk/makefile:87:
include target/$(TARGET)/tools/makefile
bootable/bootloader/lk/target/msm7630_surf/tools/makefile
====================
appsboot.mbn: appsboothd.mbn $(OUTBIN)
        cat $(APPSBOOTHEADER_DIR)/appsboothd.mbn $(OUTBIN) > $(APPSBOOTHEADER_DIR)/appsboot.mbn
        rm -f $(APPSBOOTHEADER_DIR)/appsboothd.mbn
appsboothd.mbn: mkheader $(OUTBIN)
        $(SRC_DIR)/mkheader $(OUTBIN) $(APPSBOOTHEADER_DIR)/appsboothd.mbn
========================================
hexdump out/target/product/msm7630_surf/appsboothd.mbn
40个字节的内容如下：
-----
0000000 0005 0000 0002 0000 0000 0000 0000 0000
0000010 9074 0000 9074 0000 9074 0000 0000 0000
0000020 9074 0000 0000 0000
0000028
-----
我们编译的：
0000000 0005 0000 0002 0000 0000 0000 0000 0000
0000010 9074 0000 9074 0000 9074 0000 0000 0000
0000020 9074 0000 0000 0000
高通的：
0000000 0005 0000 0002 0000 0000 0000 0000 0010
0000010 99ac 0000 99ac 0000 99ac 0010 0000 0000
0000020 99ac 0010 0000 0000

We found that the contents of the first 40 bytes is not the same
hexdump  out/target/product/msm7630_surf/appsboot.mbn > bbk.txt
hexdump  ~/appsboot.mbn > qualcomm > qualcomm.txt   (appsboot.mbn from Qualcomm, can properly programmed system.img)

the first 40 bytes below：
Content from the compilation:
0000000 0005 0000 0002 0000 0000 0000 0000 0000
0000010 9074 0000 9074 0000 9074 0000 0000 0000
0000020 9074 0000 0000 0000

Content from the Qualcomm:
0000000 0005 0000 0002 0000 0000 0000 0000 0010
0000010 99ac 0000 99ac 0000 99ac 0010 0000 0000
0000020 99ac 0010 0000 0000
========

Analysis of the generation of appsboot.mbn,from the command:

cat out/target/product/msm7630_surf/appsboothd.mbn out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin > out/target/product/msm7630_surf/appsboot.mbn
we known ,appsboot.mbn consists of two parts: appsboothd.mbn and lk.bin;
appsboothd.mbn come form command:
./bootable/bootloader/lk/target/msm7630_surf/tools/mkheader out/target/product/msm7630_surf/obj/BOOTLOADER_OBJ/build-msm7630_surf/lk.bin out/target/product/msm7630_surf/appsboothd.mbn

编译过程存在的问题
1　prelinkmap 的时候 base 0xaff00000 out of range
build/tools/apriori/prelinkmap.c(100): build/core/prelink-linux-arm.map:13 base 0xaff00000 out of range.
./build/tools/apriori/prelinkmap.c:99:
"%s:%d base 0x%08x out of range.\n",
2　LOCAL_SDK_VERSION 使应用程序不能访问hide的api
LOCAL_SDK_VERSION 会使应用程序不能访问hide的api，如果要访问自定义的api，那么在个应用的Android.mk 文件中屏蔽掉 LOCAL_SDK_VERSION := current
./packages/apps/Mms/Android.mk:13:#LOCAL_SDK_VERSION := current
./packages/apps/BBKMms/Android.mk:13:#LOCAL_SDK_VERSION := current
./packages/apps/Camera/Android.mk:9:# LOCAL_SDK_VERSION := current
3　armv5te-vfp 编译配置，导致一些游戏运行不了
device/qcom/msm7627_ffa/BoardConfig.mk:56:
TARGET_ARCH_VARIANT := armv5te-vfp
build/core/combo/TARGET_linux-arm.mk:37:
TARGET_ARCH_SPECIFIC_MAKEFILE := $(BUILD_COMBOS)/arch/$(TARGET_ARCH)/$(TARGET_ARCH_VARIANT).mk
......
$(TARGET_ARCH_SPECIFIC_MAKEFILE)
build/core/combo/arch/arm/armv5te-vfp.mk:6:
include $(BUILD_COMBOS)/arch/arm/armv5te.mk
build/core/combo/TARGET_linux-arm.mk:209:
如果 WITH_JIT 没有赋值，那么在文件 TARGET_linux-arm.mk 中会设置 WITH_JIT 默认值为 true
# Enable the Dalvik JIT compiler if not already specified.
ifeq ($(strip $(WITH_JIT)),)
    WITH_JIT := true
Endif

dalvik/vm/Android.mk

modem 代码编译流程分析
单独编译某个模块（如：qcsbl oemsbl）
编译 modem的主makefile 文件为 AMSS/products/76XX/build/ms/dmss76XXmodem.mak
. ./make-TFNCKNLYM.sh genqcsbl  单独生成
. ./make-TFNCKNLYM.sh genoemsbl

./AMSS/products/76XX/build/ms/boot_targets_sec.min:1200
genqcsbl: $(MBNPATH_QCSBL)/qcsbl.mbn $(CERTDIR_QCSBL)/exist
./AMSS/products/76XX/build/ms/boot_targets_nonsec.min:870:
genqcsbl: $(MBNPATH)/qcsbl.mbn
./AMSS/products/76XX/build/ms/dmss76XXmodem.mak:49:
ifeq ($(USES_SECBOOT),yes)
   SEC_MODE = sec
else
   SEC_MODE = nonsec
endif

include boot_targets_$(SEC_MODE).min
./AMSS/products/76XX/build/ms/boot_targets_sec.min:108:
SECBOOT=$(SRCROOT)/secboot
./AMSS/products/76XX/build/ms/boot_targets_nonsec.min:107:
SECBOOT=$(SRCROOT)/secboot

./AMSS/products/76XX/build/ms/boot_targets_nonsec.min:872:
genoemsbl: $(MBNPATH)/oemsbl.mbn $(OEMSBL_HD_FILE_NAME)
./AMSS/products/76XX/build/ms/bin/TFNCKNLY/oemsbl.mbn
./AMSS/products/76XX/build/ms/bin/TFNCKNLY/oemsblhd.mbn

AMSS/products/76XX/build/ms/dmss76XXmodem.mak
...
all : dmss
...
include dmss_flags.min
include dmss_76XXT_flags.min
include incpaths.min
include armtools.min
-include amss_lint_flags.min
include dmss_objects.min
include boot_targets_$(SEC_MODE).min
...
include dmss_rules.min
-include amss_lint_rules.min
...
corebsp_build : corebsp_build_action corebsp_create_incpaths corebsp_setup
.PHONY: corebsp_build_action
corebsp_build_action :
ifeq ($(USES_COREBSP_BUILD_SYSTEM), yes)
    $(warning COREBSP Build System Enabled)
    @echo ================= COREBSP Build =======================================
    @echo COREBSP Build System and AMSS Objects
    #$(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak $(MAKEFLAGS)
    $(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak
    #$(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak -rk
    @echo COREBSP AMSS Libraries
   @if test -f $(COREBSP_BUILD)/msm7x30_lib.mak; then $(MAKE) -f $(COREBSP_BUILD)/msm7x30_lib.mak corebsplibs ; fi
    @echo ================= COREBSP Build Done ==================================
else
    $(warning COREBSP Build System Disabled)
    include $(COREBSP_BUILD)/dmss_objects.min
    OBJECTS := $(OBJECTS) $(COREBSP_OBJECTS)
    QCTLIBS := $(QCTLIBS) $(COREBSP_QCTLIBS)
Endif
...

编译规则从 all : dmss 开始
dmss 规则 在文件 ./AMSS/products/7x30/build/ms/dmss_rules.min 中
...
ifeq ($(USES_L4), yes)
    ifeq ($(IMAGE), APPS_PROC)
        ifeq ($(USES_BUILD_NATIVELINUX_APPS), yes)
        #dmss : $(TARGETDIR)/exist setup firmware prereqs deps corebsp_build libs copybar exe bldprod
        dmss : $(TARGETDIR)/exist prereqs exe bldprod
    else
        #dmss : $(TARGETDIR)/exist setup firmware prereqs deps corebsp_build libs copybar exe bldprod
        dmss : $(TARGETDIR)/exist setup prereqs exe bldprod
    endif
    else
    ifneq ($(USES_OBJECT_ONLY), yes)
        ifeq ($(USES_SDCC_BOOT), yes)
            dmss : $(TARGETDIR)/exist setup firmware prereqs deps corebsp_build libs copybar exe bldprod create_mem_feat_html partition
        else
            dmss : $(TARGETDIR)/exist setup firmware prereqs deps corebsp_build libs copybar exe bldprod create_mem_feat_html
        endif
    else
        ifeq ($(USES_SDCC_BOOT), yes)
        dmss : $(TARGETDIR)/exist setup deps corebsp_build libs copybar exe bldprod create_mem_feat_html partition
        else
            dmss : $(TARGETDIR)/exist setup deps corebsp_build libs copybar exe bldprod create_mem_feat_html
        endif
    endif
    endif
endif
...
.PHONY: setup
setup : corebsp_create_incpaths create_incpaths amsssetup amsslinkaddress firmware
...
BUILDID 为 AAABQOLYM 的脚本，比较关心的编译选项有：
BUILD_UNIX=yes
USES_L4=yes
IMAGE=MODEM_PROC
USES_LPDDR2=yes
USES_SDCC=yes
USES_SDCC_BOOT=yes
所以 dmss 规则为：
dmss : $(TARGETDIR)/exist setup deps corebsp_build libs copybar exe bldprod create_mem_feat_html partition
下面将分别对这些规则进行分析：
1.3.3.2　$(TARGETDIR)/exist 规则解析
1.3.3.3　setup规则解析
setup : corebsp_create_incpaths create_incpaths amsssetup amsslinkaddress firmware
1.3.3.3.1　corebsp_create_incpaths
1.3.3.3.2　create_incpaths
1.3.3.3.3　amsssetup
1.3.3.3.4　amsslinkaddress
amsslinkaddress 规则在文件：
./AMSS/products/7x30/build/ms/dmss_rules.min:871 中
amsslinkaddress : amsssetup
    @echo Generate AMSS Link Address file: $(CUSTL4SCLFILE).....
    @$(GEN_AMSS_LINK_ADDRESS)
amsssetup : create_incpaths
GEN_AMSS_LINK_ADDRESS 执行的是文件：
./AMSS/products/7x30/build/ms/dmss_rules.min:696 中的函数
。。。
L4_BASE_DIR := ../../core/kernel
AMSS_RELOC_DIR := build_$(BUILD_KCFG)/amss/bin
PLATMSDIR := ../../core/kernel/build_$(BUILD_KCFG)/ms
ELFWEAVER_CMD = python tools/pyelf/elfweaver
define GEN_AMSS_LINK_ADDRESS
    @echo --------------------------------------------------------
    @echo Determining AMSS link address from target XML file...
    @-if test -f $(CUSTL4SCLFILE); then rm $(CUSTL4SCLFILE); fi
    @echo "#ifndef CUST_L4_SCL_$(BUILD_KCFG)_H" >> $(CUSTL4SCLFILE)
    @echo "#define CUST_L4_SCL_$(BUILD_KCFG)_H" >> $(CUSTL4SCLFILE)
    @echo -n "#define SCL_L4_START_ADDRESS " >> $(CUSTL4SCLFILE)
    @echo Preprocessing XML file into quartz_cfg_$(BUILD)_cpp.xml...
    @echo Preprocessing XML file into quartz_cfg_machine_$(BUILD)_cpp.xml...
    $(CC) -E $(CFLAGS) $(DMSS_CFLAGS) $(ALL_INCLUDES) $(DIRNAME) -I"$(ARMINC)" $(BUILD_VERSION) $(APPFLAGS) $(MSG_BT_SSID) $(OBJ_CMD) quartz_cfg_machine_$(BUILD)_cpp.xml quartz_cfg_machine.xml
    @echo Copying cleaned XML file to quartz_cfg_machine_$(BUILD).xml...
    ./cleanup_xml.pl -i quartz_cfg_machine_$(BUILD)_cpp.xml -o quartz_cfg_machine_$(BUILD).xml
    rm -f quartz_cfg_machine_$(BUILD)_cpp.xml
    @echo Preprocessing XML file into quartz_cfg_$(BUILD)_cpp.xml...
    $(CC) -E $(CFLAGS) -DAMSS_RELOC_LC=\"$(AMSS_RELOC_DIR)/$(BUILD_LC).reloc\" -DQUARTZ_MACHINE_XML=\"../../build/ms/quartz_cfg_machine_$(BUILD).xml\" $(DMSS_CFLAGS) $(COREBSP_XALL_INCLUDES) $(ALL_INCLUDES) $(DIRNAME) -I"$(ARMINC)" -I"../../core/systemdrivers/hwio/chipset/msm7x30/inc/plat/l4" $(BUILD_VERSION) $(APPFLAGS) $(MSG_BT_SSID) $(OBJ_CMD) quartz_cfg_$(BUILD)_cpp.xml quartz_cfg.xml
    @echo Copying cleaned XML file to quartz_cfg_$(BUILD).xml...
    ./cleanup_xml.pl -i quartz_cfg_$(BUILD)_cpp.xml -o quartz_cfg_$(BUILD).xml
    rm -f quartz_cfg_$(BUILD)_cpp.xml
    @echo Determining AMSS link address...
    @cd $(L4_BASE_DIR); $(ELFWEAVER_CMD) merge ../../build/ms/quartz_cfg_$(BUILD).xml --ignore="AMSS" --lastphys="physical" >> ../../build/ms/$(CUSTL4SCLFILE)
    @echo "#endif" >> $(CUSTL4SCLFILE)
    @echo Done.
    @echo ------------------------------------------------------------------
。。。

@echo Generate AMSS Link Address file: $(CUSTL4SCLFILE).....
中的 CUSTL4SCLFILE 为： cust_l4_scl_M.h
@echo "#ifndef CUST_L4_SCL_$(BUILD_KCFG)_H" >> $(CUSTL4SCLFILE)
@echo "#define CUST_L4_SCL_$(BUILD_KCFG)_H" >> $(CUSTL4SCLFILE)
@echo -n "#define SCL_L4_START_ADDRESS " >> $(CUSTL4SCLFILE)
@echo "#endif" >> $(CUSTL4SCLFILE)
输出文件 ./AMSS/products/7x30/build/ms/cust_l4_scl_M.h ，内容为：
#ifndef CUST_L4_SCL_M_H
#define CUST_L4_SCL_M_H
#define SCL_L4_START_ADDRESS 0x4864000
#endif

/home/gphone/ARM//RVCT/Programs/2.2/593/linux-pentium/tcc -E -c --cpu ARM1136J-S --apcs /noswst/interwork --littleend --force_new_nothrow -Otime -O1 -O1 -DT_ARM -D__ARMEL__ -DCUST_H="custaabbqolym.h" -D__MSMHW_APPS_PROC__=2 -D__MSMHW_MODEM_PROC__=1 -D__MSMHW_PROC_DEF__=__MSMHW_MODEM_PROC__ -DMSMHW_MODEM_PROC -DIMAGE_MODEM_PROC -DASSERT=ASSERT_FATAL -I../../core/api/systemdrivers/hwio/msm7x30/inc -I../../core/api/systemdrivers/hwio/msm7x30/inc/plat/l4/user -I../../core/api/systemdrivers/hwio/msm7x30/inc/proc/modem -I../../apps/nonpkbrew/pk/inc/msm -I../../apps/nonpkbrew/pk/inc -I../../apps/nonpkbrew/pk/src -I../../apps/nonpkbrew/pk/src/msm -I../../apps/nonpkbrew/pk/../sdk/inc -I../../apps/brew/sdk/inc -I../../apps/brew/inc -I../../apps/brew/pk/inc/msm -I../../apps/brew/pk/inc -I../../apps/brew/pk/src -I../../apps/brew/src/OEM -I../../apps/brew/pk/src/msm -I../../apps/brew/inc/OEM -I../../apps/brew/src/OEM/msm -I../../apps/brew/src/OEM/OEMNotify/inc -I../../apps/brew/src/OEM/OEMConfigItem/inc -I../../apps/brew/src/OEM/OEMSound -I../../apps/brew/src/OEM/OEMSysClock/msm -I../../apps/brew/src/OEM/OEMShellBeep -I../../apps/brew/src/OEM/OEMServingNetworks/msm -I../../apps/brew/src/OEM/OEMAnnunciatorControl -I../../multimedia/adsp_diag -I../../apps/staticextensions/inc -I../../apps/staticextensions/oem/inc -I../../apps/staticextensions/oem/src -I../../wconnect/bthost/soccfg/inc -I../../wconnect/bthost/ftm/inc -I../../modem/uim/gstk/inc -I../../modem/uim/gstk/src -I../../modem/uim/estk/inc -I../../modem/uim/estk/src -I../../apps/DynamicExtensions/IMediaVideo -I../../modem/uim/dctstk/src -I../../core/api/hwengines -I../../multimedia/api/adsprtossvc -I../../core/securemsm/ipsec/inc -I../../core/securemsm/ipsec/src -I../../core/securemsm/akaalgo/inc -I../../core/securemsm/akaalgo/src -I../../wconnect/api/rapi/wlanhs/inc -I../../wconnect/api/wlanhs --via ../../build/ms/corebsp.inc --via ../../build/ms/M7X30AABBQOLYM/M7X30AABBQOLYM.inc -I../../modem/qchat/inc -I ../../modem/wms/src/CMCNas/inc -I/home/gphone/ARM//RVCT/Data/2.2/349/include/unix -DBUILD_ASIC="7X30A" -DBUILD_TARGET="AABBQOLYM" -DBUILD_VER="1250" -o quartz_cfg_machine_AABBQOLYM_cpp.xml quartz_cfg_machine.xml
PLATMSDIR := ../../core/kernel/build_$(BUILD_KCFG)/ms
拷贝文件
./AMSS/products/7x30/build/ms/loadsyms_M.cmm
./AMSS/products/7x30/build/ms/loadsyms_M.men
./AMSS/products/7x30/build/ms/quartz_constants_M.cmm
./AMSS/products/7x30/core/kernel/build_M/ms/
------------------------------------------------------------------
Generate AMSS Link Address file: cust_l4_scl_M.h.....
------------------------------------------------------------------
Determining AMSS link address from target XML file...
Preprocessing XML file into quartz_cfg_AABBQOLYM_cpp.xml...
Preprocessing XML file into quartz_cfg_machine_AABBQOLYM_cpp.xml...
Copying cleaned XML file to quartz_cfg_machine_AABBQOLYM.xml...
Preprocessing XML file into quartz_cfg_AABBQOLYM_cpp.xml...
Copying cleaned XML file to quartz_cfg_AABBQOLYM.xml...
Determining AMSS link address...
Done.
1.3.3.3.5　firmware
1.3.3.4　deps规则解析
1.3.3.5　corebsp_build规则解析
corebsp_build : corebsp_build_action corebsp_create_incpaths corebsp_setup
corebsp_build 规则包括三个过程：
corebsp_build_action
corebsp_create_incpaths
corebsp_setup
通过命令 . ./make-AAABQOLYM.sh corebsp_build 可以单独对 corebsp_build 部分进行编译。

1.3.3.5.1　corebsp_build_action
corebsp_build_action :
ifeq ($(USES_COREBSP_BUILD_SYSTEM), yes)
    $(warning COREBSP Build System Enabled)
    @echo ================= COREBSP Build ====================================
    @echo COREBSP Build System and AMSS Objects
    #$(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak $(MAKEFLAGS)
    $(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak
    #$(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak -rk
    @echo COREBSP AMSS Libraries
   @if test -f $(COREBSP_BUILD)/msm7x30_lib.mak; then $(MAKE) -f $(COREBSP_BUILD)/msm7x30_lib.mak corebsplibs ; fi
    @echo ================= COREBSP Build Done ================================
else
    $(warning COREBSP Build System Disabled)
    include $(COREBSP_BUILD)/dmss_objects.min
    OBJECTS := $(OBJECTS) $(COREBSP_OBJECTS)
    QCTLIBS := $(QCTLIBS) $(COREBSP_QCTLIBS)
endif
因为：
USES_COREBSP_BUILD_SYSTEM=yes
$(COREBSP_BUILD)/msm7x30_lib.mak  文件不存在
所以 corebsp_build_action  规则
执行语句：$(MAKE) -f $(COREBSP_BUILD)/msm7x30_modem.mak
...
all : corebsp_scons corebsp
1.3.3.5.1.1　corebsp_scons 规则
./AMSS/products/7x30/core/bsp/build/dmss_rules.min 中。
corebsp_scons 包括三个部分：
corebsp_scons_start  显示信息  @echo Building CoreBSP 创建目录 mkdir -p $(COREBSP_BUILD)/data
corebsp_scons_action 主体部分
corebsp_scons_done   显示信息  @echo Completed CoreBSP
1.3.3.5.1.1.1 corebsp_scons_start
2.3.3.5.1.1.1 corebsp_scons_action
corebsp_scons_action 规则在文件 ./AMSS/products/7x30/core/bsp/build/dmss_rules.min:263 中，它完成核心bsp的编译。
./AMSS/products/7x30/core/bsp/tools/emmcbld/build/emmcbld.mbn
./AMSS/products/7x30/core/bsp/tools/flash/ehostdl/build/enandprg_AABBQOLYM.mbn
./AMSS/products/7x30/build/ms/bin/AABBQOLY/adsp.mbn
./AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/AMSS_AABBQOLYM.mbn
./AMSS/products/7x30/core/bsp/tools/flash/hostdl/build/nandprg_AABBQOLYM.mbn
./AMSS/products/7x30/build/ms/bin/AABBQOLY/partition.mbn
./AMSS/products/7x30/core/bsp/bootloaders/partition/build/partition_AABBQOLYM.mbn
./AMSS/products/7x30/build/ms/bin/AABBQOLY/osbl.mbn
./AMSS/products/7x30/core/bsp/bootloaders/osbl/build/osbl_AABBQOLYM.mbn
./AMSS/products/7x30/build/ms/bin/AABBQOLY/fsbl.mbn
./AMSS/products/7x30/core/bsp/bootloaders/fsbl/build/fsbl_AABBQOLYM.mbn
./AMSS/products/7x30/build/ms/bin/AABBQOLY/dbl.mbn
./AMSS/products/7x30/core/bsp/bootloaders/dbl/build/dbl_AABBQOLYM_preamble.mbn
./AMSS/products/7x30/core/bsp/bootloaders/dbl/build/dbl_AABBQOLYM.mbn
以上各部分都是在此过程中完成。
corebsp_scons_action : corebsp_genincpaths corebsp_genuses
    @echo -------------------------------------------------------------------
    $(scons_builder)
    @echo -------------------------------------------------------------------
corebsp_genincpaths :
产生基于 AMSS 的头文件信息，通过下面命令完成。
perl $(COREBSP_BUILD)/scripts/genpaths.pl -min incpaths.min -buildid $(BUILD) -makeflags "$(MAKEFLAGS)" > $(COREBSP_BUILD)/data/incpaths$(BUILD_LC).py
$(COREBSP_BUILD)/scripts/genpaths.pl
为文件：
./AMSS/products/7x30/core/bsp/build/scripts/genpaths.pl
$(COREBSP_BUILD)/data/incpaths$(BUILD_LC).py
为文件：
./AMSS/products/7x30/core/bsp/build/data/incpathsaaabqmazm.py
corebsp_genuses：
$(scons_builder)
$(scons_builder) 的调用在文件 ./AMSS/products/7x30/core/bsp/build/dmss_rules.min:180:中
define scons_builder
        -chmod +x $(COREBSP_BUILD)/../tools/SCons/scons
        cd $(COREBSP_BUILD); \
        ../tools/SCons/scons$(SCONS_EXT) $(PLATFORM) CHIPSET=$(CHIPSET) BUILD_ID=$(BUILDID) BUILD_VER=$(VERSION) \
                MSM_ID=$(BUILD_MSM_ID) HAL_PLATFORM=$(HAL_PLAT) BUILD_ASIC=$(BUILD_ASIC) TARGET_FAMILY=$(TARGET_FAMILY) \
                --verbose=$(VERBOSE) --frommake $(MAKE_FLAGS)
endef
COREBSP_BUILD 等于 ${SRCROOT}/core/bsp/build
./AMSS/products/7x30/core/bsp/tools/SCons/scons
所以 scons_builder 实际执行了以下命令：
chmod +x ./AMSS/products/7x30/core/bsp/tools/SCons/scons
cd ./AMSS/products/7x30/core/bsp/build
./AMSS/products/7x30/core/bsp/tools/SCons/scons modem CHIPSET=msm7x30 BUILD_ID=AAABQMAZ BUILD_VER=1220 MSM_ID=7x30 HAL_PLATFORM=7x30 BUILD_ASIC=7X30A TARGET_FAMILY=7630 --verbose=0 –frommake

1.3.3.5.1.1.2.1 命令 pboot_gen_elf image_header pboot_add_hash 解析
pboot_gen_elf image_header pboot_add_hash 是编译过程中用到的几个工具
编译过程中常用的几个工具
命令 ./AMSS/products/7x30/tools/headergen/pboot_gen_elf 用法：
pboot_gen_elf [-d] elf_file scl_file output_elf output_hash
或者：
pboot_gen_elf [-d] elf_file output_hash
-d           - debug 模式
elf_file     - 格式为ELF的输入文件，它由linker生成
scl_file     - input progressive boot scatter load file
output_elf   - output ELF file
output_hash  - output hash table
例如：
PBOOT_GEN_ELF=./AMSS/products/7x30/tools/headergen/pboot_gen_elf
elf_file=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.pbn
output_hash=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.hash
$PBOOT_GEN_ELF  $elf_file $output_hash

命令 ./AMSS/products/7x30/tools/headergen/image_header  用法：
image_header flash_type header_type input_code_file output_header_file secure_type [dbl_preamble_file or elf_src_file]
flash_type         -  flash 类型：nand 或者 nor
header_type        -  头类型可以为： dbl, dbl_preamble, osbl, appsbl, apps, amsshash, hostdl, ehostdl, dsp1hash, dsp2hash
input_code_file    - 用于生成文件头的输入文件
output_header_file - 生成的文件头
secure_type        -  'secure' or 'nonsecure'
dbl_preamble_file  - 当 header_type 等于 'dbl_preamble' 的时候指向 dbl preamble 文件的路径
elf_src_file       - 当 header_type 等于 'hash' 的时候指向 elf file corresponding to hash table 的路径
例如：
=============
IMAGE_HEADER=./AMSS/products/7x30/tools/headergen/image_header
flash_type=nand
header_type=amsshash
input_code_file=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.hash
output_header_file=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.hash.hd
secure_type=nonsecure
dbl_preamble_file=
elf_src_file=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.pbn
$IMAGE_HEADER  $flash_type $header_type $input_code_file $output_header_file $secure_type $elf_src_file

命令 AMSS/products/7x30/tools/headergen/pboot_add_hash 用法：
把 hash 头和 hash 表插入到 ELF文件作为起始段 ，在物理内存中它实际在最后。
pboot_add_hash [-d] elf_file hash_table output_elf
-d            - debug 模式
elf_file      - the ELF file
hash_table    - hash table (signed or not)
output_elf    - final output
PBOOT_ADD_HASH=./AMSS/products/7x30/tools/headergen/pboot_add_hash
elf_file=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.pbn
hash_table=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.hash_nonsec.mbn
output_elf=./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AABBQMAZM.mbn
$PBOOT_ADD_HASH  $elf_file  $hash_table  $output_elf

1.3.3.5.1.1.2.2env.BinBuilder 过程解析
1.3.3.5.1.1.2.3 env.MbnBuilder 过程解析
mbn 类型的文件是我们最终烧写到板子上的镜像文件
在文件： ./AMSS/products/7x30/core/bsp/build/scripts/mbn_builder.py 中有 mbn 文件的编译规则，要注意的是，不同的 modem 源代码版本，它里面的规则有可能不一样。
1220 以及之前的版本，编译规则中有：
def mbn_generate(env):
   #-------------------------------------------------------------------------
   # MBN builder definition
   #-------------------------------------------------------------------------
   mbn_act = env.GetBuilderAction(mbn_builder)
   mbn_bld = env.Builder(action = mbn_act,
                         emitter = mbn_emitter,
                         suffix = '.mbn')
   env.Append(BUILDERS = {'MbnBuilder' : mbn_bld})

   mbn_dummy_act = env.GetBuilderAction(mbn_dummy_file_gen)
   mbn_dummy_bld = env.Builder(action = mbn_dummy_act, suffix = '.mbn')

   env.Append(BUILDERS = {'MbnDummyBuilder' : mbn_dummy_bld})
1240 后的版本为：
def mbn_generate(env):
   env.AddMethod(mbn_builder, "MbnBuilder")
   #-------------------------------------------------------------------------
   # MBN builder definition
   #-------------------------------------------------------------------------
   stage1_mbn_act = env.GetBuilderAction(stage1_mbn_builder)
   stage1_mbn_bld = env.Builder(action = stage1_mbn_act,
                         emitter = stage1_mbn_emitter,
                         suffix = '.mbn')
   env.Append(BUILDERS = {'Stage1MbnBuilder' : stage1_mbn_bld})
   stage2_mbn_act = env.GetBuilderAction(stage2_mbn_builder)
   stage2_mbn_bld = env.Builder(action = stage2_mbn_act,
                         emitter = stage2_mbn_emitter,
                         suffix = '.mbn')
   env.Append(BUILDERS = {'Stage2MbnBuilder' : stage2_mbn_bld})
   mbn_dummy_act = env.GetBuilderAction(mbn_dummy_file_gen)
   mbn_dummy_bld = env.Builder(action = mbn_dummy_act, suffix = '.mbn')
env.Append(BUILDERS = {'MbnDummyBuilder' : mbn_dummy_bld})

MbnBuilder 方法，它对应的函数为 mbn_builder， 该函数也在文件：./AMSS/products/7x30/core/bsp/build/scripts/mbn_builder.py 中：
def mbn_builder(target, source, env):
   ...
   source_base = os.path.splitext(str(source[0]))[0]
   target_base = os.path.splitext(str(target[0]))[0]
   source_full = str(source[0])
   target_full = str(target[0])
1.3.3.5.1.1.2.4 env.MbnDummyBuilder 过程解析
./AMSS/products/7x30/core/bsp/build/scripts/mbn_builder.py:160:
。。。
mbn_dummy_act = env.GetBuilderAction(mbn_dummy_file_gen)
mbn_dummy_bld = env.Builder(action = mbn_dummy_act, suffix = '.mbn')
env.Append(BUILDERS = {'MbnDummyBuilder' : mbn_dummy_bld})
。。。
def mbn_dummy_file_gen(target, source, env):
   target_str = str(target[0])
   file = open (target_str, "w")
   file.write("\nDummy file created " + target_str + "\n")
   file.close()
   return None
# 生成 fsbl elf
FSBL_elf = env.Program('${TARGET_NAME}', FSBL_objs, LIBS=FSBL_libs, LIBPATH=libs_path)
FSBL_bin = env.BinBuilder('${TARGET_NAME}', FSBL_elf)
# Generate dummy fsbl mbn
FSBL_mbn = env.MbnDummyBuilder('${TARGET_NAME}', None)
install_target_mbn = env.InstallAs('${MBN_ROOT}/fsbl.mbn', FSBL_mbn)

1.3.3.5.1.1.2.5fsbl.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/bootloaders/fsbl/build/SConscript:203:
# 生成 fsbl elf
FSBL_elf = env.Program('${TARGET_NAME}', FSBL_objs, LIBS=FSBL_libs, LIBPATH=libs_path)
# Generate fsbl bin
FSBL_bin = env.BinBuilder('${TARGET_NAME}', FSBL_elf)
# Generate fsbl mbn
#FSBL_mbn = env.MbnBuilder('${TARGET_NAME}', FSBL_bin,
#   IMAGE_TYPE="fsbl", FLASH_TYPE=env['FLASH_TYPE'])
# 生成 dummy fsbl mbn
FSBL_mbn = env.MbnDummyBuilder('${TARGET_NAME}', None)
install_target_mbn = env.InstallAs('${MBN_ROOT}/fsbl.mbn', FSBL_mbn)

1.3.3.5.1.1.2.6 dbl.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/bootloaders/dbl/build/SConscript:230:
DBL_elf = env.Program('${TARGET_NAME}',DBL_objs , LIBS=DBL_libs, LIBPATH=libs_path)
   DBL_bin = env.BinBuilder('${TARGET_NAME}', DBL_elf)
   DBL_mbn = env.MbnBuilder(env.subst('${TARGET_NAME}'), DBL_bin, IMAGE_TYPE="dbl", FLASH_TYPE="nand")
1.3.3.5.1.1.2.7 AMSS_AABBQOLYM.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:252:
target_mbn = env.MbnBuilder('${TARGET_NAME}', target_pbn, IMAGE_TYPE="amss_mbn",
1.3.3.5.1.1.2.8 adsp.mbn 生成过程解析
生成 adsp.mbn 的 scons 脚本为：
./AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/SConscript
   adsp_bin_path = env.subst("${BUILD_ROOT}/multimedia/adspinfo/adsp.bin")
   adsp_mbn_path = env.subst("${BUILD_ROOT}/core/bsp/amsslibs/build/qdsp5/adsp.mbn")
   install_temp_adsp_mbn = env.InstallAs(adsp_mbn_path, adsp_bin_path)
   ADSP_mbn = env.MbnBuilder('${TARGET_NAME}', adsp_mbn_path, IMAGE_TYPE="qdsp5", FLASH_TYPE=env['TARGET_FLASH_TYPE'])
   install_adsp_mbn = env.InstallAs('${MBN_ROOT}/adsp.mbn', ADSP_mbn)
首先安装文件：
AMSS/products/7x30/multimedia/adspinfo/adsp.bin
到：
AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/adsp.mbn

此时的 adsp.mbn 仅仅是文件 adsp.bin 的拷贝。
然后调用 env.MbnBuilder 由文件 AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/adsp.mbn生成:./AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/AMSS_AABBQOLYM.mbn
AMSS_AABBQOLYM.mbn 是 adsp.mbn 加了40个字节的头信息，最后安装此 AMSS_AABBQOLYM.mbn 到
AMSS/products/7x30/build/ms/bin/AABBQOLY/adsp.mbn
其中 env.MbnBuilder 执行的是文件：
./AMSS/products/7x30/core/bsp/build/scripts/mbn_builder.py:299 中的函数：
def mbn_builder(target, source, env)
AMSS/products/7x30/tools/headergen/image_header nand adspq5 AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/adsp.mbn AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/AMSS_AABBQOLYM.hd nonsecure

flash_type: nand
header_type: adspq5
code_file_name: /home/shared/qualcommon/HY11-N1496-2_1.2.40/AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/adsp.mbn
output_file_name: /home/shared/qualcommon/HY11-N1496-2_1.2.40/AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/AMSS_AABBQOLYM.hd
secure_type: nonsecure
image_size = 0x614AB4, code_size= =0x614AB4

1.3.3.5.1.1.2.9osbl.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/bootloaders/osbl/build/SConscript:246:
OSBL_mbn = env.MbnBuilder('${TARGET_NAME}', OSBL_bin,
1.3.3.5.1.1.2.10enandprg_AABBQOLYM.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/tools/flash/ehostdl/build/SConscript:389:
# Generate enandprg.elf
enandprg_elf = env.Program('${TARGET_NAME}', source=[], LIBS=libs, LIBPATH=libs_path)
env.Depends(enandprg_elf, target_scl)
Clean(enandprg_elf, env.subst('${TARGET_NAME}.map'))
Clean(enandprg_elf, env.subst('${TARGET_NAME}.sym'))

# 把生成的 elf 文件拷贝到相应位置
install_enandprg_elf = env.Install("${BUILD_ROOT}/core/storage/flash/tools/src/hostdl",enandprg_elf)
# Generate ehostdl bin
target_bin = env.BinBuilder('${TARGET_NAME}', enandprg_elf)
# 生成 ehostdl mbn
target_mbn = env.MbnBuilder('${TARGET_NAME}', target_bin, IMAGE_TYPE="ehostdl", FLASH_TYPE="nand")
1.3.3.5.1.1.2.11 nandprg_AABBQOLYM.mbn 生成过程解析
./AMSS/products/7x30/core/bsp/tools/flash/hostdl/build/SConscript:401:
# Generate nandprg.elf
nandprg_elf = env.Program('${TARGET_NAME}', source=[], LIBS=libs, LIBPATH=libs_path)
env.Depends(nandprg_elf, target_scl)
Clean(nandprg_elf, env.subst('${TARGET_NAME}.map'))
Clean(nandprg_elf, env.subst('${TARGET_NAME}.sym'))
# 把生成的 elf 文件拷贝到相应位置
install_nandprg_elf = env.Install("${BUILD_ROOT}/core/storage/flash/tools/src/hostdl", nandprg_elf)
# Generate hostdl bin
target_bin = env.BinBuilder('${TARGET_NAME}', nandprg_elf)
# Generate hostdl mbn
target_mbn = env.MbnBuilder('${TARGET_NAME}', target_bin,IMAGE_TYPE="hostdl", FLASH_TYPE="nand")
# Generate nandprg.hex
nprg_hex = env.NPRGHexBuilder('NPRG${MSM_ID}_${BUILD_ID}', target_mbn,HOSTDL_ADDR=CODE_HDR_ADDR)
install_nandprg_hex = env.InstallAs('${MBN_ROOT}/NPRG${MSM_ID}.hex', nprg_hex)

1.3.3.5.1.1.2.12 emmcbld.mbn 生成过程解析
生成 emmcbld.mbn 的 scons 脚本为：
./AMSS/products/7x30/core/bsp/tools/emmcbld/build/SConscript:289:
libs = root_env['LIBS']
libs_path = env['LIBPATH']
# Source PATH
EMMCBLD_SRC = "${BUILD_ROOT}/core/storage/tools/emmcbld"
。。。
env.VariantDir('.', EMMCBLD_SRC, duplicate=0)
env.Replace(TARGET_NAME = 'emmcbld')
env.Replace(TARGET_BLD_KCFG = 'M')
env.Replace(TARGET_IMAGE = 'MODEM_PROC')
# 生成 scatter load 文件
target_scl = env.SclBuilder('${TARGET_NAME}', '${BUILD_ROOT}/core/storage/tools/emmcbld/emmcbld_in.scl')
# 生成 emmcbld elf 文件
target_elf = env.Program('${TARGET_NAME}', emmcbld_obj, LIBS=libs, LIBPATH=libs_path)
env.Depends(target_elf, target_scl)
Clean(target_elf, env.subst('${TARGET_NAME}.map'))
Clean(target_elf, env.subst('${TARGET_NAME}.sym'))
# 生成 Generate emmcbld bin 文件
target_bin = env.BinBuilder('${TARGET_NAME}', target_elf)

# 生成 Generate emmcbld mbn 文件

target_mbn = env.MbnBuilder('${TARGET_NAME}', target_bin, IMAGE_TYPE="emmcbld", FLASH_TYPE="nand")

# 生成 emmcbld.hex 文件

mprg_hex = env.NPRGHexBuilder('MPRG${MSM_ID}', target_mbn, HOSTDL_ADDR=CODE_HDR_ADDR)

install_emmcbld_hex = env.InstallAs('${MBN_ROOT}/MPRG${MSM_ID}.hex', mprg_hex)

通过语句：

target_elf = env.Program('${TARGET_NAME}', emmcbld_obj, LIBS=libs, LIBPATH=libs_path) 编译 emmcbld 相关代码生成 ./AMSS/products/7x30/core/bsp/tools/emmcbld/build/emmcbld.elf        LIBS 为程序需要引入的库，库的路径为 LIBPATH 例如：

如果程序引入了其它的库，库名为libbar.a，库的目录为/usr/local/lib:

Program('hello',Glob("*.c"),LIBS='bar',LIBPATH='/usr/local/lib')

env.Depends(target_elf, target_scl)明确以来关系，表明生成 mmcbld.elf 需要 emmcbld.scl

Clean(target_elf, env.subst('${TARGET_NAME}.map'))
Clean(target_elf, env.subst('${TARGET_NAME}.sym'))
Clean 调用的是文件：
./AMSS/products/7x30/core/bsp/tools/SCons/Environment.py:1814:中的函数
def Clean(self, targets, files):
flash_type: nand
header_type: emmcbld
code_file_name: /home/shared/HY11-N1496-3_1.2.50/AMSS/products/7x30/core/bsp/tools/emmcbld/build/emmcbld.bin
output_file_name: /home/shared/HY11-N1496-3_1.2.50/AMSS/products/7x30/core/bsp/tools/emmcbld/build/emmcbld.hd
secure_type: nonsecure
image_size = 0x23E88, code_size= =0x23E88

=== Generating tool/emmcbld/MPRG7x30.hex

Install file: "/home/shared/HY11-N1496-3_1.2.50/AMSS/products/7x30/core/bsp/tools/emmcbld/build/MPRG7x30.hex" as "/home/shared/HY11-N1496-3_1.2.50/AMSS/products/7x30/build/ms/bin/AABBQOLY/MPRG7x30.hex"



3.3.3.5.1.1.1 corebsp_scons_done

1.3.3.5.1.2　corebsp

1.3.3.5.2　corebsp_create_incpaths

1.3.3.5.3　corebsp_setup

1.3.3.6　libs

.PHONY: setup

setup : corebsp_create_incpaths create_incpaths amsssetup amsslinkaddress firmware

...

#生成 Library

ifneq ($(USES_OBJECT_ONLY), yes)

libs : $(LIBDIR)/exist copybar firmware prereqs $(LIBRARIES)

    @if test -f dmss_lib.mak; then $(MAKE) --no-print-directory -f dmss_lib.mak; fi

else

libs : $(LIBDIR)/exist copybar $(LIBRARIES)

    @if test -f dmss_lib.mak; then $(MAKE) --no-print-directory -f dmss_lib.mak; fi

endif

...

ifeq ($(IMAGE), APPS_PROC)

$(TARGETDIR)/exist $(LIBDIR)/exist $(MBNPATH)/exist:

else

$(TARGETDIR)/exist $(LIBDIR)/exist:

endif

    @echo ---------------------------------------------------------------

    @echo Creating path for ${@D}

    @echo

    @if test ! -f $@ && test ! -d ${@D}; then mkdir -p ${@D}; mkdir -p ${@D}; fi

    @echo Building ${@D} > $@

    @echo ---------------------------------------------------------------

...

1.3.3.7　copybar规则解析

1.3.3.8　exe规则解析

1.3.3.8.1　CORELIBS_AABBQOLYM.mbn生成过程解析

1.3.3.8.2　amss.mbn 生成过程解析

   # Create PBN, MBN, etc..

   install_target_reloc = env.InstallAs(AMSS_RELOC, target_elf)

   # Build env pbn files

   target_pbn = env.PbnBuilder('${TARGET_NAME}', ["${BUILD_MS_ROOT}/quartz_cfg_${BUILD_ID}",install_target_reloc])

   install_target_pbn = env.InstallAs(BOOTING_PBN, target_pbn)

   # Build CMM scripts

   quartz_constant_cmm = env.QuartzConstBuilder('quartz_const_${TARGET_NAME}', ["${L4_ROOT}/build_${TARGET_BLD_KCFG}/ms/quartz_constants_${TARGET_BLD_KCFG}.cmm",install_target_pbn,"${BUILD_MS_ROOT}/quartz_cfg_${BUILD_ID}.xml",])

   # install scripts

   install_quartz_constant_cmm = env.InstallAs("${BUILD_MS_ROOT}/quartz_constants_${TARGET_BLD_KCFG}.cmm",quartz_constant_cmm)

   install_loadsyms_cmm = env.Install("${BUILD_MS_ROOT}","${L4_ROOT}/build_${TARGET_BLD_KCFG}/ms/loadsyms_${TARGET_BLD_KCFG}.cmm",)

   install_loadsyms_men = env.Install("${BUILD_MS_ROOT}","${L4_ROOT}/build_${TARGET_BLD_KCFG}/ms/loadsyms_${TARGET_BLD_KCFG}.men",)

   install_cmm_scripts = [install_quartz_constant_cmm,install_loadsyms_cmm,install_loadsyms_men,]

   # Build env mbn files

   target_mbn = env.MbnBuilder('${TARGET_NAME}', target_pbn, IMAGE_TYPE="amss_mbn",FLASH_TYPE=env['TARGET_FLASH_TYPE'])





1.3.3.9　bldprod规则解析

1.3.3.10　create_mem_feat_html规则解析

此规则在 文件中

./AMSS/products/7x30/core/bsp/build/SConstruct:22:print "

Loading CBSP build system"

./AMSS/products/7x30/core/bsp/coreimg/build/SConscript:24:

print "   Loading CBSP Image build system"

./AMSS/products/7x30/core/bsp/tools/flash/build/SConstruct:31:

print "   Loading CBSP build system"

./AMSS/products/7x30/core/bsp/tools/boot/build/SConstruct:31:

print "   Loading CBSP build system"

./AMSS/products/7x30/core/bsp/bootloaders/build/SConstruct:31:

print "   Loading CBSP build system"



perl ./AMSS/products/7x30/build/ms/cleanup_xml.pl -i



./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/quartz_cfg_AAABQMAZM.xml

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/quartz_cfg_machine_AAABQMAZM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_AABBQMAZM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_AABBQOLYM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_AAABQMAZM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_machine_AABBQOLYM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_machine_AAABQMAZM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_machine_AABBQMAZM.xml

./AMSS/products/7x30/build/ms/quartz_cfg_machine.xml



./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AAABQMAZM.mbn

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/CORELIBS_AAABQMAZM.hash_nonsec.mbn





./AMSS/products/7x30/core/bsp/build/scripts/mbn_builder.py

      if env.subst("$IMAGE_TYPE") != "amss_mbn":

         target_bin_nonsec = os.path.join(target_bin_base, target_bin_dir, \

            str(os.path.split(target_base)[1]) + "_nonsec.mbn")

      else:

         target_bin_nonsec = os.path.join(target_bin_base, target_bin_dir, \

            str(os.path.split(target_base)[1]) + ".hash_nonsec.mbn")









    quartz_xml = env.QuartzXMLBuilder('quartz_cfg_${TARGET_NAME}',

         ['${BUILD_MS_ROOT}/quartz_cfg.xml', quartz_machine_xml],

      #-------------------------------------------------------------------------

      # Build env pbn files

      #-------------------------------------------------------------------------

      target_pbn = env.PbnBuilder(

         '${TARGET_NAME}', [quartz_xml, install_target_reloc])



      # Copy qcoreimg_M.pbn to platform\l4\build_M\bootimg.pbn for RUMI */

      install_target_pbn = env.InstallAs(

         '${L4_ROOT}/build_${TARGET_BLD_KCFG}/bootimg.pbn', target_pbn)





      target_mbn = env.MbnBuilder('${TARGET_NAME}', target_pbn, IMAGE_TYPE="amss_mbn",

         FLASH_TYPE=env['TARGET_FLASH_TYPE'])



      install_target_mbn = env.InstallAs('${MBN_ROOT}/amss.mbn', target_mbn)







因为有：

ifeq ($(USES_HW6500),yes)

    BOOT = $(SRCROOT)/drivers/boot



else

    ifeq ($(IMAGE), APPS_PROC)

        BOOT = $(SRCROOT)/core/api/boot/appsproc/

        APPSBL = $(BOOT)

    else

        BOOT=$(SRCROOT)/core/api/boot/

    BOOT +=$(SRCROOT)/core/api/boot/amssboot/

    endif



endif

且我们定义 APPS_MODEM ，所以使用的是：

BOOT=$(SRCROOT)/core/api/boot/

BOOT +=$(SRCROOT)/core/api/boot/amssboot/



boot_reset_handler.s 使用的是：

./AMSS/products/7x30/core/boot/amssboot/shared/src/boot_reset_handler.s

...

EXPORT  Reset_Handler

EXPORT  soft_breakpoints_enabled

Reset_Handler

    ;------------------------------------------------------------------

    ; process init - initializes the AMSS process

    ; returns the number of bytes to pop off the stack

    ;------------------------------------------------------------------

    ldr     r0, [sp]

    blx     process_init

    add     sp, sp, r0



soft_breakpoints_enabled ; All pages of apps code have been paged in.

    ;------------------------------------------------------------------

    ; Pop argc and argv and launch into main.  If main is compiled with

    ; thumb mode, blx will ensure that we properly change to thumb mode

    ;------------------------------------------------------------------

    ldmia   sp!, {r0}

    mov     r1, sp

    blx     main

...





./AMSS/products/7x30/core/boot/amssboot/shared/src/boot_vectors.s:49:        IMPORT  Reset_Handler

./AMSS/products/7x30/core/boot/amssboot/shared/src/boot_vectors.s:102:        DCD       Reset_Handler









./AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/SConscript:102:   install_temp_adsp_mbn = env.InstallAs(adsp_mbn_path, adsp_bin_path)

./AMSS/products/7x30/core/bsp/amsslibs/build/qdsp5/SConscript:108:   install_adsp_mbn = env.InstallAs('${MBN_ROOT}/adsp.mbn', ADSP_mbn)

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:193:   install_target_reloc = env.InstallAs(AMSS_RELOC, target_elf)

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:203:   install_target_pbn = env.InstallAs(BOOTING_PBN, target_pbn)

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:215:   install_quartz_constant_cmm = env.InstallAs(

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:261:   install_target_mbn = env.InstallAs("${MBN_ROOT}/amss.mbn", target_mbn)

./AMSS/products/7x30/core/bsp/coreimg/build/arm11/CoreImage.py:240:      install_target_mbn = env.InstallAs('${MBN_ROOT}/amss.mbn', target_mbn)

1.3.3.10.1　

1.3.3.11　partition规则解析

1.3.3.11.1　eMCC 启动和 NAND 启动的分区格式是不一样的，如果是 eMCC 启动，多了编译选项：

USES_SDCC_BOOT=yes USES_HSU_MS_FD_BOOT=yes

如果 USES_HSU_MS_FD_BOOT=yes

=== Compiling  boot/osbl/fd_storage_scsi.c

=== Compiling  boot/osbl/fd_storage_usb.c

如果 USES_SDCC_BOOT=yes

那么会进行 partition 规则的处理,它会覆盖掉 corebsp_build 规则阶段生成的 partition.mbn

./AMSS/products/7x30/build/ms/dmss_rules.min

-----------

   。。。

partition :

    @echo Generate partition.bin.....

    @$(GENERATE_PARTITION)

   。。。

不同 modem 版本中，GENERATE_PARTITION 的定义会不同

HY11-N0723-1_1.2.00  中为：

-------

define GENERATE_PARTITION

        @echo ------------------------------------------------------------------

        @echo Changing to Tools directory...

        @pwd

        cd $(TOOLSDIR_JSDCC); make; make genpart

endef

-------

HY11-N1496-2_1.2.40 中为：

-------

define GENERATE_PARTITION

        @echo ------------------------------------------------------------------

        @echo Changing to Tools directory...

        @pwd

        cd $(TOOLSDIR_JSDCC); make; make genpart

        perl $(TOOLSDIR_JSDCC)/partition_extract_mbr.pl 2 0 $(TOOLSDIR_JSDCC)/

        rm -f $(MBNPATH)/partition.mbn

        rm -f $(MBNPATH)/ENPRG7x30.hex

        cp -f $(TOOLSDIR_JSDCC)/partition.mbn $(MBNPATH)/

        cp -f $(MBNPATH)/MPRG7x30.hex $(MBNPATH)/ENPRG7x30.hex

        cp -f $(MBNPATH)/MPRG7x30.hex $(MBNPATH)/NPRG7x30.hex

endef

------

partition 规则主要完成以下功能：

1) 编译 ./AMSS/products/7x30/core/storage/tools/jsdcc/partition_load_pt 代码，生成 loadpt 和 msp

2) 解析 loadpt 解析 partition.xml 文件，把分区信息保存成二进制文件 partition.bin

3) 由 partition.bin 文件生成 partition.mbn,然后拷贝到 ./AMSS/products/7x30/build/ms/bin/AAABQMAZ/partition.mbn

-----

cd $(TOOLSDIR_JSDCC); make; make genpart 命令会到目录：

./AMSS/products/7x30/core/storage/tools/jsdcc/ 下执行 make; make genpart

因为有：

./AMSS/products/7x30/core/storage/tools/jsdcc/partition_load_pt/Makefile

   。。。

all:   $(TARGET_LOADPT)$(EXE) $(TARGET_MSP)$(EXE)

genpart:

    ./$(TARGET_LOADPT)$(EXE)

   。。。

所以 make 命令会生成 loadpt 和 mps

make genpart 执行 loadpt 命令



-------------

loadpt 命令解析 partition.xml 文件分析：

fp_input = fopen("partition.xml", "rt"); 打开文件：

AMSS/products/7x30/core/storage/tools/jsdcc/partition_load_pt/partition.xml



#define INPUT_BUFFER_SIZE     10240

input_buf = malloc(INPUT_BUFFER_SIZE);

申请一个大小为 10240的缓冲区，对 partition.xml 文件内容进行解析，然后以串的形式保存在变量: input_buf 中。



fp_bin_out = fopen("partition.bin", "wb");

生成一个 partition.bin 文件，用于保存二进制的分区信息



parse_element(input_buf, LEVEL_IMAGE);

对每个分区描述符进行解析，然后保存到文件 partition.bin 中。

---------------

命令 perl $(TOOLSDIR_JSDCC)/partition_extract_mbr.pl 2 0 $(TOOLSDIR_JSDCC)/ 会生成文件：

./AMSS/products/7x30/core/storage/tools/jsdcc/partition_load_pt/partition.mbn

---------------

./AMSS/products/7x30/core/bsp/tools/emmcbld/build/MPRG7x30.hex

./AMSS/products/7x30/build/ms/bin/AABBQOLY/MPRG7x30.hex



在 HY11-N0216-4_1.2.20 版本中不存在文件

./AMSS/products/7x30/core/bsp/tools/emmcbld/build/MPRG7x30.hex



高通平台 7630 启动流程分析
开机后，首先从rom中的pbl开始执行，pbl装载dbl
pbl是固化在高通芯片中的一段程序，没有相应的源代码。
dbl是从其__main 函数开始执行，此函数在汇编文件 dbl.s 中。
./AMSS/products/7x30/core/boot/secboot2/dbl/shared/src/dbl.s
__main
_main
    ...
    ldr    r5, =dbl_main_ctl
    blx    r5
    ...
通过 dbl_main_ctl调用会进入c函数， dbl_main_ctl 函数在文件：

./AMSS/products/7x30/core/boot/secboot2/dbl/shared/src/dbl_mc.c:282中：

void dbl_main_ctl(boot_pbl_shared_data_type *pbl_shared)

    ...

    调用表 dbl_init_func_tbl 中的每个函数

    for ( i=0; dbl_init_func_tbl[i] != NULL; i++ )

    {

        dbl_init_func_tbl[i](&dbl_shared_data);

    }

    ...

    #经过一系列的初始化，dbl 会把控制权传递给 osbl

    dbl_shared_data.entry_ptr(dbl_shared_data.dbl_data_for_osbl);

    ...



dbl_init_func_tbl 表在文件：

./AMSS/products/7x30/core/boot/secboot2/dbl/shared/src/dbl_mc.c:122 中，其中的函数：

dbl_load_osbl_image 会完成 osbl 的装载

dbl_load_osbl_image 函数在文件：

./AMSS/products/7x30/core/boot/secboot2/dbl/shared/src/dbl_loader.c:322 中:

void dbl_load_osbl_image(dbl_shared_data_type *shared_data)

{

  dbl_error_type ret_value = DBL_ERR_NONE;

  ret_value = dbl_load_image(shared_data, OSBL_IMG);

  DBL_VERIFY((ret_value == DBL_ERR_NONE),  (uint32)ret_value);

} /* dbl_load_osbl_image */



dbl_load_osbl_image

    ret_value = dbl_load_image(shared_data, OSBL_IMG);

        shared_data->dbl_state = DBL_STATE_LOAD_OSBL_IMG;

        ret_value = dbl_load_hdr_and_image(  shared_data,img_type,MIBIB_OSBL_PARTI_NAME,MI_OSBL_MAGIC1,MI_OSBL_MAGIC2);

           shared_data->entry_ptr      = (void (*)(void*))(image_hdr.image_dest_ptr);



dbl_shared_data.entry_ptr(dbl_shared_data.dbl_data_for_osbl)

在调用 dbl_shared_data.entry_ptr 之前，已经对该函数指针进行了初始化：

shared_data->entry_ptr      = (void (*)(void*))(image_hdr.image_dest_ptr);

它实际上是指向 osbl 镜像的起始地址，所以执行之后，系统进入 osbl 阶段

osbl流程

osbl的入口函数 __main 在汇编文件：

./AMSS/products/7x30/core/boot/secboot2/osbl/shared/src/osbl.s:86: 中

    ...

    IMPORT osbl_main_ctl

    ...

    AREA    OSBL_ENTRY, CODE, READONLY

    CODE32

    ENTRY

__main

_main

    ...

    ldr    r5, =osbl_main_ctl

    blx    r5

    ...



__main 函数会调用 osbl_main_ctl ，此函数在文件：

./AMSS/products/7x30/core/boot/secboot2/osbl/shared/src/osbl_mc.c:234 中。

void osbl_main_ctl(boot_dbl_if_shared_info_type *dbl_shared_info_ptr)

{

    ...

    #Process the target-dependent OSBL procedures

    osbl_do_procedures( &bl_shared_data,osbl_main_procs );

    ...

    //把控制权给 AMSS

    bl_shared_data.amss_entry_ptr();
}

在此阶段，osbl 通过 osbl_main_procs 中定义的 osbl_load_appsbl 函数把应用程序的控制权交给了android 系统，modem 端的控制权通过 bl_shared_data.amss_entry_ptr() 交割 AMSS，他们分别在两个处理器上同时运行，两个处理期间通过 smd 进行通信。

osbl_main_procs 在文件：

./AMSS/products/7x30/core/boot/secboot2/osbl/target/msm7x30/src/osbl_mc_target.c:524中，它定义了一些初始化函数：

osbl_procedure_func_type osbl_main_procs[] =

{

   ...

   //初始化 迷你usb 充电硬件

   osbl_hw_init,

   ...

   //在 osbl 阶段会提升系统时钟

   #ifndef RUMIBUILD

    osbl_increase_clk_speed,

   #endif

   ...

   //初始化 osbl 模并且锁住接口



   osbl_init_modules,

   ...

   初始化 flash 设备

   osbl_flash_init,

   ...

   //检测是否通过sd卡更新镜像文件

   osbl_sd_image_upgrade,

   ...

   //初始化数据结构，以便装载 AMSS 镜像

   osbl_init_amss_image,

   ...

#ifdef FEATURE_FOTA

  /*-----------------------------------------------------------------------

   * Initialize FOTA

   *-----------------------------------------------------------------------*/

  osbl_fota_init,

#endif /* FEATURE_FOTA */

   //amss 镜像进行授权鉴定

   osbl_auth_amss_image,

   //如果有 adsp 那么进行相应处理

#ifdef FEATURE_OSBL_LOAD_AND_BOOT_ADSP

   //装载 adsp 镜像

  osbl_load_adsp,

  //授权

  osbl_auth_adsp,

#endif



#ifdef FEATURE_SDCC_BOOT

   //装载 amss 镜像

   osbl_load_amss_image,

   ...

#endif



#ifndef FEATURE_STANDALONE_MODEM

   //从flash 设备装载 appsboot

   osbl_load_appsbl,

   ...

   //从flash中装载 OS 镜像

   * Load the OS image from flash

   osbl_load_apps_os,

   //引导 aARM 处理器

   osbl_boot_aarm,

#endif /* FEATURE_STANDALONE_MODEM */

  对于 nand 启动，AMSS 应该在 apps 引导以后再装载

#ifndef FEATURE_SDCC_BOOT

   // nand 启动，装载 amss 镜像

   osbl_load_amss_image,

#endif

   ...

};



osbl装载 appsbl过程分析
osbl 通过 osbl_load_appsbl函数装载应用程序的 boot loader到指定的 RAM 中

osbl_load_appsbl 在文件：

./AMSS/products/7x30/core/boot/secboot2/osbl/shared/src/osbl_aarm_boot.c:119 中

void osbl_load_appsbl( bl_shared_data_type *bl_shared_data )

{

...

/*从 flash 中装载 appsbl 头和镜像到 RAM 中*/

    osbl_load_header_and_image_with_cookie( APPSBL_IMG, &appsbl_image_header_with_cookie );

...

/*如果 boot loader 没有装载到地址 0x0, 那么将会拷贝 appsbl 向量表到地址 0x0, appsbl 镜像的向量表总是在在镜像的头 64字节中*/



  if ( apps_image_start_addr != 0x0 )

  {

/* 为 L4 with kernel entry 初始化复位向量，其他的向量赋值为 无限循环*/

复位向量的初始化值与 appsbl 镜像起始地址有关，复位的时候此值会被放入到 PC 寄存器

向量 0x00000020 中保存 appsbl 镜像的起始地址

向量 0x00000000 中的值为 0xE59FF018

    address 0x00000000:     ldr pc, [pc, #0x18]

address 0x00000020:     apps_image_start_addr */



    uint32 *vector_ptr = (uint32*) 0x00000020;

    *vector_ptr = apps_image_start_addr;

    for ( --vector_ptr; vector_ptr; vector_ptr-- )

    {

      *vector_ptr = 0xEAFFFFFE;         /* Infinite loop */

    }

    *vector_ptr  = 0xE59FF018;          /* ldr pc, [pc, #0x18] */

  }

} /* osbl_load_appsbl() */

osbl_load_header_and_image_with_cookie 函数完成 appsbl 镜像头和镜像的装载，它首先会尝试从 mibi 中装载 appsbl 镜像头，如果当前是 nor non-partition table 设备，那么装载将会成功，如果不成功，那么认为 appsbl 头位于它分区的起始位置，将其读出。



1.4.5 appsbl 流程（源代码在 android中）
appsbl 是 applications ARM boot loader 的简称，不同的软件框架，此分区来自不同的源代码
在android 系统中  appsbl 代码为 bootable/bootloader/lk
brew 框架的系统中 appsbl 代码在 ./AMSS/products/7x30/core/boot/appsproc/target/msm7x30/src
brew 框架系统，入口函数 __main -> appsbl_main_ctl 在文件：

./AMSS/products/7x30/core/boot/appsproc/target/msm7x30/src/appsbl_handler.s 中。
android 系统中有：
vendor/qcom/msm7630_surf/AndroidBoard.mk:25:
include bootable/bootloader/lk/AndroidBoot.mk
bootable/bootloader/lk/arch/arm/system-onesegment.ld

system-onesegment.ld 中 ENTRY(_start) 指定了 appsbl 分区从 _start 函数开始执行，此函数在文件 ./bootable/bootloader/lk/arch/arm/crt0.S:25 中：

...

.text

.globl _start

_start:

    b   reset

   ...

   bl      kmain

   ...

kmain 会跳转到 C 文件中执行，此函数文件

bootable/bootloader/lk/kernel/main.c 中：

void kmain(void)

    thread_resume(thread_create("bootstrap2", &bootstrap2, NULL, DEFAULT_PRIORITY, DEFAULT_STACK_SIZE));

static int bootstrap2(void *arg)

{

    arch_init();

    platform_init();

    target_init();

    apps_init();

    return 0;

}



apps_init 在文件：bootable/bootloader/lk/app/app.c:33 中：

/* one time setup */

void apps_init(void)

{

    const struct app_descriptor *app;



    /* call all the init routines */

    for (app = &__apps_start; app != &__apps_end; app++) {

       if (app->init)

           app->init(app);

    }



    /* start any that want to start on boot */

    for (app = &__apps_start; app != &__apps_end; app++) {

       if (app->entry && (app->flags & APP_FLAG_DONT_START_ON_BOOT) == 0) {

           start_app(app);
       }
    }
}

因为：bootable/bootloader/lk/include/app.h:45 中有：
#define APP_START(appname) struct app_descriptor _app_##appname __SECTION(".apps") = { .name = #appname,
#define APP_END };
bootable/bootloader/lk/app/aboot/aboot.c 中有：
APP_START(aboot)
    .init = aboot_init,
APP_END
所以有：

struct app_descriptor _app_aboot __SECTION(".apps") =
{
      .name = aboot,
      .init = aboot_init,
};



所以 apps_init 函数中的： app->init 实际上调用的是文件：

bootable/bootloader/lk/app/aboot/aboot.c:398 中的函数 aboot_init



1.4.5.1　aboot_init 过程分析（需要侧重关心的部分）

aboot_init 阶段我们应该关心 fastboot 模式 和 appsbl 对 android 系统的引导过程。

void aboot_init(const struct app_descriptor *app)
{
        unsigned reboot_mode = 0;
    page_size = flash_page_size();
    page_mask = page_size - 1;
    if (keys_get_state(KEY_HOME) != 0)
            boot_into_recovery = 1;
    if (keys_get_state(KEY_BACK) != 0)
       goto fastboot;
    if (keys_get_state(KEY_CLEAR) != 0)
       goto fastboot;

    reboot_mode = check_reboot_mode();
        if (reboot_mode == RECOVERY_MODE){
            boot_into_recovery = 1;
        }else if(reboot_mode == FASTBOOT_MODE){
            goto fastboot;
        }

recovery_init();
    //从falsh上读取 linux 然后引导
    boot_linux_from_flash();
    dprintf(CRITICAL, "ERROR: Could not do normal boot. Reverting "
       "to fastboot mode.\n");

fastboot:
        display_init();

    dprintf(INFO, "Diplay initialized\n");

    udc_init(&surf_udc_device);
    fastboot_register("boot", cmd_boot);
    fastboot_register("erase:", cmd_erase);
    fastboot_register("flash:", cmd_flash);
    fastboot_register("continue", cmd_continue);
    fastboot_register("reboot", cmd_reboot);
    fastboot_register("reboot-bootloader", cmd_reboot_bootloader);
    fastboot_publish("product", "swordfish");
    fastboot_publish("kernel", "lk");
    fastboot_init((void*) SCRATCH_ADDR, 100 * 1024 * 1024);
    udc_start();
}

recovery_init() 调用的是文件 bootable/bootloader/lk/app/aboot/recovery.c中的函数：
/* Read and write the bootloader command from the "misc" partition.
 * These return zero on success.
 */
从 misc 分区读取

struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[1024];
};



/* Read and write the bootloader command from the "misc" partition.

 * These return zero on success.

 */

int get_bootloader_message(struct bootloader_message *out);

int set_bootloader_message(const struct bootloader_message *in);



/* Write an update to the cache partition for update-radio or update-hboot.

 * Note, this destroys any filesystem on the cache partition!

 * The expected bitmap format is 240x320, 16bpp (2Bpp), RGB 5:6:5.

 */

int write_update_for_bootloader(
        const char *update, int update_len,
        int bitmap_width, int bitmap_height, int bitmap_bpp,
        const char *busy_bitmap, const char *error_bitmap);

ROM上又分为几个分区:boot, system, recovery, cache, userdata和misc。在boot上存着bootloader和Linux Kernel，system上面是Android系统，userdata包含登录信息／个人软件和设置等。recovery区里放着的是如何控制系统恢复的工具，如Google官方的stock恢复器。系统开机的时候，bootloader会首先检查硬件配置，然后将Radio Firmware写入RAM，接着调进kernel，最后再装载Android OS进入桌面

misc - misc分区，一般不会去动它，和我们没有太大关系
recovery -recovery分区
boot - 存储linux系统内核的分区,包括 kernel 和 ramdisk 两部分
system -系统分区，储存着Android系统的数据，刷 ROM 主要刷的是这个
cache -缓存分区，刷ROM时最好一起擦一下
Dalvik-cache- 这个是Android使用的Java虚拟机的缓存分区，刷ROM时最好一起擦一下
userdata - 用户自己使用的数据部分，存储我们自己的软件设置等等，恢复出厂设置的时候，这个分区会被格式化

还有Radio，这个部分通常负责无线信号，可以理解为手机信号收发部分的驱动，不过N1的视频编码器也集成在这里面，Radio可以随便刷，不过某些特定的ROM可能会指定Radio版本。
最后是SPL/Bootloader，这个我们一般不会用到它也不会修改它，除了第一次解锁的时候（不过解锁信息储存在哪里，现在xda上也没有定论，似乎不是在SPL里面，这个大致相当于电脑上的 BIOS，负责整个手机的最底层引导，坏了可能导致变砖。

recovery 分区的 init.rc 文件比较简单
./out/target/product/msm7630_surf/recovery/root/init.rc
service recovery /sbin/recovery
service adbd /sbin/adbd recovery
on property:persist.service.adb.enable=1
    start adbd
on property:persist.service.adb.enable=0
    stop adbd

它只启动了 /sbin/recovery /sbin/adbd

Recovery 可执行程序的mian函数在

bootable/recovery/recovery.c
    ui_init(); 初始化 recovery 模式的界面
    get_args(&argc, &argv); 获取按键选择

prompt_and_wait()

install_package()

    handle_update_package()

        try_update_binary(path, zip)

           handle_firmware_update(firmware_type, firmware_filename, zip)

              remember_firmware_update(type, data, data_size)

                  update_type = type;

                  update_data = data;

                  update_length = length;
maybe_install_firmware_update(send_intent);

write_update_for_bootloader() 写入数据

snprintf(boot.command, sizeof(boot.command), "update-%s", update_type);

set_bootloader_message(&boot)

才是真正的更新 radio.img





1、下载最新的CursorSense的ROM：CursorSense32A-Mod-0.9.1.1-signed，和要求的RADIO（radio6.35.07.29.img），SPL（hboot1.76.2007.img），HERO的RECOVERY（recovery-RA-hero-v1.5.2.img）

2、在FASTBOOT下用下面的命令开刷（文件改过名）：

fastboot flash hboot hboot.img

fastboot flash radio radio.img

fastboot erase system -w

fastboot erase boot

fastboot erase recovery

fastboot flash recovery recovery.img

1.4.5.1.1　fastboot 模式分析

1.4.5.1.1.1　什么是 fastboot 模式

fastboot 是android 系统的软件烧写模式，通过它我们可以为系统烧写软件

参考文档：

工程模式與FASTBOOT

http://android.cool3c.com/article/2260





1.4.5.1.1.2　fastboot 模式与 recovery 模式的区别

fastboot 模式 和 recovery 模式都是 android系统的软件烧写或者升级方式，通过他们可以为机器烧写软件。

他们有如下区别：

1.  代码来自不同分区，fastboot 在 appsbl 分区 ，recovery 是单独的一个分区

2.  fastboot 比 recovery 功能更强，它可以用来烧写 recovery 分区

fastboot 可以做的事情：
1.  重启机器
2．烧写分区
3. 擦除分区
4. 重启 appsbl
5. 系统更新等
常有如下命令：
fastboot reboot         重启机器
fastboot flash boot     boot.img    烧写引导分区

fastboot flash system   system.img  烧写系统分区
fastboot flash userdata data.img    烧写数据分区
fastboot flash recovery recovery.img     烧写恢复模式分区

fastboot flash splash1  mysplash.rgb565  烧写开机画面

fastboot erase  system  擦除系统分区
fastboot update update.zip  update.zip 是 boot.img, system.img和recovery.img的zip压缩包

以上子命令不一定存在，要根绝实际情况确定。fastboot 中是通过 fastboot_register 方式注册了一系列可用的子命令，如：

fastboot_register("boot", cmd_boot);

fastboot_register("erase:", cmd_erase);

fastboot_register("flash:", cmd_flash);

fastboot_register("continue", cmd_continue);

fastboot_register("reboot", cmd_reboot);

fastboot_register("reboot-bootloader", cmd_reboot_bootloader);

fastboot_register("getvar:", cmd_getvar);

fastboot_register("download:", cmd_download);

我们可以根绝实际情况修改 fastboot 代码，让其支持更多命令，如：

fastboot flashall  在当前目录寻找各种所有的image文件，在刷完所有分区后重启手机

recovery 分区只能对 system ，data，cache boot 等分区进行烧写，sd卡上放 update.zip 的升级方式就可以通过 recovery 的方式完成。

除了 fastboot 模式可以烧写 recovery ，在 android 系统的命令模式下可以通过 flash_image 命令

./out/target/product/msm7630_surf/symbols/system/bin/flash_image 进行烧写。

flash_image recovery /sdcard/recovery.img  红色部分是你解压出的文件名包含后缀名
reboot recovery
之后就能看到新的recovery 了
E/flash_image(   35): can't find recovery partition

参考资料：

最新Amon-RA 1.3.2 Recovery原创教程（完成）

http://www.androidin.net/bbs/thread-29285-1-1.html

1.4.5.1.1.3　怎样进入 fastboot 模式

在系统启动的 appsboot 阶段，通过以下几种方式可以进入 fastboot 模式

1.   通过按键，如： KEY_BACK 或者 KEY_CLEAR 等

2.   当前系统的重启模式，如果为 FASTBOOT_MODE

3.   引导linux失败

unsigned check_reboot_mode(void)

{

    unsigned mode[2] = {0, 0};

    unsigned int mode_len = sizeof(mode);

    unsigned smem_status;



    smem_status = smem_read_alloc_entry(SMEM_APPS_BOOT_MODE,

                  &mode, mode_len );

    if(smem_status)

    {

      dprintf(CRITICAL, "ERROR: unable to read shared memory for reboot mode\n");

      return 0;

    }

    return mode[0];

}

bootable/bootloader/lk/platform/msm_shared/smem.h:83:  SMEM_APPS_BOOT_MODE = 106



按键进入 fastboot 模式要根据实际情况决定，因为这些在代码中都是可以更改的，参考文档：

市面常见机器进入 Recovery 模式及 Fastboot 模式的方法：http://android.cool3c.com/article/12221



1.4.5.1.1.4　android 系统手机刷机过程分析(补充知识)

将以 G1 为样机分析刷机过程

放到SD卡根目录（不要改名，直接丢进去）
打开终端输入
su（回车r）
mount -o rw,remount -t yaffs2 /dev/block/mtdblock3 /system (回车)
cd sdcard (回车r)
flash_image recovery recovery-RAv1.2.1G.img (回车)
以上是从一个帖子里找来的。上面说不要改名，我改了名字刷成功了。

1.4.5.1.2　appsbl 引导 android 系统

appsbl 对 android 系统的引导是从 boot_linux_from_flash 开始，它首先引导 linux 内核，然后由 linux 内核装载引导上层的 android 系统。

boot_linux_from_flash() 函数在文件：

bootable/bootloader/lk/app/aboot/aboot.c:182 中。

它实际上是从 boot 分区中读取 boot.img (内核+ramdisk）然后引导执行，具体过程如下：

int boot_linux_from_flash(void)

{

    struct boot_img_hdr *hdr = (void*) buf;

    unsigned n;

    struct ptentry *ptn;

    struct ptable *ptable;

    unsigned offset = 0;

    const char *cmdline;



        首先判断是否为 eMMC启动

    if (target_is_emmc_boot()) {

       hdr = (struct boot_img_hdr *)EMMC_BOOT_IMG_HEADER_ADDR;

       if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {

           dprintf(CRITICAL, "ERROR: Invalid boot image header\n");

           return -1;

       }

       goto continue_boot;

    }

        获取分区表

    ptable = flash_get_ptable();

    if (ptable == NULL) {

       dprintf(CRITICAL, "ERROR: Partition table not found\n");

       return -1;

    }

        如果不是进入 recovery 模式

    if(!boot_into_recovery)

    {

            ptn = ptable_find(ptable, "boot");

            if (ptn == NULL) {

               dprintf(CRITICAL, "ERROR: No boot partition found\n");

               return -1;

            }

    }

        //进入 recovery 模式

    else

    {

            ptn = ptable_find(ptable, "recovery");

            if (ptn == NULL) {

               dprintf(CRITICAL, "ERROR: No recovery partition found\n");

               return -1;

            }

    }

        //读取 boot.img

    if (flash_read(ptn, offset, buf, page_size)) {

       dprintf(CRITICAL, "ERROR: Cannot read boot image header\n");

       return -1;

    }

    offset += page_size;



    if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {

       dprintf(CRITICAL, "ERROR: Invaled boot image heador\n");

       return -1;

    }



    if (hdr->page_size != page_size) {

       dprintf(CRITICAL, "ERROR: Invaled boot image pagesize. Device pagesize: %d, Image pagesize: %d\n",page_size,hdr->page_size);

       return -1;

    }

        //获取 kernel 镜像文件地址

    n = ROUND_TO_PAGE(hdr->kernel_size, page_mask);

    if (flash_read(ptn, offset, (void *)hdr->kernel_addr, n)) {

       dprintf(CRITICAL, "ERROR: Cannot read kernel image\n");

       return -1;

    }

    offset += n;

        //获取 ramdisk 镜像文件地址

    n = ROUND_TO_PAGE(hdr->ramdisk_size, page_mask);

    if (flash_read(ptn, offset, (void *)hdr->ramdisk_addr, n)) {

       dprintf(CRITICAL, "ERROR: Cannot read ramdisk image\n");

       return -1;

    }

    offset += n;



continue_boot:

    dprintf(INFO, "\nkernel  @ %x (%d bytes)\n", hdr->kernel_addr,

       hdr->kernel_size);

    dprintf(INFO, "ramdisk @ %x (%d bytes)\n", hdr->ramdisk_addr,

       hdr->ramdisk_size);



    if(hdr->cmdline[0]) {

       cmdline = (char*) hdr->cmdline;

    } else {

       cmdline = DEFAULT_CMDLINE;

    }

    dprintf(INFO, "cmdline = '%s'\n", cmdline);



    /* TODO: create/pass atags to kernel */

        //开始引导内核

    dprintf(INFO, "\nBooting Linux\n");

    boot_linux((void *)hdr->kernel_addr, (void *)TAGS_ADDR,

          (const char *)cmdline, board_machtype(),

          (void *)hdr->ramdisk_addr, hdr->ramdisk_size);



    return 0;

}



boot_linux 调用的文件：

bootable/bootloader/lk/app/aboot/aboot.c:94 中的函数：

void boot_linux(void *kernel, unsigned *tags,

       const char *cmdline, unsigned machtype,

       void *ramdisk, unsigned ramdisk_size)

{

    ...

    void (*entry)(unsigned,unsigned,unsigned*) = kernel;

    ...

    entry(0, machtype, tags);

}



entry(0, machtype, tags); 正式进入内核，将由kernel 完成 android 系统的启动。





====================================



osbl_load_appsbl：

  This function loads the applications ARM boot loader to its

  destination RAM.  It may load the aARM from flash or from data

  contained within the OEM SBL image depending on OEM security

  requirements.



osbl_load_apps_os：

 This function loads the APPS image from flash to its destination

 address in RAM. A cookie read from the APPSBL header is checked for

 the size of OS image and magic number. If the magic number matches and

 the size of apps image is greater than 0, the image is loaded.



osbl_load_apps_os 函数在文件：

./AMSS/products/7x30/core/boot/secboot2/osbl/shared/src/osbl_aarm_boot.c:394 中：

void osbl_load_apps_os( bl_shared_data_type *bl_shared_data )

    /* Initialize unified boot interface */

    unified_boot_init(&appsbl_image_header_with_cookie);



./AMSS/products/7x30/core/boot/tools/headergen/shared/src/image_header.c:492:

    boot_cookie.OS_type = atoi(argv[i+5]);

    header[12] = boot_cookie.OS_type;       /* OS type to boot */



nand_tools 工具来自源代码：
编译脚本：

./AMSS/products/7x30/core/bsp/tools/flash/nand_exe/build/SConscript

========================================

env.Replace(TARGET_NAME = 'nand_tools')

nand_tools_exe = env.Program('${TARGET_NAME}', source=[], LIBS=libs, LIBPATH=libs_path)

# copy elf and reloc to needed locations for AMSS tools to load on target

install_target_exe = env.Install(SRCPATH, nand_tools_exe)

========================================

nand_tools 工具语法：

./AMSS/products/7x30/core/bsp/tools/flash/nand_exe/build/nand_tools -fp flash_page_size -fb flash_block_size -fd flash_device_size -d device_bootloader_fname -u usr_partition_fname -m mibib_partition_fname -p binary_folder_path













每一个烧写的 *.mbn 都有一个标准的头，它由函数



  /*----------------------------------------------------------------------

    Fill up header structure

  ----------------------------------------------------------------------*/

  header[0] = id;

  header[2] = image_source;

  header[3] = image_dest;

  header[4] = image_size;/* image_size */

  header[5] = code_size; /* code_size */

  header[6] = image_dest+code_size; /* signature_ptr */

  header[7] = signature_size;            /* signature_size */

  header[8] = image_dest+code_size+signature_size;/* cert_chain_ptr */

  header[9] = cert_chain_size;                    /* cert_chain_size*/

  header[10] = UNIFIED_BOOT_COOKIE_MAGIC_NUMBER; /* boot cookie magic number */

  header[11] = 0;                         /* cookie version number */

  header[12] = boot_cookie.OS_type;       /* OS type to boot */

  header[13] = 0;          /* start address of apps partition in storage device */

  header[14] = boot_cookie.boot_apps_size_entry; /* size in bytes of apps kernel to be loaded */

  header[15] = boot_cookie.boot_apps_ram_loc; /* location in RAM to load apps kernel */

  header[16] = 0;          /* reserve pointer */

  header[17] = 0;          /* reserve for future use */

  header[18] = 0;          /* reserve for future use */

  header[19] = 0;          /* reserve for future use */

  printf("image_size = 0x%X, code_size= =0x%X\n", image_size, code_size);



  /*----------------------------------------------------------------------

    Open output header file

  ----------------------------------------------------------------------*/

因为文件：

./AMSS/products/7x30/build/ms/boot_targets_nonsec.min:346

./AMSS/products/7x30/build/ms/boot_targets_sec.min:498

中有：

GEN_AMSS_HEADER    = $(HEADERGEN)/image_header $(FLASH_TYPE) amss $(MBNPATH)/amss.mbn $(MBNPATH)/amsshd.mbn

语法如下：

Usage: ./AMSS/products/7x30/tools/headergen/image_header flash_type header_type input_code_file \

output_header_file secure_type [dbl_preamble_file or elf_src_file]

例如：

./AMSS/products/7x30/tools/headergen/image_header nand amss ./AMSS/products/7x30/build/ms/bin/AAABQMAZ/amss.mbn amssh.mbn  dbl_preamble_file





./AMSS/products/7x30/tools/headergen/image_header "nand" amss ./AMSS/products/7x30/build/ms/bin/AAABQMAZ/amss.mbn ./aaahd.mbn



文件： ./AMSS/products/7x30/build/ms/dmss_flags.min:131: 中有：

ifeq ($(USES_NOR16_CFG_DATA)$(USES_NOR32_CFG_DATA),yes)#  NAND or NOR

  FLASH_TYPE=nor

else

  FLASH_TYPE=nand

endif

从我们的环境变量可以知道使用的是 nand flash

#pragma RUNLOCAL

$(MBNPATH)/amsshd.mbn: $(IMAGE_HEADER) $(MBNPATH)/amss.mbn

    @echo ---------------------------------------------------------------

    @echo Generating image header for AMSS.

    @echo

    @$(GEN_AMSS_HEADER)

    @echo ---------------------------------------------------------------

1.4.5.1.2.1　Android 系统启动过程中存在的问题

1.4.5.1.1.1.1 linker 问题导致系统无法启动

高通 sdk 打印信息:

D/qemud   (   37): fdhandler_accept_event: accepting on fd 10

D/qemud   (   37): created client 0xe078 listening on fd 8

D/qemud   (   37): client_fd_receive: attempting registration for service 'boot-properties'

D/qemud   (   37): client_fd_receive:    -> received channel id 1

D/qemud   (   37): client_registration: registration succeeded for client 1

I/qemu-props(   47): connected to 'boot-properties' qemud service.

I/qemu-props(   47): received: qemu.sf.lcd_density=160

I/qemu-props(   47): received: dalvik.vm.heapsize=16m

D/qemud   (   37): fdhandler_accept_event: accepting on fd 10

D/qemud   (   37): created client 0xe078 listening on fd 11

D/qemud   (   37): fdhandler_event: disconnect on fd 11

D/qemud   (   37): fdhandler_accept_event: accepting on fd 10

D/qemud   (   37): created client 0xe078 listening on fd 11

D/qemud   (   37): client_fd_receive: attempting registration for service 'gsm'

D/qemud   (   37): client_fd_receive:    -> received channel id 2

D/qemud   (   37): client_registration: registration succeeded for client 2



通过ps可以看出，主要是因为 zygote 未能正常启动，手动启动该服务：

/system/bin/app_process -Xzygote /system/bin --zygote &

弹出一些错误信息：

# link_image[1729]:    71 could not load needed library 'libandroid_runtime.so' for '/system/bin/app_process' (link_image[1729]:    71 could not load needed library 'libnativehelper.so' for 'libandroid_runtime.so' (link_image[1729]:    71 could not load needed library 'libicudata.so' for 'libnativehelper.so' (alloc_mem_region[823]: OOPS:    71 cannot map library 'libicudata.so'. no vspace available.)))CANNOT LINK EXECUTABLE

从以上信息可以断定，是应用程序在linker的时候未能装载需要的库，导致服务终止。

因为我们目前编译的sdk和generic版本都是给予 armv5te ,而高通是 armv7 所以暂时不建议修改代码

我们可以先从网上 sdk 的 elair版本中考取 linker 文件，拷贝到：

out/target/product/generic/system/bin/  然后重新打包 system.img : make snod

bionic/linker/linker.c

#define LINKER_BASE ((LINKER_TEXT_BASE) & 0xfff00000)

#define LINKER_TOP  (LINKER_BASE + (LINKER_AREA_SIZE))

＝＝＝＝＝＝标准代码＝＝＝＝＝＝＝＝

LINKER_TEXT_BASE := 0xB0000100

LINKER_AREA_SIZE := 0x01000000

#define LIBBASE 0x80000000

#define LIBLAST 0x90000000

LINKER_BASE=0xB0000000

＝＝＝＝＝＝高通＝＝＝＝＝＝

LINKER_TEXT_BASE := 0x70000100

LINKER_AREA_SIZE := 0x01000000

#define LIBBASE 0x40000000

#define LIBLAST 0x50000000

#define R_ARM_REL32      3

LINKER_BASE=0x70000000

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

R_ARM_ABS32 32bit 绝对地址重定位引用

R_ARM_PC24  24bit PC相对地址重定位引用

R_ARM_ABS32 32bit 绝对地址重定位引用

R_ARM_REL32 32bit 相对地址重定位引用

REL 是 relative 的缩写，是以当前指令结束时的EIP为参考地址

ABS 是 absolute 的缩写，指绝对地址.

rel32 立即数

r32   寄存器

m32   内存访问



1.4.6 AMSS 流程

osbl 阶段，通过函数 osbl_load_amss_image 装载 amss 镜像文件

void osbl_load_amss_image( bl_shared_data_type *bl_shared_data )

{

    ...

/* 获取 elf 文件的入口点 */

     bl_shared_data->amss_entry_ptr = (amss_entry_ptr_type) boot_sec_elf_get_entry_point( bl_shared_data->amss_elf_if );

    ...

} /* osbl 装载 amss 镜像文件 */



在 osbl_main_ctl 函数中通过 bl_shared_data.amss_entry_ptr() 把控制权交给了 AMSS。

在文件：

AMSS/products/7x30/core/kernel/arch/arm/pistachio/src/gnu/linker.lds:33: 中有：

ENTRY(_start)

所以可以确定 AMSS 的镜像是从 _start 函数开始执行，该函数在文件：

AMSS/products/7x30/core/kernel/arch/arm/pistachio/src/head.spp:51: 中

。。。

BEGIN_PROC(_start)

。。。

        /* Jump to startup_system */

        adr     r0,     __phys_addr_ram

        bl      arch_init

。。。



Amss 镜像由几部分组成，

AMSS/products/7x30/core/kernel/tools/amss.py

def get_amss_kernel_platform_src(build_asic, build_target, platform_dir):

platform_src = ["%s/src/" % platform_dir + fn for fn in ["plat_asm.spp", "timer.cc", "irq.cc", "plat.cc", "init_warm.cc", "head.spp" ]]



AMSS/products/7x30/core/kernel/arch/arm/pistachio/src/head.spp:51:

BEGIN_PROC(_start)



TASK_COMMON_SOURCES =  [

   '${BUILDPATH}/mobile.c',

   '${BUILDPATH}/task.c',

   '${BUILDPATH}/task_iram.c'

]

task_objs = env.Object(TASK_COMMON_SOURCES)

task_lib = env.Library('${BUILDPATH}/task', task_objs)

./AMSS/products/7x30/core/debugtools/task/build/corelibs/arm11/task.lib



env.AddLibsToImage(['MODEM_IMAGE', 'CBSP_MODEM_IMAGE'],

   [task_lib,

   task_dog_lib,

   task_dog_keepalive_modem_lib,

   task_dog_keepalive_client_lib])



./AMSS/products/7x30/core/bsp/build/scripts/utils.py:59:

env.AddMethod(add_libs_to_image, "AddLibsToImage")

./AMSS/products/7x30/core/bsp/build/scripts/utils.py:140:

def add_libs_to_image(env, targets, libs):

./AMSS/products/7x30/core/bsp/amsslibs/build/arm11/SConscript:65:env.Replace(MODEM_IMAGE = env.subst('${PROC}'))

./AMSS/products/7x30/core/bsp/build/msm7x30_modem.mak:34:PROC = MULTI_PROC

IMAGE=MODEM_PROC



./AMSS/products/7x30/modem/rfa/rf/task/common/inc/rf_task.h:79:TASK_START_SIG     0x8000

./AMSS/products/7x30/core/boot/appsproc/target/msm7x30/src/process.c:31:                   SMSM RESET state to process_restart from err.c.

./AMSS/products/7x30/core/debugtools/err/src/err.c:808:      process_restart();



err_fatal_jettison_core

    -> err_fatal_handler





    代码运行到了Main()之后，在这个函数里面将完成操作系统（rex）的初始化工作，其实现方法是调用rex_init()。Rex_init()完成的工作很简单：

完成操作系统必要的一些数据结构（timer链表、任务链表等））的初始化之外；

接下来，它创建了三个任务，分别是：rex_idle_task、rex_dpc_task和tmc_task。

Idle任务没什么好解释的，目前这个任务为空，什么也没做，dpc_task目前不知道是做什么的，暂时可以不用管。前面的这两个任务都属于操作系统层面的，由操作系统来维护，和手机软件关系不大。哪一个和手机软件关系大呢？答案是：tmc_task。大家可以把这个当作操作系统的入口(主)任务，也可以把它当作整个手机软件的入口任务。即AMSS软件里的所有其它任务的创建和维护就是由这个tmc_task来完成的。

    到此为止，整个AMSS软件还并没有跑起来，只是跑到了tmc_task里面了。在tmc_task里面，会调用tmc_init()来完成整个AMSS软件包的初始化工作，其中最重要的一项工作就是调用tmc_define_tasks()将AMSS软件包所有需要的任务都创建起来了。比如说slee_task、dog_task、cm_task、wms_task、ui_task等。这些任务，一般不需要直接和AL层软件打交道，但请大家记住，手机上所有功能的实现最根本点就是由这些服务组件（Service Task）来完成的。将来大家跟踪一个具体的功能模块时，比如说通话模块，如果需要，可以再去深入研究它的具体实现。

    好了，到现在为止，所有的AMSS核心软件就全部跑起来了（手机的功能模块，在软件方面就体现为OS层面的一个任务）。但现在大家还根本看不到Brew和AEE的影子。呵呵，各位不要急。到了这个层面之后，我想稍微多说几句。最早的Qualcomm平台，比如说5xxx系列，是根本没有Brew的，那个时候的AL（Application Layer）层软件开发，是直接调用底层Service task所提供的API来完成相应的工作的。从这种角度来看的话，显然那时的开发是比较郁闷和难度较高的。不过，到了65xx之后，Qualcomm平台引入了Brew，手机开发商就没必要去从这么底层（Service API）的层面进行手机开发了，他们完全可以基于Brew来实现一台手机的所有功能（Qualcomm给我们的参考代码，就是全Brew平台的）。

    Brew的运行环境AEE是如何跑起来的呢？关键在于ui_task()，由于ui_task和我们手机开发的关系非常密切，其地位也相当重要，所以，后文我将单独对它进行一个深入的研究与分析。到目前为止，大家只需要知道ui_task将AEE加载起来了，并且，它起到了一个中间层的作用，即所有AMSS底层服务组件的消息，都将经由ui_task而转到AEE，并最终转到具体的App（Applet）的执行代码里面（HandleEvent()）。

注意：

上述的开机过程，在每一次按开机键都需要走一遍，即关机之后，整个系统的所有功能都将消失，而不像有些手机，看起来是关了机，但实际上底层还是有一些软件模块在跑。为什么可以肯定地说上述开机过程每次都必须走一遍，原因很简单，因为我们的平台软件是基于Nand Flash启动的，所有的代码都需要Copy到SDRAM才能运行，而关机断电之后，SDRAM里的东东会全部丢失，所以，毫无疑问，上述的过程必须每次开机都执行；

关机的过程相对比较简单，系统检测到关机中断之后，将调用tmc_powerdown_handler（）来完成关机动作，它将把所有AMSS的任务都Stop掉，并最后调用rex_exit()退出Rex，从而完成整个关机动作。

显然，关机动作前，如果有必要，每一个任务必须将它希望保存的信息保存到Flash上面，以便下次开机时可以得到这些信息；

开机流程简图

 EMBED Visio.Drawing.11  

图1 Qualcomm平台开机框图

    说明：

Tmc是操作系统层面和AMSS软件关系最密切的一个任务，不过需要OEM商在此处修改的地方应该不多；

ui_task是在操作系统层面，OEM商需要重点研究清楚的一个任务，它是连接底层Task和上层AL的一个中间层，有可能需要加入OEM商的操作流程；

CoreApp是在Brew层面的一个AL层的入口Applet，它其着管理整个上层AL层软件的作用，根据产品需求，这个App需要定做；

AEE是整个上层App的运行环境，目前Qualcomm没有公开它的源码，但它的运行机制，Amoi需要好好研究清楚，我将在另外一篇《Qualcomm平台AEE运行机制深入分析与研究》中探讨它的运行机理和调度机制，大家有兴趣可以参考此文；

Boot代码深入分析

Boot代码大部分是用汇编语言写的，也有小部分，可能需要由OEM商修改，所以用C语言来写。另外,Boot代码属于Driver范围，所以大家可以在drivers/boot目录里面找到相应的代码。Boot的代码组织得非常模块化，整个boot的入口点是在Boot_function_table.s里面，这个汇编代码里面实际上是将Boot需要完成的任务封装成了不同的函数，由不同的函数来完成相应的工作，接下来，我将深入分析这些函数所完成的工作，如下所述。

mmu_enable_instruction_cache;

这个只有在Nand启动模式时才需要，打开ARM的指令Cache.

boot_hw_ctrl_init

此函数主要是完成两条总线(EBI1、EBI2)控制器的初始化,这个函数执行完了       之后，系统就可以知道两条总线上连接了哪些设备，同时也可以找得到这些       设备，不过，至于单个设备自身的初始化，则不在这里.

[注]

这个函数很重要，OEM商如果需要加新的设备到系统中（挂在两条总线上），则需

要定做此模块，目前阶段主要是内存。另外，如前文所述，这个函数是由C语言

来写的，主要目的就是为了方便OEM商定做。内存设备的修改，可以在这个模块

里找到相应的数据结构，相对还是比较简单的。

boot_hw_tlmm_init

1.晶振时钟的初始化；

2.中断表的初始化；

3.GPIO的初始化；

4.Msm本身的驱动，除了EBI2;

boot_rom_test

这个函数非常简单，只是做一个很简单的Rom检查.（比对两个标志位来检查，并

没有一块一块地去检查）。

boot_ram_test

Ram自检，具体算法其实也很简单，就是读、写内存以判断是否成功.

boot_ram_init

1.拷贝手机代码从Nand Flash到SDRAM。

a.Image__BB_RAM__Base:Core Code;

    b.Image__APP_RAM__Base:App Code;

    [注]

    上述动作是分块进行的，原因是因为Qualcomm支持分块Boot Load.

2.将Image__ZI_REGION__ZI区域初始化为0;

3.初始化OEM要求的动态Heap;

4.至于代码段里的数据初始化，直接在Image里就完成了(编译器完成);

boot_stack_initialize

ARM栈初始化,主要是为分块代码加载而预留的.

boot_cache_mmu_init

ARM Mmu初始化

    注意：

       到此为止，整个Boot的工作就告完结了，那么，它又是如何跳到AMSS的main

入口点呢？原因很简单，ARM编译器在链接的时候会自动做出一个__rt_entry()，

由此函数来完成ARM库函数的初始化，并最后将代码执行点跳转到main（）。而

__rt_entry()会在boot_reset_handler.s里调用，具体细节，大家可以不用太过关心，

只需要明白，Boot跑完之后，手机软件就跑到了main里就Ok了。

Ui_task的深入分析

从大的方向来讲，ui_task只完成两件事，一件是必要的初始化工作（这个也是我们所关心的，即ui_task到底完成了哪些工作）；另外一件事就是各种信号量的事件处理，这也是我们比较关心的，即ui_task到底将哪些事件转发给了上层App。搞清楚了上述两点，我们也就能大致把ui_task的承上启下的工作机理研究清楚。

ui_Init;

初始化过程中，ui_task主要完成了如下几件事。

创建一个用于Kick Watchdog的定时器，这样WatchDog能够及时得到Kick，假如今后发现手机在ui_task里面自动重启，很有可能就是这个定时器的Timeout设置得过短而造成的；

注册通话相关的回调，主要是和紧急呼叫相关；

电话本初始化，之所以要进行这个工作，主要是加快开机之后AL层软件操作电话本的速度，但这样将有可能导致开机速度过慢，如果开机速度过慢，可以考虑进入待机界面之后，在后台开一个task去完成这项工作；

初始化Sound设备；

向底层服务任务wms_task注册wms回调，这个回调是在IWms组件里实现的。从这种角度来看，u帮我们把wms_task和IWMS组件联系起来了，但并没有去将AL层软件和IWMS联系起来，这个工作将是由AL层软件自己去完成。当然，注册回调的这个工作也是可以在AL层完成，之所以在这里完成，而不是在AL层完成，其主要目的是这个工作可以做到与AL层无关，即AL层不需要关心这个事情，但这个事情是短消息功能得于实现的必须步骤；

注册键盘消息回调；

通过这个回调，所有的按键消息都将经由底层的hs_task传到此回调函数里。然后回调函数将把所有的按键信息放到一个全局变量ui_key_buffer里面，接着发送一个UI_KEY_SIG信号给ui_task通知它去处理按键信息，至于ui_task如何处理按键消息的，后面的ui_handleSignals里会有详细描述。

初始化Lcd，这个工作不是LCD硬件设备的真正初始化，只是一些UI需要用到的LCD数据结构的初始化，和我们关系不大；

[注]

硬件的初始化，全部都在hs_task里面完成，从这种角度来看的话，系统能跑到ui_task里面，表明所有的硬件设备的驱动都已经成功加载。

置开机标志ui_powerup为True；

注册IPC信号量UI_IPC_SIG，这个可以暂时不管；

bridle_InitSWITable的初始化，这个目标，暂时不知道，也可以先略过；

初始化资源文件，其主要工作就是在Rom里面建立资源文件的符号链表，这样就可以让系统找到这些资源文件了（资源文件是被编译在代码段的，假如不这样做的话，系统将找不到这些资源文件）；

Brew运行环境AEE的初始化：AEE_Init，这个函数看不到代码，大家只需要知道，到了这一步，整个Brew也就Run起来了，在AEE初始化完成之后，它将自动启动一个Applet，即CoreStartApp，而CoreStartApp将把CoreApp启动起来；

到此为止，ui_task的初始化工作完成；

[注意]

    1) 从上述的ui_task的初始化工作可以看出，ui_task并没有完成手机AL层软件的

基本功能的初始化，比如说Sim卡检测、网络初始化等，这些工作，应该是在

CoreApp里完成的

       2) 真正和手机功能相关的初始化工作，是在CoreApp里完成的，这个Applet的工

作机理，后面也会有详细描述；

ui_HandleSignals;

ui_task主要完成如下事件的处理。

看门狗喂食；

TASK_STOP_SIG信号，任务Stop，目前这个任务为空，没做任何事；

TASK_OFFLINE_SIG的处理，这几个任务都属于操作系统层面的事件，目前我们可以暂时不管；

处理关机信号：CoreAppHandleStopSig（），这个只是处理ui_task在关机前需要完成的任务，比如说发送一个消息给CoreApp让它关掉自己，然后将ui_task关闭；

[A]

系统的真正关机信号是由tmc来处理，当它发现需要关机时，调用tmc_powerdown_handler来完成相应的工作，在这里就是给所有的任务发送TASK_STOP_SIG的信号。

[A]

深层次的关机处理，不需要我们了解，也没必要去知道，我们只需要知道在ui_task里面把该关的关掉就Ok了。

[A]

关机是一个层层深入的过程，每一个App或者任务只需要负责将它们自己创建的资源释放掉就Ok了。而关机的导引线，显然是在CoreApp里截获到关机键之后发送出来的，事实上也是如此。

网络掉线时，发送掉线信号给CoreApp；

[A]

其实这个信号完全可以在CoreApp里面，自己去注册，然后及时更新自己的网络状态，就不知有没有这种接口函数。

处理按键消息，其主要完成如下的工作：

打开背光；

特理按键到虚键值的转换;

按键声音的处理;

将按键消息传送到AEE执行环境，由它去负责按键的派发；

[注]

1.背光的打开是由ui默认完成的，那这样的话，假如我不想按键时有背光，是否可行？看来就得修改此处的代码；

2.AEE的按键派发机制如何？它能否保证处于显示最上层的App永远是可以得到Key的App，即假如一个Applet将自身Hide，它是否依然可以得到Key，而其它的Applet是否就不可以得到了？很怕也像EMP一样出现焦点丢失的情况；

处理AEE_APP_SIG信号量，完成AEE的调度工作，这个任务是ui完成的最重要的一项工作，因为上层的App需要定时进行调度，目前看来，这个调度工作是由AEE_APP_SIG触发的，而AEE_APP_SIG这个信号量，则由操作系统层面的一个定时器定时发送的。现在大家只要了解，AEE_Dispatch会定时调用就Ok了，至于更详细的AEE调度机制，可以参考我的另外一篇《AEE运行机制深入分析与研究》；

处理AEE_SIO_SIG信号量，这个看不到代码，暂时略过不管；

结论

通过上述对于ui_task的分析，可以看出，ui_task真正和手机功能有关系的（即可能需要定制或者修改的地方），主要就是初始化资源文件和处理按键消息这两部分。至于其它部分，目前都不需要Amoi关心。手机真正功能的实现，比如说开机Logo的显示、Sim卡的检查、Pin码校验等，都是在CoreApp里面完成的。

其它

CoreApp的深入分析

目前参考代码里面的CoreApp所完成的工作比较多且杂，主要说来有如下几件事。

系统组件初始化；

开机Logo的显示；

Sim卡检测和Pin码校验；

系统状态信息更新；

电池状态；

网络信号；

网络模式；

IAnnunciator的维护与更新；

通话处理，打电话的输入框；

主菜单处理；

手机各种设置功能的处理；

关机键的处理；

目前CoreApp里面的代码，完成了太多的事，其实完全可以剥离成不同的模块来完成，大致可以分成如下几个部分。

总控模块；(CoreApp)

总控模块，主要完成手机按下开机键之后的各种初始化工作，同时此模块也是整个手机的控制中心，由它来完成手机的一些全局性工作，主要有如下几项。

系统初始化、Sim卡检测和Pin码校验；

开机Logo或者开机动画的显示；

底层服务程序的启动；（WmsApp、DialApp等）；

系统配置信息的统一管理；

由于写配置信息到NV上面是一件非常慢的工作，每次上层App改变配置之后都去操作NV，很影响速度。所以，可以在内存中开一个配置信息的Buffer，上层App操作的实际上是这个Buffer，然后由Core在空闲的时候再统一写到NV上去。

关机处理；

[注]

由于CoreApp是在Idle Applet的界面之下，所以，为了能够实现“一键回菜单”的功用，有可能需要修改ui_task里面的Key处理函数，将所有的Key消息转发给Core，这样Core就可以得到所有的Key事件了。（现在的ui_task只把Key事件发送给了AEE，而AEE只会将Key事件发送给当前活动Applet）。

Idle模块；

主要完成待机界面的画图工作，主要有两部分：

系统信息指示栏；

待机界面（位图、动画、时钟、日历等）；

软键

[注]

Idle只负责界面工作，不负责具体的系统状态信息的获取工作，这个工作将由其它模块完成。

Polling

手机状态信息查询模块，主要是完成手机各种状态信息的更新与维护。主要有如下几种：

电池强度；

网络信号强度；

网络模式(C/G)；

PLMN网络名；

短消息、通话状态、闹铃；（这个由专门的模块完成，不在Polling之列）；

各种外设信息；（USB、耳机插入等）；

其它各种杂项信息；

Menu模块

菜单模块主要分两部分，一部分是主菜单的实现，另一个子菜单的实现。一般来讲，手机上的菜单系统应该是由Menu模块去统一完成，而不是由每一个子程序去手动完成。菜单模块一般只需要负责到主菜单、二级菜单和三级菜单就Ok了。三级菜单之后的界面，就由每一个App单独去维护了。

其它功能App模块；

每一个功能模块，由一个专门的App来完成，这样的话，模块的独立性强，便于单独开发。模块间通过App启动和消息传送的方式来发生关系和进行模块间通信。



后记

到此为止，Qualcomm整个手机从按下开机键到跑到主菜机界面，整个流程一目了然。对于Amoi而言，目前需要关心和定做的部分其实不多，最头疼的当属CoreApp的改造工作，当然这个就是后话了，笔者将在今后的文章中加以详述。

希望本文对于大家理解Qualcomm手机软件的运行流程有一定的帮助，如果有什么问题，请直接联系我，最后谢谢大家耐心把本文看完，谢谢。

参考文档

a)         80-V1072-1_E_Boot_Block_Downloader.pdf

b)        80-V5316-1_K_QCT_Ext_API_RG.pdf

c)         driver/boot目录源码

d)        service/tmc目录源码

e)         app/core目录源码





REX启动分析——基于Qualcomm平台

http://hi.baidu.com/gcmao/blog/item/5ef1ea2c12da08341e308914.html

Qualcomm手机开机全过程

Qualcomm手机开机全过程大揭密(四)

http://www.1mp.cn/DataShow.aspx?id=1642





Qualcomm手机开机全过程大揭密

chh@amoi.com.cn

2004-11-13

摘要：

    本文试图通过代码来深入剖析Qualcomm手机开机的整个过程，即从按下开机键一直到出现待机界面，Qualcomm的手机软件在整个流程中究竟完成了哪些工作。本文的主要目标是理清手机的初始化流程，并为今后Amoi定做初始化工作提供一个参考。

关键字：开机、Rex、TMC、ui_task、CoreApp

开机的简要流程分析

Qualcomm的平台软件支持两种启动方式：一种是Nor Flash启动方式，另外一种就是Nand Flash启动方式。Nor Flash启动方式就相当于硬件直接找到一个入口点开始执行代码，相比较而言会 比较简单，且Amoi没有采用此种方式，所以本文对于这种方式不做详细分析。另外一种就是Nand Flash启动方式，这种方式和PC的启动方式比较相像，也是Amoi采用的Boot方式，下面将详细分析在此方式下面的开机过程。

    按下开机键之后，将产生一个时钟中断，从而通知AMSS主芯片的Boot Load硬件去将放置于Nand Flash上面的第一个Block（8K）里面的Boot代码Copy到内核内存（RAM，这个内存应该是CPU自带的内存，同后面提到的SDRAM有一定区别，可以把它当作CPU的Cache）的0xFFFF0000地址，并开始执行Boot代码。Boot的主要任务是完成整个系统的硬件初始化工作（类似于PC上面的BIOS所完成的硬件自检工作，至于Boot的详细工作机制，后文会有详细描述）。Boot所完成的工作里面，最重要的一件事就是会将整个手机软件代码（AMSS软件包）拷贝到SDRAM中，并最后将控制权交给AMSS软件。说白了，就是Boot执行完成之后，代码的执行点将由Boot跳转到AMSS软件的的入口点函数main().（此函数在mobile.c里实现）。

代码运行到了Main()之后，在这个函数里面将完成操作系统（rex）的初始化工作，其实现方法是调用rex_init()。Rex_init()完成的工作很简单：

完成操作系统必要的一些数据结构（timer链表、任务链表等））的初始化之外；

接下来，它创建了三个任务，分别是：rex_idle_task、rex_dpc_task和tmc_task。

Idle任务没什么好解释的，目前这个任务为空，什么也没做，dpc_task目前不知道是做什么的，暂时可以不用管。前面的这两个任务都属于操作系统层面的，由操作系统来维护，和手机软件关系不大。哪一个和手机软件关系大呢？答案是：tmc_task。大家可以把这个当作操作系统的入口(主)任务，也可以把它当作整个手机软件的入口任务。即AMSS软件里的所有其它任务的创建和维护就是由这个tmc_task来完成的。

到此为止，整个AMSS软件还并没有跑起来，只是跑到了tmc_task里面了。在tmc_task里面，会调用tmc_init()来完成整个AMSS软件包的初始化工作，其中最重要的一项工作就是调用tmc_define_tasks()将AMSS软件包所有需要的任务都创建起来了。比如说slee_task、dog_task、cm_task、wms_task、ui_task等。这些任务，一般不需要直接和AL层软件打交道，但请大家记住，手机上所有功能的实现最根本点就是由这些服务组件（Service Task）来完成的。将来大家跟踪一个具体的功能模块时，比如说通话模块，如果需要，可以再去深入研究它的具体实现。

好了，到现在为止，所有的AMSS核心软件就全部跑起来了（手机的功能模块，在软件方面就体现为OS层面的一个任务）。但现在大家还根本看不到Brew和AEE的影子。呵呵，各位不要急。到了这个层面之后，我想稍微多说几句。最早的Qualcomm平台，比如说5xxx系列，是根本没有Brew的，那个时候的AL（Application Layer）层软件开发，是直接调用底层Service task所提供的API来完成相应的工作的。从这种角度来看的话，显然那时的开发是比较郁闷和难度较高的。不过，到了65xx之后，Qualcomm平台引入了Brew，手机开发商就没必要去从这么底层（Service API）的层面进行手机开发了，他们完全可以基于Brew来实现一台手机的所有功能（Qualcomm给我们的参考代码，就是全Brew平台的）。

    Brew的运行环境AEE是如何跑起来的呢？关键在于ui_task()，由于ui_task和我们手机开发的关系非常密切，其地位也相当重要，所以，后文我将单独对它进行一个深入的研究与分析。到目前为止，大家只需要知道ui_task将AEE加载起来了，并且，它起到了一个中间层的作用，即所有AMSS底层服务组件的消息，都将经由ui_task而转到AEE，并最终转到具体的App（Applet）的执行代码里面（HandleEvent()）。

注意：

上述的开机过程，在每一次按开机键都需要走一遍，即关机之后，整个系统的所有功能都将消失，而不像有些手机，看起来是关了机，但实际上底层还是有一些软件模块在跑。为什么可以肯定地说上述开机过程每次都必须走一遍，原因很简单，因为我们的平台软件是基于Nand Flash启动的，所有的代码都需要Copy到SDRAM才能运行，而关机断电之后，SDRAM里的东东会全部丢失，所以，毫无疑问，上述的过程必须每次开机都执行；

关机的过程相对比较简单，系统检测到关机中断之后，将调用tmc_powerdown_handler（）来完成关机动作，它将把所有AMSS的任务都Stop掉，并最后调用rex_exit()退出Rex，从而完成整个关机动作。

显然，关机动作前，如果有必要，每一个任务必须将它希望保存的信息保存到Flash上面，以便下次开机时可以得到这些信息；

开机流程简图

 EMBED Visio.Drawing.11  

图1 Qualcomm平台开机框图

    说明：

Tmc是操作系统层面和AMSS软件关系最密切的一个任务，不过需要OEM商在此处修改的地方应该不多；

ui_task是在操作系统层面，OEM商需要重点研究清楚的一个任务，它是连接底层Task和上层AL的一个中间层，有可能需要加入OEM商的操作流程；

CoreApp是在Brew层面的一个AL层的入口Applet，它其着管理整个上层AL层软件的作用，根据产品需求，这个App需要定做；

AEE是整个上层App的运行环境，目前Qualcomm没有公开它的源码，但它的运行机制，Amoi需要好好研究清楚，我将在另外一篇《Qualcomm平台AEE运行机制深入分析与研究》中探讨它的运行机理和调度机制，大家有兴趣可以参考此文；

Boot代码深入分析

Boot代码大部分是用汇编语言写的，也有小部分，可能需要由OEM商修改，所以用C语言来写。另外,Boot代码属于Driver范围，所以大家可以在drivers/boot目录里面找到相应的代码。Boot的代码组织得非常模块化，整个boot的入口点是在Boot_function_table.s里面，这个汇编代码里面实际上是将Boot需要完成的任务封装成了不同的函数，由不同的函数来完成相应的工作，接下来，我将深入分析这些函数所完成的工作，如下所述。

mmu_enable_instruction_cache;

这个只有在Nand启动模式时才需要，打开ARM的指令Cache.

boot_hw_ctrl_init

此函数主要是完成两条总线(EBI1、EBI2)控制器的初始化,这个函数执行完了       之后，系统就可以知道两条总线上连接了哪些设备，同时也可以找得到这些       设备，不过，至于单个设备自身的初始化，则不在这里.

[注]

这个函数很重要，OEM商如果需要加新的设备到系统中（挂在两条总线上），则需

要定做此模块，目前阶段主要是内存。另外，如前文所述，这个函数是由C语言

来写的，主要目的就是为了方便OEM商定做。内存设备的修改，可以在这个模块

里找到相应的数据结构，相对还是比较简单的。

boot_hw_tlmm_init

1.晶振时钟的初始化；

2.中断表的初始化；

3.GPIO的初始化；

4.Msm本身的驱动，除了EBI2;

boot_rom_test

这个函数非常简单，只是做一个很简单的Rom检查.（比对两个标志位来检查，并

没有一块一块地去检查）。

boot_ram_test

Ram自检，具体算法其实也很简单，就是读、写内存以判断是否成功.

boot_ram_init

1.拷贝手机代码从Nand Flash到SDRAM。

a.Image__BB_RAM__Base:Core Code;

    b.Image__APP_RAM__Base:App Code;

    [注]

    上述动作是分块进行的，原因是因为Qualcomm支持分块Boot Load.

2.将Image__ZI_REGION__ZI区域初始化为0;

3.初始化OEM要求的动态Heap;

4.至于代码段里的数据初始化，直接在Image里就完成了(编译器完成);

boot_stack_initialize

ARM栈初始化,主要是为分块代码加载而预留的.

boot_cache_mmu_init

ARM Mmu初始化

    注意：

       到此为止，整个Boot的工作就告完结了，那么，它又是如何跳到AMSS的main

入口点呢？原因很简单，ARM编译器在链接的时候会自动做出一个__rt_entry()，

由此函数来完成ARM库函数的初始化，并最后将代码执行点跳转到main（）。而

__rt_entry()会在boot_reset_handler.s里调用，具体细节，大家可以不用太过关心，

只需要明白，Boot跑完之后，手机软件就跑到了main里就Ok了。

Ui_task的深入分析

从大的方向来讲，ui_task只完成两件事，一件是必要的初始化工作（这个也是我们所关心的，即ui_task到底完成了哪些工作）；另外一件事就是各种信号量的事件处理，这也是我们比较关心的，即ui_task到底将哪些事件转发给了上层App。搞清楚了上述两点，我们也就能大致把ui_task的承上启下的工作机理研究清楚。

ui_Init;

初始化过程中，ui_task主要完成了如下几件事。

创建一个用于Kick Watchdog的定时器，这样WatchDog能够及时得到Kick，假如今后发现手机在ui_task里面自动重启，很有可能就是这个定时器的Timeout设置得过短而造成的；

注册通话相关的回调，主要是和紧急呼叫相关；

电话本初始化，之所以要进行这个工作，主要是加快开机之后AL层软件操作电话本的速度，但这样将有可能导致开机速度过慢，如果开机速度过慢，可以考虑进入待机界面之后，在后台开一个task去完成这项工作；

初始化Sound设备；

向底层服务任务wms_task注册wms回调，这个回调是在IWms组件里实现的。从这种角度来看，u帮我们把wms_task和IWMS组件联系起来了，但并没有去将AL层软件和IWMS联系起来，这个工作将是由AL层软件自己去完成。当然，注册回调的这个工作也是可以在AL层完成，之所以在这里完成，而不是在AL层完成，其主要目的是这个工作可以做到与AL层无关，即AL层不需要关心这个事情，但这个事情是短消息功能得于实现的必须步骤；

注册键盘消息回调；

通过这个回调，所有的按键消息都将经由底层的hs_task传到此回调函数里。然后回调函数将把所有的按键信息放到一个全局变量ui_key_buffer里面，接着发送一个UI_KEY_SIG信号给ui_task通知它去处理按键信息，至于ui_task如何处理按键消息的，后面的ui_handleSignals里会有详细描述。

初始化Lcd，这个工作不是LCD硬件设备的真正初始化，只是一些UI需要用到的LCD数据结构的初始化，和我们关系不大；

[注]

硬件的初始化，全部都在hs_task里面完成，从这种角度来看的话，系统能跑到ui_task里面，表明所有的硬件设备的驱动都已经成功加载。

置开机标志ui_powerup为True；

注册IPC信号量UI_IPC_SIG，这个可以暂时不管；

bridle_InitSWITable的初始化，这个目标，暂时不知道，也可以先略过；

初始化资源文件，其主要工作就是在Rom里面建立资源文件的符号链表，这样就可以让系统找到这些资源文件了（资源文件是被编译在代码段的，假如不这样做的话，系统将找不到这些资源文件）；

Brew运行环境AEE的初始化：AEE_Init，这个函数看不到代码，大家只需要知道，到了这一步，整个Brew也就Run起来了，在AEE初始化完成之后，它将自动启动一个Applet，即CoreStartApp，而CoreStartApp将把CoreApp启动起来；

到此为止，ui_task的初始化工作完成；

[注意]

    1) 从上述的ui_task的初始化工作可以看出，ui_task并没有完成手机AL层软件的

基本功能的初始化，比如说Sim卡检测、网络初始化等，这些工作，应该是在

CoreApp里完成的

       2) 真正和手机功能相关的初始化工作，是在CoreApp里完成的，这个Applet的工

作机理，后面也会有详细描述；

ui_HandleSignals;

ui_task主要完成如下事件的处理。

看门狗喂食；

TASK_STOP_SIG信号，任务Stop，目前这个任务为空，没做任何事；

TASK_OFFLINE_SIG的处理，这几个任务都属于操作系统层面的事件，目前我们可以暂时不管；

处理关机信号：CoreAppHandleStopSig（），这个只是处理ui_task在关机前需要完成的任务，比如说发送一个消息给CoreApp让它关掉自己，然后将ui_task关闭；

[A]

系统的真正关机信号是由tmc来处理，当它发现需要关机时，调用tmc_powerdown_handler来完成相应的工作，在这里就是给所有的任务发送TASK_STOP_SIG的信号。

[A]

深层次的关机处理，不需要我们了解，也没必要去知道，我们只需要知道在ui_task里面把该关的关掉就Ok了。

[A]

关机是一个层层深入的过程，每一个App或者任务只需要负责将它们自己创建的资源释放掉就Ok了。而关机的导引线，显然是在CoreApp里截获到关机键之后发送出来的，事实上也是如此。

网络掉线时，发送掉线信号给CoreApp；

[A]

其实这个信号完全可以在CoreApp里面，自己去注册，然后及时更新自己的网络状态，就不知有没有这种接口函数。

处理按键消息，其主要完成如下的工作：

打开背光；

特理按键到虚键值的转换;

按键声音的处理;

将按键消息传送到AEE执行环境，由它去负责按键的派发；

[注]

1.背光的打开是由ui默认完成的，那这样的话，假如我不想按键时有背光，是否可行？看来就得修改此处的代码；

2.AEE的按键派发机制如何？它能否保证处于显示最上层的App永远是可以得到Key的App，即假如一个Applet将自身Hide，它是否依然可以得到Key，而其它的Applet是否就不可以得到了？很怕也像EMP一样出现焦点丢失的情况；

处理AEE_APP_SIG信号量，完成AEE的调度工作，这个任务是ui完成的最重要的一项工作，因为上层的App需要定时进行调度，目前看来，这个调度工作是由AEE_APP_SIG触发的，而AEE_APP_SIG这个信号量，则由操作系统层面的一个定时器定时发送的。现在大家只要了解，AEE_Dispatch会定时调用就Ok了，至于更详细的AEE调度机制，可以参考我的另外一篇《AEE运行机制深入分析与研究》；

处理AEE_SIO_SIG信号量，这个看不到代码，暂时略过不管；

结论

通过上述对于ui_task的分析，可以看出，ui_task真正和手机功能有关系的（即可能需要定制或者修改的地方），主要就是初始化资源文件和处理按键消息这两部分。至于其它部分，目前都不需要Amoi关心。手机真正功能的实现，比如说开机Logo的显示、Sim卡的检查、Pin码校验等，都是在CoreApp里面完成的。

其它

CoreApp的深入分析

目前参考代码里面的CoreApp所完成的工作比较多且杂，主要说来有如下几件事。

系统组件初始化；

开机Logo的显示；

Sim卡检测和Pin码校验；

系统状态信息更新；

电池状态；

网络信号；

网络模式；

IAnnunciator的维护与更新；

通话处理，打电话的输入框；

主菜单处理；

手机各种设置功能的处理；

关机键的处理；

目前CoreApp里面的代码，完成了太多的事，其实完全可以剥离成不同的模块来完成，大致可以分成如下几个部分。

总控模块；(CoreApp)

总控模块，主要完成手机按下开机键之后的各种初始化工作，同时此模块也是整个手机的控制中心，由它来完成手机的一些全局性工作，主要有如下几项。

系统初始化、Sim卡检测和Pin码校验；

开机Logo或者开机动画的显示；

底层服务程序的启动；（WmsApp、DialApp等）；

系统配置信息的统一管理；

由于写配置信息到NV上面是一件非常慢的工作，每次上层App改变配置之后都去操作NV，很影响速度。所以，可以在内存中开一个配置信息的Buffer，上层App操作的实际上是这个Buffer，然后由Core在空闲的时候再统一写到NV上去。

关机处理；

[注]

由于CoreApp是在Idle Applet的界面之下，所以，为了能够实现“一键回菜单”的功用，有可能需要修改ui_task里面的Key处理函数，将所有的Key消息转发给Core，这样Core就可以得到所有的Key事件了。（现在的ui_task只把Key事件发送给了AEE，而AEE只会将Key事件发送给当前活动Applet）。

Idle模块；

主要完成待机界面的画图工作，主要有两部分：

系统信息指示栏；

待机界面（位图、动画、时钟、日历等）；

软键

[注]

Idle只负责界面工作，不负责具体的系统状态信息的获取工作，这个工作将由其它模块完成。

Polling

手机状态信息查询模块，主要是完成手机各种状态信息的更新与维护。主要有如下几种：

电池强度；

网络信号强度；

网络模式(C/G)；

PLMN网络名；

短消息、通话状态、闹铃；（这个由专门的模块完成，不在Polling之列）；

各种外设信息；（USB、耳机插入等）；

其它各种杂项信息；

Menu模块

菜单模块主要分两部分，一部分是主菜单的实现，另一个子菜单的实现。一般来讲，手机上的菜单系统应该是由Menu模块去统一完成，而不是由每一个子程序去手动完成。菜单模块一般只需要负责到主菜单、二级菜单和三级菜单就Ok了。三级菜单之后的界面，就由每一个App单独去维护了。

其它功能App模块；

每一个功能模块，由一个专门的App来完成，这样的话，模块的独立性强，便于单独开发。模块间通过App启动和消息传送的方式来发生关系和进行模块间通信。



后记

到此为止，Qualcomm整个手机从按下开机键到跑到主菜机界面，整个流程一目了然。对于Amoi而言，目前需要关心和定做的部分其实不多，最头疼的 当属CoreApp的改造工作，当然这个就是后话了，笔者将在今后的文章中加以详述。

希望本文对于大家理解Qualcomm手机软件的运行流程有一定的帮助，如果有什么问题，请直接联系我，最后谢谢大家耐心把本文看完，谢谢。

参考文档

f)         80-V1072-1_E_Boot_Block_Downloader.pdf

g)        80-V5316-1_K_QCT_Ext_API_RG.pdf

h)        driver/boot目录源码

i)          service/tmc目录源码

j)          app/core目录源码



1.5 android 系统重启关机流程分析

1.5.1 c语言中调用 reboot 函数

bionic/libc/unistd/reboot.c:33:

int reboot (int  mode)

{

    return __reboot( LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, mode, NULL );

}

1.5.2 通过 adb 让系统重启

adb reboot recovery  进入 recovery 模式

adb reboot bootloader 进入 fastboot 模式

adb reboot-bootloader

adb reboot 不带参数 系统正常重启

adb 是pc端工具，adbd是服务端，运行在手机

adbd 读取 socket 解析由 adb 传过来的命令串

int service_to_fd(const char *name)

if(!strncmp(name, "reboot:", 7)) {

        void* arg = strdup(name + 7);

        if(arg == 0) return -1;

        ret = create_service_thread(reboot_service, arg);



system/core/adb/services.c:176:

void reboot_service(int fd, void *arg)

{

    。。。

    ret = __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,

                    LINUX_REBOOT_CMD_RESTART2, (char *)arg);

    。。。

}

bionic/libc/kernel/common/linux/reboot.h

#define LINUX_REBOOT_CMD_RESTART 0x01234567

#define LINUX_REBOOT_CMD_HALT 0xCDEF0123

kernel/include/linux/reboot.h:33:

#define    LINUX_REBOOT_CMD_RESTART2   0xA1B2C3D4



arg 对应字符串： recovery  bootloader

./kernel/arch/arm/mach-msm/pm2.c

system/core/adb/commandline.c

if (!strcmp(argv[0], "reboot-bootloader"))

   snprintf(command, sizeof(command), "reboot:bootloader");

如果输入 adb reboot-bootloader adb 会对该命令进行转换 相当于执行 adb reboot bootloader

1.5.3 fastboot 模式下系统重启

fastboot reboot  系统正常重启

fastboot reboot-bootloader 重启进入fastboot 模式

fastboot 是 appboot 提供的功能，可以用它来烧写 system 等镜像文件

bootable/bootloader/lk/app/aboot/aboot.c

APP_START(aboot)

    .init = aboot_init,

void aboot_init(const struct app_descriptor *app)

｛

    。。。

    fastboot_register("reboot", cmd_reboot);

    fastboot_register("reboot-bootloader", cmd_reboot_bootloader);

    。。。

｝

void cmd_reboot(const char *arg, void *data, unsigned sz)

{

    dprintf(INFO, "rebooting the device\n");

    fastboot_okay("");

    reboot_device(0);

}



void cmd_reboot_bootloader(const char *arg, void *data, unsigned sz)

{

    dprintf(INFO, "rebooting the device\n");

    fastboot_okay("");

    reboot_device(FASTBOOT_MODE);

}

bootable/bootloader/lk/target/msm7630_surf/init.c:311:

void reboot_device(unsigned reboot_reason)

bootable/bootloader/lk/target/msm7627_ffa/init.c:174:

void reboot_device(unsigned reboot_reason)

void reboot_device(unsigned reboot_reason)

{

    reboot(reboot_reason);

}

调用的是c函数：

bionic/libc/unistd/reboot.c:33:

int reboot (int  mode)

{

    return __reboot( LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, mode, NULL );

}

bootable/bootloader/lk/app/aboot/aboot.c:59:

#define FASTBOOT_MODE   0x77665500

if (!strcmp(cmd, "bootloader")) {

           restart_reason = 0x77665500;

}



1.5.4 系统关机

正常按键关机

./frameworks/policies/base/phone/com/android/internal/policy/impl/PhoneWindowManager.java

void showGlobalActionsDialog()

  mGlobalActions.showDialog(keyguardShowing, isDeviceProvisioned());

调用文件：

./frameworks/policies/base/phone/com/android/internal/policy/impl/GlobalActions.java

中的函数：

public void showDialog(boolean keyguardShowing, boolean isDeviceProvisioned)

  mDialog = createDialog();

调用本文件中的函数：

private AlertDialog createDialog()

public void onPress() {

    ShutdownThread.shutdownAfterDisablingRadio(mContext, true);

}

调用文件：

./frameworks/policies/base/phone/com/android/internal/policy/impl/ShutdownThread.java

中的函数：

public static void shutdownAfterDisablingRadio(final Context context, boolean confirm)

  beginShutdownSequence(context)

调用本文件中的函数：

private static void beginShutdownSequence(Context context)

  sInstance.start()

进入关机线程的run函数：

public void run() {

首先关蓝牙，关射频，然后再关电源

    ...

    sBluetooth.disable(false)

    ...

    sPhone.setRadio(false)

    ...

    SystemClock.sleep(PHONE_STATE_POLL_SLEEP_MSEC);

    ...

        Power.shutdown()

}

frameworks/base/core/java/android/os/Power.java:92:

public static native void shutdown();

frameworks/base/core/jni/android_os_Power.cpp:107:

{ "shutdown", "()V", (void*)android_os_Power_shutdown },

jni 调用

static void android_os_Power_shutdown(JNIEnv *env, jobject clazz)

{

    sync();

#ifdef HAVE_ANDROID_OS

    reboot(RB_POWER_OFF);

#endif

}

因为有 bionic/libc/include/sys/reboot.h:42:

#define RB_POWER_OFF    LINUX_REBOOT_CMD_POWER_OFF

所以实际相对执行

reboot(LINUX_REBOOT_CMD_POWER_OFF);

__reboot( LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_POWER_OFF, NULL );



这里的 __reboot 是libc中的系统调用

bionic/libc/arch-x86/syscalls/__reboot.S

/* autogenerated by gensyscalls.py */

#include <sys/linux-syscalls.h>



    .text

    .type __reboot, #function

    .globl __reboot

    .align 4

    .fnstart



__reboot:

    .save   {r4, r7}

    stmfd   sp!, {r4, r7}

    ldr     r7, =__NR_reboot

    swi     #0

    ldmfd   sp!, {r4, r7}

    movs    r0, r0

    bxpl    lr

    b       __set_syscall_errno

    .fnend



1.5.5 内核中的系统调用 reboot

__NR_reboot 执行的是内核中的系统调用：

kernel/kernel/sys.c:310:

SYSCALL_DEFINE4(reboot, int, magic1, int, magic2, unsigned int, cmd,

       void __user *, arg)

{

    char buffer[256];

    int ret = 0;



    /* We only trust the superuser with rebooting the system. */

    if (!capable(CAP_SYS_BOOT))

       return -EPERM;



    /* For safety, we require "magic" arguments. */

    if (magic1 != LINUX_REBOOT_MAGIC1 ||

        (magic2 != LINUX_REBOOT_MAGIC2 &&

                    magic2 != LINUX_REBOOT_MAGIC2A &&

           magic2 != LINUX_REBOOT_MAGIC2B &&

                    magic2 != LINUX_REBOOT_MAGIC2C))

       return -EINVAL;



    /* Instead of trying to make the power_off code look like

     * halt when pm_power_off is not set do it the easy way.

     */

    if ((cmd == LINUX_REBOOT_CMD_POWER_OFF) && !pm_power_off)

       cmd = LINUX_REBOOT_CMD_HALT;



    lock_kernel();

    switch (cmd) {

    case LINUX_REBOOT_CMD_RESTART:

       kernel_restart(NULL);

       break;



    case LINUX_REBOOT_CMD_CAD_ON:

       C_A_D = 1;

       break;



    case LINUX_REBOOT_CMD_CAD_OFF:

       C_A_D = 0;

       break;



    case LINUX_REBOOT_CMD_HALT:

       kernel_halt();

       unlock_kernel();

       do_exit(0);

       panic("cannot halt");



    case LINUX_REBOOT_CMD_POWER_OFF:

       kernel_power_off();

       unlock_kernel();

       do_exit(0);

       break;



    case LINUX_REBOOT_CMD_RESTART2:

       if (strncpy_from_user(&buffer[0], arg, sizeof(buffer) - 1) < 0) {

           unlock_kernel();

           return -EFAULT;

       }

       buffer[sizeof(buffer) - 1] = '\0';



       kernel_restart(buffer);

       break;



#ifdef CONFIG_KEXEC

    case LINUX_REBOOT_CMD_KEXEC:

       ret = kernel_kexec();

       break;

#endif



#ifdef CONFIG_HIBERNATION

    case LINUX_REBOOT_CMD_SW_SUSPEND:

       ret = hibernate();

       break;

#endif



    default:

       ret = -EINVAL;

       break;

    }

    unlock_kernel();

    return ret;

}

void kernel_restart(char *cmd)

{

    kernel_restart_prepare(cmd);

    if (!cmd)

       printk(KERN_EMERG "Restarting system.\n");

    else

       printk(KERN_EMERG "Restarting system with command '%s'.\n", cmd);

    machine_restart(cmd);

}

kernel/kernel/sys.c:341:

void kernel_power_off(void)

{

    kernel_shutdown_prepare(SYSTEM_POWER_OFF);

    if (pm_power_off_prepare)

       pm_power_off_prepare();

    disable_nonboot_cpus();

    sysdev_shutdown();

    printk(KERN_EMERG "Power down.\n");

    machine_power_off();

}



./kernel/arch/arm/kernel/process.c:219:

void machine_restart(char *cmd)

{

    arm_pm_restart(reboot_mode, cmd);

}



void machine_power_off(void)

{

    if (pm_power_off)

       pm_power_off();

}



因为./kernel/arch/arm/mach-msm/pm2.c:1740:中有：

arm_pm_restart = msm_pm_restart;

pm_power_off = msm_pm_power_off;

所以 arm_pm_restart 调用的是：

static void msm_pm_restart(char str, const char *cmd)

{

    msm_rpcrouter_close();

    msm_proc_comm(PCOM_RESET_CHIP, &restart_reason, 0);



    for (;;)

       ;

}

pm_power_off 调用的是：

static void msm_pm_power_off(void)

{

    msm_rpcrouter_close();

    msm_proc_comm(PCOM_POWER_DOWN, 0, 0);

    for (;;)

       ;

}

msm_proc_comm 是芯片级别的操作

msm_proc_comm_reset_modem_now 对modem芯片进行重启

kernel/arch/arm/mach-msm/proc_comm.c:98:

int msm_proc_comm(unsigned cmd, unsigned *data1, unsigned *data2)

{

    。。。

    writel(cmd, base + APP_COMMAND);

    writel(data1 ? *data1 : 0, base + APP_DATA1);

    writel(data2 ? *data2 : 0, base + APP_DATA2);

    。。。

}

内核里面writel是如何实现的

http://blog.chinaunix.net/u2/77776/showart_1404857.html

补充信息：

static int msm_reboot_call

    (struct notifier_block *this, unsigned long code, void *_cmd)

{

    if ((code == SYS_RESTART) && _cmd) {

       char *cmd = _cmd;

       if (!strcmp(cmd, "bootloader")) {

           restart_reason = 0x77665500;

       } else if (!strcmp(cmd, "recovery")) {

           restart_reason = 0x77665502;

       } else if (!strcmp(cmd, "eraseflash")) {

           restart_reason = 0x776655EF;

       } else if (!strncmp(cmd, "oem-", 4)) {

           unsigned code = simple_strtoul(cmd + 4, 0, 16) & 0xff;

           restart_reason = 0x6f656d00 | code;

       } else {

           restart_reason = 0x77665501;

       }

    }

    return NOTIFY_DONE;

}

static struct notifier_block msm_reboot_notifier = {

    .notifier_call = msm_reboot_call,

};

static int __init msm_pm_init(void)

  register_reboot_notifier(&msm_reboot_notifier);



内核编译相关：

kernel/arch/arm/mach-msm/pm2.c:1701:          restart_reason = 0x77665500;

kernel/arch/arm/mach-msm/pm.c:696:        restart_reason = 0x77665500;

kernel/arch/arm/mach-msm/Makefile:84:

ifdef CONFIG_MSM_N_WAY_SMSM

    obj-$(CONFIG_PM) += pm2.o

else

    obj-$(CONFIG_PM) += pm.o

endif



kernel/arch/arm/configs/msm7630-perf_defconfig:256:CONFIG_MSM_N_WAY_SMSM=y

kernel/arch/arm/configs/msm7627-perf_defconfig:247:CONFIG_MSM_N_WAY_SMSM=y

make -C kernel O=../out/target/product/msm7627_ffa/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE=arm-eabi- msm7627-perf_defconfig

out/target/product/msm7627_ffa/obj/KERNEL_OBJ/.config

CONFIG_MSM_N_WAY_SMSM=y



1.6 软件调用流程分析

1.6.1 设置sim卡状态

工程模式中显

private void tryChangeSimLockState()

  Message callback = Message.obtain(mHandler, ENABLE_SIM_PIN_COMPLETE);

  mPhone.getSimCard().setSimLockEnabled(mToState, mPin, callback);

进入sim lock 菜单会显示初始化pin状态，是通过下面语句得到：

mPinToggle.setChecked(mPhone.getSimCard().getSimLockEnabled());



mPhone.getSimCard().setSimLockEnabled(mToState, mPin, callback)调用的是文件：

GsmSimCard.java (frameworks\base\telephony\java\com\android\internal\telephony\gsm)中的函数：

public void setSimLockEnabled (boolean enabled,String password, Message onComplete) {

  int serviceClassX;

  serviceClassX = CommandsInterface.SERVICE_CLASS_VOICE +

                CommandsInterface.SERVICE_CLASS_DATA +

                CommandsInterface.SERVICE_CLASS_FAX;

  mDesiredPinLocked = enabled;

  phone.mCM.setFacilityLock(CommandsInterface.CB_FACILITY_BA_SIM,

                enabled, password, serviceClassX,

                obtainMessage(EVENT_CHANGE_FACILITY_LOCK_DONE, onComplete));

phone.mCM.setFacilityLock 调用的是文件：

RIL.java (frameworks\base\telephony\java\com\android\internal\telephony\gsm)中的函数：

    public void

    setFacilityLock (String facility, boolean lockState, String password,

                        int serviceClass, Message response)

    {

        String lockString;

         RILRequest rr

                = RILRequest.obtain(RIL_REQUEST_SET_FACILITY_LOCK, response);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        // count strings

        rr.mp.writeInt(4);

        rr.mp.writeString(facility);

        lockString = (lockState)?"1":"0";

        rr.mp.writeString(lockString);

        rr.mp.writeString(password);

        rr.mp.writeString(Integer.toString(serviceClass));

        send(rr);

    }

设置应用程序向 rild 发送 RIL_REQUEST_SET_FACILITY_LOCK　请求的 socket消息，

android的初始源代码中 RIL_REQUEST_SET_FACILITY_LOCK 请求，在参考实现　Reference-ril.c

(hardware\ril\reference-ril) 中没有实现。

我们需要做得工作是：

vendor/qcom/proprietary/qcril/

vendor/qcom/proprietary/qcril/qcril_original/qcril_original.mk

LOCAL_MODULE:= libril-qc-1

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

device/qcom/msm7627_ffa/system.prop:5:

rild.libpath=/system/lib/libril-qc-1.so



./build/target/board/generic/system.prop

rild.libpath=/system/lib/libreference-ril.so

rild.libargs=-d /dev/ttyS0
1.6.2 设置背光

工程模式中显示电池信息：

./hardware/msm7k/liblights/Android.mk:28:LOCAL_MODULE := lights.$(TARGET_BOARD_PLATFORM)

1.6.3 获取电池信息

工程模式中显示电池信息：

*#*#4636#*#* 进入android 自带的工程模式

packages/apps/Settings/src/com/android/settings/BatteryInfo.java:88:

电压发生变化就会收到 intent 为 Intent.ACTION_BATTERY_CHANGED 的广播消息：

action.equals(Intent.ACTION_BATTERY_CHANGED)

从intent 中获取电压值通过 TextView 来显示

int plugType = intent.getIntExtra("plugged", 0);

mLevel.setText("" + intent.getIntExtra("level", 0));

mScale.setText("" + intent.getIntExtra("scale", 0));

mVoltage.setText("" + intent.getIntExtra("voltage", 0) + " "

                        + getString(R.string.battery_info_voltage_units));

int status = intent.getIntExtra("status", BatteryManager.BATTERY_STATUS_UNKNOWN);



./packages/apps/Settings/res/values-zh-rCN/strings.xml

<string name="battery_info_power_label" msgid="7465140230991349382">"是否已插电："</string>

<string name="battery_info_status_label">"电池状态："</string>

<string name="battery_info_scale_label">"电池容量："</string>

<string name="battery_info_level_label">"电池级别："</string>

<string name="battery_info_voltage_label">"电池电压："</string>

mStatus.setText(statusString);



布局文件 packages/apps/Settings/res/layout/battery_info.xml

id="@+id/status"   text="@string/battery_info_status_label"

id="@+id/power"    text="@string/battery_info_power_label"

id="@+id/level"    text="@string/battery_info_level_label"

id="@+id/scale"    text="@string/battery_info_scale_label"

id="@+id/voltage"  text="@string/battery_info_voltage_label"



电池状态：

<string name="battery_info_status_charging_ac" >"（交流电）"</string>

<string name="battery_info_status_charging_usb" >"(USB 绑定选项)"</string>

<string name="battery_info_status_discharging" >"正在放电"</string>

<string name="battery_info_status_not_charging" >"未在充电"</string>

<string name="battery_info_status_full" >"已满"</string>



电源状态

switch (plugType) {

case 0:

//未插电

            mPower.setText(getString(R.string.battery_info_power_unplugged));

            break;

case BatteryManager.BATTERY_PLUGGED_AC:

        //交流电

            mPower.setText(getString(R.string.battery_info_power_ac));

            break;

case BatteryManager.BATTERY_PLUGGED_USB:

        //usb供电

            mPower.setText(getString(R.string.battery_info_power_usb));

            break;

case (BatteryManager.BATTERY_PLUGGED_AC|BatteryManager.BATTERY_PLUGGED_USB):

        // "交流电 + USB"

            mPower.setText(getString(R.string.battery_info_power_ac_usb));

            break;

default:

        // "未知"

            mPower.setText(getString(R.string.battery_info_power_unknown));

            break;

     }



关心电池信息的服务或者应用：

frameworks/base/services/java/com/android/server/PowerManagerService.java

frameworks/base/services/java/com/android/server/WifiService.java

frameworks/base/services/java/com/android/server/UiModeManagerService.java:324

frameworks/base/services/java/com/android/server/status/StatusBarPolicy.java

frameworks/base/services/java/com/android/server/NotificationManagerService.java

frameworks/base/services/java/com/android/server/NotificationManagerService.java

frameworks/base/tests/BatteryWaster/src/com/android/batterywaster/BatteryWaster.java

frameworks/base/tests/BatteryWaster/src/com/android/batterywaster/BatteryWaster.java

frameworks/policies/base/phone/com/android/internal/policy/impl/KeyguardUpdateMonitor.java

通过 filter.addAction(Intent.ACTION_BATTERY_CHANGED);





电池服务发送电池信息的 intent

frameworks/base/services/java/com/android/server/BatteryService.java

    private final void sendIntent() {

        //  Pack up the values and broadcast them to everyone

        Intent intent = new Intent(Intent.ACTION_BATTERY_CHANGED);

        intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY

                | Intent.FLAG_RECEIVER_REPLACE_PENDING);

        try {

            mBatteryStats.setOnBattery(mPlugType == BATTERY_PLUGGED_NONE, mBatteryLevel);

        } catch (RemoteException e) {

            // Should never happen.

        }

        int icon = getIcon(mBatteryLevel);



        intent.putExtra(BatteryManager.EXTRA_STATUS, mBatteryStatus);

        intent.putExtra(BatteryManager.EXTRA_HEALTH, mBatteryHealth);

        intent.putExtra(BatteryManager.EXTRA_PRESENT, mBatteryPresent);

        intent.putExtra(BatteryManager.EXTRA_LEVEL, mBatteryLevel);

        intent.putExtra(BatteryManager.EXTRA_SCALE, BATTERY_SCALE);

        intent.putExtra(BatteryManager.EXTRA_ICON_SMALL, icon);

        intent.putExtra(BatteryManager.EXTRA_PLUGGED, mPlugType);

        intent.putExtra(BatteryManager.EXTRA_VOLTAGE, mBatteryVoltage);

        intent.putExtra(BatteryManager.EXTRA_TEMPERATURE, mBatteryTemperature);

        intent.putExtra(BatteryManager.EXTRA_TECHNOLOGY, mBatteryTechnology);

        。。。

        ActivityManagerNative.broadcastStickyIntent(intent, null);

    }



private synchronized final void update() {

    native_update();

    shutdownIfNoPower(); //点亮不足关机处理

    shutdownIfOverTemp();//温度过高，关机处理

        。。。

        sendIntent();

        。。。

}



在电池服务启动，或者有相关时间的时候会调用 update 得到电池信息，然后通过广播发出去

 private UEventObserver mUEventObserver = new UEventObserver() {

        @Override

        public void onUEvent(UEventObserver.UEvent event) {

            update();

        }

    };

根据jni调用规则，native_update 实际调用的是 android_server_BatteryService_update

frameworks/base/services/jni/com_android_server_BatteryService.cpp:226:

{"native_update", "()V", (void*)android_server_BatteryService_update},



#define POWER_SUPPLY_PATH "/sys/class/power_supply"

struct FieldIds {

    // members

    jfieldID mAcOnline;

    jfieldID mUsbOnline;

    jfieldID mBatteryStatus;

    jfieldID mBatteryHealth;

    jfieldID mBatteryPresent;

    jfieldID mBatteryLevel;

    jfieldID mBatteryVoltage;

    jfieldID mBatteryTemperature;

    jfieldID mBatteryTechnology;

};



static void android_server_BatteryService_update(JNIEnv* env, jobject obj)

{

    setBooleanField(env, obj, gPaths.acOnlinePath, gFieldIds.mAcOnline);

    setBooleanField(env, obj, gPaths.usbOnlinePath, gFieldIds.mUsbOnline);

    setBooleanField(env, obj, gPaths.batteryPresentPath, gFieldIds.mBatteryPresent);



    setIntField(env, obj, gPaths.batteryCapacityPath, gFieldIds.mBatteryLevel);

    setVoltageField(env, obj, gPaths.batteryVoltagePath, gFieldIds.mBatteryVoltage);

    setIntField(env, obj, gPaths.batteryTemperaturePath, gFieldIds.mBatteryTemperature);



    const int SIZE = 128;

    char buf[SIZE];



    if (readFromFile(gPaths.batteryStatusPath, buf, SIZE) > 0)

        env->SetIntField(obj, gFieldIds.mBatteryStatus, getBatteryStatus(buf));

    else

        env->SetIntField(obj, gFieldIds.mBatteryStatus,

                         gConstants.statusUnknown);



    if (readFromFile(gPaths.batteryHealthPath, buf, SIZE) > 0)

        env->SetIntField(obj, gFieldIds.mBatteryHealth, getBatteryHealth(buf));



    if (readFromFile(gPaths.batteryTechnologyPath, buf, SIZE) > 0)

        env->SetObjectField(obj, gFieldIds.mBatteryTechnology, env->NewStringUTF(buf));

}



cat /sys/class/power_supply/battery/type

cat /sys/class/power_supply/battery/status

cat /sys/class/power_supply/battery/health

cat /sys/class/power_supply/battery/present

cat /sys/class/power_supply/battery/technology

cat /sys/class/power_supply/battery/capacity

cat /sys/class/power_supply/battery/voltage_now

cat /sys/class/power_supply/battery/temp

cat /sys/class/power_supply/battery/current_now

cat /sys/class/power_supply/battery/current_avg

cat /sys/class/power_supply/battery/charge_counterBattery

$ Battery

$ Charging

$ Good

$ 1

$ Li-ion

$ 100

$ 4172644

$ 296

$ 85626

$ 86229

$ 1363200


高通常用工具使用

2.1 QPST

QPST(Qualcomm Product Support Tool),这个工具类似电脑上的资源管理器，可用于手机和PC间的数据传输，手机软件版本升级，部分内置参数如 APN的修改，上行加密，完整性保护参数设置等。

2.2 QXDM

QXDM(Qualcomm eXtensive Diagnostic Monitor),主要用于抓取up口的上，下行信令业务数据包，并可多窗口实时显示诸如功率，速率，误码率等参数，是系统测试人员最常用的软件之一。

常用功能

2.3 QCAT

QCAT(Qualcomm log Analysis Tool) ，它主要用于对 QXDM捕获的二进制代码文件进行解码分析，并可转换为文本格式输出。APEX(Analysis and Post-processing Expert)是QCAT的升级替换版本，功能是类似的。



3、工程模式

Making a Factory Image

1. To make a factory image, the file mibib.mbn must exist in the

   \build\ms\bin\<build_id> directory.

2. To verify the factory image with USB, the file \tools\mjnand.cmm must include

   the option “Program Factory Image”.

有谁用过QSC6085平台采用NAND FLASH烧录，具体怎么打包烧录文件？还有想把文件系统也打包到烧录文件里怎么实现？请高手不啬赐教！！！小弟在此谢过！！！！

用tools目录下的qfit生成，有文档介绍的。多看看资料。。。。。。。



./AMSS/products/7x30/build/ms/dmss_rules.min

genmibib:

    @echo ----------------------------------------------------------------------

    @echo Building mibib.mbn

    @pwd

    #$(BUILD_MIBIB_CMD)

    @echo ----------------------------------------------------------------------

BUILD_MIBIB_CMD 在文件 ./AMSS/products/7x30/build/ms/armtools.min:333 中

ifeq ($(BUILD_UNIX), yes)

ac_EXE=

else

ac_EXE=.exe

endif

BUILD_MIBIB_CMD = $(FLASH_NAND_TOOLS_SRC)/nand_tools$(ac_EXE) -fp $(NAND_PAGE_SIZE) -fb $(NAND_BLK_SIZE) -fd $(NAND_DEVICE_SIZE) \

                  -u partition.mbn -q qcsblhd_cfgdata.mbn -ah amsshd.mbn -m mibib.mbn -oh oemsblhd.mbn \

                  -wh appshd.mbn -bh appsboothd.mbn -p $(MIBIB_DEST_PATH)

把 genmibib 规则中的 #$(BUILD_MIBIB_CMD) 选项打开

它实际上是进入目录： AMSS/products/7x30/build/ms  然后执行命令：

../../core/storage/flash/tools/src/nand/nand_tools -fp 4096 -fb 64 -fd 1024 -u partition.mbn -q qcsblhd_cfgdata.mbn -ah amsshd.mbn -m mibib.mbn -oh oemsblhd.mbn -wh appshd.mbn -bh appsboothd.mbn -p ../../build/ms/bin/AABBQOLY

要生成 qcsblhd_cfgdata.mbn  还必须打开文件

./AMSS/products/7x30/build/ms/dmss7X30modem.mak:49 中的

#include boot_targets_$(SEC_MODE).min 语句



. ./make-AAABQOLYM.sh  gencfgdata

Generating Configuration Generation Tool

make: *** ../../secboot/cfg_data: 没有该文件或目录。 停止。

make: *** [../../secboot/cfg_data/Config_Gen.exe] 错误 2

. ./make-AAABQOLYM.sh  genmibib

. ./make-AAABQOLYM.sh  gencfgdata



从目前的版本中可以看出，只有文件 partition.mbn 存在，其他都不存在。Oemsblhd.mbn 等这些只存在老的软件版本中。



运行 QFIT 工具

Perl 工具 /opt/shared/win_exe/ActivePerl-5.6.1.638-MSWin32-x86.msi

Windows dos 窗口下，进入 qfit 目录执行 QFIT.cmd 启动 qfit程序



./AMSS/products/7x30/core/bsp/build/data/incpathsaaabqmazm.py:472:

env.Replace(FLASH_NAND_TOOLS_CFG = "config_v2")

./AMSS/products/7x30/core/bsp/build/data/incpathsaabbqclym.py:166:

env.Replace(FLASH_HAL_CFG = "config_v2")



HAL  headware abstract layer

DAL

./AMSS/products/7x30/build/ms/bin/AABBQOLY/NPRG7x30.hex

./AMSS/products/7x30/core/bsp/tools/flash/hostdl/build/NPRG7x30_AABBQOLYM.hex



QFIT工具的位置

./AMSS/products/7x30/tools/qfit

qfit 工具只能在 windows 环境下使用，通过 QFIT.cmd 脚本启动 QFIT 工具

运行 QFIT.cmd 脚本之前设置环境变量：set PERL5.6.1=c:\perl\bin

qfit.pl ->

use Make_Factory_Image;

Make_Factory_Image::make_factory_image



生成 Factory Image

./AMSS/products/7x30/tools/qfit/QFIT.pl:1108:

make_frame('MakeFactoryImage','$tab21');

$show_layout_button = make_button("Preview\n Image Layout",$action_no_target_color,'&preview_image_layout');

$MakeFactoryButton  = make_button("Make\nFactory Image",$action_no_target_color,'&make_factory_image');

make_frame('MakeFactoryImage2','$tab21');

make_button("Select Image  \nFile",$action_not_configured_color,'&SelectFactoryFile');

make_entry_box('','$FactoryImageFileName',$file_box_size);

make_frame('MakeFactoryImage3','$tab21');

make_progress_bar('\$factory_make_percent');



Make_Factory_Image::make_factory_image({

                                 factory_image_file      => $FactoryImageFileName,

                                 include_partitions_ref  => \@image_include,

                                 single_partition_file   => $make_factory_image_single_partition_output_file,

                                 partition_mbn_file      => $partition_mbn_file,

                                 local_directory         => $LocalDir,

                                 include_factory_header  => $include_factory_header,

                                 make_dataio_files       => $make_dataio_files,

                                 reuse_ecc_file          => $reuse_ecc_file,

                                 abort_after_error       => $abort_after_error,

                                 percent_done_ref        => \$factory_make_percent,

                           });

Make_Factory_Image::make_factory_image 执行的是文件：

./AMSS/products/7x30/tools/qfit/Make_Factory_Image.pm:

中的函数 make_factory_image

AMSS build must contain mibib.mbn to use QFIT 1.5.02

同时需要的有：

mibib.mbn amss.mbn appsboot.mbn apps.mbn

通过 QFIT 生成 factory image 必须存在 mibib.mbn



烧写 Factory Image 通过 qpst

make_frame('ProgramFactoryImage','$tab22');

$ProgramButton = make_button("Program Factory\nImage into Phone",$action_qpst_target_color,'&program_factory_image_qpst');



$ProgramFactoryImage_fr->Label(-text=>'   Factory image is programmed into phone using QPST') -> pack( -side => 'left');

make_frame('ProgramFactoryImage2','$tab22');

make_button("    Select Image    \nFile",$action_not_configured_color,'&SelectProgramFile');

make_entry_box('','$ImageProgramFileName',$file_box_size);



sub program_factory_image_qpst

exec ("perl Program_Factory_Image.pl $com_port $timeout $factory_file");


4、Android 系统更新升级总结
4.1 刷机基本知识
1. 恢复模式，用update.zip文件
2. 工程模式刷机，刷NBH文件（官方版本）
3. fastboot模式刷机（烧写img文件）

第一种，这也是最常用的刷机方式。  首先，需要有testkey和ROOT，因为绝大部分的update.zip文件都是自制的，需要有足够的权限，才能使用测试签名的文件。将ROM 放进SD，并改名为update.zip，关机，按特定的键开机进入恢复模式。不同厂商的手机进入恢复模式的方法会不一样。
第二种，通常刷官方发布的NBH文件，刷完会失去ROOT。 NBH文件对应地区，如果没有开发版的工程模式，只能刷与你手机地区对应的NBH文件。要刷NBH，首先格式化SD卡，选择FAT32格式。  放入NBH文件，改名成DreaIMG.NBH，NBH是扩展名。 接着关机，按特定的键进入工程模式，加载完成后，按关机键，开始加载NBH文件，期间5-10分钟，不用管，完成后，按任意键，然后特定键关机，重启进入系统。
第三种，fastboot 模式刷机我们应该比较熟悉，在我们软件开发阶段，我们就是用这种方法来烧写软件的，它的原理是直接擦出、烧写、替换分区。  手机端需要SPL支持，这里的SPL就是我们通常所说的 appsboot.mbn , 这个文件来来自android 源代码，编译后通常生成为：
out/target/product/msm7630_surf/appsboot.mbn 他的源代码在：
android/bootable/bootloader/lk/
PC端有fastboot命令与手机端的 fastboot 进行通信，传入特定的命令，让手机端完成分区擦出、烧写、重启等任务。原理和具体使用方法参考 fastboot 刷机
目前我们已经能做到第一和第三种的刷机方式，第二种因为原理还不是很明白，会根据实际情况再介绍。
4.1.1 各品牌代表手机刷机模式进入方法

4.1.1.1　HTC G1

Home+电源  进入recovery模式

电源+照相  进入工程模式

回键+电源  进入fastboot模式

4.1.1.2　三星 Galaxy i7500

专用刷机模式  进入方法：音量向下+OK+电源

recovery模式  进入方法：音量向下+通话+电源

Fastboot模式，进入方法：通话+电源

也就是看SPL版本那个画面

4.1.1.3　Google Nexus One

Fastboot 模式  进入方法：＂电源键+轨迹球＂

进入 fastboot 模式后，可以通过音量键和电源键在 recovery 、hboot、fastboot 集中模式间进行切换。

4.1.2 fastboot 模式
4.1.2.1　fastboot 模式概述
Fastboot 和 HBoot 模式在 appsboot.mbn 中



终端执行以下命令 sudo fastboot devices ，如果设备连接正常且进入fastboot 模式那么会返回类似如下的信息：

HT849GZ58999  fastboot

Fastboot pc客户端和手机端通常需要版本一致或者接近，否则会出现找不到设备，或者某些命令无法支持。

除了按键能进入 fastboot 模式外，系统重启也可以进入。

reboot_device(FASTBOOT_MODE);

./bootloader/lk/app/aboot/aboot.c:59:#define FASTBOOT_MODE   0x77665500

./bootloader/lk/target/msm7630_surf/init.c:251:

void reboot_device(unsigned reboot_reason)

    reboot(reboot_reason);

./bootloader/lk/platform/msm_shared/proc_comm.c:243:

void reboot(unsigned reboot_reason)



void reboot(unsigned reboot_reason)

{

        msm_proc_comm(PCOM_RESET_CHIP, &reboot_reason, 0);

        for (;;) ;

}



内核中的重启：

void machine_restart(char * __unused)

{

        arm_pm_restart(reboot_mode);

}

./kernel/kernel/sys.c:308:  machine_restart(cmd);



void kernel_restart(char *cmd)

{

    kernel_restart_prepare(cmd);

    if (!cmd)

       printk(KERN_EMERG "Restarting system.\n");

    else

       printk(KERN_EMERG "Restarting system with command '%s'.\n", cmd);

    machine_restart(cmd);

}

./kernel/kernel/sys.c:414:

kernel_restart(buffer);



kernel/arch/alpha/kernel/process.c:102:

if (how->mode == LINUX_REBOOT_CMD_RESTART) {

kernel/arch/alpha/kernel/process.c:146:

if (! alpha_using_srm && how->mode != LINUX_REBOOT_CMD_RESTART) {

kernel/arch/alpha/kernel/process.c:171:

common_shutdown(LINUX_REBOOT_CMD_RESTART, restart_cmd);

4.1.2.2　PC端fastboot 命令分析

4.1.2.2.1　命令选项 –w –s  -p –c

sudo fastboot –w

将会擦除 userdata  和 cache  分区

erasing 'userdata'... OKAY

erasing 'cache'... OKAY

如果设备未就绪提示：

< waiting for device >



sudo fastboot boot –s

sudo fastboot -s WH9AMP200206 getvar version

返回：

sudo fastboot -s WH9AMP200206 getvar product

返回：



usb_handle *usb_open(ifc_match_func callback)

{

    return find_usb_device("/dev/bus/usb", callback);

}

./system/core/fastboot/usb_linux.c

通过读取设备文件 /dev/bus/usb/

我们的大板插入usb 进入 fastboot 模式，会有设备：

/dev/bus/usb/001/022

如果我们设备的驱动中没有指定，那么将显示 '???????'



sudo fastboot boot –c xxxx

-c 用于启动内核的时候指定命令行参数，覆盖内核的命令行参数



jresult_t core_handle_ep0(void *core_ctx, juint8_t *buffer, device_speed_t speed)

./AMSS/products/7x30/core/wiredconnectivity/hsusb/core/src/jslave/core/jusb_core.c:1060:

return chp9_handle_usb_command(core, buffer, core->ep0_req_tag);

chp9_handle_usb_command调用的是文件：

./AMSS/products/7x30/core/wiredconnectivity/hsusb/core/src/jslave/core/jusb_chp9.c:2057 中的函数：

jresult_t chp9_handle_usb_command(core_t *core, void *buffer, juint8_t ep0_req_tag)

if (req_type == USB_TYPE_STANDARD)

{

rc = core_usb_command(core, ctrl_req, ep0_req_tag);

if (rc != JENOTSUP)

goto Exit;

}

case USB_REQ_GET_DESCRIPTOR:

DBG_V(DSLAVE_CORE, ("CH9: Get descriptor request reached core\n"));

rc = handle_get_descriptor(core, wValue, wIndex, wLength, ep0_req_tag);

break;



static jresult_t handle_get_descriptor(core_t *core, juint16_t wValue,

    juint16_t wIndex, juint16_t wLength, juint8_t ep0_req_tag)

    case USB_DT_DEVICE:

        rc = send_descriptor_device(core, wLength, ep0_req_tag);

        break;



static jresult_t send_descriptor_device(core_t *core, juint16_t wLength,

    juint8_t ep0_req_tag)

....

    device_desc.iProduct = core->i_product;

    device_desc.iManufacturer = core->i_manufacturer;

    device_desc.iSerialNumber = core->i_serial_number;

    device_desc.idVendor        = htole16(dev_info->vendor);

    device_desc.idProduct       = htole16(dev_info->product);





4.1.2.3　手机端fastboot 命令分析

fastboot_register("boot", cmd_boot);

fastboot_register("erase:", cmd_erase);

fastboot_register("flash:", cmd_flash);

fastboot_register("continue", cmd_continue);

fastboot_register("reboot", cmd_reboot);

fastboot_register("reboot-bootloader", cmd_reboot_bootloader);

fastboot_register("getvar:", cmd_getvar);

fastboot_register("download:", cmd_download);

4.1.2.3.1　boot

4.1.2.3.2　erase

4.1.2.3.3　flash

4.1.2.3.4　continue

4.1.2.3.5　reboot

4.1.2.3.6　reboot-bootloader

4.1.2.3.7　getvar

bootable/bootloader/lk/app/aboot/aboot.c

fastboot_publish("product", "swordfish");

fastboot_publish("kernel", "lk");

fastboot_publish("version", "0.5");

调用

sudo fastboot  getvar product

sudo fastboot  getvar kernel

sudo fastboot  getvar lk

只有 fastboot_publish 某变量以后，才能用 sudo fastboot  getvar 获取某变量的值

4.1.2.3.8　download

dd

4.1.2.3.9　update

4.1.2.3.9.1　system/core/fastboot/fastboot.c:294:

4.1.2.3.9.2　fprintf(stderr, "archive does not contain '%s'\n", name);

4.1.2.4　fastboot 模式流程分析


4.1.3 recovery 模式
4.1.3.1　recovery 模式概述
Recovery 模式是 android 手机进行系统恢复/升级的一种方式，在设置模块中通过菜单 ”恢复出厂设置”把系统能恢复到出厂默认状态（会清空用户保存在手机上的电话本，短信,电子邮件，应用程序数据等信息）；通过按键进入该模式可以把 sdcard 上的系统升级包写入到手机中实现系统软件更新。

进入 recovery 模式需要一定条件，通常以下几种方式可以进入：

1. 关机状态下通过按键进入（如：同时按 HOME + 电源 进入 recovery 模式）

2. 恢复出厂设置进入 recovery 模式

3. 系统升级进入 recovery 模式

4. 开机状态下通过重启进入



进入 recovery 分区的前提是：

手机必须存在 recovery 分区，并且烧写了 recovery.img 镜像文件。

recovery.img 镜像文件主要包括两部分：
1. 标准的内核镜像 out/target/product/msm7630_surf/kernel
2. 一个小的文件系统 out/target/product/msm7630_surf/ramdisk-recovery.img，这个小系统提供了一套字符 ui 程序，用于与用户的交互。

相对于正常启动的 ramdisk.img ，ramdisk-recovery.img 主要有如下区别：
1. init.rc 的内容进行了简化，原始文件为：bootable/recovery/etc/init.rc
2. sbin 目录下多了可执行程序 /sbin/recovery  源代码为： bootable/recovery
3. 根目录下多了 res 资源文件夹， 原始文件为 bootable/recovery/res
4. 资源文件夹下有 res/keys 文件，用于软件安装时候的文件验证
5. 根目录下文件 default.prop 的内容分别来自：

out/target/product/msm7630_surf/root/default.prop out/target/product/msm7630_surf/system/build.prop

系统升级可以通过多种方式进行：

1. 手机开机状态下，通过手机自带的升级程序连接指定的网站下载特定的经过认证的软件包进行升级，此种升级方式通常是官方自带的，软件包也是官方（手机软件提供商）提供。

2. 通过按键进入 手机的 recovery 模式，把待升级的软件包放在手机SD卡的根目录，然后选择 recovery的相应菜单进行升级。此中升级方式在 android 的破解版中很流行，目前安卓网等论坛提供的 android 手机刷机方法都推荐这种方法。



不管是主动还是被动进入 recovery 模式，其目的都是为了给手机软件升级或者恢复系统的默认设置，如果是升级系统，那么我们就得制作升级包。



4.1.3.2　软件升级包

能够被Recovery程序识别并处理的软件升级包通常包括一些特有的规则：

1）必须是标准的zip压缩包；

2）recovery脚本必须存入在META-INF\com\google\android\update-script；

3）其它的一些证书和签名信息应该都放在\META-INF\下。



升级一般通过运行升级包中的META-INF/com/google/android/update-script脚本来执行自定义升级，脚本中是一组recovery 系统能识别的控制命令，文件系统操作命令，例如write_raw_image（写FLASH分区），copy_dir（复制目录）。该包一般被下载至SDCARD

不同版本的 recovery 程序，它的 update-script 命令格式是不一样的，例如：

Cupcake 的recovery 程序支持 copy_dir 命令，1.6以后的版本已经取消。



4.1.3.3　recovery v1跟recovery v2的区别

recovery v2是从donut开始出现的， 相对于recovery v1做了不少的改动，增强了原有的功能的同时，尽量将可能变化的部分隔离出来，让各厂商可以添加自己的特性，这些体现在UI，设备功能上。 下面是从v1到v2改动的地方：

1、ui部分独立出来放到default_recovery_ui.c

界面上的修改一般改这里就可以了。



2、脚本解析器从amend切换到edify

edify支持简单的逻辑语法，脚本语法也不兼容了，不要迷恋recovery/etc/META-INF/com/google/android/update-script，这个脚本已经

不适合recovery v2。



4.1.3.4　软件升级脚本语法解析

eclair 版本的 recovery 程序支持以下脚本控制命令：

./bootable/recovery/updater/install.c

RegisterFunction("mount", MountFn);

RegisterFunction("is_mounted", IsMountedFn);

RegisterFunction("unmount", UnmountFn);

RegisterFunction("format", FormatFn);

RegisterFunction("show_progress", ShowProgressFn);

RegisterFunction("set_progress", SetProgressFn);

RegisterFunction("delete", DeleteFn);

RegisterFunction("delete_recursive", DeleteFn);

RegisterFunction("package_extract_dir", PackageExtractDirFn);

RegisterFunction("package_extract_file", PackageExtractFileFn);

RegisterFunction("symlink", SymlinkFn);

RegisterFunction("set_perm", SetPermFn);

RegisterFunction("set_perm_recursive", SetPermFn);

RegisterFunction("getprop", GetPropFn);

RegisterFunction("file_getprop", FileGetPropFn);

RegisterFunction("write_raw_image", WriteRawImageFn);

RegisterFunction("write_firmware_image", WriteFirmwareImageFn);

RegisterFunction("apply_patch", ApplyPatchFn);

RegisterFunction("apply_patch_check", ApplyPatchFn);

RegisterFunction("apply_patch_space", ApplyPatchFn);

RegisterFunction("ui_print", UIPrintFn);

RegisterFunction("run_program", RunProgramFn);



./bootable/recovery/edify/expr.c:

RegisterFunction("ifelse", IfElseFn);

RegisterFunction("abort", AbortFn);

RegisterFunction("assert", AssertFn);

RegisterFunction("concat", ConcatFn);

RegisterFunction("is_substring", SubstringFn);

RegisterFunction("stdout", StdoutFn);

RegisterFunction("sleep", SleepFn);

RegisterFunction("less_than_int", LessThanIntFn);

RegisterFunction("greater_than_int", GreaterThanIntFn);

RegisterFunction(const char* name, Function fn);



4.1.3.4.1.1　mount

mount 命令解析

高通 7630 分区信息：

$ cat /proc/mtd

dev:    size   erasesize  name

mtd0: 00500000 00040000 "boot"

mtd1: 06900000 00040000 "system"

mtd2: 00500000 00040000 "cache"

mtd3: 13480000 00040000 "userdata"

mtd4: 00300000 00040000 "persist"

mtd5: 00500000 00040000 "recovery"

语法为：Mount(type, location, mount_point)

如果 type 为 "MTD", 那么 location 可以为设备分区名称,默认该分区以 "yaffs" 的方式挂载，如挂载 system 分区：

mount("MTD", "system", "/system")

如果 type 不为 "MTD" ，那么必须写明具体的文件系统类型， location 必须填设备名称，例如是sdcard ，那么 type 为 "vfat"  设备名称 /dev/block/mmcblk0 ：

mount("vfat","/dev/block/mmcblk0","sdcard")





system 分区挂载过程：

mount("MTD", "system", "/system")

mkdir("system", 0755);

mtd = mtd_find_partition_by_name("system");

mtd_mount_partition(mtd, "/system", "yaffs2", 0 /* rw */)

const unsigned long flags = MS_NOATIME | MS_NODEV | MS_NODIRATIME;

sprintf(devname, "/dev/block/mtdblock%d", mtd->device_index);

mount(/dev/block/mtdblock, "/system", "yaffs2" , flags, NULL);

如果 mtd_mount_partition 第四个参数不为 0,那么以只读方式挂载：

mount(devname, mount_point, filesystem, flags | MS_RDONLY, 0)

平时无法写系统分区，是因为 init 在读取 init.rc 后，默认挂载system 为只读

系统更新可以写system分区，是因为这里把该分区挂载成了可读写



sdcard 挂载过程：

mount("vfat","/dev/block/mmcblk0","sdcard")

mkdir("system", 0755);

mount("/dev/block/mmcblk0", "/sdcard", "vfat",MS_NOATIME | MS_NODEV | MS_NODIRATIME, "")



4.1.3.4.1.2　getprop

从系统环境中获取属性值

如：



4.1.3.4.1.3　file_getprop

语法：

从属性文件中获取属性值，可以为android系统中的任意属性配置文件

例如：

getprop ro.bootloader



4.1.3.4.1.4　assert

语法：

此命令相当于程序中的断言

RegisterFunction("assert", AssertFn); 它执行的是 AssertFn 函数

assert() 函数用法

assert宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行。

4.1.3.4.1.5　format

格式化分区

format BOOT:

format SYSTEM:

4.1.3.4.1.6　apply_patch_check

4.1.3.4.1.7　apply_patch_space

4.1.3.4.1.8　apply_patch

apply_patch_check

apply_patch_space

apply_patch

以上命令在手机端都是执行函数 ApplyPatchFn

./bootable/recovery/updater/install.c

RegisterFunction("apply_patch", ApplyPatchFn);

RegisterFunction("apply_patch_check", ApplyPatchFn);

RegisterFunction("apply_patch_space", ApplyPatchFn);

语法：

apply_patch(srcfile, tgtfile, tgtsha1, tgtsize, sha1:patch, ...)

apply_patch_check(file, sha1, ...)

apply_patch_space(bytes)



apply_patch_check

对升级包进行验证，patch 包是否正确，如果不正确，异常退出

例如：

assert(apply_patch_check("/system/app/Settings.apk", "682e475c6411bb454d5999f6abff50ca8eb373f4", "196c0891869ec4b93a738519daf30a56a96c570d"));



apply_patch_space

检查可用的存储空间

bootable/recovery/updater/install.c:

char* ApplyPatchFn(const char* name, State* state, int argc, Expr* argv[])



对软件包进行打patch

apply_patch("/system/app/Settings.apk", "-",

            682e475c6411bb454d5999f6abff50ca8eb373f4, 1485651,

            "196c0891869ec4b93a738519daf30a56a96c570d:/tmp/patchtmp/system/app/Settings.apk.p");

4.1.3.4.1.9　package_extract_file

pac

package_extract_file("system/app/Settings.apk", "/system/app/ Settings.apk");

4.1.3.4.1.10　ui_print

语法：

ui_print("Verifying current system...");

在屏幕上打印信息： 正在验证当前系统...

4.1.3.4.1.11　META-INF/com/google/android/update-script 脚本分析

下面以一个升级包的实例分析系统升级具体做了些什么？

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

因为要更新系统内容,所以首先挂载系统分区为可读写

mount("MTD", "system", "/system");

检测属性文件 /system/build.prop 中的属性值

assert(file_getprop("/system/build.prop", "ro.build.fingerprint") == "google/passion/passion/mahimahi:2.1/ERD79/22607:user/release-keys" ||

       file_getprop("/system/build.prop", "ro.build.fingerprint") == "google/passion/passion/mahimahi:2.1-update1/ERE27/24178:user/release-keys");

检测系统运行过程中的属性值

assert(getprop("ro.product.device") == "passion" ||

       getprop("ro.build.product") == "passion");

assert(getprop("ro.bootloader") == "0.33.2012" ||

       getprop("ro.bootloader") == "0.33.0012");

ui_print("Verifying current system...");

show_progress(0.100000, 0);

set_progress(0.001918);

检测 patch 需要的空间

assert(apply_patch_space(6741176));

ui_print("Unpacking patches...");

释放 patch 目录下的内容到 /tmp/patchtmp

package_extract_dir("patch", "/tmp/patchtmp");

删除不需要的文件...

ui_print("Removing unneeded files...");

删除不需要的文件...

delete("/system/app/GoogleGoggles.apk", "/system/app/GoogleGoggles.odex",

       "/system/framework/svc.jar",

       "/system/recovery.img");

show_progress(0.800000, 0);

开始打补丁...

ui_print("Patching system files...");

升级设置程序的apk文件

apply_patch("/system/app/Settings.apk", "-",

            682e475c6411bb454d5999f6abff50ca8eb373f4, 1485651,

            "196c0891869ec4b93a738519daf30a56a96c570d:/tmp/patchtmp/system/app/Settings.apk.p");

set_progress(1.000000);

package_extract_dir("recovery", "/system");

show_progress(0.100000, 10);

ui_print("Unpacking new files...");

释放 system 目录下的内容到系统的 /system 目录

package_extract_dir("system", "/system");

ui_print("Symlinks and permissions...");

set_perm_recursive(0, 0, 0755, 0644, "/system");

set_perm_recursive(0, 2000, 0755, 0755, "/system/bin");

set_perm(0, 3003, 02755, "/system/bin/netcfg");

set_perm(0, 3004, 02755, "/system/bin/ping");

set_perm(1001, 1005, 0444, "/system/etc/AudioPara4.csv");

set_perm_recursive(1002, 1002, 0755, 0440, "/system/etc/bluez");

set_perm(0, 0, 0755, "/system/etc/bluez");

set_perm(1002, 1002, 0440, "/system/etc/dbus.conf");

set_perm(1014, 2000, 0550, "/system/etc/dhcpcd/dhcpcd-run-hooks");

set_perm(0, 2000, 0550, "/system/etc/init.goldfish.sh");

set_perm(0, 0, 0544, "/system/etc/install-recovery.sh");

set_perm_recursive(0, 0, 0755, 0555, "/system/etc/ppp");

ui_print("Writing radio image...");

write_firmware_image("PACKAGE:radio.img", "radio");

unmount 系统目录

unmount("/system");show_progress(0.100000, 10);





bootable/recovery/updater/updater.c

#define SCRIPT_NAME "META-INF/com/google/android/updater-script"



bootable/recovery/install.c:37:

#define ASSUMED_UPDATE_BINARY_NAME  "META-INF/com/google/android/update-binary"

mzFindZipEntry(zip, ASSUMED_UPDATE_BINARY_NAME);



int status = install_package(SDCARD_PACKAGE_FILE);

bootable/recovery/recovery.c:487:

status = install_package(update_package);

通过 adb 登录 G1 的分区信息：

/ # cat /proc/mtd

dev:    size   erasesize  name

mtd0: 00040000 00020000 "misc"

mtd1: 00500000 00020000 "recovery"

mtd2: 00280000 00020000 "boot"

mtd3: 05a00000 00020000 "system"

mtd4: 01e00000 00020000 "cache"

mtd5: 059c0000 00020000 "userdata"

高通 7630 分区信息：

$ cat /proc/mtd

dev:    size   erasesize  name

mtd0: 00500000 00040000 "boot"

mtd1: 06900000 00040000 "system"

mtd2: 00500000 00040000 "cache"

mtd3: 13480000 00040000 "userdata"

mtd4: 00300000 00040000 "persist"

mtd5: 00500000 00040000 "recovery"

这些信息是在文件 ./bootable/bootloader/lk/target/msm7630_surf/init.c 中指定的：

static struct ptentry board_part_list[] = {

    {

       .start = 0,

       .length = 20  /* 5MB */,

       .name = "boot",

    },

    {

       .start = 20,

       .length = 380 /* 95MB */,

       .name = "system",

    },

    {

       .start = 400,

       .length = 20 /* 5MB */,

       .name = "cache",

    },

    {

       .start = 420,

       .length = VARIABLE_LENGTH,

       .name = "userdata",

    },

    {

       .start = DIFF_START_ADDR,

       .length = 4 /* 1MB */,

       .name = "persist",

    },

    {

       .start = DIFF_START_ADDR,

       .length = 20 /* 5MB */,

       .name = "recovery",

    },

};

系统支持哪些flash的定义在文件 bootable/bootloader/lk/platform/msm_shared/nand.c 中：

static struct flash_identification supported_flash[] =

{

    /* Flash ID   ID Mask Density(MB)  Wid Pgsz   Blksz  oobsz onenand Manuf */

    {0x00000000, 0xFFFFFFFF,    0, 0, 0,     0,  0, 0}, /*ONFI*/

    {0x1500aaec, 0xFF00FFFF, (256<<20), 0, 2048, (2048<<6), 64, 0}, /*Sams*/

    {0x5500baec, 0xFF00FFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Sams*/

    {0x1500aa98, 0xFFFFFFFF, (256<<20), 0, 2048, (2048<<6), 64, 0}, /*Tosh*/

    {0x5500ba98, 0xFFFFFFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Tosh*/

    {0xd580b12c, 0xFFFFFFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Micr*/

    {0x5590bc2c, 0xFFFFFFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Micr*/

    {0x1580aa2c, 0xFFFFFFFF, (256<<20), 0, 2048, (2048<<6), 64, 0}, /*Micr*/

    {0x5580baad, 0xFFFFFFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Hynx*/

    {0x5510baad, 0xFFFFFFFF, (256<<20), 1, 2048, (2048<<6), 64, 0}, /*Hynx*/

    {0x004000ec, 0xFFFFFFFF, (256<<20), 0, 2048, (2048<<6), 64, 1}, /*Sams*/

    {0x005c00ec, 0xFFFFFFFF, (256<<20), 0, 2048, (2048<<6), 64, 1}, /*Sams*/

    {0x005800ec, 0xFFFFFFFF, (256<<20), 0, 2048, (2048<<6), 64, 1}, /*Sams*/

    {0x6600bcec, 0xFF00FFFF, (512<<20), 1, 4096, (2048<<6), 128, 0}, /*Sams*/

    /* Note: Width flag is 0 for 8 bit Flash and 1 for 16 bit flash    */

    /* Note: Onenand flag is 0 for NAND Flash and 1 for OneNAND flash   */

    /* Note: The First row will be filled at runtime during ONFI probe  */

};



pbl -> dbl -> osbl -> appsbl ->进入 recovery 模式

系统运行到 appsbl 阶段才开始是否进入 recovery 模式的处理。





int device_toggle_display(volatile char* key_pressed, int key_code) {

    return key_code == KEY_HOME;

}







check if nand misc partition has boot message. If yes, fill argc/argv.

If no, get arguments from /cache/recovery/command, and fill argc/argv.



读取 /cache/recovery/command 文件的内容

FILE *fp = fopen_root_path(COMMAND_FILE, "r");



G1 的分区信息：

# cat /proc/mtd

dev:    size   erasesize  name

mtd0: 00040000 00020000 "misc"

mtd1: 00500000 00020000 "recovery"

mtd2: 00280000 00020000 "boot"

mtd3: 05a00000 00020000 "system"

mtd4: 01e00000 00020000 "cache"

mtd5: 059c0000 00020000 "userdata"



recovery_init() 调用的是文件 bootable/bootloader/lk/app/aboot/recovery.c中的函数：

/* Read and write the bootloader command from the "misc" partition.

 * These return zero on success.

 */

从 misc 分区读取

misc         0.25MB：是存放开机画面的分区,开机时那个白色的大G1字样(默认为白色"T-mobile G1")

recovery    5MB：是G1的恢复模式镜像区(即开机按Home+End进入的界面)

Boot         2.5MB：Linux操作系统内核和ramdisk镜像一起打包后存放的分区

system      90MB：是system.img存放的分区，启动后此镜像挂载在根目录下的system文件夹中。

cache        30MB：是缓存临时文件夹，挂载为/cache目录，据说是除了T-mobile的OTA更新外，别无用处。

userdata     89.75MB：用户安装的软件以及各种数据，挂载为/data目录

msm_nand     256MB：代表整个Nand Flash，不是一个分区。但是所有的分区加起来才217.5MB，小于256MB，说明还有剩余的flash空间。（此分区在大部分机上不存在，不影响使用）
恢复模式的 init.rc 文件很简单，只启动了 recovery 和 adbd 服务

on init

    export PATH /sbin

    export ANDROID_ROOT /system

    export ANDROID_DATA /data

    export EXTERNAL_STORAGE /sdcard

    symlink /system/etc /etc

    mkdir /sdcard

    mkdir /system

    mkdir /data

    mkdir /cache

    mount /tmp /tmp tmpfs

on boot

    ifup lo

    hostname localhost

    domainname localdomain

    class_start default

service recovery /sbin/recovery

service adbd /sbin/adbd recovery

on property:persist.service.adb.enable=1

    start adbd

on property:persist.service.adb.enable=0

    stop adbd

4.1.3.4.2　Recovery 模式中 install_package 函数解析

设置模块中系统升级和sd卡方式软件升级会调用 install_package 函数

int status = install_package(SDCARD_PACKAGE_FILE);

status = install_package(update_package);

bootable/recovery/install.c:322:install_package(const char *root_path)

int install_package(const char *root_path)

{

    ui_set_background(BACKGROUND_ICON_INSTALLING);

    ui_print("Finding update package...\n");

    ui_show_indeterminate_progress();

    LOGI("Update location: %s\n", root_path);



    if (ensure_root_path_mounted(root_path) != 0) {

        LOGE("Can't mount %s\n", root_path);

        return INSTALL_CORRUPT;

    }



    char path[PATH_MAX] = "";

    if (translate_root_path(root_path, path, sizeof(path)) == NULL) {

        LOGE("Bad path %s\n", root_path);

        return INSTALL_CORRUPT;

    }



    ui_print("Opening update package...\n");

    LOGI("Update file path: %s\n", path);



int numKeys;

# #define PUBLIC_KEYS_FILE "/res/keys"

    RSAPublicKey* loadedKeys = load_keys(PUBLIC_KEYS_FILE, &numKeys);

    if (loadedKeys == NULL) {

        LOGE("Failed to load keys\n");

        return INSTALL_CORRUPT;

    }

    LOGI("%d key(s) loaded from %s\n", numKeys, PUBLIC_KEYS_FILE);



    // Give verification half the progress bar...

    ui_print("Verifying update package...\n");

    ui_show_progress(

            VERIFICATION_PROGRESS_FRACTION,

            VERIFICATION_PROGRESS_TIME);



    int err;

    err = verify_file(path, loadedKeys, numKeys);

    free(loadedKeys);

    LOGI("verify_file returned %d\n", err);

    if (err != VERIFY_SUCCESS) {

        LOGE("signature verification failed\n");

        return INSTALL_CORRUPT;

    }



    /* Try to open the package.

     */

    ZipArchive zip;

    err = mzOpenZipArchive(path, &zip);

    if (err != 0) {

        LOGE("Can't open %s\n(%s)\n", path, err != -1 ? strerror(err) : "bad");

        return INSTALL_CORRUPT;

    }



    /* Verify and install the contents of the package.

     */

    int status = handle_update_package(path, &zip);

    mzCloseZipArchive(&zip);

    return status;

}



4.1.3.5　Recovery 流程分析

4.1.3.5.1　恢复出厂设置

addPreferencesFromResource(R.xml.privacy_settings);

菜单描述在文件 packages/apps/Settings/res/xml/privacy_settings.xml

<PreferenceScreen

                android:title="@string/master_clear_title"

                android:summary="@string/master_clear_summary">

            <intent android:action="android.intent.action.MAIN"

                    android:targetPackage="com.android.settings"

                    android:targetClass="com.android.settings.MasterClear" />

</PreferenceScreen>

恢复出厂设置执行的是 activiy MasterClear

packages/apps/Settings/src/com/android/settings/MasterClear.java

-> service.masterClear() 调用文件：

frameworks/base/services/java/com/android/server/FallbackCheckinService.java 中的函数：

public void masterClear()

RecoverySystem.rebootAndWipe()

bootCommand("--wipe_data");

    把内容 "--wipe_data" 写入到文件 /cache/recovery/command 中

Power.reboot("recovery");

补充：

private static File RECOVERY_DIR = new File("/cache/recovery");

private static File COMMAND_FILE = new File(RECOVERY_DIR, "command");

private static File LOG_FILE = new File(RECOVERY_DIR, "log");

命令文件： /cache/recovery/command

log文件/cache/recovery/log



Install: out/target/product/msm7630_surf/root/sbin/adbd

target Non-prelinked: init (out/target/product/msm7630_surf/symbols/init)

target Non-prelinked: adbd (out/target/product/msm7630_surf/symbols/sbin/adbd)



4.1.3.5.2　系统更新流程

在设置模块 -> 关于手机 里面预设了 “系统更新” 的菜单项配置， 如果安装的软件包里面有过滤器为：

<intent-filter>

<action android:name="android.settings.SYSTEM_UPDATE_SETTINGS" />

    <category android:name="android.intent.category.DEFAULT" />

</intent-filter>

的软件包，那么才会显示 ”系统更新” 的菜单项，否则没有该项菜单。此类菜单的处理方式通过文件：

packages/apps/Settings/src/com/android/settings/Utils.java 中的函数

updatePreferenceToSpecificActivityOrRemove 来实现。

Android 源代码中为系统升级包预留了位置  packages/apps/Updater/ 但是没有具体的源代码，这个源代码可以由厂商自己开发，这样做是为了方便厂商灵活的定制系统升级方式。

在 HTC 手机中该软件包通常为： SystemUpdater.apk 我们可以从他们的手机中提取。



流程应该为：

应用模块中调用：service.systemUpdate() 实际调用的是文件：

frameworks/base/services/java/com/android/server/FallbackCheckinService.java

中的函数：

public void systemUpdate()

{

    RecoverySystem.rebootAndUpdate();

}



frameworks/base/services/java/com/android/server/SystemUpdateReceiver.java

public class SystemUpdateReceiver extends BroadcastReceiver {

    private static final String TAG = "MasterClear";

    @Override

    public void onReceive(Context context, Intent intent) {

        ICheckinService service =

            ICheckinService.Stub.asInterface(

                ServiceManager.getService("checkin"));

              ...

                service.systemUpdate();

              ...

}

frameworks/base/core/java/android/os/ICheckinService.aidl

中加函数函数声明：

void systemUpdate();

rebootAndUpdate 函数应该为：

frameworks/base/core/java/com/android/internal/os/RecoverySystem.java

    public static void rebootAndUpdate(File update) throws IOException {

        String path = update.getCanonicalPath();

        if (path.startsWith("/cache/")) {

            path = "CACHE:" + path.substring(7);

        } else if (path.startsWith("/data/")) {

            path = "DATA:" + path.substring(6);

        } else {

            throw new IllegalArgumentException(

                    "Must start with /cache or /data: " + path);

        }

        bootCommand("--update_package=" + path);

}

getCanonicalPath() 获取文件的绝对路径

getAbsolutePath()和getCanonicalPath()的不同

public static void test2() throws Exception{

        File file = new File("../update.zip");

        System.out.println(file.getAbsolutePath());

        System.out.println(file.getCanonicalPath());

    }

得到的结果

/data/bin/../update.zip

/data/update.zip



bootCommand("--update_package=" + path) 会写命令：

--update_package= CACHE:/cache/xxxx  或者 --update_package= DATA:/data/xxxx 到命令文件：

/cache/recovery/command



重启以后，会检查命令文件 /cache/recovery/command

    // --- if that doesn't work, try the command file

    if (*argc <= 1) {

        FILE *fp = fopen_root_path(COMMAND_FILE, "r");

        if (fp != NULL) {

            char *argv0 = (*argv)[0];

            *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);

            (*argv)[0] = argv0;  // use the same program name



            char buf[MAX_ARG_LENGTH];

            for (*argc = 1; *argc < MAX_ARGS; ++*argc) {

                if (!fgets(buf, sizeof(buf), fp)) break;

                (*argv)[*argc] = strdup(strtok(buf, "\r\n"));  // Strip newline.

            }



            check_and_fclose(fp, COMMAND_FILE);

            LOGI("Got arguments from %s\n", COMMAND_FILE);

        }

    }

static const struct option OPTIONS[] = {

  { "send_intent", required_argument, NULL, 's' },

  { "update_package", required_argument, NULL, 'u' },

  { "wipe_data", no_argument, NULL, 'w' },

  { "wipe_cache", no_argument, NULL, 'c' },

  { NULL, 0, NULL, 0 },

};



while ((arg = getopt_long(argc, argv, "", OPTIONS, NULL)) != -1) {

        switch (arg) {

        case 'p': previous_runs = atoi(optarg); break;

        case 's': send_intent = optarg; break;

        case 'u': update_package = optarg; break;

        case 'w': wipe_data = wipe_cache = 1; break;

        case 'c': wipe_cache = 1; break;

        case '?':

            LOGE("Invalid command argument\n");

            continue;

        }

}

因为  /cache/recovery/command 内容为 --update_package= CACHE:/cache/xxxx 所以 update_package 不为空，



如果不为空，那么将会执行命令 install_package 对包进行安装

if (update_package != NULL) {

status = install_package(update_package);

升级包具体做什么内容，完全由脚本： META-INF/com/google/android/update-script 和 包的内容所决定。具体内容参考章节 软件升级包以及软件升级脚本语法解析
4.1.3.5.3　通过sd卡实现刷机
首先通过特定按键进入 recovery 模式，符合规定的软件升级包命名为 update.zip 放在sd卡根目录，

具体升级过程由升级包中的脚本决定，过程跟系统升级类似

static const char *SDCARD_PACKAGE_FILE = "SDCARD:update.zip";

int status = install_package(SDCARD_PACKAGE_FILE);





fastboot 模式下烧写 recovery 分区



手动打包 ramdisk-recovery.img

out/host/linux-x86/bin/mkbootfs out/target/product/msm7630_surf/recovery/root | out/host/linux-x86/bin/minigzip > out/target/product/msm7630_surf/ramdisk-recovery.img

MKBOOTFS=out/host/linux-x86/bin/mkbootfs

ROOT_DIR=out/target/product/$TARGET_PRODUCT/recovery/root

MINIGZIP=out/host/linux-x86/bin/minigzip

OUTPUT=out/target/product/$TARGET_PRODUCT/ramdisk-recovery.img

$MKBOOTFS  $ROOT_DIR | $MINIGZIP > $OUTPUT

手动生成 recovery.img

out/host/linux-x86/bin/mkbootimg  --kernel out/target/product/msm7630_surf/kernel  --ramdisk out/target/product/msm7630_surf/ramdisk-recovery.img --pagesize 4096 --cmdline "console=ttyDCC0 androidboot.hardware=qcom" --base 0x00200000 --output out/target/product/msm7630_surf/recovery.img

MKBOOTIMG=out/host/linux-x86/bin/mkbootimg

KERNEL=out/target/product/$TARGET_PRODUCT/kernel

RAMDISK=out/target/product/$TARGET_PRODUCT/ramdisk-recovery.img

PAGESIZE=4096

CMDLINE="console=ttyDCC0 androidboot.hardware=qcom"

BASE=0x00200000

OUTPUT=out/target/product/$TARGET_PRODUCT/recovery.img

$MKBOOTIMG --kernel $KERNEL --ramdisk $RAMDISK --pagesize $PAGESIZE --cmdline "$CMDLINE" --base $BASE --output $OUTPUT

sudo fastboot-eclair2.2 flash recovery out/target/product/msm7630_surf/recovery.img



4.1.3.6　系统升级包案例分析

可以改善G网通话质量的update.zip

http://bbs.91android.com/thread-221208-1-1.html

从此告别延迟，来电痛快接听

http://www.nduoa.com/android_gphone_forum-viewthread-tid-4735-from-home.html

解决G3误刷SPL无法开机卡在RA界面

http://android.tgbus.com/lab/break/201005/307573.shtml



4.1.4 工程模式(HBoot) 模式

工程模式又叫 hboot 模式 ，它在 appsboot.mbn 中进行处理，该部分内容暂略

4.1.4.1　HBOOT降级方法

Revert HBOOT 0.35.0017 to 0.33.0012

http://forum.xda-developers.com/showthread.php?t=726258





exec ("perl Program_Factory_Image.pl $com_port $timeout $factory_file");



vim system/core/rootdir/etc/init.qcom.rc

symlink /persist/qcom_wlan_nv.bin /etc/firmware/wlan/qcom_wlan_nv.bin

make  out/target/product/msm7630_surf/persist.img

out/host/linux-x86/bin/mkyaffs2image -f -s 4096 out/target/product/msm7630_surf/persist out/target/product/msm7630_surf/persist.img

chmod a+r out/target/product/msm7630_surf/persist.img



# cat /proc/mtd

dev:    size   erasesize  name

mtd0: 00480000 00020000 "recovery"

mtd1: 00480000 00020000 "boot"

mtd2: 00060000 00020000 "misc"

mtd3: 00c80000 00020000 "cache"

mtd4: 09600000 00020000 "system"

mtd5: 127e0000 00020000 "userdata"

mtd6: 00180000 00020000 "persist"

cat /dev/mtd/mtd0 > /sdcard/recovery.img

cat /dev/mtd/mtd1 > /sdcard/boot.img

cat /dev/mtd/mtd2 > /sdcard/misc.img

cat /dev/mtd/mtd3 > /sdcard/cache.img

cat /dev/mtd/mtd4 > /sdcard/system.img

cat /dev/mtd/mtd5 > /sdcard/userdata.img

cat /dev/mtd/mtd6 > /sdcard/persist.img



./external/qemu/android/main.c:1962:

char*  out = getenv("ANDROID_PRODUCT_OUT");

external/qemu/android/avd/info.c:1176:

#define  PREBUILT_KERNEL_PATH   "prebuilt/android-arm/kernel/kernel-qemu"

external/qemu/android/main.c:2017:

opts->system = _getSdkSystemImage(opts->sysdir, "-image", "system.img");

external/qemu/android/main.c:2022:

opts->kernel = _getSdkSystemImage(opts->sysdir, "-kernel", "kernel-qemu");

external/qemu/android/main.c:2027:

opts->ramdisk = _getSdkSystemImage(opts->sysdir, "-ramdisk", "ramdisk.img");





external/qemu/hw/goldfish_mmc.c:503:    s->dev.name = "goldfish_mmc";



如果不存在文件 prebuilt/android-arm/kernel/kernel-qemu 运行模拟器会弹出错误：

emulator: ERROR: bad workspace: cannot find prebuilt kernel in: prebuilt/android-arm/kernel/kernel-qemu





android_avdParams->skinName     = opts->skin;

android_avdParams->skinRootPath = opts->skindir;

android_avdInfo = avdInfo_newForAndroidBuild(

                            android_build_root,

                            android_build_out,

                            android_avdParams );



avdInfo_newForAndroidBuild -> _getBuildImagePaths ->

#define  PREBUILT_KERNEL_PATH   "prebuilt/android-arm/kernel/kernel-qemu"

return qemu_main(n, args);

4.2 如何制作升级包 update.zip

升级包可以按照自己的需求定制，只要按照它的制作规范生成就行。

一个小的升级包可能仅仅是为了替换系统分区的某个应用(system/app/Settings.apk)或者某个配置文件(system/etc/apns-conf.xml) ,或者仅仅是执行某项操作（删除system/app/FM.apk）。这种简单的升级包我们完全可以通过手动的方式创建、添加然后再签名。但是复杂一点的升级包，如系统升级，这个可能涉及到整个系统的应用，甚至framework层，可能会涉及到数百个文件，手动显然是不现实的，而且完全替换会花费过多的资源和时间，所以这个过程我们将用源代码中自带的一套工具来完成。

4.2.1 手动制作升级包

略

4.2.2 自动制作升级包

./build/core/Makefile

# -----------------------------------------------------------------

# OTA update package

ifneq ($(TARGET_SIMULATOR),true)

ifneq ($(TARGET_PRODUCT),sdk)

#ifneq ($(TARGET_DEVICE),generic)

ifneq ($(TARGET_PRODUCT),generic)



name := $(TARGET_PRODUCT)

ifeq ($(TARGET_BUILD_TYPE),debug)

  name := $(name)_debug

endif

name := $(name)-ota-$(FILE_NAME_TAG)



INTERNAL_OTA_PACKAGE_TARGET := $(PRODUCT_OUT)/$(name).zip



ifeq ($(TARGET_USERIMAGES_USE_EXT2),true)

INTERNAL_OTA_PACKAGE_TARGET_MMC := $(PRODUCT_OUT)/$(name)_mmc.zip

else

INTERNAL_OTA_PACKAGE_TARGET_MMC :=

endif



$(INTERNAL_OTA_PACKAGE_TARGET): KEY_CERT_PAIR := $(DEFAULT_KEY_CERT_PAIR)

$(INTERNAL_OTA_PACKAGE_TARGET_MMC): KEY_CERT_PAIR := $(DEFAULT_KEY_CERT_PAIR)

ifeq ($(TARGET_OTA_SCRIPT_MODE),)

# default to "auto"

$(INTERNAL_OTA_PACKAGE_TARGET): scriptmode := auto

$(INTERNAL_OTA_PACKAGE_TARGET_MMC): scriptmode := auto

else

$(INTERNAL_OTA_PACKAGE_TARGET): scriptmode := $(TARGET_OTA_SCRIPT_MODE)

$(INTERNAL_OTA_PACKAGE_TARGET_MMC): scriptmode := $(TARGET_OTA_SCRIPT_MODE)

endif



$(INTERNAL_OTA_PACKAGE_TARGET): $(BUILT_TARGET_FILES_PACKAGE) $(OTATOOLS)

    @echo "Package OTA: $@"

    $(hide) ./build/tools/releasetools/ota_from_target_files \

       -m $(scriptmode) \

       -p $(HOST_OUT) \

       -k $(KEY_CERT_PAIR) \

       $(BUILT_TARGET_FILES_PACKAGE) $@



.PHONY: otapackage

otapackage: $(INTERNAL_OTA_PACKAGE_TARGET)



$(INTERNAL_OTA_PACKAGE_TARGET_MMC): $(BUILT_TARGET_FILES_PACKAGE) $(OTATOOLS)

    @echo "Package OTA: $@"

    $(hide) ./build/tools/releasetools/ota_from_target_files \

       -m $(scriptmode) \

       -p $(HOST_OUT) \

       -t MMC\

       -k  $(KEY_CERT_PAIR) \

       $(BUILT_TARGET_FILES_PACKAGE) $@



.PHONY: otapackage_mmc

otapackage_mmc: $(INTERNAL_OTA_PACKAGE_TARGET_MMC)



在 ./build/core/Makefile 文件中，会通过 ota_from_target_files 命令生成系统安装包。

./build/tools/releasetools/ota_from_target_files

  if OPTIONS.incremental_source is None:

    WriteFullOTAPackage(input_zip, output_zip)

  else:

    print "unzipping source target-files..."

    OPTIONS.source_tmp = common.UnzipTemp(OPTIONS.incremental_source)

    source_zip = zipfile.ZipFile(OPTIONS.incremental_source, "r")

WriteIncrementalOTAPackage(input_zip, source_zip, output_zip)

在源代码中，默认情况下OPTIONS.incremental_source 是None，所以调用 WriteFullOTAPackage 生成的包是完全更新的升级包，如果我们通过参数 "-i", "--incremental_from" 指定基础版本的 target_files 文件（通常对应：out/target/product/msm7630_surf/obj/PACKAGING/target_files_intermediates/msm7630_surf-target_files-eng.gphone.zip），那么将会调用 WriteIncrementalOTAPackage 生成带patch的升级包（默认路径为：out/target/product/msm7630_surf/msm7630_surf-ota-eng.gphone.zip）





4.3 Android 签名机制

升级包制作完成以后，我们必须还要为其进行签名，否则升级认证过程会出错。

Android提供了为jar/zip文件签名的程序signapk.jar 。它的用法如下：

Usage: signapk publickey.x509[.pem] privatekey.pk8 input.jar output.jar

第一个参数是公钥，即前面第二步产生的testkey.x509.pem。

第二个参数是私钥，即前面第三步产生的testkey.pk8。

第三个参数是要签名的文件。

第四个参数是输出的文件（即签名后的文件）。

例如：

java -jar signapk.jar testkey.x509.pem testkey.pk8 update.zip update-signed.zip

signapk.jar         out/host/linux-x86/framework/signapk.jar

testkey.x509.pem   build/target/product/security/platform.x509.pem

testkey.pk8         build/target/product/security/platform.pk8

注意：

我们进行系统进行升级，必须使用源代码中的signapk.jar testkey.x509.pem testkey.pk8

4.4 android 文件系统权限概述

关于Android系统中system.img和data.img中文件系统的权限设置

http://blog.chinaunix.net/u3/103613/showart_2218437.html

{ 04755, AID_ROOT,      AID_ROOT,      "system/bin/fota" },



文件系统中各目录和文件的默认属性定义：

./system/core/include/private/android_filesystem_config.h

#define AID_ROOT             0  /* traditional unix root user */



#define AID_SYSTEM        1000  /* system server */



#define AID_RADIO         1001  /* telephony subsystem, RIL */

#define AID_BLUETOOTH     1002  /* bluetooth subsystem */

#define AID_GRAPHICS      1003  /* graphics devices */

#define AID_INPUT         1004  /* input devices */

#define AID_AUDIO         1005  /* audio devices */

#define AID_CAMERA        1006  /* camera devices */

#define AID_LOG           1007  /* log devices */

#define AID_COMPASS       1008  /* compass device */

#define AID_MOUNT         1009  /* mountd socket */

#define AID_WIFI          1010  /* wifi subsystem */

#define AID_ADB           1011  /* android debug bridge (adbd) */

#define AID_INSTALL       1012  /* group for installing packages */

#define AID_MEDIA         1013  /* mediaserver process */

#define AID_DHCP          1014  /* dhcp client */

#define AID_SDCARD_RW     1015  /* external storage write access */

#define AID_VPN           1016  /* vpn system */

#define AID_KEYSTORE      1017  /* keystore subsystem */

#define AID_FM_RADIO   1018  /* FM radio */

#define AID_LOCATION      1019  /* Location */



#define AID_SHELL         2000  /* adb and debug shell user */

#define AID_CACHE         2001  /* cache access */

#define AID_DIAG          2002  /* access to diagnostic resources */



/* The 3000 series are intended for use as supplemental group id's only.

 * They indicate special Android capabilities that the kernel is aware of. */

#define AID_NET_BT_ADMIN  3001  /* bluetooth: create any socket */

#define AID_NET_BT        3002  /* bluetooth: create sco, rfcomm or l2cap sockets */

#define AID_INET          3003  /* can create AF_INET and AF_INET6 sockets */

#define AID_NET_RAW       3004  /* can create raw INET sockets */

#define AID_NET_ADMIN     3005  /* can configure interfaces and routing tables. */

#define AID_QCOM_ONCRPC   3006  /* can read/write /dev/oncrpc/* files .*/



#define AID_MISC          9998  /* access to misc storage */

#define AID_NOBODY        9999



#define AID_APP          10000 /* first app user */



{ 06755, AID_ROOT,      AID_ROOT,      "system/xbin/su" },

{ 04755, AID_ROOT,      AID_ROOT,      "system/bin/fota" },



获取手机的 root 权限

ui_print("Rooting your phone...");

package_extract_file("system/bin/su", "/system/bin/su");

set_perm(0, 0, 04755, "/system/bin/su");



我看到F大的updater-script里面有这么一句话。是不是可以理解为如果要ROOT只要把SU复制到SYSTEM/BIN/目录下并且设置属性为04755就行了？

http://code.google.com/p/superuser/source/checkout





















4.4.1 获取手机root权限
root 权限
root 权限主要由 ramdisk 中 ./default.prop 文件决定

====

ro.secure=0
ro.debuggable=0
persist.service.adb.enable=1
那么默认进入 root 模式
====

ro.secure=1
ro.debuggable=0
persist.service.adb.enable=1

默认进入的是 shell 模式，因为 ro.debuggable=0

所以 sudo adb root 弹出错误：

adbd cannot run as root in production builds

====

ro.secure=0

ro.debuggable=1

persist.service.adb.enable=1

默认进入的是 shell 模式 ，因为 ro.debuggable=1

所以可以通过 sudo adb root 切换到 root 权限模式

====

ro.secure=1

ro.debuggable=1

persist.service.adb.enable=1

默认进入的是 shell 模式 ，因为 ro.debuggable=1
所以可以通过 sudo adb root 切换到 root 权限模式
=================================================
如果当前不是 user ，那么adb调试模式默认开启





三星Galaxy s i9000获取root权限详细教程

http://android.sj.91.com/content/2010-07-14/20100714181645219.shtml

https://github.com/cmsgs/android_device_samsung_galaxys/blob/master/extract-files.sh

冒死体验～～～Root Explorer文件管理

http://www.hiapk.com/bbs/thread-7350-1-1.html

获取root是否成功，可以通过 Root Explorer 软件查看

4.4.2 adb默认权限分析

int adb_main(int is_daemon)

{

    ...

    property_get("ro.kernel.qemu", value, "");

    if (strcmp(value, "1") != 0) {

        property_get("ro.secure", value, "");

        if (strcmp(value, "1") == 0) {

            // don't run as root if ro.secure is set...

            secure = 1;

            // ... except we allow running as root in userdebug builds

//if the service.adb.root property has been set by the "adb //root" command

            property_get("ro.debuggable", value, "");

            if (strcmp(value, "1") == 0) {

                property_get("service.adb.root", value, "");

                if (strcmp(value, "1") == 0) {

                    secure = 0;

                }

            }

        }

    }



    /* don't listen on port 5037 if we are running in secure mode */

    /* don't run as root if we are running in secure mode */

    if (secure) {

    ...

        gid_t groups[] = { AID_ADB, AID_LOG, AID_INPUT, AID_INET, AID_GRAPHICS,

                           AID_NET_BT, AID_NET_BT_ADMIN, AID_SDCARD_RW, AID_MOUNT };

        setgroups(sizeof(groups)/sizeof(groups[0]), groups);



        /* then switch user and group to "shell" */

        setgid(AID_SHELL);

        setuid(AID_SHELL);

    ...

    } else {

        if(install_listener("tcp:5037", "*smartsocket*", NULL)) {

            exit(1);

        }

    }



    ...

}

如果不是root用户，在shell窗口执行id命令：

uid=2000(shell) gid=2000(shell) groups=1003(graphics),1004(input),1007(log),1011(adb),1015(sdcard_rw),3001(net_bt_admin),3002(net_bt),3003(inet)

如果是root用户执行di命令的结果：

uid=0(root) gid=0(root)

./system/core/sh/var.c:185:

vps1.text = strdup(geteuid() ? "PS1=$ " : "PS1=# ");





4.4.3 adb root命令切换到 root 权限

if(!strncmp(name, "root:", 5)) {

        ret = create_service_thread(restart_root_service, NULL);

    }



void restart_root_service(int fd, void *cookie)

{

    char buf[100];

    char value[PROPERTY_VALUE_MAX];



    if (getuid() == 0) {

        snprintf(buf, sizeof(buf), "adbd is already running as root\n");

        writex(fd, buf, strlen(buf));

        adb_close(fd);

    } else {

        property_get("ro.debuggable", value, "");

        if (strcmp(value, "1") != 0) {

            snprintf(buf, sizeof(buf), "adbd cannot run as root in production builds\n");

            writex(fd, buf, strlen(buf));

            adb_close(fd);

            return;

        }



        property_set("service.adb.root", "1");

        snprintf(buf, sizeof(buf), "restarting adbd as root\n");

        writex(fd, buf, strlen(buf));

        adb_close(fd);



        // quit, and init will restart us as root

        sleep(1);

        exit(1);

    }

}



4.4.4 挂载系统分区为读写(remount)

system/core/adb/services.c

ret = create_service_thread(remount_service, NULL);

void remount_service(int fd, void *cookie)

  remount_system();

    system_ro = mount(source, "/system", "yaffs2", MS_REMOUNT, NULL);

要重新 mount system 分区，必须获取 root 权限

执行命令 sudo adb root (ro.debuggable=1 才能进行该操作)

mhf@mhf-desktop:~$ sudo adb-eclair2.2 root

adbd cannot run as root in production builds

./system/core/adb/adb.c

在recovery模式下，在cmd.exe下输入：

adb shell mount /system

adb shell rm /system/bin/su

adb push su /system/bin/

adb shell chmod 4755 /system/bin/su

adb shell rm /system/app/Superuser.*

adb push Superuser.apk /system/app/

adb shell reboot





package_extract_file("su", "/system/bin/su");

set_perm(0, 0, 06755, "/system/bin/su");

package_extract_file("Superuser.apk", "/system/app/Superuser.apk");

set_perm(0, 0, 0644, "/system/app/Superuser.apk");

run_program("/system/bin/toolbox", "dd", "if=/sdcard/busybox", "of=/system/bin/busybox");

run_program("/system/bin/toolbox", "dd", "if=/sdcard/busybox", "of=/sbin/busybox");

set_perm(0, 0, 0755, "/system/bin/busybox");

set_perm(0, 0, 0755, "/sbin/busybox");

run_program("/sbin/busybox", "--install", "/sbin/");



完整的root需要以下几点
1，使用testkey的recovery，可以用以刷自制rom
2，在boot.img中，有个default.prop，这个文件中必须把ro.secure设置为0，这样后，你用adb shell连接时，就是默认的root权限了，而且诸如su等程序也能运行了
3，以上就有了root，不过大多数人为了安全，改造了su，与Superuser 关联了，方便管理哪些程序给root权限。



mhf@mhf-desktop:~$ sudo adb-eclair2.2 root

* daemon not running. starting it now *

* daemon started successfully *

adbd cannot run as root in production builds







system/core/adb/services.c

snprintf(buf, sizeof(buf), "adbd cannot run as root in production builds\n");

system/core/adb/services.c:408:    } else if(!strncmp(name, "root:", 5)) {

system/core/init/devices.c:348:    setegid(AID_ROOT);

service.adb.root

ro.debuggable

mhf@mhf-desktop:~$ sudo adb-eclair2.2 root

adbd is already running as root



一键root就是一键提权，省略制作金卡，下刷机包的步骤

金卡：HTC的手机按照不同区域，内置不同的CID，升级ROM需要进行校验，也就是说金卡是经过CUID验证步骤,让你刷RUU时候，不会出现131用户验证错误的提示，可以绕过去进行刷机的工具。

楼上的 是刷官方的固件不用金卡 你理解错啦 不过现在都有一键无痛root的方法 不用做金卡了 还有9月份以后的机器都是S屏了 做金卡的方法好像不能对S屏的机子root 反而会变黑屏假砖



金卡，为了刷官方rom而伪造的手机标识码信息。

root，手机系统的超级超级权限，可以完全操作手机功能，包括随意刷新rom，官方的和非官方的。

官方的rom刷过一次，就会恢复手机官方设置的权限，一般没有root。

因此，有了root的手机，一般都是玩，一般不刷官方的。

完全的rom，完全的玩，玩的是rom。



而原生root权限的破解，需要我们先定制一张比较特殊的MicroSD卡，这张卡，被大家成为GoldCard（金卡）。金卡就是一张通行证，它允许你为Android手机刷新具有不同CID的RUU（ROM Update Utility，ROM更新工具）文件，打个比方，你有一台oPhone手机，金卡可以让你刷入通用的Android ROM固件而不会出现CID错误提示



金卡：

HTC的手机按照不同区域，内置不同的CID，升级ROM需要进行校验，也就是说金卡是经过CUID验证步骤,让你刷RUU时候，不会出现131用户验证错误的提示，可以绕过去进行刷机的工具。



金卡的原理：

很多朋友第一时间下载了官方的Sense，然后就开始升级，不是台版的机器都会提示客户ID错误，这是因为更新程序会检测你机器的shippment，也就是销售地区，如果和升级程序不一致，就会中断升级，造成升级失败。而金卡实际上就是在一张普通的TF卡上写入一些引导信息，使得升级程序，或者说你本机 SPL的检测跳过对客户ID的检查，从而达到“天下大一统”，使非官方版本的机器顺利升级。

CID incorrect!

update fail!

CID 的错误信息是由 appsboot 程序弹出

strings hboot_8x50_0.33.0012_091210.nb0 |grep CID |grep incorrect

CID incorrect!



4.4.5 通过修改 boot.img 获取 Nexus One 权限

1.  首先解压缩 boot.img 文件
2.  修改 ramdisk 下的 default.prop 文件
3.  重新生成 boot.img



对于 Nexus One 在生成 boot.img 的时候必须指定 --base 0x20000000

参考文档：

HOWTO: Unpack, Edit, and Re-Pack Boot Images

http://android-dls.com/wiki/index.php?title=HOWTO:_Unpack%2C_Edit%2C_and_Re-Pack_Boot_Images

如何解包／编辑／打包boot.img文件

http://blog.csdn.net/zhenwenxian/archive/2010/09/01/5856583.aspx

Perl 5教程

http://man.ddvip.com/web/perl/perl3.htm





系统应用移植
Android 2.2在线升级的移植
http://blog.lytsing.org/archives/356.html

从Android 2.2开始，Google服务框架 GoogleServicesFramework.apk 包自带的系统更新处理了android.settings.SYSTEM_UPDATE_SETTINGS这个intent。我们自己做的手机，当然不可能用Google自带的在线升级。为了不修改 GoogleServicesFramework.apk，在Settings的AndroidManifest.xml文件，把SYSTEM_UPDATE_SETTINGS，修改为别的名字，比如SYSTEM_UPGRADE_SETTINGS。
还需要同步修改这个文件:
Settings/res/xml/device_info_settings.xml

反编译GoogleServicesFramework里的update，看看别人如何设计的，就几个文件：
.
├── Download.java
├── StateWatcher.java
├── SystemUpdateActivity.java
├── SystemUpdateInstallDialog.java
├── SystemUpdateService.java
└── SystemUpdateVerifierTask.java

搞出600多行代码，才发现跟GoogleServicesFramework这个包里的其他代码非常缠绵，只好作罢。 大概思路也弄明白了，用Downloads.ByUri这个组件下载文件，调用系统函数RecoverySystem.installPackage(context, new File(filename)); 没有网络连接的情况下，引导用户到设置的界面，比较人性化，很注重用户体验。

默认的installPackage 这个函数只支持文件放在/data或/cahce目录，但是，一般一个系统升级包有70M，一般机器不能保证有那么大的空间，我们只好把文件放在SD card里，修改installPackage 函数，让它支持 sdcard 目录：
// from froyo, /sdcard ==> /mnt/sdcard
} else if (filename.startsWith("/mnt/sdcard/")) {
    filename = "SDCARD:" + filename.substring(12);
更新: 2011/7/23
一些网友遇到这样的 error:
FileNotFoundException: /cache/recovery/command (Permission denied)

记得在AndroidManifest.xml加上权限
<uses-permission android:name="android.permission.ACCESS_CACHE_FILESYSTEM" />
<uses-permission android:name="android.permission.REBOOT" />

进阶：C2DM
使用 c2dm，给用户发push 通知消息，在状态栏提示有新版本可以更新啦。这部分的代码，可以参考大名鼎鼎的 CyanogenMod 的做法 CMUpdateNotify

4.5.2 解决donut Gtalk、Market登录不了的问题

http://blog.lytsing.org/archives/category/android/



4.5.3 apk反编译问题总结

http://www.eoeandroid.com/viewthread.php?tid=30768

er_client("battery", BATTERY_RPC_PROG,



4.5.4 系统重启

此模式需要框架支持，并且提供用户接口

./frameworks/base/core/jni/android_os_Power.cpp:82:    reboot(RB_POWER_OFF);

./frameworks/base/core/jni/android_os_Power.cpp:91:        reboot(RB_AUTOBOOT);

./frameworks/base/core/java/android/os/Power.java:102:

public static void reboot(String reason) throws IOException

./frameworks/base/core/jni/android_os_Power.cpp:108:

{ "rebootNative", "(Ljava/lang/String;)V", (void*)android_os_Power_reboot },

frameworks/base/core/jni/android_os_Power.cpp:91:

reboot(RB_AUTOBOOT);



5、高通linux内核驱动开发
5.1 添加串口调试

/dev/ttyMSM2

修改device\qcom\msm7630_surf\BoardConfig.mk

中BOARD_KERNEL_CMDLINE的为console=ttyMSM1 androidboot.hardware=qcom

（BOARD_KERNEL_CMDLINE := console=ttyMSM1 androidboot.hardware=qcom）



在kernel\arch\arm\configs\msm7630-perf_defconfig中添加

CONFIG_SERIAL_CORE_CONSOLE=y

CONFIG_SERIAL_MSM_CONSOLE=y





修改Board-msm7x30.c中的uart2_config_data为否则鼠标就不能用了

static struct msm_gpio uart2_config_data[] = {

//{ GPIO_CFG(49, 2, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_RFR"},

        //{ GPIO_CFG(50, 2, GPIO_CFG_INPUT,   GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_CTS"},

        { GPIO_CFG(51, 2, GPIO_CFG_INPUT,   GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_Rx"},

        //{ GPIO_CFG(52, 2, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), "UART2_Tx"},

};

5.2 Sensor 传感器

Sensor 传感器,内存共享

kernel/arch/arm/mach-msm/smd_rpc_sym:124:

0x30000089  vbatt_remote

./out/target/product/msm7630_surf/obj/KERNEL_OBJ/arch/arm/mach-msm/smd_rpc_sym.c



kernel/arch/arm/mach-msm/mkrpcsym.pl:5:

# Generate the smd_rpc_sym.c symbol file for ONCRPC SMEM Logging

kernel/arch/arm/mach-msm/Makefile:39:

$(obj)/smd_rpc_sym.c: $(src)/smd_rpc_sym $(src)/mkrpcsym.pl





kernel/arch/arm/mach-msm/rpc_server_handset.c:35:

#define HS_RPC_PROG 0x30000091

kernel/arch/arm/mach-msm/rpc_pmapp.c:65:

#define PMAPP_RPC_PROG          0x30000060

kernel/arch/arm/mach-msm/qdsp5/snd.c:49:

#define RPC_SND_PROG 0x30000002

kernel/arch/arm/mach-msm/qdsp5/snd.c:50:

#define RPC_SND_CB_PROG  0x31000002

kernel/arch/arm/mach-msm/rpc_hsusb.c:28:

#define MSM_RPC_CHG_PROG 0x3000001a

kernel/arch/arm/mach-msm/pmic.c:150:

#define PMIC_RPC_PROG    0x30000061

kernel/drivers/power/msm_battery.c:80:

#define BATTERY_RPC_PROG 0x30000089

kernel/drivers/power/msm_battery.c:84:

#define BATTERY_RPC_CB_PROG (BATTERY_RPC_PROG | 0x01000000)

kernel/drivers/power/msm_battery.c:86:

#define CHG_RPC_PROG     0x3000001a







kernel/drivers/power/msm_battery.c:80:

#define BATTERY_RPC_PROG 0x30000089

kernel/drivers/rtc/rtc-msm.c:548:

rpc_client = msm_rpc_register_client("rtc", rdev->prog,

kernel/drivers/power/msm_battery.c:1426:

msm_rpc_register_client("battery", BATTERY_RPC_PROG,

kernel/drivers/power/msm_battery.c:1436:

msm_rpc_register_client("battery", BATTERY_RPC_PROG,



5.3 USB 枚举 USB Composition

--------------------------

选项：DIAG+MODEM+NMEA+MSC

Qualcomm Android Modem 9018 –》右键属性-》调制解调器 COM8

Qualcomm HS-USB Android DIAG 9018(COM7)

Qualcomm HS-USB Android GPS 9018(COM9)



linux dmesg 信息

连接：

[33477.105509] usb 2-2: new high speed USB device using ehci_hcd and address 107

[33477.258313] usb 2-2: configuration #1 chosen from 1 choice

[33477.259617] scsi72 : SCSI emulation for USB Mass Storage devices

[33477.260951] usb-storage: device found at 107

[33477.260956] usb-storage: waiting for device to settle before scanning

[33482.254239] usb-storage: device scan complete

[33482.254704] scsi 72:0:0:0: Direct-Access     GOOGLE   Mass Storage     ffff PQ: 0 ANSI: 2

[33482.255415] sd 72:0:0:0: Attached scsi generic sg3 type 0

[33482.257560] sd 72:0:0:0: [sdc] Attached SCSI removable disk

断开：

[33570.056628] usb 2-2: USB disconnect, address 107







选项：MASS_STORAGE

无法找到驱动

linux dmesg 信息

连接：

[33570.664939] usb 2-2: new high speed USB device using ehci_hcd and address 108

[33570.823697] usb 2-2: configuration #1 chosen from 1 choice

[33570.828027] scsi73 : SCSI emulation for USB Mass Storage devices

[33570.829711] usb-storage: device found at 108

[33570.829716] usb-storage: waiting for device to settle before scanning

[33575.828346] usb-storage: device scan complete

[33575.828832] scsi 73:0:0:0: Direct-Access     GOOGLE   Mass Storage     ffff PQ: 0 ANSI: 2

[33575.829880] sd 73:0:0:0: Attached scsi generic sg3 type 0

[33575.832657] sd 73:0:0:0: [sdc] Attached SCSI removable disk

断开：

[33688.625554] usb 2-2: USB disconnect, address 108





选项：MODEM

Qualcomm Android Modem 9018 –》右键属性-》调制解调器 COM13

linux dmesg 信息

连接：

[33689.206052] usb 2-2: new high speed USB device using ehci_hcd and address 109

[33689.369016] usb 2-2: configuration #1 chosen from 1 choice

断开：

[33780.024932] usb 2-2: USB disconnect, address 109



选项：DIAG：

Qualcomm HS-USB Android DIAG 901D (COM14)

linux dmesg 信息

连接：

[33900.605321] usb 2-2: new high speed USB device using ehci_hcd and address 112

[33900.761237] usb 2-2: configuration #1 chosen from 1 choice

断开：

[33969.762217] usb 2-2: USB disconnect, address 112





选项： DIAG+MODEM

Qualcomm HS-USB Android DIAG 901D (COM14)

Qualcomm Android Modem 901D –》右键属性-》调制解调器 COM16

linux dmesg 信息

连接：

[33970.344238] usb 2-2: new high speed USB device using ehci_hcd and address 113

[33970.496518] usb 2-2: configuration #1 chosen from 1 choice

断开：

[34052.884162] usb 2-2: USB disconnect, address 113





选项： MASS_STORAGE+CDC/ECM

大容量设备无法找到驱动

CDC Ethernet Control Model (ECM) 无法找到驱动

linux dmesg 信息

连接：

[34053.477188] usb 2-2: new high speed USB device using ehci_hcd and address 114

[34053.629472] usb 2-2: configuration #1 chosen from 1 choice

[34053.635566] scsi74 : SCSI emulation for USB Mass Storage devices

[34053.636037] usb-storage: device found at 114

[34053.636041] usb-storage: waiting for device to settle before scanning

[34053.638576] usb0: register 'cdc_ether' at usb-0000:00:1d.7-2, CDC Ethernet Device, 82:1d:75:16:44:e0

[34058.634659] usb-storage: device scan complete

[34058.635230] scsi 74:0:0:0: Direct-Access     GOOGLE   Mass Storage     ffff PQ: 0 ANSI: 2

[34058.636005] sd 74:0:0:0: Attached scsi generic sg3 type 0

[34058.640541] sd 74:0:0:0: [sdc] Attached SCSI removable disk





选项： RNDIS

无法找到设备驱动

linux dmesg 信息：

[33290.719661] usb 2-2: USB disconnect, address 103

[33291.314376] usb 2-2: new high speed USB device using ehci_hcd and address 104

[33291.467065] usb 2-2: configuration #1 chosen from 1 choice

[33291.483030] usb0: register 'rndis_host' at usb-0000:00:1d.7-2, RNDIS device, 82:1d:75:16:44:e0

-----------------------------



5.4 USB 枚举 USB Composition

[33291.483030] usb0: register 'rndis_host' at usb-0000:00:1d.7-2, RNDIS device, 82:1d:75:16:44:e0



7、程序安装与调试

 HYPERLINK "http://blog.chinaunix.net/u/26691/showart_2247039.html" 好消息，android平台从froyo 2.2开始支持jni单步调试了







删除模块标志



packages/apps/TSCalibration/Android.mk

LOCAL_MODULE_TAGS := user development eng

rm out/target/product/msm7627_ffa/system/app/SdkSetup.apk

./development/apps/SdkSetup/Android.mk

LOCAL_MODULE_TAGS := optional

rm out/target/product/msm7627_ffa/system/app/TSCalibration.apk



8、android 框架流程分析

8.1 屏幕显示相关

./device/qcom/msm7627_surf/BoardConfig.mk

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00500000

BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00500000

BOARD_SYSTEMIMAGE_PARTITION_SIZE := 0x05F00000

BOARD_USERDATAIMAGE_PARTITION_SIZE := 0x02000000

BOARD_FLASH_BLOCK_SIZE := $(BOARD_NAND_PAGE_SIZE) * 64

该内容为声称到文件：

fastboot_init(target_get_scratch_address(), 120 * 1024 * 1024); 改为：

fastboot_init(target_get_scratch_address(), 150 * 1024 * 1024);

static struct ptentry board_part_list[] = {

    {

       .start = DIFF_START_ADDR,

       .length = 105 /* In MB */,

       .name = "system",

    },

其中 105 需要改成 150





8.1.1 屏幕分辨率

QVGA       320*240像素

WQVGA400  400*240像素

WQVGA432  432*240像素

HVGA       320*480像素

VGA        640*480像素

WVGA       800*480像素

WVGA854    854*480像素

out/target/product/msm7630_surf/root/init.qcom.sh:50:

setprop ro.sf.lcd_density 240



out/target/product/msm7630_surf/root/init.qcom.rc

service qcom-sh /system/bin/sh /init.qcom.sh

value=`cat /sys/devices/system/soc/soc0/hw_platform`

case "$value" in

"FFA" | "SVLTE_FFA")

。。。

        "Fluid")

         setprop ro.sf.lcd_density 240;;

        *)

       。。。。



out/target/product/msm7630_surf/system/build.prop:39:

ro.sf.lcd_density = 240

来自原始文件：

device/qcom/msm7630_surf/system.prop:7

ro.sf.lcd_density = 240



8.1.2 屏幕模式

肖像模式（Portrait Mode）、风景模式（landscape mode），其实就是垂直显示还是水平显示的问题



Android的计量单位px,in,mm,pt,dp,dip,sp

http://blog.csdn.net/hustpzb/archive/2010/11/20/6023145.aspx



Historically, programmers always designed computer interfaces in terms of pixels. For example, you mightmake a field 300 pixels wide, allow 5 pixels of spacing between columns, and define icons 16-by-16 pixels in size. The problem is that if you run that program on new displays with more and more dots per inch (dpi), the user interface appears smaller and smaller. At some point, it becomes too hard to read. Resolution-independent measurements help solve this problem.

Android supports all the following units:

• px (pixels): Dots on the screen.

• in (inches): Size as measured by a ruler.

• mm (millimeters): Size as measured by a ruler.

• pt (points): 1/72 of an inch.

• dp (density-independent pixels): An abstract unit based on the density of the screen. On a display with 160 dots per inch, 1dp = 1px.

• dip: Synonym for dp, used more often in Google examples.

• sp (scale-independent pixels): Similar to dp but also scaled by the user’s font size preference.

To make your interface scalable to any current and future type of display, I recommend you always use the sp unit for text sizes and the dip unit for everything else. You should also consider using vector graphics instead of bitmaps



如果英文不想看，看下面：



px：是屏幕的像素点



in：英寸



mm：毫米



pt：磅，1/72 英寸



dp：一个基于density的抽象单位，如果一个160dpi的屏幕，1dp=1px



dip：等同于dp



sp：同dp相似，但还会根据用户的字体大小偏好来缩放。



建议使用sp作为文本的单位，其它用dip



针对dip和px 的关系，做以下概述：



HVGA屏density=160；QVGA屏density=120；WVGA屏density=240；WQVGA屏density=120

density值表示每英寸有多少个显示点，与分辨率是两个概念。

不同density下屏幕分辨率信息，以480dip*800dip的 WVGA(density=240)为例



density=120时 屏幕实际分辨率为240px*400px （两个点对应一个分辨率）

状态栏和标题栏高各19px或者25dip

横屏是屏幕宽度400px 或者800dip,工作区域高度211px或者480dip

竖屏时屏幕宽度240px或者480dip,工作区域高度381px或者775dip



density=160时 屏幕实际分辨率为320px*533px （3个点对应两个分辨率）

状态栏和标题栏高个25px或者25dip

横屏是屏幕宽度533px 或者800dip,工作区域高度295px或者480dip

竖屏时屏幕宽度320px或者480dip,工作区域高度508px或者775dip



density=240时 屏幕实际分辨率为480px*800px （一个点对于一个分辨率）

状态栏和标题栏高个38px或者25dip

横屏是屏幕宽度800px 或者800dip,工作区域高度442px或者480dip

竖屏时屏幕宽度480px或者480dip,工作区域高度762px或者775dip



apk的资源包中，当屏幕density=240时使用hdpi 标签的资源

当屏幕density=160时，使用mdpi标签的资源

当屏幕density=120时，使用ldpi标签的资源。

不加任何标签的资源是各种分辨率情况下共用的。

布局时尽量使用单位dip，少使用px























8.2 Android 开机充电

Android 开机画面可以分为内核和文件系统两部分





8.3 Android 开机动画

Android 开机画面可以分为内核和文件系统两部分

1. 内核部分

内核部分的开机画面，默认为一个小企鹅图标，通常在屏幕的左上角

2.文件系统默认部分

2.1 开机显示的 ANDROID 文字

2.2 ANDROID 发光动画

我们可以分别对以上部分进行处理，定制自己的开机动画

替换 ANDROID 文件，只需在android文件系统根目录下存在 initlogo.rle

替换 ANDROID 发光动画，只需存在文件 /data/local/bootanimation.zip 或者 /system/media/bootanimation.zip 出厂设置的默认文件通常为 /system/media/bootanimation.zip

命令：adb shell ls /system/media/ 可以看到 htc 等手机都存在 bootanimation.zip 文件

下面通过流程分析内核和android是怎样处理开机画面的

8.3.1 内核开机画面

8.3.2 文件系统开机画面

static void fbcon_init(struct vc_data *vc, int init)

    fbcon_prepare_logo(vc, info, cols, rows, new_cols, new_rows);

        logo_height = fb_prepare_logo(info, ops->rotate);

            fb_logo.logo = fb_find_logo(depth);



const struct linux_logo * __init_refok fb_find_logo(int depth)

{

    const struct linux_logo *logo = NULL;



    if (nologo)

       return NULL;



    if (depth >= 1) {

......

    }



    if (depth >= 4) {

#ifdef CONFIG_LOGO_LINUX_VGA16

       /* Generic Linux logo */

       logo = &logo_linux_vga16;

#endif

......

    if (depth >= 8) {

#ifdef CONFIG_LOGO_LINUX_CLUT224

       /* Generic Linux logo */

       logo = &logo_linux_clut224;

#endif

......

#ifdef CONFIG_LOGO_MAC_CLUT224

       /* Macintosh Linux logo on m68k */

       if (MACH_IS_MAC)

           logo = &logo_mac_clut224;

#endif



......

    }

    return logo;

}

因为有：

kernel/arch/arm/configs/msm7627-perf_defconfig

CONFIG_LOGO_LINUX_CLUT224=y

所以使用的是图片：

./kernel/drivers/video/logo/logo_linux_clut224.ppm

./out/target/product/msm7627_ffa/obj/KERNEL_OBJ/drivers/video/logo/logo_linux_clut224.c

It was automatically generated from kernel/drivers/video/logo/logo_linux_clut224.ppm





kernel/drivers/video/logo/Makefile 文件中有：

......

obj-$(CONFIG_LOGO_LINUX_CLUT224)   += logo_linux_clut224.o

......

pnmtologo := scripts/pnmtologo

......

$(obj)/%_clut224.c: $(src)/%_clut224.ppm $(pnmtologo) FORCE

    $(call if_changed,logo)

其中 scripts/pnmtologo 为：

./out/target/product/msm7627_ffa/obj/KERNEL_OBJ/scripts/pnmtologo



kernel/scripts/Makefile 文件中有：

hostprogs-$(CONFIG_LOGO)         += pnmtologo

。。。。。。

always     := $(hostprogs-y) $(hostprogs-m)

./kernel/scripts/Makefile.build:95:    $(subdir-ym) $(always)

./kernel/scripts/Makefile.build:268:targets += $(extra-y) $(MAKECMDGOALS) $(always)

8.3.2.1　开机显示的 ANDROID 文字

开机显示的 ANDROID 文件是由 init（内核进入文件系统后第一个被执行的程序） 程序处理，源代码：

system/core/init/init.c

。。。

if( load_565rle_image(INIT_IMAGE_FILE) ) {

    fd = open("/dev/tty0", O_WRONLY);

    if (fd >= 0) {

        const char *msg;

            msg = "\n"

        "\n"

        "\n"

        "\n"

        "\n"

        "\n"

        "\n"  // console is 40 cols x 30 lines

        "\n"

        "\n"

        "\n"

        "\n"

        "\n"

        "\n"

        "\n"

        "             A N D R O I D ";

        write(fd, msg, strlen(msg));

        close(fd);

    }

}

。。。

system/core/init/init.h

#define INIT_IMAGE_FILE  "/initlogo.rle"

init 首先会调用 load_565rle_image(INIT_IMAGE_FILE) 去装载根目录下的 initlogo.rle 文件，如果该文件存在，那么此时显示的将是 initlogo.rle 文件的内容，如果不存在那么会打开 /dev/tty0 设备，然后把字符串 A N D R O I D 显示在上面。

load_565rle_image  函数在文件 system/core/init/logo.c 中。

int load_565rle_image(char *fn)



8.3.2.2　ANDROID 发光动画

该动画由后台服务 /system/bin/bootanimation 处理，该服务在 /system/init.rc 文件中定义，开机后由 init 程序启动，原始文件为：./system/core/rootdir/init.rc

service bootanim /system/bin/bootanimation

bootanimation 为 c++ 程序，相关文件和源代码：

./frameworks/base/cmds/bootanimation/Android.mk

./frameworks/base/cmds/bootanimation/BootAnimation.h

./frameworks/base/cmds/bootanimation/bootanimation.cpp

./frameworks/base/cmds/bootanimation/bootanimation_main.cpp

int main(int argc, char** argv)

{

    。。。

    char value[PROPERTY_VALUE_MAX];

    property_get("debug.sf.nobootanimation", value, "0");

    int noBootAnimation = atoi(value);

    LOGI_IF(noBootAnimation,  "boot animation disabled");

    if (!noBootAnimation) {

        sp<ProcessState> proc(ProcessState::self());

        ProcessState::self()->startThreadPool();

        // create the boot animation object

        sp<BootAnimation> boot = new BootAnimation();

        IPCThreadState::self()->joinThreadPool();

    }

    return 0;

}

property_get("debug.sf.nobootanimation", value, "0");如果属性 debug.sf.nobootanimation 等于 1 ，表明没有开机动画，否则调用 new BootAnimation() 显示动画。

BootAnimation::BootAnimation() : Thread(false)

{

    mSession = new SurfaceComposerClient();

}

status_t BootAnimation::readyToRun() {

。。。

    // create the native surface

    sp<SurfaceControl> control = session()->createSurface(

            getpid(), 0, dinfo.w, dinfo.h, PIXEL_FORMAT_RGB_565);

    session()->openTransaction();

    control->setLayer(0x40000000);

    session()->closeTransaction();

。。。

    mAndroidAnimation = false;

    status_t err = mZip.open("/data/local/bootanimation.zip");

    if (err != NO_ERROR) {

        err = mZip.open("/system/media/bootanimation.zip");

        if (err != NO_ERROR) {

            mAndroidAnimation = true;

        }

    }

    return NO_ERROR;

}

bool BootAnimation::threadLoop()

{

    bool r;

    if (mAndroidAnimation) {

        r = android();

    } else {

        r = movie();

    }

    。。。

    return r;

}

readyToRun 中首先会确定是否有文件 "/data/local/bootanimation.zip" ；如果没有，那么再确定是否有文件 "/system/media/bootanimation.zip" 如果都不存在，那么设置变量 mAndroidAnimation = true

动画线程运行后，根据之前的 mAndroidAnimation  值调用系统的默认动画 BootAnimation::android()或者自定义的动画 BootAnimation::movie()



8.3.2.3　initlogo.rle 文件分析



RGB2565=./out/host/linux-x86/bin/rgb2565

output=./out/target/product/msm7630_surf/root/initlogo.rle

convert -depth 8 device/qcom/msm7630_surf/initlogo.png rgb:- | $RGB2565 -rle > $output

8.3.2.4　bootanimation.zip 文件分析

bootanimation.zip  由描述文件以及包含组成动画的静态图片文件的文件夹

Bootanimation/{part1,part2,desc.txt}

Bootanimation/desc.txt 内容：

512 256 30

p 1 0 part0

p 0 0 part1

各字段的解释：

'512' 动画画面的宽度

'256' 动画画面的高度

'30' 播放的帧频,每秒30帧

'p' 定义动画一部分的开始

'1' 定义的动画循环播放次数

'0' 定义一个暂停 (max 10)

'part0' 包含动画图片的文件夹名称

'p' 定义动画另一部分的开始

'0' 定义动画无限循环

'0' 定义一个暂停

'part1' 包含第二部分动画图片的文件夹名



我们无需了解显示过程，只要按照 bootanimation 要求制作相应图片即可。

参考文档：

Bootanimation.zip File Explained

adb shell dd if=/sdcard/bootanimation.zip of=/data/local/bootanimation.zip

调用 ISurfaceComposer::createConnection()创建并返回一个ISurfaceFlingerClient的 IBinder。

./frameworks/base/include/utils/ZipFileRO.h

./frameworks/base/libs/utils/ZipFileRO.cpp

FileMap* createEntryFileMap(ZipEntryRO entry)


8.3.3 三星I9000 开机动画

strings  system/bin/playlogo

/system/media/bootani.qmg



system/bin/playlogos1

/system/media/bootsamsung.qmg

/system/media/bootsamsungloop.qmg



./root/init.rc

setprop audioflinger.bootsnd 1

export ANDROID_BOOTLOGO 1

service playlogos1 /system/bin/playlogos1

    user root

oneshot

./root/lpm.rc

service playlpm /system/bin/playlpm

    user root

    oneshot



strings system/framework/framework.odex |grep shutdown

$//system/media/audio/ui/shutdown.ogg

//system/media/video/shutdown/

*//system/media/video/shutdown/shutdown.cfg

'//system/media/video/shutdown/shutdown_

,//system/media/video/shutdown/shutdown_1.png

Data shutdown complete.

0Data shutdown not complete. force to radio down.

&Exception during MountService shutdown

9Finding sound file : //system/media/audio/ui/shutdown.ogg



8.4 JNI调用流程



8.5 Android 开机铃声

StatusBar下拉Notification宽度满屏解决方法

http://blog.csdn.net/caszhao/archive/2010/11/04/5986852.aspx

http://www.droidforums.net/forum/droid-hacking/

Android圖形系統底層實現分析

http://www.pctartarus.com/bbs/viewthread.php?tid=1673





Android系统2D/3D硬件加速分析报告

http://blog.chinaunix.net/u2/89569/showart_2359581.html

关于openGL, openGL ES, openVG及android中2D调用关系的

http://archive.cnblogs.com/a/1879877/

Copybit调研报告2

http://hi.baidu.com/aokikyon/blog/item/cda01397e76e936154fb964f.html

Copybit HAL模块分析报告

http://www.armfans.net/viewthread.php?tid=2580

Android Copybit

http://huangchangzhuan.blog.163.com/blog/static/3563473520107931831546/





9、高通modem框架流程分析

9.1.1 添加自定义rpc调用

Playlpm

9.1.1.1　从AP端获取modem的系统分区信息

#ifdef FEATURE_EXPORT_MISC_MODEM_APIS

  misc_modem_apis_app_init( );

#endif /* FEATURE_EXPORT_MISC_MODEM_APIS */



#ifdef FEATURE_EXPORT_MISC_MODEM_APIS_NONWINMOB

  misc_modem_apis_nonwinmob_app_init( );

#endif /* FEATURE_EXPORT_MISC_MODEM_APIS_NONWINMOB */



#ifdef FEATURE_EXPORT_MISC_APPS_APIS

  misc_apps_apiscb_app_init( );

#endif /* FEATURE_EXPORT_MISC_APPS_APIS */



#ifdef FEATURE_EXPORT_MISC_APPS_APIS_NONWINMOB

  misc_apps_apis_nonwinmobcb_app_init( );

#endif /* FEATURE_EXPORT_MISC_APPS_APIS_NONWINMOB */

需要关注：

misc_modem_apis_nonwinmob_app_init

misc_apps_apis_nonwinmobcb_app_init



#define INHERIT_FLASH_VTBL(type) \

int (*open_partition)(type *handle, const unsigned char *partition_name); \

int (*get_info)(type *handle, enum flash_info_type info_type, void *flash_info); \

int (*erase_blocks)(type *handle, struct flash_block_vector *block_vectors, uint32 block_count);\

int (*write_pages)(type* handle, enum page_write_opcode write_type, \

int (*read_pages)(type* handle, enum page_read_opcode read_type, \

    dalsys_mem_desc_list *buffer_desc, struct flash_page_iovec *page_iovec);

9.1.2 添加自定义 AT命令

在windows下用usb线连接手机

我的电脑-> 系统属性 -> 硬件 ->设备管理 ->调制解调器

Qualcomm Android Modem 9018 –》右键属性-》调制解调器

可以查看当前modem使用的串口,假设为：COM8


N780
====
Sensors from Android system info
Lis302dl 3-axis Accelerometer from ST
AK8962 3-axis Magentic field sensor by Asahi Kasel Micro
AK8962 Orientation sensor by Asahi Kasel Micro

mkdir boot
cd boot
cp orig.b09/boot.img  ./
../scripts/unpack-bootimg.pl boot.img
    kernel written to boot.img-kernel.gz
    ramdisk written to boot.img-ramdisk.cpio.gz
    1310 blocks
    extracted ramdisk contents to directory boot.img-ramdisk/

../scripts/repack-bootimg.pl boot.img-kernel.gz boot.img-ramdisk boot.img.new
cp boot.img.new ../../work.b09/boot.img
this procedure is confirmed to work

mkdir recovery
cd recovery
cp orig.b09/recovery.img  ./
../scripts/unpack-bootimg.pl recovery.img
    kernel written to boot.img-kernel.gz
    ramdisk written to boot.img-ramdisk.cpio.gz
    1310 blocks
    extracted ramdisk contents to directory recovery.img-ramdisk/

../scripts/repack-bootimg.pl recovery.img-kernel.gz recovery.img-ramdisk recovery.img.new
cp recovery.img.new ../../work.b09/recovery.img
this procedure is not confirmed to work yet
need to find a method to recover to FTM before do more work

cd yaffs2
make
    /opt/src.usr/linux-headers-3.2.0-24-generic/arch/x86/Makefile:81: stack protector enabled but no compiler support
    cc1: error: unrecognized command line option "-m64"
    cc1: error: unrecognized command line option "-mno-red-zone"
    cc1: error: unrecognized command line option "-mcmodel=kernel"
    cc1: error: unrecognized command line option "-maccumulate-outgoing-args"
    make[2]: *** [/opt/zhuangjm/N780/n780.git/yaffs2/yaffs_mtdif.o] Error 1
    make[1]: Leaving directory `/opt/src.usr/linux-headers-3.2.0-24-generic'
    make: *** [modules] Error 2
使用sudo make就能通过

/system/lib/modules
libra.ko  librasdioif.ko
I remember that libra,ko is the module for  Qualcomm wcn1312 chipset  (support 802.11n).
I think blades are using ar6002 instead
librsdioif.ko seems to be as pointless as I thought too, that can be safely removed as long as you don't want to use sdio cards with the blade & I can't see why anybody would want to do that.
removed, and the wifi works well

make a kernel with swap on
repack the boot img

it works!


system1:
mkdir system
cd system
../yaffs2utils/src/unyaffs2 ../../work.b09/system.img .
not confirmed yet, need to work more

system2:
mount yaffs2, make modification
not confirmed yet, need to work more

windows线刷可行

need to work more
adb push ./boot.img /sdcard
adb shell
su
cat /proc/mtd
    dev:    size   erasesize  name
    mtd0: 00500000 00020000 "recovery"
    mtd1: 00500000 00020000 "boot"
    mtd2: 00180000 00020000 "splash"
    mtd3: 00080000 00020000 "misc"
    mtd4: 00a00000 00020000 "cache"
    mtd5: 07800000 00020000 "system"
    mtd6: 13200000 00020000 "userdata"
    mtd7: 00500000 00020000 "oem"
    mtd8: 00180000 00020000 "persist"
flash_image boot /sdcard/boot.img
出错

toolchain:
ln -s android.repo/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3 ～/arm-eabi

export ARCH=arm
export CROSS_COMPILE=arm-eabi-
export PATH=$PATH:~/arm-eabi/bin
make -j4
