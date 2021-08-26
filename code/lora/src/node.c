#include "node.h"
#include "protocol.h"

#ifdef NODE

typedef enum {
    NO_WORKING = 0,
    NETWORKING = 1,
    RUNNING = 2,
    SEND = 4,
    RECEIVE = 8,
}LoraState;

struct node_workstate {
    uint8_t len;
    uint32_t buf[32];
    PairState_t pari_state;     // 组网状态
    frameType_t frame;          // 头消息
    uint32_t machine_id;        // 机器id
    UTIL_TIMER_Object_t Send_timer;
    // UTIL_TIMER_Object_t Receive_timer;
};

/*!
 * @brief Function to be executed on Radio Tx Done event
 * @param  none
 * @retval none
 */
static void OnTxDone(void);

/*!
 * @brief Function to be executed on Radio Rx Done event
 * @param  payload sent
 * @param  payload size
 * @param  rssi
 * @param  snr
 * @retval none
 */
static void OnRxDone(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr);

/*!
 * @brief Function executed on Radio Tx Timeout event
 * @param  none
 * @retval none
 */
static void OnTxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Timeout event
 * @param  none
 * @retval none
 */
static void OnRxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Error event
 * @param  none
 * @retval none
 */
static void OnRxError(void);

/**
 * @brief CAD结束回调
 * 
 * @param channelActivityDetected 
 */
static void OnCadDone(bool channelActivityDetected);

/**
 * 
 * @brief 找机器id的索引
 * 
 * @param machine_id 
 * @return uint8_t 
 */
static uint8_t find_machine_index(uint32_t machine_id);

/**
 * @brief 配对组网配置
 */
static void lora_config_networking(void);

/**
 * @brief lora配对信息发送
 */
static void lora_networking_Send(void);

static uint32_t machine_id; // 机器id
static LoraState lora_state; // LORA状态
static uint8_t lora_is_send; // lora发送标志位

static struct WorkState_t gate_state;
static struct node_workstate node_state;

void (*network_event)(PairingEvent_t*); // 配网回调

/* 定时器 */
static UTIL_TIMER_Object_t networking_timer;
static UTIL_TIMER_Object_t Receive_timer;

/* lora回调事件 */
static RadioEvents_t radio_event;

/**
 * @brief 初始化
 */
void EasyLoraInit(void)
{
    radio_event.RxDone = lora_data_receive_handler;
    radio_event.RxError = OnRxError;
    radio_event.RxTimeout = OnRxTimeout;
    radio_event.TxDone = OnTxDone;
    radio_event.TxTimeout = OnTxTimeout;
    radio_event.CadDone = OnCadDone;
    Radio.Init(&radio_event);
}

/**
 * @brief 开始组网
 */
void StartNetworking(PairConfig_t *config)
{
    if (config->machine_id != 0 && config->event != NULL) {
        machine_id = config->machine_id;
        network_event = config->event;

        /* 自动退出设置 */
        if (config->exit_time != 0) {
            UTIL_TIMER_Create(&networking_timer, config->exit_time * 1000, UTIL_TIMER_ONESHOT, (void (*)(void*))StopNetworking, NULL);
            UTIL_TIMER_Start(&networking_timer);
        }

        UTIL_TIMER_Create(&node_state.Send_timer, 5000, UTIL_TIMER_PERIODIC, (void (*)(void*))lora_start_send, NULL);
        UTIL_TIMER_Start(&node_state.Send_timer);

        lora_config_networking();
    }
}

// /**
//  * @brief 允许改设备id配网
//  * 
//  * @param machine_id 
//  * @return enum networking_ret 
//  */
// enum networking_ret AcceptNetworking(uint32_t machine_id)
// {
//     uint8_t null_index;

//     for (uint8_t i = 0; i < NODE_NUM; i++) {
//         if (node_state[i].machine_id == machine_id) 
//             return NW_FAIL;

//         if (node_state[i].pari_state == UNNETWORKING)
//             null_index = i;
//     }

//     node_state[null_index].frame.slaveNum = null_index + 2;
//     lora_accept_networking_pack(null_index, node_state[null_index].buf, node_state[null_index].len);
//     lora_start_send(); // 发送给该节点
//     return NW_OK;
// }

// /**
//  * @brief 删除该设备id配网信息
//  * 
//  * @param machine_id 
//  * @return enum networking_ret 
//  */
// enum networking_ret RemovingNetworking(uint32_t machine_id)
// {
//     for (uint8_t i = 0; i < NODE_NUM; i++) {
//         if (node_state[i].machine_id == machine_id) {
//             node_state[i].machine_id = 0;
//             node_state[i].pari_state = UNNETWORKING;

//             return NW_OK;
//         }
//     }

//     return NW_FAIL;
// }

/**
 * @brief lora组网数据处理
 * 
 * @param data 
 * @param lenght 
 */
static void lora_networking_handler(const uint8_t *data, uint16_t lenght, uint8_t rx_delay)
{
    /* 检查数据接收长度是否合法 */
    if (sizeof (uint32_t) < lenght) {
        PairingEvent_t event;

        node_state.frame.netId = *(uint16_t*)data << 12;
        node_state.frame.slaveNum = *(uint16_t*)data;

        event.type = DEVICE_OK;

        StopNetworking();
        network_event(&event);
    }
}

/**
 * @brief lora数据接收处理
 * 
 * @param payload 
 * @param size 
 * @param rssi 
 * @param snr 
 */
void lora_data_receive_handler(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr)
{
    frameType_t temp;
    const uint8_t *data;
    uint16_t pay_lenght;
    
    /* 检查数据接收长度是否合法 */
    if (sizeof(frameType_t) > size)
        return;

    temp = *(frameType_t*)payload; // 帧头
    data = payload + sizeof(frameType_t); // 载体
    pay_lenght = payload - sizeof(frameType_t); // 载体长度

    switch (temp.head) {
    case JOIN_REQ: {
        
    } break;

    case DATA_UP: {
        // lora_app_data_handler(data, pay_lenght);
    } break;

    case QUERY: {

    } break;

    case JOIN_RESP: {
        lora_networking_handler(data, pay_lenght, temp.rxDelay);
    } break;

    case CONFIG: {

    } break;

    case DATA_DOWN: {

    } break;
    }
}

/**
 * @brief 结束组网
 */
void StopNetworking(void)
{
    Radio.Standby();
    UTIL_TIMER_Stop(&networking_timer);
    UTIL_TIMER_Stop(&node_state.Send_timer);
}

static void lora_config_networking(void)
{
    Radio.Standby();
    Radio.SetTxConfig(MODEM_LORA, NETWORKING_GATEWAY_TX_OUTPUT_POWER, 0, NETWORKING_BANDWIDTH,
        NETWORKING_SPREADING_FACTOR, LORA_CODINGRATE_4_7,
        NETWORKING_PREAMBLE_LENGTH, NETWORKING_FIX_LENGTH_PAYLOAD,
        NETWORKING_CRC_ON, FREQ_HOP_ON, 0, NETWORKING_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

    Radio.SetRxConfig(MODEM_LORA, NETWORKING_BANDWIDTH, NETWORKING_SPREADING_FACTOR,
        LORA_CODINGRATE_4_7, 0, NETWORKING_PREAMBLE_LENGTH,
        NETWORKING_SYMBOL_TIMEOUT, NETWORKING_FIX_LENGTH_PAYLOAD,
        0, NETWORKING_CRC_ON, FREQ_HOP_ON, 0, NETWORKING_IQ_INVERSION_ON, true);

    Radio.SetChannel(NETWORKING_FREQUENCY);
    Radio.SetMaxPayloadLength(MODEM_LORA, 16);
    Radio.Rx(1);
}

static void lora_networking_pack(uint8_t *buf_out, uint8_t *len_out)
{
    frameType_t frame;
    uint8_t len = 0;

    frame.head = JOIN_REQ;
    frame.rxDelay = 0;
    frame.netId = 0;
    frame.slaveNum = 0;
    frame.ver = PROTOCOL_VER;
    frame.cnt = 0;

    memcpy(buf_out, &frame, sizeof(frame));
    len += sizeof(frame);
    memcpy(buf_out + len, &machine_id, sizeof(machine_id));
    len += sizeof(machine_id);

    *len_out = len;
}

static void lora_start_send(void)
{
    if (lora_is_send == 0) {
        Radio.Send(node_state.buf, node_state.len);
        lora_is_send = 1;
    }
}

static void OnCadDone(bool channelActivityDetected)
{
    if (channelActivityDetected == true) {

    } else {

    }
}

static void OnTxDone(void)
{
    lora_is_send = 0;
}


static void OnTxTimeout(void)
{
    lora_is_send = 0;
}

static void OnRxTimeout(void)
{

}

static void OnRxError(void)
{

}

#endif
