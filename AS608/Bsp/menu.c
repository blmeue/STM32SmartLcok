#include "menu.h"
#include "oled.h"
#include "AS608.h"
#include "key.h"
#include "hhStore.h"
#include "stdio.h"
#include "string.h"
#include "myusart.h"
#include "SYN6288.h"

int keynum;

uint16_t input_user_password[6]={0};//��������Ľ�������
uint32_t k=0;//������±�

uint16_t input_admin_password[6]={0};//��������Ĺ���Ա����
uint32_t k_admin=0;//����Ĺ���Ա�����±�

unsigned char user_flag=0;//�ж��޸������Ƿ�ɹ�
unsigned char admin_flag=0;//�ж��޸Ĺ���Ա�����Ƿ�ɹ�

unsigned char unlock=0;

unsigned char line_flag=0;//ѡ��˵�
unsigned char figure_flag=0;//ָ���Ƿ���ñ�־
unsigned char view=0;//����

unsigned char key_user_count=0;//��������������
unsigned char key_user_flag=0;//����������ñ�־
unsigned char key_user_flag0=0;//�����������������־

unsigned char key_admin_count=0;//����Ա��������������
unsigned char key_admin_flag=0;//����Ա����������ñ�־
unsigned char key_admin_flag0=0;//�����������������־

uint16_t admin_data[6]={0};//�µĹ���Ա����
uint16_t user_data[6]={0};//�µĽ�������

uint16_t admin_index=0;//�µĽ��������±�
uint16_t user_index=0;//�µĹ���Ա�����±�

unsigned char figuer_count=0;//ָ����֤ʧ�ܴ���

//��������
void main_menu(void)
{
     OLED_ShowChar(0+16,0,'S',16);
     OLED_ShowChar(8+16,0,'T',16);
     OLED_ShowChar(16+16,0,'M',16);
     OLED_ShowNum(22+16,0,32,2,16);

     OLED_ShowCN(38+16,0,3);//��
     OLED_ShowCN(54+16,0,4);//��
     OLED_ShowCN(70+16,0,5);//��
     OLED_ShowCN(86+16,0,6);//��
     
     OLED_ShowCN(0,2,9)  ; //��
     OLED_ShowCN(16,2,10); //��
     OLED_ShowCN(32,2,11); //��
     OLED_ShowCN(48,2,12); //��
     OLED_ShowCN(64,2,6) ; //��
     OLED_ShowCN(80,2,13); //��
     OLED_ShowCN(96,2,14); //��
     OLED_ShowCN(112,2,15); //: 
    
     OLED_ShowCN(0,6,46);  //��
     OLED_ShowCN(16,6,47); //��
     
    OLED_ShowCN(96,6,48);  //ȷ
    OLED_ShowCN(112,6,49); //�� 
    
    Figure();
}

//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
//m[0~16]:0��������Ϊ������16���������������
//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
//t[0~5]:0�ʶ�����������5�ʶ��������
//SYN_FrameInfo(2, "[v7][m1][t5]��ӭʹ��SYN6288");
uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����

//�ж������Ƿ���ȷ
void denglu_jduge(void)
{
    //�ж����������Ƿ���ȷ
    if(memcmp(input_user_password,Store_Data,sizeof(input_user_password))==0)
    {
        //�����ɹ�
        OLED_CLS();
        view=1;
        SYN_FrameInfo(2,"[v7][m1][t5]�����ɹ�");
        HAL_Delay(1000);
        key_user_count=0;
        key_user_flag=0;
    }
    else
    {
        //����ʧ��
        OLED_CLS();
        
        SYN_FrameInfo(0,"[v7][m1][t5]����ʧ��");
        //k=0;
        HAL_Delay(1000);
        key_user_count++;
        if(key_user_count==3)
        {
            key_user_flag=1;
            OLED_CLS();
            view=17;
        }
        else
        {
           view=2; 
        }
    }
//    memset(input_user_password,0,sizeof(input_user_password));
//    k=0;
}

//�����ɹ�����
void Unlock_success(void)
{
     OLED_ShowCN(0,2,12)  ; //��
     OLED_ShowCN(16,2,6); //��
     OLED_ShowCN(32,2,16); //��
     OLED_ShowCN(48,2,17); //��
     OLED_ShowCN(90,6,23); //��
     OLED_ShowCN(106,6,24); //��
     figure_flag=0;
     figuer_count=0;
//     k_admin=0;
//     memset(input_admin_password,0,sizeof(input_admin_password));
     //k=0;
     //memset(input_user_password,0,sizeof(input_user_password));
}

//����ʧ�ܽ���
void Unlock_failed(void)
{
     OLED_ShowCN(0,2,12)  ; //��
     OLED_ShowCN(16,2,6)  ; //��
     OLED_ShowCN(32,2,18) ; //ʧ
     OLED_ShowCN(48,2,19) ; //��
     OLED_ShowCN(64,2,22) ; //��
     OLED_ShowCN(80,2,9)  ; //��
     OLED_ShowCN(96,2,20) ; //��
     OLED_ShowCN(112,2,21); //�� 
     if(figure_flag==0&&key_user_flag==0)
     {
         Figure();//ָ�ƽ���ģ��
         OLED_ClearGivenChar(0);
     }
     else
     {
         OLED_ShowCN(16,0,35) ; //ָ
         OLED_ShowCN(32,0,36) ; //��
         OLED_ShowCN(48,0,50) ; //��
         OLED_ShowCN(64,0,56) ; //��
         OLED_ShowCN(80,0,57) ; //��
     }
     
     OLED_ShowCN(0,6,46);  //��
     OLED_ShowCN(16,6,47); //��
     
    OLED_ShowCN(96,6,48);  //ȷ
    OLED_ShowCN(112,6,49); //��
     
}

//��ֹ�����û��������
void forbid_user(void)
{
    //����������Σ��������
    OLED_ShowCN(16,0,13); //��
    OLED_ShowCN(32,0,14); //��
    OLED_ShowCN(48,0,58); //��
    OLED_ShowCN(64,0,59); //��
    OLED_ShowNum(80,0,3,1,16);//3
    OLED_ShowCN(96-8,0,60); //��
    
    OLED_ShowNum(16,2,30-sec1,2,16);//3
    OLED_ShowCN(32,2,61); //��
    OLED_ShowCN(48,2,62); //��
    OLED_ShowCN(64,2,54); //��
    OLED_ShowCN(80,2,21); //��
        
}

//�������Ա�������
void admin_input(void)
{
     OLED_ShowCN(32,0,9)  ; //��
     OLED_ShowCN(48,0,10); //��
     OLED_ShowCN(64,0,11); //��
     OLED_ShowCN(0,2,27); //��
     OLED_ShowCN(16,2,28) ; //��
     OLED_ShowCN(32,2,29) ;//Ա
     OLED_ShowCN(48,2,13); //��
     OLED_ShowCN(64,2,14); //��
     OLED_ShowCN(80,2,15); //: 
    
    OLED_ShowCN(0,6,46);  //��
    OLED_ShowCN(16,6,47); //��
    
    //���أ��ص�ϵͳ���ý���
    OLED_ShowCN(90,6,44);  //��
    OLED_ShowCN(106,6,45); //��  
}

//�жϹ���Ա�����Ƿ���ȷ
void admin_judge(void)
{
    //�ж����������Ƿ���ȷ
    if(memcmp(input_admin_password,Store_Admin,sizeof(input_admin_password))==0)
    {
        //����Ա������ȷ����������ģʽ������Աģʽ��
        OLED_CLS();
        view=3;
        SYN_FrameInfo(2,"[v7][m1][t5]�������Աģʽ");
        //k_admin=0;
        line_flag=0;
        HAL_Delay(1000);
        key_admin_count=0;
        key_user_flag=0;
    }
    else
    {
        //����Ա�������
        OLED_CLS();
        //view=19;
        SYN_FrameInfo(2,"[v7][m1][t5]����Ա�������");
//        k_admin=0;
        HAL_Delay(1000);
        key_admin_count++;
        if(key_admin_count==3)
        {
            key_admin_flag=1;
        }
        else
        {
            view=19;
        }
    }
    k_admin=0;
    memset(input_admin_password,0,sizeof(input_admin_password));
}

//����Ա��¼ʧ��
void admin_fail(void)
{
    //����Ա�������
     OLED_ShowCN(16-8,0,27)  ; //��
     OLED_ShowCN(32-8,0,28)  ; //��
     OLED_ShowCN(48-8,0,29) ; //Ա
     OLED_ShowCN(64-8,0,13); //��
     OLED_ShowCN(80-8,0,14); //��
     OLED_ShowCN(96-8,0,58); //��
     OLED_ShowCN(112-8,0,59); //��
    
     OLED_ShowCN(48,2,9)  ; //��
     OLED_ShowCN(64,2,20) ; //��
     OLED_ShowCN(80,2,21); //�� 
    
    OLED_ShowCN(0,6,46);  //��
    OLED_ShowCN(16,6,47); //��
    
    //���أ��ص�ϵͳ���ý���
    OLED_ShowCN(90,6,44);  //��
    OLED_ShowCN(106,6,45); //��
}

//���ù���Ա��¼
void forbid_admin(void)
{
    //����Ա����������Σ��������
    OLED_ShowCN(16-8,0,27); //��
    OLED_ShowCN(32-8,0,28); //��
    OLED_ShowCN(48-8,0,29); //Ա
    OLED_ShowCN(64-8,0,13); //��
    OLED_ShowCN(80-8,0,14); //��
    OLED_ShowCN(96-8,0,58); //��
    OLED_ShowCN(112-8,0,59); //��
    
    OLED_ShowNum(8,2,3,1,16);//3
    OLED_ShowCN(16,2,60); //��
    OLED_ShowCN(32,2,9); //��
    OLED_ShowNum(48,2,30-sec2,2,16);//3
    OLED_ShowCN(64,2,61); //��
    OLED_ShowCN(80,2,62); //��
    OLED_ShowCN(96,2,54); //��
    OLED_ShowCN(112,2,21); //��
}

//�˵�ҳ��
void admin_menu(void)
{
    //1���޸Ľ�������
    if(line_flag==0)
    {
        OLED_ShowCN(0,0,34) ; // ��
    }
    else
    {
        OLED_ShowChar(0,0,' ',16) ;
    }
    OLED_ShowCN(16+4,0,25); //��
    OLED_ShowCN(32+4,0,26); //��
    OLED_ShowCN(48+4,0,12); //��
    OLED_ShowCN(64+4,0,6) ; //��
    OLED_ShowCN(80+4,0,13); //��
    OLED_ShowCN(96+4,0,14); //��
    
    //2���޸Ĺ���Ա����
    if(line_flag==1)
    {
        OLED_ShowCN(0,2,34) ; // ��
    }
    else{
    OLED_ShowChar(0,2,' ',16) ; // 
    }
    OLED_ShowCN(16,2,25) ; //��
    OLED_ShowCN(32,2,26) ; //��
    OLED_ShowCN(48,2,27) ; //��
    OLED_ShowCN(64,2,28) ; //��
    OLED_ShowCN(80,2,29) ;//Ա
    OLED_ShowCN(96,2,13) ; //��
    OLED_ShowCN(112,2,14); //��
    
    //3�����ָ��
    if(line_flag==2)
    {
        OLED_ShowCN(0,4,34) ; // ��
    }
    else{
     OLED_ShowChar(0,4,' ',16) ; // 
    }
    OLED_ShowCN(16,4,30) ; //��
    OLED_ShowCN(32,4,31) ; //��
    OLED_ShowCN(48,4,35) ; //ָ
    OLED_ShowCN(64,4,36) ; //��
    //4��ɾ��ָ��
    if(line_flag==3)
    {
        OLED_ShowCN(0,6,34) ; // ��
    }
    else{
    OLED_ShowChar(0,6,' ',16) ; // 
    }
    OLED_ShowCN(16,6,32) ; //ɾ
    OLED_ShowCN(32,6,33) ; //��
    OLED_ShowCN(48,6,35) ; //ָ
    OLED_ShowCN(64,6,36) ; //��
    
    OLED_ShowCN(90,6,44); //��
    OLED_ShowCN(106,6,45); //��
    
    k_admin=0;
    memset(input_admin_password,0,sizeof(input_admin_password));
    k=0;
    memset(input_user_password,0,sizeof(input_user_password));
}

//ָ�ƽ���ģ��
void Figure(void)
{
    //��ָ�ư���ʱ
    if(IsModuleWakingUp1())
    {
        press_FR();
    }
}


//�޸Ľ�������ҳ��
void change_user(void)
{
     OLED_ShowCN(0,0,9)  ; //��
     OLED_ShowCN(16,0,10); //��
     OLED_ShowCN(32,0,11); //��
     OLED_ShowCN(48,0,43); //��
     OLED_ShowCN(64,0,41); //��
     OLED_ShowCN(16,2,12); //��
     OLED_ShowCN(32,2,6) ; //��
     OLED_ShowCN(48,2,13); //��
     OLED_ShowCN(64,2,14); //��
     OLED_ShowCN(80,2,15); //: 
    
     OLED_ShowCN(90,6,44);  //��
     OLED_ShowCN(106,6,45); //��
    
     OLED_ShowCN(0,6,46);  //��
     OLED_ShowCN(16,6,47); //��
}

//�޸ĳɹ���ʾ
void changed_success_view(void)
{
       OLED_ShowCN(0,2,25)  ; //��
       OLED_ShowCN(16,2,26); //��
       OLED_ShowCN(32,2,16); //��
       OLED_ShowCN(48,2,17); //��
}

//�޸Ľ���ʧ�ܽ���
void changed_user_fail_view(void)
{
    OLED_ShowCN(0,2,25)  ; //��
    OLED_ShowCN(16,2,26) ; //��
    OLED_ShowCN(32,2,18) ; //ʧ
    OLED_ShowCN(48,2,19) ; //��
    OLED_ShowCN(64,2,22) ; //��
    OLED_ShowCN(80,2,9)  ; //��
    OLED_ShowCN(96,2,20) ; //��
    OLED_ShowCN(106,2,21); //�� 
    
    OLED_ShowCN(0,6,46);  //��
    OLED_ShowCN(16,6,47); //��
    
    //���أ��ص�ϵͳ���ý���
    OLED_ShowCN(90,6,44);  //��
    OLED_ShowCN(106,6,45); //��  
}

//�޸Ĺ���Ա����ʧ�ܽ���
void changed_admin_fail_view(void)
{
    OLED_ShowCN(0,2,25)  ; //��
    OLED_ShowCN(16,2,26) ; //��
    OLED_ShowCN(32,2,18) ; //ʧ
    OLED_ShowCN(48,2,19) ; //��
    OLED_ShowCN(64,2,22) ; //��
    OLED_ShowCN(80,2,9)  ; //��
    OLED_ShowCN(96,2,20) ; //��
    OLED_ShowCN(112,2,21); //�� 
    
    OLED_ShowCN(0,6,46);  //��
    OLED_ShowCN(16,6,47); //��
    
    //���أ��ص�ϵͳ���ý���
    OLED_ShowCN(90,6,44);  //��
    OLED_ShowCN(106,6,45); //��  
}

//�ж��޸Ľ��������Ƿ�ɹ�
void change_user_fail_success(void)
{
    if(user_flag==1)//�޸��û�����ɹ�
    {
        //�޸ĳɹ�����
        OLED_CLS();
        view=8;
       memcpy(Store_Data,user_data,sizeof(user_data));
       Store_Save();        
       user_flag=0;  
    }
    else if(user_flag==2)//�޸��û�����ʧ��
    {
        //�޸ĳɹ�ʧ�ܽ���
        OLED_CLS();
        view=9;
        user_flag=0;
    }
    user_index=0;
    memset(user_data,0,sizeof(user_data));
}

//�ж��޸Ĺ���Ա�����Ƿ�ɹ�
void change_admin_fail_success(void)
{
    if(admin_flag==1)//�޸Ĺ���Ա����ɹ�
    {
        //�޸ĳɹ�����
       OLED_CLS();
       view=8;
       memcpy(Store_Admin,admin_data,sizeof(admin_data));
       Store_Save();
       user_flag=0;
    }
    else if(admin_flag==2)//�޸Ĺ���Ա����ʧ��
    {
        //�޸�ʧ�ܽ���
        OLED_CLS();
        view=10;
        user_flag=0;
    }
    admin_index=0;  
    memset(admin_data,0,sizeof(admin_data));
}

//�޸Ĺ���Ա�������
void change_admin(void)
{
     OLED_ShowCN(16,0,9) ; //��
     OLED_ShowCN(32,0,10); //��
     OLED_ShowCN(48,0,11); //��
     OLED_ShowCN(64,0,43); //��
     OLED_ShowCN(80,0,41); //��
     OLED_ShowCN(0,2,27) ; //��
     OLED_ShowCN(16,2,28); //��
     OLED_ShowCN(32,2,29); //Ա
     OLED_ShowCN(48,2,13); //��
     OLED_ShowCN(64,2,14); //��
     OLED_ShowCN(80,2,15); //: 
     OLED_ShowCN(90,6,44); //��
     OLED_ShowCN(106,6,45); //��
     OLED_ShowCN(0,6,46);  //��
     OLED_ShowCN(16,6,47); //��
}

//ָ����ӳɹ���ʾ����
void add_success(void)
{
    OLED_ShowCN(16,2,30) ; //��
    OLED_ShowCN(32,2,31) ; //��
    OLED_ShowCN(32+16,2,16); //��
    OLED_ShowCN(48+16,2,17); //��
}

//ָ�����ʧ����ʾ����
void add_fail(void)
{
    OLED_ShowCN(16,2,30) ; //��
    OLED_ShowCN(32,2,31) ; //��
    OLED_ShowCN(32+16,2,18); //ʧ
    OLED_ShowCN(48+16,2,19); //��
}

//ָ���Ѿ����ˣ�ָ�����ʧ��
void figuer_fill(void)
{
    //�����3��ָ�ƣ��޷��������
    OLED_ShowCN(16,2,50) ; //��
    OLED_ShowCN(32,2,30) ; //��
    OLED_ShowCN(48,2,31) ; //��
    OLED_ShowNum(64,2,3,2,16);//3
    OLED_ShowCN(80,2,51); //��
    OLED_ShowCN(32,4,52); //��
    OLED_ShowCN(48,4,53); //��
    OLED_ShowCN(64,4,54); //��
    OLED_ShowCN(80,4,30) ; //��
    OLED_ShowCN(96,4,31) ; //��
}

//���ָ�ƽ���
void add_figure(void)
{
    OLED_ShowCN(0,0,9) ; //��
    OLED_ShowCN(16,0,39) ; //��
    OLED_ShowCN(32,0,40) ; //��
    OLED_ShowCN(48,0,37) ; //��
    OLED_ShowCN(64,0,38) ; //Ҫ
    OLED_ShowCN(16,2,30) ; //��
    OLED_ShowCN(32,2,31) ; //��
    OLED_ShowCN(48,2,41) ; //��
    OLED_ShowCN(64,2,35) ; //ָ
    OLED_ShowCN(80,2,36) ; //��
    
    OLED_ShowCN(90,6,44); //��
    OLED_ShowCN(106,6,45); //��
    
    Add_FR();
}

//ɾ��ָ�ƽ���
void delete_figure(void)
{
    OLED_ShowCN(0,0,9)   ; //��
    OLED_ShowCN(16,0,10) ; //��
    OLED_ShowCN(32,0,11) ; //��
    OLED_ShowCN(48,0,37) ; //��
    OLED_ShowCN(64,0,38) ; //Ҫ
    OLED_ShowCN(80,0,32) ; //ɾ
    OLED_ShowCN(96,0,33) ; //��
    OLED_ShowCN(112,0,41) ; //��
    OLED_ShowCN(32-16,2,35) ; //ָ
    OLED_ShowCN(48-16,2,36) ; //��
    OLED_ShowStr(64-16,2,"ID",2);//ID
    OLED_ShowNum(80-16,2,2,2,16);
    OLED_ShowChar(96-16,2,'-',16);
    OLED_ShowNum(112-16-8,2,4,1,16);
    
    OLED_ShowCN(90,6,44); //��
    OLED_ShowCN(106,6,45); //��
    
    OLED_ShowCN(0,6,48);  //ȷ
    OLED_ShowCN(16,6,49); //�� 
}
//ɾ���ɹ���ʾ����
void delete_success(void)
{
    OLED_ShowCN(16,2,32) ; //ɾ
    OLED_ShowCN(32,2,33) ; //��
    OLED_ShowCN(32+16,2,16); //��
    OLED_ShowCN(48+16,2,17); //��
}
//ɾ��ʧ�ܽ��棬IDδ¼��ָ��
void delete_fail(void)
{
    OLED_ShowCN(16,2,32) ; //ɾ
    OLED_ShowCN(32,2,33) ; //��
    OLED_ShowCN(32+16,2,18); //ʧ
    OLED_ShowCN(48+16,2,19); //��
    
    OLED_ShowStr(16,4,"ID",2);//ID
    OLED_ShowCN(32,4,55) ; //δ
    OLED_ShowCN(48,4,30) ; //��
    OLED_ShowCN(64,4,31) ; //��
    OLED_ShowCN(80,4,35) ; //ָ
    OLED_ShowCN(96,4,36) ; //��
}

//ɾ��ָ��ʧ�ܽ��棬ָ�Ƹ���Ϊ0
void figuer_empty(void)
{
    OLED_ShowCN(16,2,32) ; //ɾ
    OLED_ShowCN(32,2,33) ; //��
    OLED_ShowCN(32+16,2,18); //ʧ
    OLED_ShowCN(48+16,2,19); //��
    
    OLED_ShowCN(16,4,55) ; //δ
    OLED_ShowCN(32,4,30) ; //��
    OLED_ShowCN(48,4,31) ; //��
    OLED_ShowCN(64,4,35) ; //ָ
    OLED_ShowCN(80,4,36) ; //��
}



__IO uint32_t uwTick_view;
__IO uint32_t uwTick_key;


//������ʾ
void view_display(void)
{
    if((uwTick-uwTick_view)<100)return;
    uwTick_view=uwTick;
    switch(view)
    {
        case 0://��������
            main_menu();
            //Figure();
            break;
        case 1://�����ɹ�����
            Unlock_success();
            break;
        case 2://����ʧ�ܽ���
            Unlock_failed();
            break;
        case 3://���ý���
            admin_menu();
            //admin_jduge();
            break;
        case 4://�޸Ľ����������
            change_user();
            break;
        case 5://�޸Ĺ���Ա����
            change_admin();
            break;
        case 6://���ָ�ƽ���
            add_figure();//������5��ָ��
            break;
        case 7://ɾ��ָ�ƽ���
            delete_figure();
            break;
        case 8://�޸ĳɹ����棬����������ؿ�������
            changed_success_view();
            break;
        case 9://�޸��û�����ʧ��
            changed_user_fail_view();
            break;
        case 10://�޸Ĺ���Ա����ʧ��
            changed_admin_fail_view();
            break;
        case 11://���ָ�Ƴɹ�
            add_success();
            break;
        case 12://���ָ��ʧ��
            add_fail();
            break;
        case 13://ɾ��ָ�Ƴɹ�
            delete_success();
            break;
        case 14://ָ���Ѿ�����
            figuer_fill();
            break;
        case 15://ָ��Ϊ��
            figuer_empty();
            break;
        case 16://ָ��IDɾ��ʧ��,IDδ���ָ��
            delete_fail();
            break;
        case 17://��ֹ��������������
            forbid_user();
            break;
        case 18://�������Ա�������
            admin_input();
            break;
        case 19://����Ա��¼ʧ��
            admin_fail();
            break;
        case 20://��ֹ�������Ա�������
            forbid_admin();
            break;
    }
}

unsigned char keep_menu=0;

//��������ģ��
void key_display(void)
{
    if((uwTick-uwTick_key)<20)return;
    uwTick_key=uwTick;
    
    keynum=button4_4_Scan();
    if(key_admin_flag0==1)//���ù���Ա��������
    {
        OLED_CLS();
        view=18;
        key_admin_flag0=0;
    }
    if(key_user_flag0==1)//�����û���������
    {
        OLED_CLS();
        view=0;
        key_user_flag0=0;
    }
    
    if(keynum==0)
    {
        //if(view!=20&&view!=17)
        keep_menu=1; 
    }
    else
    {
        keep_menu=0;
    }

    if(key_user_flag==1)//��ֹ����
    {
        return;
    }
    if(long_view==1&&view==0)//�������棬һ�����޲���ֻ��Ҫ��������е����ݼ���
    {
       OLED_ClearGivenChar(4); 
       k=0;
       memset(input_user_password,0,sizeof(input_user_password));
    }
    else if(long_view==1&&view!=0)//һ�����޲����Զ�����
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]������");
        view=0;
        k=0;k_admin=0;
        long_view=0;
        line_flag=0;
        key_user_count=0;
        key_user_flag=0;
        figuer_count=0;
        figure_flag=0;
        user_index=0;admin_index=0;
        memset(input_user_password,0,sizeof(input_user_password));
        memset(input_admin_password,0,sizeof(input_admin_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data));
        HAL_Delay(1000);
    }
    
    if(keynum!=0&&(view==8||view==11||view==13))//���ָ�Ƴɹ���ɾ��ָ�ơ��޸�����ɹ�������������ص���������
    {
        line_flag=0;
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]������");
        view=0;
        key_user_count=0;
        key_user_flag=0;
        figuer_count=0;
        figure_flag=0;
        user_index=0;admin_index=0;k=0;k_admin=0;
        memset(input_user_password,0,sizeof(input_user_password));
        memset(input_admin_password,0,sizeof(input_admin_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data));
        HAL_Delay(1000);
    }
    else if(keynum!=0&&(view==14||view==15||view==16))//ָ���������桢ָ��Ϊ�ս��桢ָ��IDɾ��ʧ�ܣ������������ϵͳ���ý���
    {
        line_flag=0;
        OLED_CLS();
        view=3;
        key_user_count=0;
        key_user_flag=0;
        figuer_count=0;
        figure_flag=0;k_admin=0;
        user_index=0;admin_index=0;k=0;
        memset(input_user_password,0,sizeof(input_user_password));
        memset(input_admin_password,0,sizeof(input_admin_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data)); 
    }
    else
    switch(keynum){
        case 1://*
            if(view==1)//�������ý���
            {
                input_user_password[k++]=255;
                //view=3;//�������ý���
            }
            break;
        case 2://7
            if(view==0||view==2){//�û���¼����
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,7,2,16);
                    input_user_password[k++]=7;
                }
            }
            else if(view==18||view==19)//����Ա�û���¼����7
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,7,2,16);
                    input_admin_password[k_admin++]=7;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,7,2,16);
                    user_data[user_index++]=7;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,7,2,16);
                    admin_data[admin_index++]=7;
                }
            }
            break;
        case 3://4
            if(view==0||view==2){//�û���¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,4,2,16);
                        input_user_password[k++]=4;
                    }
                }
            else if(view==18||view==19)//����Ա��¼����4
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,4,2,16);
                    input_admin_password[k_admin++]=4;
                }
            }
            else if(view==7)//����ɾ��ָ��ID����
            {
                ID_NUM_delete=4;
                OLED_ShowNum(48,4,4,2,16);
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,4,2,16);
                    user_data[user_index++]=4;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,4,2,16);
                    admin_data[admin_index++]=4;
                }
            }
            
            break;
        case 4://1
            if(view==0||view==2){//��¼����
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,1,2,16);
                    input_user_password[k++]=1;
                }
            }
            else if(view==18||view==19)//����Ա��¼����1
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,1,2,16);
                    input_admin_password[k_admin++]=1;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,1,2,16);
                    user_data[user_index++]=1;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,1,2,16);
                    admin_data[admin_index++]=1;
                }
            }
            break;
        case 5://0
            if(view==0||view==2){//��¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,0,2,16);
                        input_user_password[k++]=0;
                    }
                }
            else if(view==18||view==19)//����Ա��¼����0
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,0,2,16);
                    input_admin_password[k_admin++]=0;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,0,2,16);
                    user_data[user_index++]=0;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,0,2,16);
                    admin_data[admin_index++]=0;
                }
            }
            break;
        case 6://8
            if(view==0||view==2){//��¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,8,2,16);
                        input_user_password[k++]=8;
                    }
            }
            else if(view==18||view==19)//����Ա��¼����8
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,8,2,16);
                    input_admin_password[k_admin++]=8;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,8,2,16);
                    user_data[user_index++]=8;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,8,2,16);
                    admin_data[admin_index++]=8;
                }
            }
            break;
        case 7://5
            if(view==0||view==2){//��¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,5,2,16);
                        input_user_password[k++]=5;
                    }
            }
            else if(view==18||view==19)//����Ա��¼����5
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,5,2,16);
                    input_admin_password[k_admin++]=5;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,5,2,16);
                    user_data[user_index++]=5;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,5,2,16);
                    admin_data[admin_index++]=5;
                }
            }
            break;
        case 8://2
            if(view==0||view==2){//��¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,2,2,16);
                        input_user_password[k++]=2;
                    }
                }
            else if(view==18||view==19)//����Ա��¼����2
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,2,2,16);
                    input_admin_password[k_admin++]=2;
                }
            }
            else if(view==7)//����ɾ��ָ��ID����
            {
                ID_NUM_delete=2;
                OLED_ShowNum(48,4,2,2,16);
            }
            else if(view == 4||view ==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,2,2,16);
                    user_data[user_index++]=2;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,2,2,16);
                    admin_data[admin_index++]=2;
                }
            }
            break;
        case 9://#  ȷ�ϼ�
            switch(view)
            {
                case 0://��ʼ���� ��¼��ť
                        if(k==6)
                        {
                            denglu_jduge();
                            //k=0;
                        }
                        else
                        {
                            //k=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]����ʧ��");       
                            HAL_Delay(1000);
                            key_user_count++;
                            if(key_user_count==3)
                            {
                                OLED_CLS();
                                view=17;
                                key_user_flag=1;
                            }
                            else
                            {
                                view=2;
                            }
                        }
                        k=0;
                        memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 1://��¼�ɹ�����,����������
                    if(k==1&&input_user_password[0]==255)
                    {
                        OLED_CLS();
                        //view=3;//�������ý���
                        view=18;//�����������Ա�������
                    }
                     //k=0;
                     memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 2://����ʧ�ܽ��棬��¼��ť
                        if(k==6)
                        {
                            denglu_jduge();
                            //k=0;
                        }
                        else
                        {
                            //k=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]����ʧ��");                         
                            //view=2;
                            HAL_Delay(1000);
                             key_user_count++;
                            if(key_user_count==3)
                            {
                                OLED_CLS();
                                view=17;
                                key_user_flag=1;
                            }
                            else
                            {
                                view=2;
                            }
                        }
                        k=0;
                        memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 3://ϵͳ�������
                     switch(line_flag)
                      {
                        case 0:
                            //�����޸Ľ����������
                            OLED_CLS();
                            view=4;
                            break;
                        case 1:
                            //�����޸Ĺ���Ա�������
                            OLED_CLS();
                            view=5;
                            break;
                        case 2:
                            //�������ָ�ƽ���
                            OLED_CLS();
                            if(ID_NUM_store>=3)
                            {
                                view=14;
                                break;
                            }
                            view=6;
                            break;
                        case 3:
                            //����ɾ��ָ�ƽ���
                            OLED_CLS();
                            if(ID_NUM_store==0)
                            {
                                view=15;
                                break;
                            }
                            view=7;
                            break;
                        }
                    break;
                case 4://�޸��û��������,ȷ���޸İ���
                        if(user_index==6)
                        {
                            user_flag=1;
                            user_index=0;
                        }
                        else
                        {
                            user_index=0;
                            user_flag=2;
                        }
                        change_user_fail_success();
                        memset(user_data,0,sizeof(user_data));
                    break;
                case 5://�޸Ĺ���Ա������棬ȷ���޸İ���
                    if(admin_index==6)
                        {
                            admin_flag=1;
                            admin_index=0;
                        }
                        else
                        {
                            admin_index=0;
                            admin_flag=2;
                        }
                        change_admin_fail_success();
                        memset(admin_data,0,sizeof(admin_data));
                    break;
                case 6://���ָ�ƽ���
                    break;
                case 7://ɾ��ָ�ƽ���
                    if(ID_NUM_delete>=2&&ID_NUM_delete<=4)
                    {
                        OLED_CLS();
                        //view=13;//ɾ��ָ�Ƴɹ�
                        Del_FR(ID_NUM_delete);
                        ID_NUM_delete=0; 
                    }
                    else
                    {
                        OLED_CLS();
                        view=7;
                    }
                    break;
                case 9://�޸��û��������,ȷ���޸İ���
                        if(user_index==6)
                        {
                            user_flag=1;
                            user_index=0;
                        }
                        else
                        {
                            user_index=0;
                            user_flag=2;
                        }
                        change_user_fail_success();
                        memset(user_data,0,sizeof(user_data));
                    break;
                case 10://�޸Ĺ���Ա������棬ȷ���޸İ���
                    if(admin_index==6)
                        {
                            admin_flag=1;
                            admin_index=0;
                        }
                        else
                        {
                            admin_index=0;
                            admin_flag=2;
                        }
                        change_admin_fail_success();
                        memset(admin_data,0,sizeof(admin_data));
                    break;
                case 18://�������Ա�������
                    if(k_admin==6)
                        {
                            admin_judge();
                            //k_admin=0;
                        }
                        else
                        {
                            //k_admin=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]����Ա�������");       
                            HAL_Delay(1000);
                            key_admin_count++;
                            if(key_admin_count==3)
                            {
                                //OLED_ClearGivenChar(2);//����ڶ���
                                OLED_CLS();
                                view=20;
                                //key_admin_count=0;
                                key_admin_flag=1;
                            }
                            else
                            {
                                view=19;
                            }
                        }
                        k_admin=0;
                        memset(input_admin_password,0,sizeof(input_admin_password));
                    break;
                case 19://����Ա��������������Ա�������
                    if(k_admin==6)
                        {
                            admin_judge();
                            //k_admin=0;
                        }
                        else
                        {
                            //k_admin=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]����Ա�������");       
                            HAL_Delay(1000);
                            key_admin_count++;
                            //k_admin=0;
                            if(key_admin_count==3)
                            {
                                OLED_CLS();
                                view=20;
                                key_admin_flag=1;
                            }
                            else
                            {
                                view=19;
                            }
                        }
                        k_admin=0;
                        memset(input_admin_password,0,sizeof(input_admin_password));
                    break;
            }         
            break;
        case 10://9
            if(view==0||view==2){//�û���¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,9,2,16);
                        input_user_password[k++]=9;
                    }
            }
            else if(view==18||view==19)//����Ա��¼����9
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,9,2,16);
                    input_admin_password[k_admin++]=9;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,9,2,16);
                    user_data[user_index++]=9;
                }
            }
            else if(view==5)//�޸Ĺ���Ա����
            {
                if(admin_index<6||view==10)
                {
                    OLED_ShowNum(16+16*admin_index,4,9,2,16);
                    admin_data[admin_index++]=9;
                }
            }
            
            break;
        case 11://6
            if(view==0||view==2){//�û���¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,6,2,16);
                        input_user_password[k++]=6;
                    }
            }
            else if(view==18||view==19)//����Ա�û���¼����6
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,6,2,16);
                    input_admin_password[k_admin++]=6;
                }
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,6,2,16);
                    user_data[user_index++]=6;
                }
            }
            else if(view==5)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,6,2,16);
                    admin_data[admin_index++]=6;
                }
            }
            break;
        case 12://3
            if(view==0||view==2){//�û���¼����
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,3,2,16);
                        input_user_password[k++]=3;
                    }
            }
            else if(view==18||view==19)//����Ա��¼����3
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,3,2,16);
                    input_admin_password[k_admin++]=3;
                }
            }
            else if(view==7)//����ɾ��ָ��ID����
            {
                ID_NUM_delete=3;
                OLED_ShowNum(48,4,3,2,16);
            }
            else if(view == 4||view==9)//�޸Ľ�������
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,3,2,16);
                    user_data[user_index++]=3;
                }
            }
            else if(view==5||view==10)//�޸Ĺ���Ա����
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,3,2,16);
                    admin_data[admin_index++]=3;
                }
            }
            break;
        case 13://D ���ذ���
            if(view==4||view==5||view==6||view==7)//���ص��˵�����
            {
                OLED_CLS();
                line_flag=0;
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                view=3;
            }
            else if(view==3||view==18||view==19)//���ص������ɹ�����
            {
                OLED_CLS();
                line_flag=0;
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                k=0;
                k_admin=0;
                memset(input_admin_password,0,sizeof(input_admin_password));
                memset(input_user_password,0,sizeof(input_user_password));
                view=1;
            }
            else if(view==9||view==10)//���ص�ϵͳ���ý���
            {
                OLED_CLS();
                line_flag=0;
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                k=0;
                k_admin=0;
                memset(input_admin_password,0,sizeof(input_admin_password));
                memset(input_user_password,0,sizeof(input_user_password));
                view=3;
            }
            else if(view==1)//�����ɹ�����D������
            {
                OLED_CLS();
                SYN_FrameInfo(0,"[v7][m1][t5]������");
                view=0; 
                line_flag=0;
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                k=0;
                k_admin=0;
                memset(input_admin_password,0,sizeof(input_admin_password));
                memset(input_user_password,0,sizeof(input_user_password));
                HAL_Delay(1000);
            }
            break;
        case 14://C  �������
            if(view==4||view==5||view==9||view==10)
            {
                user_index=0;admin_index=0;k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                OLED_ClearGivenChar(4);
            }
           else if(view==0||view==2)//�û���������������
            {
                k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                OLED_ClearGivenChar(4);
            }
            else if(view==18||view==19)//����Ա����������
            {
                k_admin=0;
                memset(input_admin_password,0,sizeof(input_admin_password));
                OLED_ClearGivenChar(4);
            }
            else if(view == 7)
            {
                ID_NUM_delete=0;
                
                OLED_ClearGivenChar(4);
            }
               break;
        case 15://B
            if(view==3)//���ý���
            {
                OLED_CLS();
                //line_flag--;
                if(line_flag==0)
                {
                    line_flag=3;
                }
                else{
                    line_flag--;
                }
            }
            break;
        case 16://A
            if(view==3)//���ý���
            {
                OLED_CLS();
                line_flag++;
                if(line_flag==4)
                    line_flag=0;
            }
            break;
    }
    
    keynum=0;
}


//��������APP���͵����ݣ���������ش���
unsigned add_flag=0;//����������ӡ�ɾ��ָ�Ʊ�־  1����ӣ�2��ɾ��

void usart_disp(void)
{ 
    if(uart1_rx_buf[0]=='A')//���ָ��
    {
        OLED_CLS();
        
        if(add_flag==0)
        {
            
            if(ID_NUM_store>=3)//ָ���Ѿ�����
            {
                //view=14;
                HAL_UART_Transmit(&huart1,"Addfull",7,50);
            }
            else
            {
                view=6;
                add_flag=1;
                //Add_FR();
                HAL_UART_Transmit(&huart1,"Press",5,50);
            }
      }
    }
    else if(uart1_rx_buf[0]=='D')//ɾ��ָ��
    {
        if(add_flag==0)
        {
            if(ID_NUM_store==0)//¼��ָ��Ϊ��
            {
                HAL_UART_Transmit(&huart1,"Empty",5,50);
            }
            else 
            {
                    add_flag=2;
                    Del_FR(uart1_rx_buf[1]-'0');
            }
      }
    }
    else if(uart1_rx_buf[0]=='O')//ϵͳ���������������
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]�ѿ��ţ�������");
        view=1;
        HAL_Delay(1000);
    }
    else if(uart1_rx_buf[0]=='C')//ϵͳ�ѿ������������
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]������");
        view=0;
        HAL_Delay(1000);
    }
    else if(uart1_rx_buf[0]=='U')//�޸Ľ�������
    {
        Store_Data[0]=uart1_rx_buf[1]-'0';
        Store_Data[1]=uart1_rx_buf[2]-'0';
        Store_Data[2]=uart1_rx_buf[3]-'0';
        Store_Data[3]=uart1_rx_buf[4]-'0';
        Store_Data[4]=uart1_rx_buf[5]-'0';
        Store_Data[5]=uart1_rx_buf[6]-'0';
        Store_Save();
    }
    else if(uart1_rx_buf[0]=='R')//�޸Ĺ���Ա����
    {
        Store_Admin[0]=uart1_rx_buf[1]-'0';
        Store_Admin[1]=uart1_rx_buf[2]-'0';
        Store_Admin[2]=uart1_rx_buf[3]-'0';
        Store_Admin[3]=uart1_rx_buf[4]-'0';
        Store_Admin[4]=uart1_rx_buf[5]-'0';
        Store_Admin[5]=uart1_rx_buf[6]-'0';
        Store_Save();
    }    
    uart1_rx_len=0;
    memset(uart1_rx_buf,0,sizeof(uart1_rx_buf));
}

//�����������Ϸ��͵����ݴ�������APP
void usart1_disp(void)
{
    if(add_flag==0&&uart1_rx_len==0){
        if(view==0||view==2)
        {
            HAL_UART_Transmit(&huart1,"Close",5,50);
        }
        else
        {
           HAL_UART_Transmit(&huart1,"Open",4,50);
        }      
    }
}
