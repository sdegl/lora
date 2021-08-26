#ifndef __EASYLORA_H__
#define __EASYLORA_H__

#include <stdint.h>
#include <string.h>

//#define GATEWAY
#define NODE

/* 配对信息 */
typedef enum {
    EXIT_PARING,
    FIND_DEVICE,
    DEVICE_OK,
} PairingType_t;

/* 配对事件 */
typedef struct {
    uint32_t machine_id; // 机器id
    PairingType_t type; // 配对消息类型
    uint8_t salve_type; // 从机类型
} PairingEvent_t;


typedef struct {
    uint16_t exit_time;                 // 退出时间
    uint32_t machine_id;                // 机器id
    void (*event)(PairingEvent_t*);     // 事件回调
} PairConfig_t;

/**
 * @brief 开始组网
 * 
 * @param config 组网配置
 */
void StartJoinNetwork(PairConfig_t *config);

/**
 * @brief 
 * 
 * @param data 
 * @param lenght 
 */
void lora_app_data_handler(const uint8_t *data, uint16_t lenght);

/**
 * @brief 初始化
 */
void EasyLoraInit(void);

/**
 * @brief 开始组网
 */
void StartNetworking(PairConfig_t *config);

/**
 * @brief 结束组网
 */
void StopNetworking(void);

/**
 * @brief 允许改设备id配网
 * 
 * @param machine_id 
 * @return enum networking_ret 
 */
enum networking_ret AcceptNetworking(uint32_t machine_id);

/**
 * @brief 删除该设备id配网信息
 * 
 * @param machine_id 
 * @return enum networking_ret 
 */
enum networking_ret RemovingNetworking(uint32_t machine_id);

#endif
