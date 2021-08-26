#include "subghz_app.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "radio.h"
#include "radio_driver.h"
#include "timer.h"
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

static void OnCadDone(bool channelActivityDetected);

static RadioEvents_t init;

#define LORA_BANDWIDTH 0 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_SPREADING_FACTOR 12 /* [SF7..SF12] */
#define LORA_CODINGRATE 1 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_PREAMBLE_LENGTH 15 /* Same for Tx and Rx */
#define LORA_SYMBOL_TIMEOUT 5 /* Symbols */
#define LORA_FIX_LENGTH_PAYLOAD false
#define LORA_IQ_INVERSION_ON false
#define TX_OUTPUT_POWER 22 /* dBm */
#define RX_TIMEOUT_VALUE 5000
#define TCXO_WORKAROUND_TIME_MARGIN 50 /* 50ms margin */
#define TX_TIMEOUT_VALUE 3000
#define LORA_CRC_ON true
#define FREQ_HOP_ON false
#define RF_FREQUENCY 868000000 /* Hz */

uint32_t time_on_air;

void SubghzApp_Init(void)
{
    init.RxDone = OnRxDone;
    init.RxError = OnRxError;
    init.RxTimeout = OnRxTimeout;
    init.TxDone = OnTxDone;
    init.TxTimeout = OnTxTimeout;
    init.CadDone = OnCadDone;

    Radio.Init(&init);

    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
        LORA_SPREADING_FACTOR, LORA_CODINGRATE,
        LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD,
        LORA_CRC_ON, FREQ_HOP_ON, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
        LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
        LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD,
        0, LORA_CRC_ON, FREQ_HOP_ON, 0, LORA_IQ_INVERSION_ON, true);
	
    Radio.SetMaxPayloadLength(MODEM_LORA, 128);
	
    Radio.SetChannel(RF_FREQUENCY);
	Radio.Rx(0);
}

void SubghzApp_Process(void)
{

}

static void OnCadDone(bool channelActivityDetected)
{

}

static void OnTxDone(void)
{

}

static void OnRxDone(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr)
{
    payload[size] = 0;

    if (strcmp(payload, "open") == 0) {
        LED_OpenUntil(LED_GET, 0);
    } 
    else if (strcmp(payload, "close") == 0){
        LED_Close(LED_GET);
    }

    Radio.Send((uint8_t*)"ack", sizeof("ack"));
}

static void OnTxTimeout(void)
{

}

static void OnRxTimeout(void)
{

}

static void OnRxError(void)
{

}
