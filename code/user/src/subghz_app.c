#include "subghz_app.h"
#include "button_server.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "radio.h"
#include "radio_driver.h"
#include "relay.h"
#include "timer.h"

#define LORA_BANDWIDTH 0 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_SPREADING_FACTOR 11 /* [SF7..SF12] */
#define LORA_CODINGRATE 1 /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_PREAMBLE_LENGTH 5 /* Same for Tx and Rx */
#define LORA_SYMBOL_TIMEOUT 5 /* Symbols */
#define LORA_FIX_LENGTH_PAYLOAD false
#define LORA_IQ_INVERSION_ON false
#define TX_OUTPUT_POWER 22 /* dBm */
#define RX_TIMEOUT_VALUE 5000
#define TCXO_WORKAROUND_TIME_MARGIN 50 /* 50ms margin */
#define TX_TIMEOUT_VALUE 3000
#define LORA_CRC_ON true
#define FREQ_HOP_ON false
#define RF_FREQUENCY 433000000 /* Hz */

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

#define LOST_LORA_TIME 60000
static UTIL_TIMER_Object_t lora_lost_timer;
static uint8_t lora_lost_state;

static uint8_t get_state;

#define HEART_BEAT_TIME 20000
static UTIL_TIMER_Object_t hear_beat_timer;
static uint8_t buf[128] = "close";
static uint8_t buf_len = sizeof("close");

static UTIL_TIMER_Object_t send_ack_timer;
static uint8_t is_send;
static uint8_t need_send;

static uint8_t resend;

void LoraLost(void * parm)
{
    lora_lost_state = true;
    get_state = false;
    UTIL_TIMER_StartWithPeriod(&hear_beat_timer, HEART_BEAT_TIME);
}

void LoraSend(void * parm)
{
    if (is_send == false) {
        is_send = true;
        Radio.Send(buf, buf_len);
        if (lora_lost_state == false && resend < 10) {
            resend++;
            UTIL_TIMER_StartWithPeriod(&hear_beat_timer, 3000);
        } else {
            UTIL_TIMER_StartWithPeriod(&hear_beat_timer, HEART_BEAT_TIME);
        }
    }
}

void send_ack(void * parm)
{
    if (is_send == false) {
        is_send = true;
        Radio.Send("ack", strlen("ack"));
    }
}


void SubghzApp_Init(void)
{
    uint32_t time;

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
	
    time = Radio.TimeOnAir(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, LORA_CODINGRATE, 
                           LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD, sizeof("close"), LORA_CRC_ON);

    Radio.SetMaxPayloadLength(MODEM_LORA, 128);
	
    Radio.SetChannel(RF_FREQUENCY);
	Radio.Rx(0);

    UTIL_TIMER_Create(&send_ack_timer, 0, UTIL_TIMER_ONESHOT, send_ack, NULL);

    UTIL_TIMER_Create(&lora_lost_timer, LOST_LORA_TIME, UTIL_TIMER_PERIODIC, LoraLost, NULL);
    UTIL_TIMER_Start(&lora_lost_timer);

    UTIL_TIMER_Create(&hear_beat_timer, HEART_BEAT_TIME, UTIL_TIMER_PERIODIC, (void (*)(void*))LoraSend, NULL);
    UTIL_TIMER_Start(&hear_beat_timer);

    LoraSend(NULL);
}

void SubghzApp_Process(void)
{
    static enum state last_button_state;

    /* 掉电 */
    if (Key_GetPower() == 0) {
        LED_OpenUntil(LED_LOST_POWER, 0);
        Relay_Close(RELAY_LOST_POWER);

        LED_Close(LED_LORA_MISS);
        Relay_Open(RELAY_LORA_MISS);
    } else {
        /* 上电 */
        LED_Close(LED_LOST_POWER);
        Relay_Open(RELAY_LOST_POWER);
        
        /* LORA信号丢失 */
        if (lora_lost_state == true) {
            LED_OpenUntil(LED_LORA_MISS, 0);
            Relay_Close(RELAY_LORA_MISS);
        } else {
            LED_Close(LED_LORA_MISS);
            Relay_Open(RELAY_LORA_MISS);
        }
    }

    /* 收料动作关 */
    if (get_state == true) {
        LED_OpenUntil(LED_GET, 0);
        Relay_Close(RELAY_GET);
    } else {
        LED_Close(LED_GET);
        Relay_Open(RELAY_GET);
    }

    switch (GetButtonStatus()) {
    case button_up:
        if (last_button_state != button_up) {
            last_button_state = button_up;

            memcpy(buf, "close", sizeof("close"));
            buf_len = sizeof("close");
            need_send = true;
        }
        break;

    case button_down:
        if (last_button_state != button_down) {
            last_button_state = button_down;

            memcpy(buf, "open", sizeof("open"));
            buf_len = sizeof("open");
            need_send = true;
        }
        break;

    case button_shake:
        break;
    }

    if (need_send == true && is_send == false) {
        need_send = false;
        LoraSend(NULL);
    }
}

static void OnCadDone(bool channelActivityDetected)
{

}

static void OnTxDone(void)
{
    Radio.Rx(0);
	is_send = false;
}

static void OnRxDone(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr)
{
    payload[size] = 0;

    if (strcmp((char*)payload, "open") == 0) {
        get_state = true;
        UTIL_TIMER_StartWithPeriod(&send_ack_timer, 5); /* 应答 */
    } else if (strcmp((char*)payload, "close") == 0) {
        get_state = false;
        UTIL_TIMER_StartWithPeriod(&send_ack_timer, 5); /* 应答 */
    } else if(strcmp((char*)payload, "ack") == 0) {
        
    } else {
        return;
    }
    resend = 0;
    UTIL_TIMER_StartWithPeriod(&hear_beat_timer, HEART_BEAT_TIME);
    UTIL_TIMER_StartWithPeriod(&lora_lost_timer, LOST_LORA_TIME);
    lora_lost_state = false;
}

static void OnTxTimeout(void)
{
	Radio.Rx(0);
}

static void OnRxTimeout(void)
{
	Radio.Rx(0);
}

static void OnRxError(void)
{
	Radio.Rx(0);
}
