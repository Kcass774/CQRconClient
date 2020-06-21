## Rcon简介

> Rcon协议是被Source专用服务器使用的一个基于TCP/IP协议的通信协议,可以通过rcon向服务器发送控制台命令,rcon最常见的用法是让服主在不接触服务器的情况下控制自己的游戏服务器

#### 使用了Rcon的游戏（不完全）

- Minecraft
- CS:S
- CS:GO

## 已知BUG

- ~~在插件已经启用的情况下将插件禁用启用会引起酷q崩溃~~
- 也许还有其他Bug？

------

## 使用教程

1. ##### 启动Rcon服务端

   以Minecraft服务器为例

   你需要在配置文件server.properties中修改以下内容

   ```
   rcon.port=25575					#Rcon端口
   rcon.password=123456			#Rcon密码
   broadcast-rcon-to-ops=true 		#Rcon是否拥有OP权限
   ```

   修改完成重启服务端就完成了第一步

2. ##### 将插件安装进酷Q

   这个我就不用说明了吧

3. ##### 配置插件

   [示例配置文件](https://raw.githubusercontent.com/Kcass774/CQRconClient/master/config.yaml)

   配置文件应该放在酷q主程序目录下/data/app/ink.dalao.rcon/中

   > mcmode的作用是过滤控制台返回的颜色代码§

4. ##### 开始使用插件

   在群聊/私聊/讨论组中发送操作指令

   ```
%prefix%%server_name% %command% %optional%
   ```

   以上面的配置文件为例

   ```
连接
   #test connect
登录
   #test login 123456
运行远程指令
   #test command list
```

------

## 指令

- connect

  描述: 连接服务器

- close

  描述: 断开连接

- login

  描述: 使用%optional%进行登录

- command

  描述: 发送控制台指令%optional%

- add

  描述: 服务器返回的信息将发送到当前群聊/私聊/讨论组

- del

  描述: 服务器返回的信息将不再发送到当前群聊/私聊/讨论组

