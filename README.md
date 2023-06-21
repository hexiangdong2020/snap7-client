# snap7-client
对Snap7中Client的Plain C绑定的封装。

参考文章：https://zhuanlan.zhihu.com/p/387439079
## 编译client.exe
```bash
gcc -o client.exe ./client.c -static-libgcc ./snap7.lib -I"./"
```

## 运行client.exe
### 查找PLC的IP地址
在STEP 7-MicroWIN SMART中可以查找到PLC的IP地址为172.31.1.145，而S7-200-SMART的rack=0，slot=1

![查找PLC的IP地址](https://github.com/hexiangdong2020/snap7-client/blob/main/%E6%9F%A5%E6%89%BEPLC%E7%9A%84IP%E5%9C%B0%E5%9D%80.PNG?raw=true 查找PLC的IP地址)

![西门子PLC的机架和卡槽](https://github.com/hexiangdong2020/snap7-client/blob/main/PLC%E7%9A%84%E6%9C%BA%E6%9E%B6%E5%92%8C%E5%8D%A1%E6%A7%BD.png?raw=true 西门子PLC的机架和卡槽)

### 设置主机的IP地址
将主机与PLC的有线连接设为固定IP地址，前三个字段需要和PLC地址保持一致，可以设为172.31.1.1
### 运行client.exe
使用如下方法可以简单测试与PLC的连接
```bash
./client.exe 172.31.1.145 0 1
```
运行结果如下：
![client.exe的运行结果](https://github.com/hexiangdong2020/snap7-client/blob/main/client%E8%BF%90%E8%A1%8C%E7%BB%93%E6%9E%9C.PNG?raw=true client.exe的运行结果)

## 编译plc.exe
```bash
gcc -o plc.exe ./plc.c -static-libgcc ./snap7.lib -I"./"
```
