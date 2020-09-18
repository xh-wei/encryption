#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "DES.h"

using namespace std;

int Sn[8][64] = {
        {
            14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
            0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
            4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
            15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
        },
        {
            15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
            3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
            0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
            13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
        },
        {
            10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
            13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
            13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
            1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
        },
        {
            7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
            13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
            10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
            3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
        },
        {
            2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
            14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
            4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
            11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
        },
        {
            12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
            10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
            9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
            4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
        },
        {
            4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
            13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
            1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
            6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
        },
        {
            13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
            1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
            7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
            2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
        }
    };
int Hex[16] = {0};//暂存输入的16位16进制明文
int Bin[64] = {0};//暂存2进制64位的明文
char cha[16] = {0};//暂时存放输入的数转换为16进制存在其他数组中
int L[32] = {0},R[32] = {0},Ri[32] = {0};//Ri暂存未改变的R0值
int K1[16] = {0},K2[64] = {0},C0_D0[56] = {0},K3[48] = {0};
    //分别为16进制16位的密钥，2进制64位密钥，密钥的置换结果以及2进制48位密钥
int E1[48] = {0};//初始变换E
int Displacement_1[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};//加密时的左移位数
int Displacement_2[16] = {0,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};//解密时的右移位数

DES::DES()
{
    Hexadecimal = Hex;
    Binary = Bin;
    ch = cha;
    L0 = L;
    R0 = R;
    E = E1;
    Key_16 = K1;
    Key_64 = K2;
    Key_48 = K3;
    C_D = C0_D0;
}

DES::DES(char str[])
{
    memcpy(ch,str,16);
    Hexadecimal = Hex;
    Binary =Bin;
    L0 = L;
    R0 = R;
    E = E1;
    Key_16 = K1;
    Key_64 = K2;
    Key_48 = K3;
    C_D = C0_D0;
}

void DES::setHexadecimal()//输入十六进制的明文
{
    std::cout << "Please enter clear text:" << std::endl;
    std::cin >>ch;
    stringstream ss;
    for(int i = 0 ; i < 16 ; i++)
    {
        ss << hex << ch[i];
        ss >> Hexadecimal[i];
        ss.clear();
    }
}

void DES::setBinary(int A[16],int B[64])
{
    for(int i = 0 ; i < 16 ; i++)
    {
        B[i*4+3] = A[i]%2;
        B[i*4+2] = (A[i]/2)%2;
        B[i*4+1] = (A[i]/4)%2;
        B[i*4] = (A[i]/8)%2;
    }
}

void DES::Initial_replacement_IP()
{
    int count = 0;
    int A[64] = {0};
    int B[8] = {2,4,6,8,1,3,5,7};
    memcpy(A,Binary,256);
    for(int i = 0; i < 8 ; i++)
    {
        for(int j = 7 ; j >= 0 ; j--)
        {
            Binary[count] = A[j*8 + B[i]-1];
            count++;
        }
    }
}

void DES::setS(int n)
{
    S = Sn[n];
}

void DES::setL0_R0()
{
    memcpy(L,Binary,128);
    L0 = L;
    memcpy(R,Binary+32,128);
    memcpy(Ri,R,sizeof(R));
    R0 = R;

}

void DES::text_32_48()
{
    memcpy(Ri,R,128);
    int count = 0;
    E1[count] = R0[31];
    for(count = 1; count < 48 ; count ++)
        E1[count] = R0[count - (count/6)*2-1];
        E = E1;
}

void DES::setKey()//输入密钥
{
    std::cout << "Please enter clear key:" << std::endl;
    std::cin >>ch;
    stringstream ss;
    for(int i = 0 ; i < 16 ; i++)
    {
        ss << hex << ch[i];
        ss >> Key_16[i];
        ss.clear();
    }
}

void DES::Key_Substitution_Selection_1()
{
    int A[56] = {   57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,
                    63,55,47,39,31,33,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
    for(int i = 0 ; i < 56 ; i++)
        C_D[i] = Key_64[A[i]-1];
    memcpy(Key_64,C_D,256);
}

void DES::Left_shift_of_circulation(int n)
{
    int A[48] = {   14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,
                41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
    for(int j = 0 ; j < n ; j++)
    {
        int t = C_D[0];
        for(int i = 0 ; i < 28 ; i++)
            C_D[i] = C_D[i+1];
        C_D[27] = t;
        t = C_D[28];
        for(int i = 28 ; i < 56 ; i++)
            C_D[i] = C_D[i+1];
        C_D[55] = t;
    }
    for(int i = 0 ; i < 48  ; i++)
    {
        Key_48[i] = C_D[A[i]-1];//因为A是按56位的位置输入的，但数组是从零开始的，所以输入时减一;
    }
}

void DES::Right_shift_of_circulation(int n)
{
    int A[48] = {   14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,
                41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
    for(int j = 0 ; j < n ; j++)
    {
        int t = C_D[27];
        for(int i = 26 ; i >= 0 ; i--)
            C_D[i+1] = C_D[i];
        C_D[0] = t;
        t = C_D[55];
        for(int i = 54 ; i >= 28 ; i--)
            C_D[i+1] = C_D[i];
        C_D[28] = t;
    }
    for(int i = 0 ; i < 48  ; i++)
    {
        Key_48[i] = C_D[A[i]-1];//因为A是按56位的位置输入的，但数组是从零开始的，所以输入时减一;
    }
}

void DES::XOR_48()
{
    for(int i = 0 ; i < 48 ; i++)
    {
        if(E[i] == Key_48[i])
            E[i] = 0;
        else
            E[i] = 1;
    }
}

void DES::Compression_transformation_S()
{
    for(int i = 0 ; i < 8 ; i++)
    {
        setS(i);
        int a = E[i*6+0]*2 + E[i*6+5];
        int b = E[i*6+1]*8 + E[i*6+2]*4 + E[i*6+3]*2 + E[i*6+4];
        int c = S[a*16+b];
        R0[i*4+3] = c%2;
        R0[i*4+2] = (c/2)%2;
        R0[i*4+1] = (c/4)%2;
        R0[i*4] = (c/8)%2;
    }
}

void DES::Permutation_operation_P()
{
    int A[32] = {   16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
                    2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25,};
    int B[32]  = {0};
    memcpy(B,R0,128);
    for(int i = 0 ; i < 32 ; i++)
        R0[i] = B[A[i]-1];
}

void DES::XOR_32()
{
    for(int i = 0 ; i < 32 ; i++)
    {
        if(L[i] == R[i])
            R[i] = 0;
        else
            R[i] = 1;
    }
    memcpy(L,Ri,128);
}

void DES::L0_R0_Show(int *P)
{
    for(int i = 0 ; i < 8 ; i++)
    {
        std::cout << hex <<P[i*4]*8 + P[i*4+1]*4 + P[i*4+2]*2 + P[i*4+3];
    }
    cout <<"\t";
}

void DES::Key_48_Show()
{
    for(int i = 0 ; i < 12 ; i++)
    {
        std::cout << hex <<Key_48[i*4]*8 + Key_48[i*4+1]*4 + Key_48[i*4+2]*2 + Key_48[i*4+3];
    }
    std::cout << std::endl;
}

void DES::Inverse_Initial_Permutation_IP()
{
    int A[64] = {0};
    int count = 0;
    int IP_1[64] = {40,     8,      48,    16,   56,    24,    64,   32,
                    39,     7,      47,    15,    55,   23,    63,   31,
                    38,     6,      46,    14,    54,   22,    62,   30,
                    37,     5,      45,    13,    53,   21,    61,   29,
                    36,     4,      44,    12,    52,   20,    60,   28,
                    35,     3,      43,    11,    51,   19,    59,   27,
                    34,     2,      42,    10,    50,   18,    58,   26,
                    33,     1,      41,    9,     49,   17,    57,   25};
    memcpy(Binary,L0,128);
    memcpy(Binary+32,R0,128);
    memcpy(A,Binary,256);
    for(int i = 0; i < 64 ; i++)
            Binary[i] = A[IP_1[i]-1];
}

void DES::text_show(int *q,string str)
{
        cout <<  str <<"\t";
    for(int i = 0 ; i < 16 ; i++)
    {
        std::cout << hex <<q[i*4]*8 + q[i*4+1]*4 + q[i*4+2]*2 + q[i*4+3] <<" ";
    }
    cout <<endl;
}

void DES::jiami()
{
    setHexadecimal();
    setBinary(Hexadecimal,Binary);
    Initial_replacement_IP();    
    setL0_R0();
    setKey();
    setBinary(Key_16,Key_64);
    Key_Substitution_Selection_1();
    cout <<"\t L\t\t" <<" R\t\t" <<"K\t\t" <<endl;
    cout <<"0\t";
    L0_R0_Show(L0);
    L0_R0_Show(R0);
    cout <<endl;
    for(int i = 0 ; i < 16 ; i++)
    {
        cout << i+1 << "\t";
        text_32_48();
        Left_shift_of_circulation(Displacement_1[i]);
        XOR_48();
        Compression_transformation_S();
        Permutation_operation_P();
        XOR_32();
        if(i == 15)
        {
            memcpy(L0,R0,128);
            memcpy(R0,Ri,128);
        }
        L0_R0_Show(L0);
        L0_R0_Show(R0);
        Key_48_Show();
    }

    Inverse_Initial_Permutation_IP();
    text_show(Binary,"cipher");
}

void DES::jiemi()
{
    Initial_replacement_IP();    
    setL0_R0();
    setBinary(Key_16,Key_64);
    Key_Substitution_Selection_1();
    cout <<"\t L\t\t" <<" R\t\t" <<"K\t\t" <<endl;
    cout <<"0\t";
    L0_R0_Show(L0);
    L0_R0_Show(R0);
    cout <<endl;
    for(int i = 0 ; i < 16 ; i++)
    {
        cout << i+1 << "\t";
        text_32_48();
        Right_shift_of_circulation(Displacement_2[i]);
        XOR_48();
        Compression_transformation_S();
        Permutation_operation_P();
        XOR_32();
        if(i == 15)
        {
            memcpy(L0,R0,128);
            memcpy(R0,Ri,128);
        }
        L0_R0_Show(L0);
        L0_R0_Show(R0);
        Key_48_Show();
    }
    Inverse_Initial_Permutation_IP();
    text_show(Binary,"text");
}

