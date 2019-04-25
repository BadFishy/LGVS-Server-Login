# LGVS-Server-Login
[凉糕对战平台登陆服务端](https://github.com/BadFishy/LGVS-Server-Login)

 \- 施工中 -
## 运作原理流程

### 一、初始化

#### 1. 启动日志

日志目录不存在则在运行目录下创建log文件夹

#### 2. 检测配置文件 config.ini 是否存在

若不存在则提示修改 config_origin.ini 内的配置后将其更名为 config.ini

### 二、开始

#### 1. 加载配置文件

初始化Config类，加载配置文件

#### 2. 连接数据库

##### 若数据库未初始化

（配置文件中的 INIT = 0)

###### - 新建数据表 USER

（具体的列参考 LGVS数据库.xls）

###### - 然后将 INIT = 1 后自动重启程序

#### 3. 创建登录服务器线程

## 登录服务器线程