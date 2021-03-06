/*-------------------------------------------------------
      Data Encryption Standard  56位密钥加密64位数据 
				  2011.10
--------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "des_encode.h"
#include "tables.h"
#include "inifile.h"
#define BUFFER_SIZE	1024
#if 0
int main(int argc, char *argv[])
{
	int i,tmp;	
	
	char key[9] = {"dingxuyan"}; 
	char usrstr[BUFFER_SIZE];
	char pwdstr[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	
	void *fpini = IniOpenFile(INI_PATH, 1);
    	if(fpini == NULL)
    	{
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "read ini file failure!\n");
		printf("%s",buffer);
		printf("read ini file failed!\n");
		return -1;
    	}        
  
	IniGetString("DB", "username", usrstr, BUFFER_SIZE, fpini);
    	printf("usr:%s\n", usrstr);

	get_pass("DB", "password",pwdstr,fpini);
	printf("pass=%s\n",pwdstr);
	IniCloseFile(fpini);	

}
#endif
int get_pass(char *section, char *key, char *value, void *fpini)
{
	char pass[BUFFER_SIZE], aft_code[BUFFER_SIZE];
	int tmp;
	if(fpini == NULL)
		return -1;

	IniGetString( section,key, pass, BUFFER_SIZE, fpini);
        //printf("pwd:%s\n", pass);
	if(0x1D == pass[0] && 0x1E == pass[1] && 0x1F == pass[2])
	{
		decode_all( pass+3, BUFFER_SIZE, value, BUFFER_SIZE, key);
		return 1;
	}
	else
	{
		strncpy( value, pass, strlen(pass)+1);
		encode_all( value, BUFFER_SIZE, aft_code+3, BUFFER_SIZE, key);
		aft_code[0] = 0x1D;
		aft_code[1] = 0x1E;
		aft_code[2] = 0x1F;

		tmp = IniSetString(section, key, aft_code,fpini);
		return 0;
	}

}
int decode_all( char *in, int size1, char *out, int size2, char *key)
{
	int i, tmp;
	tmp = strlen(in)/16;
	for(i=0; i<tmp; i++)
	{
		decode(in+16*i, key, out+8*i);
	}
//	printf("decode msg:\n");
//	for(i=0; i<tmp*8; i++)
//		printf("%c", out[i]);
//	printf("\n");
	//printf("%s\n",out);

}
int encode_all( char *in, int size1,  char *out, int size2, char *key)
{
	int len1, len2, i, tmp;

	len1 = strlen(in);
	
	if( 0 != len1%8)
	{
		tmp = 8 - len1%8;
	}
	
	for(i=0; i< tmp; i++)
		in[len1+i] = ' ';
	in[len1+i] = 0;

	tmp = strlen(in)/8;
	for(i=0; i<tmp; i++)
	{
		encode(in+i*8, key, out+i*16);
	}
	out[tmp*16] = 0;
	printf("总的密文为：\n");
	for(i=0; i<tmp*16; i++)
		printf("%c",out[i]);
	printf("\n");
//	printf("%s\n",out);
}
//in为8为数组，out为16位数组
int encode(char *in, char *key, char *out)
{

	int i=0; 
	char mes_hex[16]={0};         // 16个字符数组用于存放 64位16进制的密文
 	char MyKey[8]={0};           // 初始密钥 8字节*8
	char YourKey[8]={0};         // 输入的解密密钥 8字节*8
	char MyMessage[8]={0};       // 初始明文 
	
	SetKey(key);               // 设置密钥 得到子密钥Ki
	
	PlayDes(out,in);   // 执行DES加密
	printf("after encode\n");
	for(i=0; i<16; i++)
	printf("%c", out[i]);
	printf("\n");
}
//in 为16位的数组，out为8为数组
int decode(char *in, char *key, char *out)
{

	SetKey(key);                                       // 设置密钥

	KickDes(out,in);                     // 解密输出到MyMessage
}
int test(char *in, char *key, char *out)
{
	
	
	
	//printf("Welcome! Please input your Message(64 bit):\n");
	//gets(MyMessage);            // 明文
	//printf("Please input your Secret Key:\n");
	//gets(MyKey);                // 密钥
/*
	while(MyKey[i]!='\0')        // 计算密钥长度
	{
		i++;
	}

	while(i!=8)                  // 不是8 提示错误
	{
		//printf("Please input a correct Secret Key!\n");
		//gets(MyKey);
		i=0;
		while(MyKey[i]!='\0')    // 再次检测
		{
			i++;
		}
	}
*/
//	MesHex[9]='r';
/*
	printf("Your Message is Encrypted!:\n");  // 信息已加密
	for(i=0;i<16;i++)           
	{
		printf("%c ",MesHex[i]);
	}

	printf("\n");
	printf("\n");
	
	printf("Please input your Secret Key to Deciphering:\n");  // 请输入密钥以解密
	gets(YourKey);                                         // 得到密钥
	*/





	
	/*
	printf("Deciphering Over !!:\n");                     // 解密结束
	for(i=0;i<8;i++)
	{
		printf("%c ",MyMessage[i]);
	}
	printf("\n");
	*/
		
/*------------------------------------------------*/	
}

/*-------------------------------
 把DatIn开始的长度位Len位的二进制
 复制到DatOut后
--------------------------------*/
void BitsCopy(bool *DatOut,bool *DatIn,int Len)     // 数组复制 OK 
{
	int i=0;
	for(i=0;i<Len;i++)
	{
		DatOut[i]=DatIn[i];
	}
}

/*-------------------------------
 字节转换成位函数 
 每8次换一个字节 每次向右移一位
 和1与取最后一位 共64位 
--------------------------------*/
void ByteToBit(bool *DatOut,char *DatIn,int Num)       // OK
{
	int i=0;
	for(i=0;i<Num;i++)
	{
		DatOut[i]=(DatIn[i/8]>>(i%8))&0x01;   
	}                                       
}

/*-------------------------------
 位转换成字节函数
 字节数组每8次移一位
 位每次向左移 与上一次或   
---------------------------------*/
void BitToByte(char *DatOut,bool *DatIn,int Num)        // OK
{
	int i=0;
	for(i=0;i<(Num/8);i++)
	{
		DatOut[i]=0;
	} 
	for(i=0;i<Num;i++)
	{
		DatOut[i/8]|=DatIn[i]<<(i%8);	
	}		
}


/*----------------------------------
 二进制密文转换为十六进制
 需要16个字符表示
-----------------------------------*/
void BitToHex(char *DatOut,bool *DatIn,int Num)
{
	int i=0;
	for(i=0;i<Num/4;i++)
	{
		DatOut[i]=0;
	}
	for(i=0;i<Num/4;i++)
	{
		DatOut[i] = DatIn[i*4]+(DatIn[i*4+1]<<1)
					+(DatIn[i*4+2]<<2)+(DatIn[i*4+3]<<3);
		if((DatOut[i]%16)>9)
		{
			DatOut[i]=DatOut[i]%16+'7';       //  余数大于9时处理 10-15 to A-F
		}                                     //  输出字符 
		else
		{
			DatOut[i]=DatOut[i]%16+'0';       //  输出字符	   
		}
	}
	
}

/*---------------------------------------------
 十六进制字符转二进制
----------------------------------------------*/
void HexToBit(bool *DatOut,char *DatIn,int Num)
{
	int i=0;                        // 字符型输入 
	for(i=0;i<Num;i++)
	{
		if((DatIn[i/4])>'9')         //  大于9 
		{
			DatOut[i]=((DatIn[i/4]-'7')>>(i%4))&0x01;   			
		}
		else
		{
			DatOut[i]=((DatIn[i/4]-'0')>>(i%4))&0x01; 	
		} 
	}	
}

// 表置换函数  OK
void TablePermute(bool *DatOut,bool *DatIn,const char *Table,int Num)  
{
	int i=0;
	static bool Temp[256]={0};
	for(i=0;i<Num;i++)                // Num为置换的长度 
	{
		Temp[i]=DatIn[Table[i]-1];  // 原来的数据按对应的表上的位置排列 
	}
	BitsCopy(DatOut,Temp,Num);       // 把缓存Temp的值输出 
} 

// 子密钥的移位
void LoopMove(bool *DatIn,int Len,int Num) // 循环左移 Len数据长度 Num移动位数
{
	static bool Temp[256]={0};    // 缓存   OK
	BitsCopy(Temp,DatIn,Num);       // 将数据最左边的Num位(被移出去的)存入Temp 
	BitsCopy(DatIn,DatIn+Num,Len-Num); // 将数据左边开始的第Num移入原来的空间
	BitsCopy(DatIn+Len-Num,Temp,Num);  // 将缓存中移出去的数据加到最右边 
} 

// 按位异或
void Xor(bool *DatA,bool *DatB,int Num)           // 异或函数
{
	int i=0;

	for(i=0;i<Num;i++)
	{
		DatA[i]=DatA[i]^DatB[i];                  // 异或 
	}
} 

// 输入48位 输出32位 与Ri异或
void S_Change(bool DatOut[32],bool DatIn[48])     // S盒变换
{
	int i,X,Y;                                    // i为8个S盒 
	for(i=0,Y=0,X=0;i<8;i++,DatIn+=6,DatOut+=4)   // 每执行一次,输入数据偏移6位 
	{    										  // 每执行一次,输出数据偏移4位
		Y=(DatIn[0]<<1)+DatIn[5];                          // af代表第几行
		X=(DatIn[1]<<3)+(DatIn[2]<<2)+(DatIn[3]<<1)+DatIn[4]; // bcde代表第几列
		ByteToBit(DatOut,&S_Box[i][Y][X],4);      // 把找到的点数据换为二进制	
	}
}

// F函数
void F_Change(bool DatIn[32],bool DatKi[48])       // F函数
{
	static bool MiR[48]={0};             // 输入32位通过E选位变为48位
	TablePermute(MiR,DatIn,E_Table,48); 
	Xor(MiR,DatKi,48);                   // 和子密钥异或
	S_Change(DatIn,MiR);                 // S盒变换
	TablePermute(DatIn,DatIn,P_Table,32);   // P置换后输出
}



void SetKey(char KeyIn[8])               // 设置密钥 获取子密钥Ki 
{
	int i=0;
	static bool KeyBit[64]={0};                // 密钥二进制存储空间 
	static bool *KiL=&KeyBit[0],*KiR=&KeyBit[28];  // 前28,后28共56
	ByteToBit(KeyBit,KeyIn,64);                    // 把密钥转为二进制存入KeyBit 
	TablePermute(KeyBit,KeyBit,PC1_Table,56);      // PC1表置换 56次
	for(i=0;i<16;i++)
	{
		LoopMove(KiL,28,Move_Table[i]);       // 前28位左移 
		LoopMove(KiR,28,Move_Table[i]);	      // 后28位左移 
	 	TablePermute(SubKey[i],KeyBit,PC2_Table,48); 
	 	// 二维数组 SubKey[i]为每一行起始地址 
	 	// 每移一次位进行PC2置换得 Ki 48位 
	}		
}

void PlayDes(char MesOut[8],char MesIn[8])  // 执行DES加密
{                                           // 字节输入 Bin运算 Hex输出 
	int i=0;
	static bool MesBit[64]={0};        // 明文二进制存储空间 64位
	static bool Temp[32]={0};
	static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // 前32位 后32位 
	ByteToBit(MesBit,MesIn,64);                 // 把明文换成二进制存入MesBit
	TablePermute(MesBit,MesBit,IP_Table,64);    // IP置换 
	for(i=0;i<16;i++)                       // 迭代16次 
	{
		BitsCopy(Temp,MiR,32);            // 临时存储
		F_Change(MiR,SubKey[i]);           // F函数变换
		Xor(MiR,MiL,32);                  // 得到Ri 
		BitsCopy(MiL,Temp,32);            // 得到Li 
	}					
	TablePermute(MesBit,MesBit,IPR_Table,64);
	BitToHex(MesOut,MesBit,64);
}

void KickDes(char MesOut[8],char MesIn[8])       // 执行DES解密
{												// Hex输入 Bin运算 字节输出 
	int i=0;
	static bool MesBit[64]={0};        // 密文二进制存储空间 64位
	static bool Temp[32]={0};
	static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // 前32位 后32位
	HexToBit(MesBit,MesIn,64);                 // 把密文换成二进制存入MesBit
	TablePermute(MesBit,MesBit,IP_Table,64);    // IP置换 
	for(i=15;i>=0;i--)
	{
		BitsCopy(Temp,MiL,32);
		F_Change(MiL,SubKey[i]);
		Xor(MiL,MiR,32);
		BitsCopy(MiR,Temp,32);
	}	
	TablePermute(MesBit,MesBit,IPR_Table,64);
	BitToByte(MesOut,MesBit,64);		
} 








