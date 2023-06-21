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
### 设置主机的IP地址
将主机与PLC的有线连接设为固定IP地址，前三个字段需要和PLC地址保持一致，可以设为172.31.1.1
### 运行client.exe
使用如下方法可以简单测试与PLC的连接
```bash
./client.exe 172.31.1.145 0 1
```
运行结果如下：
