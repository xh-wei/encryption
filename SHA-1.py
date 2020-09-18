from math import floor
import binascii
def circular_shift_left(int_value, k, bit=32):  #实现循环左移，因为python中左移不是循环左移
    bit_string = '{:0%db}' % bit
    bin_value = bit_string.format(int_value)
    bin_value = bin_value[k:] + bin_value[:k]
    int_value = int(bin_value, 2)
    return int_value

def encode(s):  #转二进制保存
    a = []
    for c in s:
        a = a + [0]
        for n in list(bin(ord(c)).replace('0b', '')):
            a.append(int(n))
    return a

def create_W(s):
    W = []
    for b in range(0, 16):
        W.append(bin_hex(s[b * 32: b * 32 + 32]))
        b = b + 1
    for i in range(16, 80):
        M = circular_shift_left(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16],1,32)%(2**32)
        W.append(M)
        i = i + 1
    return W

def bin_hex(S):
    T = 0
    a = 0
    for b in range(0,8):
        a = int(S[b*4]*8 + S[b*4+1] *4 + S[b*4+2] *2 + S[b*4+3])
        T = T + a * (16**(7-b))
        b = b + 1
    return T
    # return eval(hex(T).replace('0x',''))


def f(t,B,C,D) :    #四个原始函数
    i = floor(t/20)
    return {
        0 : (B&C)|((~B)&D),
        1 : B^C^D,
        2 : (B&C)|(B&D)|(C&D),
        3 : B^C^D,
    }.get(i,'error')




K = [ 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 ]  #每一个循环需要加入的常数K

str = input("input:") #存放输入的字符串
s_bin = list(encode(str)) #将输入的字符转换为2进制
length = len(s_bin) #保存二进制字符的长度
s_bin = s_bin + [1] + (447-(length%512)) * [0]
for n in list('{:064b}'.format(length)):
    s_bin.append(int(n))
H0 = 0x67452301
H1 = 0xefcdab89
H2 = 0x98badcfe
H3 = 0x10325476
H4 = 0xc3d2e1f0
W = create_W(s_bin)
A = H0
B = H1
C = H2
D = H3
E = H4
print('\t\t',hex(H0),hex(H1),hex(H2),hex(H3),hex(H4))
for i in range(0,80):
    temp1 = circular_shift_left(A,5,32)
    temp = (temp1 + f(i,B,C,D) + E + W[i] + K[floor(i/20)])%2**32
    E = D
    D = C
    C = (circular_shift_left(B,30,32))%2**32
    B = A
    A = temp
    print(i,'\t\t',hex(A),hex(B),hex(C),hex(D),hex(E))
H0 = (H0 + A)%2**32
H1 = (H1 + B)%2**32
H2 = (H2 + C)%2**32
H3 = (H3 + D)%2**32
H4 = (H4 + E)%2**32

print('result\t',hex(H0),hex(H1),hex(H2),hex(H3),hex(H4))