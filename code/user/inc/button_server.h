#ifndef __BUTTON_SERVER_H__
#define __BUTTON_SERVER_H__

enum state{
    button_up,
    button_down,
    button_shake,
};

/**
 * @brief ��ť��ʼ��
 * 
 */
void ButtonServer_Init(void);
#endif
