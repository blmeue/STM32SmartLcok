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

uint16_t input_user_password[6]={0};//保存输入的解锁密码
uint32_t k=0;//输入的下标

uint16_t input_admin_password[6]={0};//保存输入的管理员密码
uint32_t k_admin=0;//输入的管理员密码下标

unsigned char user_flag=0;//判断修改密码是否成功
unsigned char admin_flag=0;//判断修改管理员密码是否成功

unsigned char unlock=0;

unsigned char line_flag=0;//选择菜单
unsigned char figure_flag=0;//指纹是否禁用标志
unsigned char view=0;//界面

unsigned char key_user_count=0;//密码输入错误次数
unsigned char key_user_flag=0;//密码输入禁用标志
unsigned char key_user_flag0=0;//重新启用密码输入标志

unsigned char key_admin_count=0;//管理员密码输入错误次数
unsigned char key_admin_flag=0;//管理员密码输入禁用标志
unsigned char key_admin_flag0=0;//重新启用密码输入标志

uint16_t admin_data[6]={0};//新的管理员密码
uint16_t user_data[6]={0};//新的解锁密码

uint16_t admin_index=0;//新的解锁密码下标
uint16_t user_index=0;//新的管理员密码下标

unsigned char figuer_count=0;//指纹验证失败次数

//上锁界面
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

//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//m[0~16]:0背景音乐为静音，16背景音乐音量最大
//v[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快
//SYN_FrameInfo(2, "[v7][m1][t5]欢迎使用SYN6288");
uint8_t SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
uint8_t SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
uint8_t SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
uint8_t SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
uint8_t SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令

//判断密码是否正确
void denglu_jduge(void)
{
    //判断输入密码是否正确
    if(memcmp(input_user_password,Store_Data,sizeof(input_user_password))==0)
    {
        //解锁成功
        OLED_CLS();
        view=1;
        SYN_FrameInfo(2,"[v7][m1][t5]开锁成功");
        HAL_Delay(1000);
        key_user_count=0;
        key_user_flag=0;
    }
    else
    {
        //解锁失败
        OLED_CLS();
        
        SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");
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

//解锁成功界面
void Unlock_success(void)
{
     OLED_ShowCN(0,2,12)  ; //解
     OLED_ShowCN(16,2,6); //锁
     OLED_ShowCN(32,2,16); //成
     OLED_ShowCN(48,2,17); //功
     OLED_ShowCN(90,6,23); //设
     OLED_ShowCN(106,6,24); //置
     figure_flag=0;
     figuer_count=0;
//     k_admin=0;
//     memset(input_admin_password,0,sizeof(input_admin_password));
     //k=0;
     //memset(input_user_password,0,sizeof(input_user_password));
}

//解锁失败界面
void Unlock_failed(void)
{
     OLED_ShowCN(0,2,12)  ; //解
     OLED_ShowCN(16,2,6)  ; //锁
     OLED_ShowCN(32,2,18) ; //失
     OLED_ShowCN(48,2,19) ; //败
     OLED_ShowCN(64,2,22) ; //，
     OLED_ShowCN(80,2,9)  ; //请
     OLED_ShowCN(96,2,20) ; //重
     OLED_ShowCN(112,2,21); //试 
     if(figure_flag==0&&key_user_flag==0)
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
     
     OLED_ShowCN(0,6,46);  //清
     OLED_ShowCN(16,6,47); //除
     
    OLED_ShowCN(96,6,48);  //确
    OLED_ShowCN(112,6,49); //定
     
}

//禁止输入用户密码界面
void forbid_user(void)
{
    //密码错误三次，秒后再试
    OLED_ShowCN(16,0,13); //密
    OLED_ShowCN(32,0,14); //码
    OLED_ShowCN(48,0,58); //错
    OLED_ShowCN(64,0,59); //误
    OLED_ShowNum(80,0,3,1,16);//3
    OLED_ShowCN(96-8,0,60); //次
    
    OLED_ShowNum(16,2,30-sec1,2,16);//3
    OLED_ShowCN(32,2,61); //秒
    OLED_ShowCN(48,2,62); //后
    OLED_ShowCN(64,2,54); //再
    OLED_ShowCN(80,2,21); //试
        
}

//输入管理员密码界面
void admin_input(void)
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
    
    OLED_ShowCN(0,6,46);  //清
    OLED_ShowCN(16,6,47); //除
    
    //返回，回到系统设置界面
    OLED_ShowCN(90,6,44);  //返
    OLED_ShowCN(106,6,45); //回  
}

//判断管理员密码是否正确
void admin_judge(void)
{
    //判断输入密码是否正确
    if(memcmp(input_admin_password,Store_Admin,sizeof(input_admin_password))==0)
    {
        //管理员密码正确，进入设置模式（管理员模式）
        OLED_CLS();
        view=3;
        SYN_FrameInfo(2,"[v7][m1][t5]进入管理员模式");
        //k_admin=0;
        line_flag=0;
        HAL_Delay(1000);
        key_admin_count=0;
        key_user_flag=0;
    }
    else
    {
        //管理员密码错误
        OLED_CLS();
        //view=19;
        SYN_FrameInfo(2,"[v7][m1][t5]管理员密码错误");
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

//管理员登录失败
void admin_fail(void)
{
    //管理员密码错误
     OLED_ShowCN(16-8,0,27)  ; //管
     OLED_ShowCN(32-8,0,28)  ; //理
     OLED_ShowCN(48-8,0,29) ; //员
     OLED_ShowCN(64-8,0,13); //密
     OLED_ShowCN(80-8,0,14); //码
     OLED_ShowCN(96-8,0,58); //错
     OLED_ShowCN(112-8,0,59); //误
    
     OLED_ShowCN(48,2,9)  ; //请
     OLED_ShowCN(64,2,20) ; //重
     OLED_ShowCN(80,2,21); //试 
    
    OLED_ShowCN(0,6,46);  //清
    OLED_ShowCN(16,6,47); //除
    
    //返回，回到系统设置界面
    OLED_ShowCN(90,6,44);  //返
    OLED_ShowCN(106,6,45); //回
}

//禁用管理员登录
void forbid_admin(void)
{
    //管理员密码错误三次，秒后再试
    OLED_ShowCN(16-8,0,27); //管
    OLED_ShowCN(32-8,0,28); //理
    OLED_ShowCN(48-8,0,29); //员
    OLED_ShowCN(64-8,0,13); //密
    OLED_ShowCN(80-8,0,14); //码
    OLED_ShowCN(96-8,0,58); //错
    OLED_ShowCN(112-8,0,59); //误
    
    OLED_ShowNum(8,2,3,1,16);//3
    OLED_ShowCN(16,2,60); //次
    OLED_ShowCN(32,2,9); //请
    OLED_ShowNum(48,2,30-sec2,2,16);//3
    OLED_ShowCN(64,2,61); //秒
    OLED_ShowCN(80,2,62); //后
    OLED_ShowCN(96,2,54); //再
    OLED_ShowCN(112,2,21); //试
}

//菜单页面
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
    
    k_admin=0;
    memset(input_admin_password,0,sizeof(input_admin_password));
    k=0;
    memset(input_user_password,0,sizeof(input_user_password));
}

//指纹解锁模块
void Figure(void)
{
    //有指纹按下时
    if(IsModuleWakingUp1())
    {
        press_FR();
    }
}


//修改解锁密码页面
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

//修改成功提示
void changed_success_view(void)
{
       OLED_ShowCN(0,2,25)  ; //修
       OLED_ShowCN(16,2,26); //改
       OLED_ShowCN(32,2,16); //成
       OLED_ShowCN(48,2,17); //功
}

//修改解锁失败界面
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

//修改管理员密码失败界面
void changed_admin_fail_view(void)
{
    OLED_ShowCN(0,2,25)  ; //修
    OLED_ShowCN(16,2,26) ; //改
    OLED_ShowCN(32,2,18) ; //失
    OLED_ShowCN(48,2,19) ; //败
    OLED_ShowCN(64,2,22) ; //，
    OLED_ShowCN(80,2,9)  ; //请
    OLED_ShowCN(96,2,20) ; //重
    OLED_ShowCN(112,2,21); //试 
    
    OLED_ShowCN(0,6,46);  //清
    OLED_ShowCN(16,6,47); //除
    
    //返回，回到系统设置界面
    OLED_ShowCN(90,6,44);  //返
    OLED_ShowCN(106,6,45); //回  
}

//判断修改解锁密码是否成功
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

//判断修改管理员密码是否成功
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

//修改管理员密码界面
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

//指纹添加成功提示界面
void add_success(void)
{
    OLED_ShowCN(16,2,30) ; //添
    OLED_ShowCN(32,2,31) ; //加
    OLED_ShowCN(32+16,2,16); //成
    OLED_ShowCN(48+16,2,17); //功
}

//指纹添加失败提示界面
void add_fail(void)
{
    OLED_ShowCN(16,2,30) ; //添
    OLED_ShowCN(32,2,31) ; //加
    OLED_ShowCN(32+16,2,18); //失
    OLED_ShowCN(48+16,2,19); //败
}

//指纹已经满了，指纹添加失败
void figuer_fill(void)
{
    //已添加3个指纹，无法继续添加
    OLED_ShowCN(16,2,50) ; //已
    OLED_ShowCN(32,2,30) ; //添
    OLED_ShowCN(48,2,31) ; //加
    OLED_ShowNum(64,2,3,2,16);//3
    OLED_ShowCN(80,2,51); //个
    OLED_ShowCN(32,4,52); //无
    OLED_ShowCN(48,4,53); //法
    OLED_ShowCN(64,4,54); //再
    OLED_ShowCN(80,4,30) ; //添
    OLED_ShowCN(96,4,31) ; //加
}

//添加指纹界面
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

//删除指纹界面
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
//删除成功提示界面
void delete_success(void)
{
    OLED_ShowCN(16,2,32) ; //删
    OLED_ShowCN(32,2,33) ; //除
    OLED_ShowCN(32+16,2,16); //成
    OLED_ShowCN(48+16,2,17); //功
}
//删除失败界面，ID未录入指纹
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

//删除指纹失败界面，指纹个数为0
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


//界面显示
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
        case 17://禁止输入解锁密码界面
            forbid_user();
            break;
        case 18://输入管理员密码界面
            admin_input();
            break;
        case 19://管理员登录失败
            admin_fail();
            break;
        case 20://禁止输入管理员密码界面
            forbid_admin();
            break;
    }
}

unsigned char keep_menu=0;

//按键控制模块
void key_display(void)
{
    if((uwTick-uwTick_key)<20)return;
    uwTick_key=uwTick;
    
    keynum=button4_4_Scan();
    if(key_admin_flag0==1)//禁用管理员密码输入
    {
        OLED_CLS();
        view=18;
        key_admin_flag0=0;
    }
    if(key_user_flag0==1)//禁用用户密码输入
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

    if(key_user_flag==1)//禁止输入
    {
        return;
    }
    if(long_view==1&&view==0)//解锁界面，一分钟无操作只需要清除第四行的内容即可
    {
       OLED_ClearGivenChar(4); 
       k=0;
       memset(input_user_password,0,sizeof(input_user_password));
    }
    else if(long_view==1&&view!=0)//一分钟无操作自动上锁
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]已上锁");
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
    
    if(keynum!=0&&(view==8||view==11||view==13))//添加指纹成功、删除指纹、修改密码成功，按任意键返回到锁屏界面
    {
        line_flag=0;
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]已上锁");
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
    else if(keynum!=0&&(view==14||view==15||view==16))//指纹已满界面、指纹为空界面、指纹ID删除失败，按任意键返回系统设置界面
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
            if(view==1)//进入设置界面
            {
                input_user_password[k++]=255;
                //view=3;//进入设置界面
            }
            break;
        case 2://7
            if(view==0||view==2){//用户登录界面
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,7,2,16);
                    input_user_password[k++]=7;
                }
            }
            else if(view==18||view==19)//管理员用户登录界面7
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,7,2,16);
                    input_admin_password[k_admin++]=7;
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
            if(view==0||view==2){//用户登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,4,2,16);
                        input_user_password[k++]=4;
                    }
                }
            else if(view==18||view==19)//管理员登录界面4
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,4,2,16);
                    input_admin_password[k_admin++]=4;
                }
            }
            else if(view==7)//输入删除指纹ID界面
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
            if(view==0||view==2){//登录界面
              if(k!=6)
                {
                    OLED_ShowNum(16+16*k,4,1,2,16);
                    input_user_password[k++]=1;
                }
            }
            else if(view==18||view==19)//管理员登录界面1
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,1,2,16);
                    input_admin_password[k_admin++]=1;
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
            if(view==0||view==2){//登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,0,2,16);
                        input_user_password[k++]=0;
                    }
                }
            else if(view==18||view==19)//管理员登录界面0
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,0,2,16);
                    input_admin_password[k_admin++]=0;
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
            if(view==0||view==2){//登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,8,2,16);
                        input_user_password[k++]=8;
                    }
            }
            else if(view==18||view==19)//管理员登录界面8
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,8,2,16);
                    input_admin_password[k_admin++]=8;
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
            if(view==0||view==2){//登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,5,2,16);
                        input_user_password[k++]=5;
                    }
            }
            else if(view==18||view==19)//管理员登录界面5
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,5,2,16);
                    input_admin_password[k_admin++]=5;
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
            if(view==0||view==2){//登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+16*k,4,2,2,16);
                        input_user_password[k++]=2;
                    }
                }
            else if(view==18||view==19)//管理员登录界面2
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,2,2,16);
                    input_admin_password[k_admin++]=2;
                }
            }
            else if(view==7)//输入删除指纹ID界面
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
            switch(view)
            {
                case 0://初始界面 登录按钮
                        if(k==6)
                        {
                            denglu_jduge();
                            //k=0;
                        }
                        else
                        {
                            //k=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");       
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
                case 1://登录成功界面,进入管理界面
                    if(k==1&&input_user_password[0]==255)
                    {
                        OLED_CLS();
                        //view=3;//进入设置界面
                        view=18;//进入输入管理员密码界面
                    }
                     //k=0;
                     memset(input_user_password,0,sizeof(input_user_password));
                    break;
                case 2://解锁失败界面，登录按钮
                        if(k==6)
                        {
                            denglu_jduge();
                            //k=0;
                        }
                        else
                        {
                            //k=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]解锁失败");                         
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
                case 18://输入管理员密码界面
                    if(k_admin==6)
                        {
                            admin_judge();
                            //k_admin=0;
                        }
                        else
                        {
                            //k_admin=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]管理员密码错误");       
                            HAL_Delay(1000);
                            key_admin_count++;
                            if(key_admin_count==3)
                            {
                                //OLED_ClearGivenChar(2);//清除第二行
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
                case 19://管理员密码错误，输入管理员密码界面
                    if(k_admin==6)
                        {
                            admin_judge();
                            //k_admin=0;
                        }
                        else
                        {
                            //k_admin=0;
                            OLED_CLS();
                            SYN_FrameInfo(0,"[v7][m1][t5]管理员密码错误");       
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
            if(view==0||view==2){//用户登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,9,2,16);
                        input_user_password[k++]=9;
                    }
            }
            else if(view==18||view==19)//管理员登录界面9
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,9,2,16);
                    input_admin_password[k_admin++]=9;
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
            if(view==0||view==2){//用户登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,6,2,16);
                        input_user_password[k++]=6;
                    }
            }
            else if(view==18||view==19)//管理员用户登录界面6
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,6,2,16);
                    input_admin_password[k_admin++]=6;
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
            if(view==0||view==2){//用户登录界面
                if(k!=6)
                    {
                        OLED_ShowNum(16+k*16,4,3,2,16);
                        input_user_password[k++]=3;
                    }
            }
            else if(view==18||view==19)//管理员登录界面3
            {
                if(k_admin!=6)
                {
                    OLED_ShowNum(16+16*k_admin,4,3,2,16);
                    input_admin_password[k_admin++]=3;
                }
            }
            else if(view==7)//输入删除指纹ID界面
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
            if(view==4||view==5||view==6||view==7)//返回到菜单界面
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
            else if(view==3||view==18||view==19)//返回到解锁成功界面
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
            else if(view==9||view==10)//返回到系统设置界面
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
            else if(view==1)//解锁成功，按D键上锁
            {
                OLED_CLS();
                SYN_FrameInfo(0,"[v7][m1][t5]已上锁");
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
        case 14://C  清除按键
            if(view==4||view==5||view==9||view==10)
            {
                user_index=0;admin_index=0;k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                memset(user_data,0,sizeof(user_data));
                memset(admin_data,0,sizeof(admin_data));
                OLED_ClearGivenChar(4);
            }
           else if(view==0||view==2)//用户密码输入界面清除
            {
                k=0;
                memset(input_user_password,0,sizeof(input_user_password));
                OLED_ClearGivenChar(4);
            }
            else if(view==18||view==19)//管理员输入界面清除
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
            break;
    }
    
    keynum=0;
}


//接收蓝牙APP发送的数据，并进行相关处理
unsigned add_flag=0;//蓝牙发送添加、删除指纹标志  1：添加；2：删除

void usart_disp(void)
{ 
    if(uart1_rx_buf[0]=='A')//添加指纹
    {
        OLED_CLS();
        
        if(add_flag==0)
        {
            
            if(ID_NUM_store>=3)//指纹已经满了
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
    else if(uart1_rx_buf[0]=='D')//删除指纹
    {
        if(add_flag==0)
        {
            if(ID_NUM_store==0)//录入指纹为空
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
    else if(uart1_rx_buf[0]=='O')//系统已上锁，点击开门
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]已开门，请上锁");
        view=1;
        HAL_Delay(1000);
    }
    else if(uart1_rx_buf[0]=='C')//系统已开锁，点击关锁
    {
        OLED_CLS();
        SYN_FrameInfo(0,"[v7][m1][t5]已上锁");
        view=0;
        HAL_Delay(1000);
    }
    else if(uart1_rx_buf[0]=='U')//修改解锁密码
    {
        Store_Data[0]=uart1_rx_buf[1]-'0';
        Store_Data[1]=uart1_rx_buf[2]-'0';
        Store_Data[2]=uart1_rx_buf[3]-'0';
        Store_Data[3]=uart1_rx_buf[4]-'0';
        Store_Data[4]=uart1_rx_buf[5]-'0';
        Store_Data[5]=uart1_rx_buf[6]-'0';
        Store_Save();
    }
    else if(uart1_rx_buf[0]=='R')//修改管理员密码
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

//将串口助手上发送的数据传给蓝牙APP
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
