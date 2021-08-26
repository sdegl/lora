#ifndef __EASYLORA_H__
#define __EASYLORA_H__

#include <stdint.h>
#include <string.h>

//#define GATEWAY
#define NODE

/* �����Ϣ */
typedef enum {
    EXIT_PARING,
    FIND_DEVICE,
    DEVICE_OK,
} PairingType_t;

/* ����¼� */
typedef struct {
    uint32_t machine_id; // ����id
    PairingType_t type; // �����Ϣ����
    uint8_t salve_type; // �ӻ�����
} PairingEvent_t;


typedef struct {
    uint16_t exit_time;                 // �˳�ʱ��
    uint32_t machine_id;                // ����id
    void (*event)(PairingEvent_t*);     // �¼��ص�
} PairConfig_t;

/**
 * @brief ��ʼ����
 * 
 * @param config ��������
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
 * @brief ��ʼ��
 */
void EasyLoraInit(void);

/**
 * @brief ��ʼ����
 */
void StartNetworking(PairConfig_t *config);

/**
 * @brief ��������
 */
void StopNetworking(void);

/**
 * @brief ������豸id����
 * 
 * @param machine_id 
 * @return enum networking_ret 
 */
enum networking_ret AcceptNetworking(uint32_t machine_id);

/**
 * @brief ɾ�����豸id������Ϣ
 * 
 * @param machine_id 
 * @return enum networking_ret 
 */
enum networking_ret RemovingNetworking(uint32_t machine_id);

#endif
