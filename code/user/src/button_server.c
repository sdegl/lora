#include "button_server.h"
#include "key.h"
#include "stm32_timer.h"

enum state{
    button_up,
    button_down,
    button_shake,
};

static UTIL_TIMER_Object_t timer;

static uint8_t key_status;



void ButtonServer_Handler(void *parm)
{
    switch (key_status) {
    case button_up:
        if (Key_GetInput1() == )
        break;

    case button_down:

        break;

    case button_shake:

        break;

    }
}

/**
 * @brief ∞¥≈•≥ı ºªØ
 * 
 */
void ButtonServer_Init(void)
{
    UTIL_TIMER_Create(&timer, 20, UTIL_TIMER_ONESHOT, ButtonServer_Handler, NULL);
    UTIL_TIMER_Start(&timer);
}



