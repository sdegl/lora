/*
 * @Author: sdegl
 * @LastEditTime: 2021-04-02 15:34:28
 * @FilePath: \RWC\code\user\inc\test.h
 * @LastEditors: sdegl
 */

#ifndef __TEST_H__
#define __TEST_H__
#include <stdint.h>

/* 串口状态 */
typedef enum
{
    Uart_Success,
    Uart_Fail,
}UartStatus_t;

/* 硬件序号 */
typedef enum
{
    UART_NUM_1, // 串口1
    UART_NUM_2, // 串口2
}UartNum_t;

/* 事件类型 */
typedef enum
{
    UART_Rx_Done,
    UART_Tx_Done,
}UartEnevt_t;

/* 事件回调函数 */
typedef void (*uart_callback)(UartEnevt_t);

/* 串口驱动结构 */
struct Uart_s
{
    UartStatus_t (*Init)(UartNum_t, uart_callback);    // 串口初始化
    void (*DeInit)(void);                               // 串口反初始化
    void (*Send)(uint8_t *source, uint16_t lenght);     // 串口发送
    uint16_t (*Receive)(uint8_t*, uint16_t lenght);     // 查询串口接收
    void (*SetConfig)(UartEnevt_t, bool enbale);        // 设置串口
};
/////////////////////////////////////////////////////////////////////

/* flash状态 */
typedef enum
{
    Flash_Success,
    Flash_Fail,
}FlashStatus_t;

/* flash驱动结构 */
struct Flash_s
{
    FlashStatus_t (*Creat)(void);    
    void (*Write)(uint8_t source, uint16_t lenght);
    uint16_t (*Read)(uint8_t destination, uint16_t lenght);
};
/////////////////////////////////////////////////////////////////////

/* 编程状态 */
typedef enum
{
    Program_Success,
    Program_Fail,
}ProgramStatus_t;

/* 编程事件 */
typedef struct 
{
    void (*start_event)(void);
    void (*stop_event)(void);
    void (*exit_event)(void);
}program_callback_t;

typedef enum
{
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
}ProgramWeek_t;

/* 编程结构 */
struct Program_s
{
    ProgramStatus_t (*Creat)(ProgramWeek_t, uint8_t start_time, uint8_t end_time, program_callback_t*); 
    void (*Delete)(void);
    ProgramStatus_t (*SetTime)(uint8_t start_time, uint8_t endtime);
    void (*GetTime)(uint8_t start_time, uint8_t end_time);
};
/////////////////////////////////////////////////////////////////////

/* 定时器回调函数 */
typedef void (*timer_callback)(void*);

/* 定时器结构体 */
typedef struct __soft_timer
{
    uint16_t repeck;
    uint32_t timeout_ms;
    timer_callback func;
    void *parm;
    struct __soft_timer *next;
}SoftTimer_t;
/////////////////////////////////////////////////////////////////////

/* 按键事件类型 */
typedef enum
{
    Button_Up,
    Button_Down,
    Button_Long,
    Button_DoubleClick,
}ButtonEvent_t;

/* 按键事件回调函数 */
typedef void (*button_callbak)(ButtonEvent_t);

/* 按键结构 */
typedef struct __button
{
    uint8_t (*KeyStatus)(void);
    button_callbak func;
    struct __button *next;
};
/////////////////////////////////////////////////////////////////////

/* 电源状态 */
typedef enum
{
    Battery_Hight,
    Battery_Middle,
    Battery_Low,
    Power_Supply,
}PowerStatus_t;

#endif
