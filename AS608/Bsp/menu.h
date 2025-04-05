#ifndef __MENU_H__
#define __MENU_H__

#include "timer.h"

extern unsigned char view;
extern unsigned char figuer_count;
extern unsigned char figure_flag;
extern unsigned char keep_menu;


void main_menu(void);
void denglu_jduge(void);
void Unlock_success(void);
void Unlock_failed(void);
void admin_menu(void);
void key_display(void);

void Figure(void);
void view_display(void);

void add_figure(void);
void delete_figure(void);
void change_admin(void);
void change_user(void);
void change_admin_fail_success(void);
void change_user_fail_success(void);
void changed_success_view(void);
void changed_admin_fail_view(void);
void changed_user_fail_view(void);
void add_success(void);
void add_fail(void);
void figuer_fill(void);
void delete_success(void);
void figuer_empty(void);
void delete_fail(void);




void usart1_disp(void);
void usart_disp(void);



#endif
