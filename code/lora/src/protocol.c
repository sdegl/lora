#include "protocol.h"


// void lora_send_data_handler(const uint8_t *in_buf, uint8_t *out_buf, )

/**
 * @brief lora应用层数据
 * 
 * @param data 数据源地址
 * @param lenght 数据长度
 */
__WEAK void lora_app_data_handler(const uint8_t *data, uint16_t lenght) { }
