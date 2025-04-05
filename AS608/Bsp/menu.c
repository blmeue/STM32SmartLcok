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

uint16_t input_user_password[6]={0};
uint32_t k=0;

unsigned char user_flag=0;
unsigned char admin_flag=0;

unsigned char unlock=0;

unsigned char line_flag=0;
unsigned char figure_flag=0;
unsigned char view=0;

uint16_t admin_data[6]={0};
uint16_t user_data[6]={0};

uint16_t admin_index=0;//解锁密码下标
uint16_t user_index=0;//管理员密码下标

unsigned char figuer_count=0;

void main_menu(void)
{
     OLED_ShowChar(0+16,0,'S',16);
     OLED_ShowChar(8+16,0,'T',16);
     OLED_ShowChar(16+16,0,'M',16);
     OLED_ShowNum(22+16,0,32,2,16);

     OLED_ShowCN(38+16,0,3);//智
     OLED_ShowCN(54+16,0,4);//能
     OLED_ShowCN(70+16,0,5);//门
     OLED_ShowCN(86+16,0,6);//锁
     
     OLED_ShowCN(0,2,9)  ; //请
     OLED_ShowCN(16,2,10); //输
     OLED_ShowCN(32,2,11); //入
     OLED_ShowCN(48,2,12); //解
     OLED_ShowCN(64,2,6) ; //锁
     OLED_ShowCN(80,2,13); //密
     OLED_ShowCN(96,2,14); //码
     OLED_ShowCN(112,2,15); //: 
    
     OLED_ShowCN(0,6,46);  //清
     OLED_ShowCN(16,6,47); //除
     
    OLED_ShowCN(96,6,48);  //确
    OLED_ShowCN(112,6,49); //定 
    
    Figure();
}

uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令


void denglu_jduge(void)
{
    //判断输入密码是否正确
    if(memcmp(input_user_password,Store_Data,sizeof(input_user_password))==0)
    {
        //解锁成功
        //Unlock_success();
        //unlock=1;
        //YS_SYN_Set(SYN_RecoverCom);
        OLED_CLS();
        
        //SYN_FrameInfo(2,"[v7][m1][t5]解锁成功");
        view=1;
        //HAL_Delay(1000);
        //HAL_Delay(1000);
        //memset(input_user_password,0,sizeof(input_user_password));
    }
    else
    {
        //解锁失败
        OLED_CLS();
        //SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");
        //HAL_Delay(1000);
        view=2;
        //HAL_Delay(1000);
        //HAL_Delay(1000);
        k=0;
        //memset(input_user_password,0,sizeof(input_user_password));
        //Unlock_failed();
    }
    //memset(input_user_password,0,sizeof(input_user_password));
}

//解锁成功界面
void Unlock_success(void)
{
     OLED_ShowCN(0,2,12)  ; //解
     OLED_ShowCN(16,2,6); //锁
     OLED_ShowCN(32,2,16); //成
     OLED_ShowCN(48,2,17); //功
     OLED_ShowCN(90,6,23); //设
     OLED_ShowCN(106,6,24); //置
     //view=1;
    figure_flag=0;
    figuer_count=0;
 //   memset(input_user_password,0,sizeof(input_user_password));
//     unlock=1;
}

void Unlock_failed(void)
{
   // OLED_CLS();
     //view=2;
     OLED_ShowCN(0,2,12)  ; //解
     OLED_ShowCN(16,2,6)  ; //锁
     OLED_ShowCN(32,2,18) ; //失
     OLED_ShowCN(48,2,19) ; //败
     OLED_ShowCN(64,2,22) ; //，
     OLED_ShowCN(80,2,9)  ; //请
     OLED_ShowCN(96,2,20) ; //重
     OLED_ShowCN(112,2,21); //试 
     if(figure_flag==0)
     {
         Figure();//指纹解锁模块
         OLED_ClearGivenChar(0);
     }
     else
     {
         OLED_ShowCN(16,0,35) ; //指
         OLED_ShowCN(32,0,36) ; //纹
         OLED_ShowCN(48,0,50) ; //已
         OLED_ShowCN(64,0,56) ; //禁
         OLED_ShowCN(80,0,57) ; //用
     }
     
}

void admin_jduge(void)
{
     OLED_ShowCN(32,0,9)  ; //请
     OLED_ShowCN(48,0,10); //输
     OLED_ShowCN(64,0,11); //入
     OLED_ShowCN(0,2,27); //管
     OLED_ShowCN(16,2,28) ; //理
     OLED_ShowCN(32,2,29) ;//员
     OLED_ShowCN(48,2,13); //密
     OLED_ShowCN(64,2,14); //码
     OLED_ShowCN(80,2,15); //: 
}

void admin_menu(void)
{
    //1、修改解锁密码
    if(line_flag==0)
    {
        OLED_ShowCN(0,0,34) ; // 》
    }
    else
    {
        OLED_ShowChar(0,0,' ',16) ;
    }
    OLED_ShowCN(16+4,0,25); //修
    OLED_ShowCN(32+4,0,26); //改
    OLED_ShowCN(48+4,0,12); //解
    OLED_ShowCN(64+4,0,6) ; //锁
    OLED_ShowCN(80+4,0,13); //密
    OLED_ShowCN(96+4,0,14); //码
    
    //2、修改管理员密码
    if(line_flag==1)
    {
        OLED_ShowCN(0,2,34) ; // 》
    }
    else{
    OLED_ShowChar(0,2,' ',16) ; // 
    }
    OLED_ShowCN(16,2,25) ; //修
    OLED_ShowCN(32,2,26) ; //改
    OLED_ShowCN(48,2,27) ; //管
    OLED_ShowCN(64,2,28) ; //理
    OLED_ShowCN(80,2,29) ;//员
    OLED_ShowCN(96,2,13) ; //密
    OLED_ShowCN(112,2,14); //码
    
    //3、添加指纹
    if(line_flag==2)
    {
        OLED_ShowCN(0,4,34) ; // 》
    }
    else{
     OLED_ShowChar(0,4,' ',16) ; // 
    }
    OLED_ShowCN(16,4,30) ; //添
    OLED_ShowCN(32,4,31) ; //加
    OLED_ShowCN(48,4,35) ; //指
    OLED_ShowCN(64,4,36) ; //纹
    //4、删除指纹
    if(line_flag==3)
    {
        OLED_ShowCN(0,6,34) ; // 》
    }
    else{
    OLED_ShowChar(0,6,' ',16) ; // 
    }
    OLED_ShowCN(16,6,32) ; //删
    OLED_ShowCN(32,6,33) ; //除
    OLED_ShowCN(48,6,35) ; //指
    OLED_ShowCN(64,6,36) ; //纹
    
    OLED_ShowCN(90,6,44); //返
    OLED_ShowCN(106,6,45); //回
}

void Figure(void)
{
    //有指纹按下时
    if(IsModuleWakingUp1())
    {
        press_FR();
    }
}



void change_user(void)
{
     OLED_ShowCN(0,0,9)  ; //请
     OLED_ShowCN(16,0,10); //输
     OLED_ShowCN(32,0,11); //入
     OLED_ShowCN(48,0,43); //新
     OLED_ShowCN(64,0,41); //的
     OLED_ShowCN(16,2,12); //解
     OLED_ShowCN(32,2,6) ; //锁
     OLED_ShowCN(48,2,13); //密
     OLED_ShowCN(64,2,14); //码
     OLED_ShowCN(80,2,15); //: 
    
     OLED_ShowCN(90,6,44);  //返
     OLED_ShowCN(106,6,45); //回
    
     OLED_ShowCN(0,6,46);  //清
     OLED_ShowCN(16,6,47); //除
}

void changed_success_view(void)
{
       OLED_ShowCN(0,2,25)  ; //修
       OLED_ShowCN(16,2,26); //改
       OLED_ShowCN(32,2,16); //成
       OLED_ShowCN(48,2,17); //功
}

void changed_user_fail_view(void)
{
    OLED_ShowCN(0,2,25)  ; //修
    OLED_ShowCN(16,2,26) ; //改
    OLED_ShowCN(32,2,18) ; //失
    OLED_ShowCN(48,2,19) ; //败
    OLED_ShowCN(64,2,22) ; //，
    OLED_ShowCN(80,2,9)  ; //请
    OLED_ShowCN(96,2,20) ; //重
    OLED_ShowCN(106,2,21); //试 
    
    OLED_ShowCN(0,6,46);  //清
    OLED_ShowCN(16,6,47); //除
    
    //返回，回到系统设置界面
    OLED_ShowCN(90,6,44);  //返
    OLED_ShowCN(106,6,45); //回  
}

void changed_admin_fail_view(void)
{
    OLED_ShowCN(0,2,25)  ; //修
    OLED_ShowCN(16,2,26) ; //改
    OLED_ShowCN(32,2,18) ; //失
    OLED_ShowCN(48,2,19) ; //败
    OLED_ShowCN(64,2,22) ; //，
    OLED_ShowCN(80,2,9)  ; //请
    OLED_ShowCN(96,2,20) ; //重
    OLED_ShowCN(106,2,21); //试 
    
    OLED_ShowCN(0,6,46);  //清
    OLED_ShowCN(16,6,47); //除
    
    //返回，回到系统设置界面
    OLED_ShowCN(90,6,44);  //返
    OLED_ShowCN(106,6,45); //回  
}

void change_user_fail_success(void)
{
    if(user_flag==1)//修改用户密码成功
    {
        //修改成功界面
        OLED_CLS();
        view=8;

       memcpy(Store_Data,user_data,sizeof(user_data));
       Store_Save();        
       user_flag=0;  
    }
    else if(user_flag==2)//修改用户密码失败
    {
        //修改成功失败界面
        OLED_CLS();
        view=9;
        user_flag=0;
    }
    user_index=0;
    memset(user_data,0,sizeof(user_data));
}

void change_admin_fail_success(void)
{
    if(admin_flag==1)//修改管理员密码成功
    {
        //修改成功界面
       OLED_CLS();
       view=8;
       memcpy(Store_Admin,admin_data,sizeof(admin_data));
       Store_Save();
       user_flag=0;
    }
    else if(admin_flag==2)//修改管理员密码失败
    {
        //修改失败界面
        OLED_CLS();
        view=10;
        user_flag=0;
    }
    admin_index=0;  
    memset(admin_data,0,sizeof(admin_data));
}

void change_admin(void)
{
     OLED_ShowCN(16,0,9) ; //请
     OLED_ShowCN(32,0,10); //输
     OLED_ShowCN(48,0,11); //入
     OLED_ShowCN(64,0,43); //新
     OLED_ShowCN(80,0,41); //的
     OLED_ShowCN(0,2,27) ; //管
     OLED_ShowCN(16,2,28); //理
     OLED_ShowCN(32,2,29); //员
     OLED_ShowCN(48,2,13); //密
     OLED_ShowCN(64,2,14); //码
     OLED_ShowCN(80,2,15); //: 
    
     OLED_ShowCN(90,6,44); //返
     OLED_ShowCN(106,6,45); //回
     
     OLED_ShowCN(0,6,46);  //清
     OLED_ShowCN(16,6,47); //除
}

void add_success(void)
{
    OLED_ShowCN(16,2,30) ; //添
    OLED_ShowCN(32,2,31) ; //加
    OLED_ShowCN(32+16,2,16); //成
    OLED_ShowCN(48+16,2,17); //功
}

void add_fail(void)
{
    OLED_ShowCN(16,2,30) ; //添
    OLED_ShowCN(32,2,31) ; //加
    OLED_ShowCN(32+16,2,18); //失
    OLED_ShowCN(48+16,2,19); //败
}

//指纹已经满了
void figuer_fill(void)
{
    //已添加3个指纹，无法继续添加
    OLED_ShowCN(16,2,50) ; //已
    OLED_ShowCN(32,2,30) ; //添
    OLED_ShowCN(48,2,31) ; //加
    OLED_ShowNum(64,2,3,2,16);//3
    OLED_ShowCN(80,2,51); //个
    OLED_ShowCN(32,4,52); //无
    OLED_ShowCN(48,4,52); //法
    OLED_ShowCN(64,4,52); //再
    OLED_ShowCN(80,4,30) ; //添
    OLED_ShowCN(96,4,31) ; //加
}

void add_figure(void)
{
    OLED_ShowCN(0,0,9) ; //请
    OLED_ShowCN(16,0,39) ; //按
    OLED_ShowCN(32,0,40) ; //下
    OLED_ShowCN(48,0,37) ; //需
    OLED_ShowCN(64,0,38) ; //要
    OLED_ShowCN(16,2,30) ; //添
    OLED_ShowCN(32,2,31) ; //加
    OLED_ShowCN(48,2,41) ; //的
    OLED_ShowCN(64,2,35) ; //指
    OLED_ShowCN(80,2,36) ; //纹
    
    OLED_ShowCN(90,6,44); //返
    OLED_ShowCN(106,6,45); //回
    
    Add_FR();
}

void delete_figure(void)
{
    OLED_ShowCN(0,0,9)   ; //请
    OLED_ShowCN(16,0,10) ; //输
    OLED_ShowCN(32,0,11) ; //入
    OLED_ShowCN(48,0,37) ; //需
    OLED_ShowCN(64,0,38) ; //要
    OLED_ShowCN(80,0,32) ; //删
    OLED_ShowCN(96,0,33) ; //除
    OLED_ShowCN(112,0,41) ; //的
    OLED_ShowCN(32-16,2,35) ; //指
    OLED_ShowCN(48-16,2,36) ; //纹
    OLED_ShowStr(64-16,2,"ID",2);//ID
    OLED_ShowNum(80-16,2,2,2,16);
    OLED_ShowChar(96-16,2,'-',16);
    OLED_ShowNum(112-16-8,2,4,1,16);
    
    OLED_ShowCN(90,6,44); //返
    OLED_ShowCN(106,6,45); //回
    
    OLED_ShowCN(0,6,48);  //确
    OLED_ShowCN(16,6,49); //定 
}
//删除成功
void delete_success(void)
{
    OLED_ShowCN(16,2,32) ; //删
    OLED_ShowCN(32,2,33) ; //除
    OLED_ShowCN(32+16,2,16); //成
    OLED_ShowCN(48+16,2,17); //功
}
////删除失败，ID未录入指纹
void delete_fail(void)
{
    OLED_ShowCN(16,2,32) ; //删
    OLED_ShowCN(32,2,33) ; //除
    OLED_ShowCN(32+16,2,18); //失
    OLED_ShowCN(48+16,2,19); //败
    
    OLED_ShowStr(16,4,"ID",2);//ID
    OLED_ShowCN(32,4,55) ; //未
    OLED_ShowCN(48,4,30) ; //添
    OLED_ShowCN(64,4,31) ; //加
    OLED_ShowCN(80,4,35) ; //指
    OLED_ShowCN(96,4,36) ; //纹
}

//指纹个数为0
void figuer_empty(void)
{
    OLED_ShowCN(16,2,32) ; //删
    OLED_ShowCN(32,2,33) ; //除
    OLED_ShowCN(32+16,2,18); //失
    OLED_ShowCN(48+16,2,19); //败
    
    OLED_ShowCN(16,4,55) ; //未
    OLED_ShowCN(32,4,30) ; //添
    OLED_ShowCN(48,4,31) ; //加
    OLED_ShowCN(64,4,35) ; //指
    OLED_ShowCN(80,4,36) ; //纹
}

__IO uint32_t uwTick_view;
__IO uint32_t uwTick_key;
void view_display(void)
{
    if((uwTick-uwTick_view)<100)return;
    uwTick_view=uwTick;
    switch(view)
    {
        case 0://锁屏界面
            main_menu();
            //Figure();
            break;
        case 1://开锁成功界面
            Unlock_success();
            break;
        case 2://开锁失败界面
            Unlock_failed();
            break;
        case 3://设置界面
            admin_menu();
            //admin_jduge();
            break;
        case 4://修改解锁密码界面
            change_user();
            break;
        case 5://修改管理员界面
            change_admin();
            break;
        case 6://添加指纹界面
            add_figure();//最多添加5个指纹
            break;
        case 7://删除指纹界面
            delete_figure();
            break;
        case 8://修改成功界面，按任意键返回开锁界面
            changed_success_view();
            break;
        case 9://修改用户密码失败
            changed_user_fail_view();
            break;
        case 10://修改管理员密码失败
            changed_admin_fail_view();
            break;
        case 11://添加指纹成功
            add_success();
            break;
        case 12://添加指纹失败
            add_fail();
            break;
        case 13://删除指纹成功
            delete_success();
            break;
        case 14://指纹已经满了
            figuer_fill();
            break;
        case 15://指纹为空
            figuer_empty();
            break;
        case 16://指纹ID删除失败,ID未添加指纹
            delete_fail();
            break;
    }
}


unsigned char keep_menu=0;

void key_display(void)
{
    if((uwTick-uwTick_key)<20)return;
    uwTick_key=uwTick;
    
    keynum=button4_4_Scan();
    
    if(keynum==0&&view!=0)
    {
        keep_menu=1; 
    }
    else if(keynum==0)
    {
        keep_menu=0;
    }
    if(long_view==1)
    {
        OLED_CLS();
        view=0;
        long_view=0;
        line_flag=0;
        user_index=0;admin_index=0;
        //memset(input_user_password,0,sizeof(input_user_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data));
        //return;
    }
    
    if(keynum!=0&&(view==8||view==11||view==13))//添加指纹成功、删除指纹、修改密码成功，按任意键返回到锁屏界面
    {
        line_flag=0;
        OLED_CLS();
        view=0;
        user_index=0;admin_index=0;
        //memset(input_user_password,0,sizeof(input_user_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data));
    }
    else if(keynum!=0&&(view==14||view==15||view==16))//指纹已满界面、指纹为空界面、指纹ID删除失败，按任意键返回系统设置界面
    {
        line_flag=0;
        OLED_CLS();
        view=3;
        user_index=0;admin_index=0;
        //memset(input_user_password,0,sizeof(input_user_password));
        memset(user_data,0,sizeof(user_data));
        memset(admin_data,0,sizeof(admin_data));
    }
    else
    switch(keynum){
        case 1://*
            //
            if(view==1)//进入设置界面
            {
                input_user_password[k++]=255;
            }
            break;
        case 2://7
            if(view==0||view==2){
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,7,2,16);
                    input_user_password[k++]=7;
                }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,7,2,16);
                    user_data[user_index++]=7;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,7,2,16);
                    admin_data[admin_index++]=7;
                }
            }
            break;
        case 3://4
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,4,2,16);
                        input_user_password[k++]=4;
                    }
                }
            else if(view==7)
            {
                ID_NUM_delete=4;
                OLED_ShowNum(48,4,4,2,16);
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,4,2,16);
                    user_data[user_index++]=4;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,4,2,16);
                    admin_data[admin_index++]=4;
                }
            }
            
            break;
        case 4://1
            if(view==0||view==2){
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,1,2,16);
                    input_user_password[k++]=1;
                }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,1,2,16);
                    user_data[user_index++]=1;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,1,2,16);
                    admin_data[admin_index++]=1;
                }
            }
            break;
        case 5://0
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,0,2,16);
                        input_user_password[k++]=0;
                    }
                }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,0,2,16);
                    user_data[user_index++]=0;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,0,2,16);
                    admin_data[admin_index++]=0;
                }
            }
            break;
        case 6://8
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,8,2,16);
                        input_user_password[k++]=8;
                    }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,8,2,16);
                    user_data[user_index++]=8;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,8,2,16);
                    admin_data[admin_index++]=8;
                }
            }
            break;
        case 7://5
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,5,2,16);
                        input_user_password[k++]=5;
                    }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,5,2,16);
                    user_data[user_index++]=5;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,5,2,16);
                    admin_data[admin_index++]=5;
                }
            }
            break;
        case 8://2
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,2,2,16);
                        input_user_password[k++]=2;
                    }
                }
            else if(view==7)
            {
                ID_NUM_delete=2;
                OLED_ShowNum(48,4,2,2,16);
            }
            else if(view == 4||view ==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,2,2,16);
                    user_data[user_index++]=2;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,2,2,16);
                    admin_data[admin_index++]=2;
                }
            }
            break;
        case 9://#  确认键
            //OLED_ShowNum(16,2,keynum,2,16);
            switch(view)
            {
                case 0://初始界面 登录按钮
                        if(k==6)
                        {
                            denglu_jduge();
                            k=0;
                        }
                        else
                        {
                            k=0;
                            OLED_CLS();
                            //SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");
                            
                            view=2;
                            //HAL_Delay(1000);
                            //HAL_Delay(1000);
                            //Unlock_failed();
                        }
                        memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 1://登录成功界面,进入管理界面
                    if(k==1&&input_user_password[0]==255)
                    {
                        OLED_CLS();
                        view=3;
                    }
                     k=0;
                     memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 2://解锁失败界面，登录按钮
                        if(k==6)
                        {
                            denglu_jduge();
                            k=0;
                        }
                        else
                        {
                            k=0;
                            OLED_CLS();
                            //SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");
                            
                            view=2;
                            //HAL_Delay(1000);
                            //HAL_Delay(1000);
                            //Unlock_failed();
                        }
                        memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 3://系统管理界面
                     switch(line_flag)
                      {
                        case 0:
                            //进入修改解锁密码界面
                            OLED_CLS();
                            view=4;
                            break;
                        case 1:
                            //进入修改管理员密码界面
                            OLED_CLS();
                            view=5;
                            break;
                        case 2:
                            //进入添加指纹界面
                            OLED_CLS();
                            if(ID_NUM_store>=3)
                            {
                                view=14;
                                break;
                            }
                            view=6;
                            break;
                        case 3:
                            //进入删除指纹界面
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
                case 4://修改用户密码界面,确认修改按键
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
                case 5://修改管理员密码界面，确认修改按键
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
                case 6://添加指纹界面
                    break;
                case 7://删除指纹界面
                    if(ID_NUM_delete>=2&&ID_NUM_delete<=4)
                    {
                        OLED_CLS();
                        //view=13;//删除指纹成功
                        Del_FR(ID_NUM_delete);
                        ID_NUM_delete=0; 
                    }
                    else
                    {
                        OLED_CLS();
                        view=7;
                    }
                    break;
                case 9://修改用户密码界面,确认修改按键
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
                case 10://修改管理员密码界面，确认修改按键
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
            }         
            break;
        case 10://9
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,9,2,16);
                        input_user_password[k++]=9;
                    }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,9,2,16);
                    user_data[user_index++]=9;
                }
            }
            else if(view==5)//修改管理员密码
            {
                if(admin_index<6||view==10)
                {
                    OLED_ShowNum(16+16*admin_index,4,9,2,16);
                    admin_data[admin_index++]=9;
                }
            }
            
            break;
        case 11://6
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,6,2,16);
                        input_user_password[k++]=6;
                    }
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,6,2,16);
                    user_data[user_index++]=6;
                }
            }
            else if(view==5)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,6,2,16);
                    admin_data[admin_index++]=6;
                }
            }
            break;
        case 12://3
            if(view==0||view==2){
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,3,2,16);
                        input_user_password[k++]=3;
                    }
            }
            else if(view==7)
            {
                ID_NUM_delete=3;
                OLED_ShowNum(48,4,3,2,16);
            }
            else if(view == 4||view==9)//修改解锁密码
            {
                if(user_index<6)
                {
                    OLED_ShowNum(16+16*user_index,4,3,2,16);
                    user_data[user_index++]=3;
                }
            }
            else if(view==5||view==10)//修改管理员密码
            {
                if(admin_index<6)
                {
                    OLED_ShowNum(16+16*admin_index,4,3,2,16);
                    admin_data[admin_index++]=3;
                }
            }
            break;
        case 13://D 返回按键
            if(view==4||view==5||view==6||view==7)
            {
                OLED_CLS();
                line_flag=0;
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                view=3;
            }
            else if(view==3)
            {
                OLED_CLS();
                line_flag=0;
                view=1;
            }
            else if(view==9||view==10)
            {
                user_index=0;
                admin_index=0;
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                OLED_CLS();
                view=3;
            }
            //OLED_ShowNum(80,2,keynum,2,16);
            break;
        case 14://C  清除按键
            if(view==4||view==5||view==9||view==10)
            {
                user_index=0;admin_index=0;
                //memset(input_user_password,0,sizeof(input_user_password));
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                OLED_ClearGivenChar(4);
            }
           else if(view==0||view==2)
            {
                k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                OLED_ClearGivenChar(4);
            }
            else if(view == 7)
            {
                ID_NUM_delete=0;
                
                OLED_ClearGivenChar(4);
            }
            
            //OLED_ShowNum(96,2,keynum,2,16);
            break;
        case 15://B
            //OLED_ShowNum(0,4,keynum,2,16);
            if(view==3)//设置界面
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
            if(view==3)//设置界面
            {
                OLED_CLS();
                line_flag++;
                if(line_flag==4)
                    line_flag=0;
            }
            //OLED_ShowNum(16,4,keynum,2,16);
            break;
    }
    
    keynum=0;
}

//将蓝牙APP上发送的数据在（发送到）单片机，并在电脑串口助手上显示
void usart_disp(void)
{
    
    if(uart2_rx_buf[0]=='A')//添加指纹
    {
        OLED_CLS();
        if(ID_NUM_store>=3)//指纹已经满了
        {
            view=14;
            //HAL_UART_Transmit(&huart2,"添加指纹失败，已录入3个指纹",200,50);
            //HAL_UART_Transmit(&huart2,"2",15,50);
            HAL_UART_Transmit(&huart2,"addfull",15,50);
        }
        else
        {
            view=6;
        }
    }
    else if(uart2_rx_buf[0]=='D')//删除指纹
    {
        //OLED_ShowStr(0,2,"DeleteFigure",2);//ID
        //HAL_UART_Transmit(&huart2,"deletesuccess",15,50);
        OLED_CLS();
        if(ID_NUM_store==0)//录入指纹为空
        {
            //view=15;
            HAL_UART_Transmit(&huart2,"empty",15,50);
        }
        else
        {
            //view=7;
            if((uart2_rx_buf[1]-'0')<=4&&(uart2_rx_buf[1]-'0')>=2)
            {
                Del_FR(uart2_rx_buf[1]-'0');
            }
            else
            {
                HAL_UART_Transmit(&huart2,"iderror",15,50);
                //HAL_UART_Transmit(&huart2,"删除指纹失败,请重新输入编号",100,50);
            }
        }
    }
    else if(uart2_rx_buf[0]=='O')//开门
    {
        //OLED_ShowStr(0,4,"Open",2);//ID
        OLED_CLS();
        view=1;
    }
    else if(uart2_rx_buf[0]=='U')//修改解锁密码
    {
//        OLED_ShowStr(0,6,"User",2);//ID
//        user_data[0]=uart2_rx_buf[1]-'0';
//        user_data[1]=uart2_rx_buf[2]-'0';
//        user_data[2]=uart2_rx_buf[3]-'0';
//        user_data[3]=uart2_rx_buf[4]-'0';
//        user_data[4]=uart2_rx_buf[5]-'0';
//        user_data[5]=uart2_rx_buf[6]-'0';
        Store_Data[0]=uart2_rx_buf[1]-'0';
        Store_Data[1]=uart2_rx_buf[2]-'0';
        Store_Data[2]=uart2_rx_buf[3]-'0';
        Store_Data[3]=uart2_rx_buf[4]-'0';
        Store_Data[4]=uart2_rx_buf[5]-'0';
        Store_Data[5]=uart2_rx_buf[6]-'0';
        Store_Save();
    }
    else if(uart2_rx_buf[0]=='R')//修改管理员密码
    {
        Store_Admin[0]=uart2_rx_buf[1]-'0';
        Store_Admin[1]=uart2_rx_buf[2]-'0';
        Store_Admin[2]=uart2_rx_buf[3]-'0';
        Store_Admin[3]=uart2_rx_buf[4]-'0';
        Store_Admin[4]=uart2_rx_buf[5]-'0';
        Store_Admin[5]=uart2_rx_buf[6]-'0';
        Store_Save();
    }    
    uart2_rx_len=0;
    memset(uart2_rx_buf,0,sizeof(uart2_rx_buf));
    //HAL_UART_Transmit(&huart2,"Success",15,50);
    //HAL_UART_Transmit(&huart2,"1",15,50);
}
//将串口助手上发送的数据传给蓝牙APP
void usart1_disp(void)
{
    HAL_UART_Transmit(&huart2,uart1_rx_buf,15,50);

    //printf("%s\r\n",uart1_rx_buf);

    uart1_rx_len=0;
    memset(uart1_rx_buf,0,sizeof(uart1_rx_buf));
}
