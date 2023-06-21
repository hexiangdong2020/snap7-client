#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "snap7.h"

#ifdef OS_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

S7Object Client;

uint8_t exist_client = 0;
unsigned char Buffer[65536]; // 64 K buffer
int SampleDBNum = 1000;

char *Address;     // PLC IP Address
int Rack=0,Slot=2; // Default Rack and Slot

int ok = 0; // Number of test pass
int ko = 0; // Number of test failure

int JobDone=false;
int JobResult=0;

// 异步完成回调
void S7API CliCompletion(void *usrPtr, int opCode, int opResult)
{
    JobResult=opResult;
    JobDone = true;
}

//------------------------------------------------------------------------------
// hexdump, a very nice function, it's not mine.
// I found it on the net somewhere some time ago... thanks to the author ;-)
//------------------------------------------------------------------------------
#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif
void hexdump(void *mem, unsigned int len)
{
    unsigned int i, j;

    for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
    {
            /* print offset */
            if(i % HEXDUMP_COLS == 0)
            {
                    printf("0x%04x: ", i);
            }

            /* print hex data */
            if(i < len)
            {
                    printf("%02x ", 0xFF & ((char*)mem)[i]);
            }
            else /* end of block, just aligning for ASCII dump */
            {
                    printf("   ");
            }

            /* print ASCII dump */
            if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
            {
                    for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                    {
                            if(j >= len) /* end of block, not really printing */
                            {
                                    putchar(' ');
                            }
                            else if(isprint((((char*)mem)[j] & 0x7F))) /* printable char */
                            {
                                    putchar(0xFF & ((char*)mem)[j]);
                            }
                            else /* other char */
                            {
                                    putchar('.');
                            }
                    }
                    putchar('\n');
            }
    }
}

// 创建客户端
void plc_create(){
    Client=Cli_Create();
    Cli_SetAsCallback(Client, CliCompletion, NULL);
    exist_client = 1;
}

// 连接的原函数
int CliConnect()
{
    int Requested, Negotiated, res;
    res = Cli_ConnectTo(Client, Address,Rack,Slot);
    return !res;
}

// 连接
int plc_connect(){
    return CliConnect();
}

// 断开连接的原函数
void CliDisconnect()
{
     Cli_Disconnect(Client);
}

// 断开连接
void plc_disconnect(){
    CliDisconnect();
}

// 销毁
void plc_destory(){
    Cli_Destroy(&Client);
    exist_client = 0;
}

// 获取连接状态
int get_ping_timeout(){
    int x=0;
    Cli_GetParam(Client, p_i32_PingTimeout, &x);
    return x;
}

// 获取是否存在client
int get_exist_client(){
    return exist_client;
}

// 读取merker
uint8_t plc_read_merker(int addr){
    // 准备缓冲区
    uint8_t MB; // 1 Merker bytes
    Cli_MBRead(Client, addr, 1, &MB);
    return MB;
}

// 写入merker
void plc_write_merker(int addr, uint8_t value){
    Cli_MBWrite(Client, addr, 1, &value);
}

// 读取输入
uint8_t plc_read_input(int addr){
    // 准备缓冲区
    uint8_t PE; // 1 Input bytes
    Cli_EBRead(Client, addr, 1, &PE);
    return PE;
}

// 写入输入，实际上是无法写入的
void plc_write_input(int addr, uint8_t value){
    Cli_EBWrite(Client, addr, 1, &value);
}

// 读取输出
uint8_t plc_read_output(int addr){
    // 准备缓冲区，实际上是整个Qaddress的值，单个IO口是这个字节的一个位
    uint8_t PA; // 1 Output bytes
    Cli_ABRead(Client, addr, 1, &PA);
    return PA;
}

// 写入输出
void plc_write_output(int addr, uint8_t value){
    Cli_ABWrite(Client, addr, 1, &value);
}

// 读取定时器
uint8_t plc_read_timer(int addr){
    // 准备缓冲区
    uint8_t TM; // 1 Timer bytes
    Cli_TMRead(Client, addr, 1, &TM);
    return TM;
}

// 写入定时器，实际上也是没法写入的
void plc_write_timer(int addr, uint8_t value){
    Cli_TMWrite(Client, addr, 1, &value);
}

// 读取计数器
uint8_t plc_read_counter(int addr){
    // 准备缓冲区
    uint8_t CT; // 1 Timer bytes
    Cli_CTRead(Client, addr, 1, &CT);
    return CT;
}

// 写入计数器，实际上也是没法写入
void plc_write_counter(int addr, uint8_t value){
    Cli_CTWrite(Client, addr, 1, &value);
}

// 读取VB
uint8_t plc_read_vb(int addr){
    // 准备缓冲区
    uint8_t VB; // 1 VB bytes
    Cli_DBRead(Client, 1, addr, 1, &VB);
    return VB;
}

// 写入VB
void plc_write_vb(int addr, uint8_t value){
    Cli_DBWrite(Client, 1, addr, 1, &value);
}

// 读取VW
uint16_t plc_read_vw(int addr){
    // 准备缓冲区
    uint16_t VW; // 2 bytes
    Cli_DBRead(Client, 1, addr, 2, &VW);
    return VW;
}

// 写入VW
void plc_write_vw(int addr, uint16_t value){
    Cli_DBWrite(Client, 1, addr, 2, &value);
}

// 读取VD
uint32_t plc_read_vd(int addr){
    // 准备缓冲区
    uint32_t VD; // 4 bytes
    Cli_DBRead(Client, 1, addr, 4, &VD);
    return VD;
}

// 写入VD
void plc_write_vd(int addr, uint32_t value){
    Cli_DBWrite(Client, 1, addr, 4, &value);
}


int main(){
    Address="172.31.1.145";
    Rack=0;
    Slot=1;
    // 创建Client
    plc_create();
    // 连接PLC
    if(plc_connect()){
        printf("Connected!");
        plc_write_merker(0, 0xff);
        uint8_t x = plc_read_merker(0);
        printf("read merk: %x\n", x);
        uint8_t y = plc_read_input(0);
        printf("read input: %x\n", y);
        plc_write_output(0, 0xff);
        uint8_t z = plc_read_output(0);
        printf("read output: %x\n", z);
        uint8_t u = plc_read_timer(0);
        printf("read timer: %x\n", u);
        uint8_t v = plc_read_counter(0);
        printf("read counter: %x\n", v);
        plc_write_vb(0, 0xff);
        uint8_t w = plc_read_vb(0);
        printf("read vb: %x\n", w);
        plc_write_vw(0, 0xffff);
        uint16_t a = plc_read_vw(0);
        printf("read vw: %x\n", a);
        plc_write_vd(0, 0xffffffff);
        uint32_t b = plc_read_vd(0);
        printf("read vd: %x\n", b);
        plc_disconnect();
    } else{
        printf("Cannot connected!");
    }
    plc_destory();
}