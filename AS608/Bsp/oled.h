#ifndef __OLED_H
#define __OLED_H	 
#include "i2c.h"
 #define u8 uint8_t
 #define u32 uint32_t
//    һЩʹ������
//	OLED_ShowStr(0, 0, "hello world", 2);//��ʾ�ַ���
//	OLED_ShowStr(0, 2, "hello world", 1);//��ʾ�ַ���
//	OLED_ShowCN_STR(0, 4 , 0 , 8);
//	sprintf(num_temp_buffer,"show num:%0.2f",num);
//	OLED_ShowStr(0, 6, num_temp_buffer, 2);//��ʾ�ַ���
//	OLED_CLS();
//	OLED_DrawBMP(0,0,128,7,BMP2);
#define OLED0561_ADD	0x78  // OLED��I2C��ַ����ֹ�޸ģ�
#define COM				0x00  // OLED ָ���ֹ�޸ģ�
#define DAT 			0x40  // OLED ���ݣ���ֹ�޸ģ�

void WriteCmd(unsigned char I2C_Command);//д����
void WriteDat(unsigned char I2C_Data);//д����
void OLED_Init(void);//��ʼ��
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);//ȫ�����
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);//��ʾ�ִ�
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);//��ʾ����
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);//��ʾͼƬ��ȡģ���������У��棬�������������
void OLED_ShowCN_STR(u8 x , u8 y , u8 begin , u8 num);  //�Լ�д����������ַ���
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);//size2(16|12)

void OLED_ClearGivenChar(uint8_t Line);

#endif
