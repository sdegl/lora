#include "button_server.h"
#include "key.h"
#include "stm32_timer.h"



static UTIL_TIMER_Object_t timer;

static uint8_t key_status;


void ButtonServer_Handler(void *parm)
{
    switch (key_status) {
    case button_up:
        if (Key_GetInput() == 0)
            key_status = button_shake;
        break;

    case button_down:
        if (Key_GetInput() == 1)
            key_status = button_up;
        break;

    case button_shake:
        if (Key_GetInput() == 0)
            key_status = button_down;
        else
            key_status = button_up;

        break;

    }
}

/**
 * @brief ∞¥≈•≥ı ºªØ
 */
void ButtonServer_Init(void)
{
    UTIL_TIMER_Create(&timer, 20, UTIL_TIMER_ONESHOT, ButtonServer_Handler, NULL);
    UTIL_TIMER_Start(&timer);
}


enum state GetButtonStatus(void)
{
    return key_status;
}
