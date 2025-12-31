# sc2-replay-fix

一个用于修复《星际争霸 II》录像版本号的小工具，能够自动监听并修复新增的立项文件，使当前客户端能够正常观看录像。

## 背景

国服回归后，由于网易的一次编辑器更新导致录像文件无法打开，原因是当前客户端生成的录像文件版本号不合法。

## 功能

- 自动监听新增的录像文件；
- 修改新增录像文件的版本号字段；
- 备份修改前的立项文件。

## 限制

- 不保证所有录像文件都能够修复，比如多人对战中，玩家在对局结束前退出了游戏，此时录像大概率修复失败。
- 仅修改录像头中的版本号（实际是版本号的major,minor,patch字段，build字段不处理）；
- 仅测试过当前版本的录像。
## 构建

### 环境要求
- Windows 10 / 11
- C++20
- CMake >= 3.20
- MinGW 或 MSVC

### 构建步骤

```bash
git clone https://github.com/sym191/sc2replayfix.git
cd sc2replayfix
mkdir build
cd build
cmake ..
cmake --build .
```

## 使用方法
1. 修改`config.conf`文件
```angular2html
# 星际争霸录像保存路径
path = C:\Users\xxx\Documents\StarCraft II\Accounts\\Replays\Multiplayer
# 当前游戏的版本号，在战网界面查看，保证前三位一致即可，默认不需要修改
version = 5.0.15.95740
```
2. 保证在游戏过程中，进程常驻在后台。

## 声明

本项目仅用于学习和研究文件格式。
使用本工具修改录像文件可能导致文件损坏，
请自行承担风险。