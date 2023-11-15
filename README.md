# UN-MCU

#### 介绍
统一MCU  驱动层和外设层API   因项目需求 mcu被经常更换/不同项目使用不同MCU 导致业务代码需要重构    这边将多MCU进行封装一层进行统一接口调度  仅更换MCU的项目业务逻辑层无需更改  只需要修改配置文件即可 新的项目若同事也·按照接口封装了该型号mcu 则新建工程直接用
目前已部分封装STM32、杰发科技、芯旺微、赛腾微的驱动只封装我用到的部分  后面应该是华大、灵动微了 更多的是需要大家一起努力    
#### 注意事项  
因为这个主要本来用所以存在很多不完善地方
RTThread虽然也做了中间层 但是那个可能我不会使用把 不然可以将我的集成上去  以下没集成原因纯属个人理解
首先我没在正式项目用过他，我觉得他用接口是通用但是需要调用时候传一致的参数进去，而我用的是枚举类优先错了编译器会报错
另外我觉得听他的工程比较大而且涉及到内核很多东西我不懂  我想的是裸机可控性高  当然我这个改改也能兼容os的 但是我没有相关项目 平常做的东西都很简单
这个项目主要是抛转引玉 有经验的帮忙完善或者是集成到已有的开源项目
 1.IAR没测试
 2.OS没测试
 3.我所有用到的功能比较少 接口地方还需要扩展

#### 安装教程

1.  将所有文件拷贝到pack包文件夹 或者压缩成zip把后缀改成pack就可以双击安装成pack/导入到IAR 
(这里方法是安装法)修改驱动后会影响所有项目 如果想隔离单独拉源码添加到项目或使用时限定版本（推荐）


#### 使用说明（这里以keil示例）

1. 添加到现有项目

![输入图片说明](https://foruda.gitee.com/images/1700012864348942998/58e1376e_1270025.png "屏幕截图")

勾选后就会出现项目源码

![](https://foruda.gitee.com/images/1700012886151206977/6595fce0_1270025.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1700012929447285876/ea5a85e8_1270025.png "屏幕截图")

这里以STM32 cubemx生成的代码调用为例
引脚定义到配置文件.h中（这是为了方便业务分离 换了mcu只需要修改对应.c   这里自己手动新建  后面使用该库时自动创建）

![](https://foruda.gitee.com/images/1700013117247994108/7d4bb97c_1270025.png "屏幕截图")

实现一下

![](https://foruda.gitee.com/images/1700013148179154502/1bb4cffc_1270025.png "屏幕截图")

引脚映射

![](https://foruda.gitee.com/images/1700013221648502180/75dd3cdf_1270025.png "屏幕截图")

业务中调用

![](https://foruda.gitee.com/images/1700013247293314391/dffeb2bc_1270025.png "屏幕截图")

 例如板载外设ads115和sht30/sht3x 只需要映射引脚配置参数就可以用了  内部调用了模拟iic

 
####文件目录树

![输入图片说明](https://foruda.gitee.com/images/1700013503692340805/702bf58d_1270025.png "屏幕截图")

 


#### 特技

1.   板内支持的在HAL中能看到最新的   目前（也封装了个模拟IIC的 首次更新）如下

![](https://foruda.gitee.com/images/1700013633551600493/a6724bef_1270025.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1700013975033018191/7b447703_1270025.png "屏幕截图")

2.板外外设在 FML文件夹中 jfml.h中查看的到引用  如果路径/文件报错就是更新时候没适配/早期一些东西拉进来暂时没用到就没适配

![输入图片说明](https://foruda.gitee.com/images/1700013805873784332/beb4190a_1270025.png "屏幕截图")

 ![输入图片说明](https://foruda.gitee.com/images/1700013868020190623/3ed33c9b_1270025.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1700013880084111003/1e9ebab8_1270025.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1700013889200625095/b40b2688_1270025.png "屏幕截图")

![输入图片说明](https://foruda.gitee.com/images/1700013900110320478/52fd405d_1270025.png "屏幕截图")

3.工具库就没啥了

![输入图片说明](https://foruda.gitee.com/images/1700014019309150109/99cb0bdc_1270025.png "屏幕截图")
 