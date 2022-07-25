# 目的

设备码量产工具实现了从**手持扫码枪**（一维码、二维码）到**设备码生成**再到**设备标签贴纸打印**的自动化

其中设备码生成规则支持 lua 脚本自定义，可适用于更多场景

# 注意

* 仅支持 win 系统，当前已测试 win10 及以上版本
* 关于扫码枪，硬件需使用 USB-KBW 接口
* 关于标签打印机 （当前标签排版尺寸，60mm x 40mm）
  * 须在控制面板中可看到待使用的打印机
  * 需将待使使用的打印机**设置为默认打印机**
  
![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/1.png)

*关于编码转换规则：程序会使用扫码枪或手动输入的原始值**经过 LUA 自定义转换规则**生成最终的设备码
 * 规则转换文件 `numconv.lua` 在安装目录下可见
 * 返回 ”“ 表示错误
 
 ![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/12.png)
 
 ![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/2.png)
 
# 安装

1. 双击 `设备码量产工具_setup.exe`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/3.png)

2. 点击 `下一步`（可选的更换安装路径）

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/4.png)

3. 点击 `下一步`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/5.png)

4. 点击 `安装`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/6.png)

5. 点击`完成`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/7.png)

# 使用

1. 双击快捷图标 `设备码量产工具`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/8.png)

2. 点击 `OK`

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/9.png)

3. 保证桌面焦点始终处于 `设备码量产工具` 最上面的文本输入框 （可看到光标闪烁）
 * 启动方式
   * 扫码枪扫码得到一次数值
   * 手动编辑数值到 **最上面的文本输入框**，按下回车
   
![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/10.png)

4. 数据操作，点击 `导出本次` 按钮，将导出自程序打开后所有有效的设备编码值（.csv）

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/11.png)


# 注意
> 需按照实际使用的打印机与标签纸设置对应的打印参数 (ZEBRA-ZD888 为例)

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/13.png)

![](https://github.com/cocowts/DeviceCodeGenerationTool/blob/master/Image/14.png)

