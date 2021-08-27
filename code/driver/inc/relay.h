#ifndef __RELAY_H__
#define __RELAY_H__

enum relay_type{
    RELAY_LORA_MISS,
    RELAY_LOST_POWER,
    RELAY_GET,

    RELAY_TYPE_NUM,
};

/**
 * @brief �̵�����ʼ��
 */
void Relay_Init(void);

/**
 * @brief �򿪼̵���
 * 
 * @param type 
 */
void Relay_Open(enum relay_type type);

/**
 * @brief �رռ̵���
 * 
 * @param type 
 */
void Relay_Close(enum relay_type type);
#endif
