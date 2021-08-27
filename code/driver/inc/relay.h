#ifndef __RELAY_H__
#define __RELAY_H__

enum relay_type{
    RELAY_LORA_MISS,
    RELAY_LOST_POWER,
    RELAY_GET,

    RELAY_TYPE_NUM,
};

/**
 * @brief 继电器初始化
 */
void Relay_Init(void);

/**
 * @brief 打开继电器
 * 
 * @param type 
 */
void Relay_Open(enum relay_type type);

/**
 * @brief 关闭继电器
 * 
 * @param type 
 */
void Relay_Close(enum relay_type type);
#endif
