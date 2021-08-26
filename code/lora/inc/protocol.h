#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "easylora.h"
#include "radio.h"
#include "stm32_timer.h"
#include <stdint.h>
#include <string.h>

#define PROTOCOL_VER        0x01
#define delog(s, ...)

/* 弱定义 */
#ifndef __WEAK
#if defined(__ICCARM__)
#define __WEAK __WEAK __ATTRIBUTES
#elif defined(__CC_ARM)
#define __WEAK __weak
#elif defined(__GUNC__)
#define __WEAK __weak
#else
#define __WEAK
#endif
#endif

#define LORA_BANDWIDTH_500K 2 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_BANDWIDTH_250K 1 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_BANDWIDTH_125K 0 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */

#define LORA_SPREADING_FACTOR_12 12 /* [SF7..SF12] */
#define LORA_SPREADING_FACTOR_11 11 /* [SF7..SF12] */
#define LORA_SPREADING_FACTOR_10 10 /* [SF7..SF12] */
#define LORA_SPREADING_FACTOR_9 9 /* [SF7..SF12] */
#define LORA_SPREADING_FACTOR_8 8 /* [SF7..SF12] */
#define LORA_SPREADING_FACTOR_7 7 /* [SF7..SF12] */

#define LORA_CODINGRATE_4_5 1 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_CODINGRATE_4_6 2 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_CODINGRATE_4_7 3 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_CODINGRATE_4_8 4 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */

/* 配网固定Lora配置 */
#define NETWORKING_GATEWAY_TX_OUTPUT_POWER 22 /* dBm */
#define NETWORKING_BANDWIDTH LORA_BANDWIDTH_250K
#define NETWORKING_SPREADING_FACTOR LORA_SPREADING_FACTOR_9
#define NETWORKING_PREAMBLE_LENGTH 20 /* Same for Tx and Rx */
#define NETWORKING_SYMBOL_TIMEOUT 20 /* Symbols */
#define NETWORKING_FIX_LENGTH_PAYLOAD true // 使用固定长度
#define NETWORKING_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 3000
#define TX_TIMEOUT_VALUE 3000
#define NETWORKING_CRC_ON true
#define FREQ_HOP_ON false
#define NETWORKING_FREQUENCY 868000000 /* Hz */

enum networking_ret {
    NW_OK,
    NW_FAIL,
};

typedef enum {
    JOIN_REQ = 1,               // 组网请求
    JOIN_RESP,                  // 组网回应
    DATA_DOWN,                  // 下行数据
    DATA_UP,                    // 上行数据
    CONFIG,                     // lora配置
    QUERY                       // 查询通讯质量
} FrameHead_t;

typedef enum {
    UNNETWORKING,               // 未组网
    NETWORKING_OK,              // 组网ok
    RUNNING,                    // 正在运行
} PairState_t;

typedef struct {
    uint8_t head:4;             // 头码选择
    uint8_t ver:4;              // 协议版本号
    uint8_t cnt:5;              // 帧计数
    uint8_t rxDelay:3;          // 接收延时，x07表示关闭接收窗口,0x00表示常接收，温控器rxDelay默认2秒
    uint16_t netId:12;          // 组网ID
    uint16_t slaveNum:4;        // 从机号
} frameType_t;

/**
 * @brief lora数据接收
 * 
 * @param payload 
 * @param size 
 * @param rssi 
 * @param snr 
 */
void lora_data_receive_handler(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr);

#endif //__PROTOCOL_H__
