#ifndef DES_CPP
#define DES_CPP

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

class DES
{
    public:
        int *Hexadecimal;//长度为16的数组，用来存放输入的16进制明文
        int *Binary;//用来
        char *ch;//暂时存放输入的数转换为16进制存在其他数组中
        int * S;//存放S盒的内容
        int *L0 , *R0, *E;//分别为存放明文的左半部分与右半部分以及存放R0转换成的48位数组
        int *Key_16,*Key_64,*C_D,*Key_48;//分别为16进制16位的密钥，2进制64位密钥，密钥的置换结果以及2进制48位密钥
    public:
        DES();
        DES(char str[]);
        void setHexadecimal();//输入16进制的明文
        void setBinary(int A[16],int B[64]);//将16进制的明文或者密钥转换为2进制
        void setS(int n);//n表示S使用第几个表
        void Initial_replacement_IP();//初始置换IP
        void setL0_R0();//将明文分为L0与R0两个部分
        void text_32_48();//将32位的R0扩展为48位
        void setKey();//输入密钥
        void Key_Substitution_Selection_1();//密钥的置换选择1
        void Left_shift_of_circulation(int n);//密钥的循环左移+置换选择2
        void Right_shift_of_circulation(int n);//解密时密钥循环右移+置换选择2
        void XOR_48();//进行异或操作
        void Compression_transformation_S();//压缩替换S盒
        void XOR_32();//L0与R0进行异或操作
        void Permutation_operation_P();//置换运算P
        void L0_R0_Show(int *p);//每一步加密后的输出
        void Key_48_Show();//密钥变化的输出
        void jiami();//加密操作
        void Inverse_Initial_Permutation_IP();//逆初始置换IP
        void text_show(int *q,string str);//输出得到的明文
        void jiemi();//解密操作
};

#endif
