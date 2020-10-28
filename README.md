## 关于项目

**项目名称：基于C控制台与MySQL的简易学生选课系统（C/S端不分离）**

这里是2019213314、16、23、29的大二开学前计算机实习（小学期）作业。

具体题目见题目.pdf，更多关于项目的实现与描述见设计报告、测试报告与源码。

~~小组成员水平有限，代码是一坨屎山，开源仅供记录与参考hhhh~~

## 开发、运行环境

整个系统在64位Windows 10（保持自动更新）的Visual Studio 2019 Community (16.7.2)的环境下开发。

- 系统环境变量在安装mysql时添加`%MYSQL_HOME%`=`D:\mysql-8.0.21-winx64`。（你的mysql安装路径）

- VS开发采用Debug x64环境：
  - 配置属性 -> C/C++ -> 预处理器 -> **预处理器定义**添加`_CRT_SECURE_NO_WARNINGS`以使用gets等被VS认为内存不安全的函数
  - 配置属性 -> VC++ 目录 ->**包含目录**添加`%MYSQL_HOME%\include`
  - 配置属性 -> VC++ 目录 ->**库目录**添加`%MYSQL_HOME%\lib`
  - 配置属性 -> 调试 ->**环境**为`PATH=%MYSQL_HOME%\bin; `
  - 警告等级为/W3时代码最终0 error，0 warning。

编译出可执行文件后需要调用到`libcrypto-1_1-x64.dll`、`libmysql.dll`、`libssl-1_1-x64.dll`、`vcruntime140_1.dll`

