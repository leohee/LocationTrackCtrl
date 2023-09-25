#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init (void)
{
	GPIOB_ModeCfg(PIN_LED_SHOW, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(PIN_4G_CTRL, GPIO_ModeOut_PP_20mA);

	// PA5 light_det
	GPIOA_ModeCfg(PIN_LIGHT_DET, GPIO_ModeIN_PU);
	GPIOA_ITModeCfg(PIN_LIGHT_DET, GPIO_ITMode_RiseEdge);

	NVIC_EnableIRQ(GPIO_IRQn);
	PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE);

}

void GPIO_IRQHandler (void)
{

}

void set_led (uint8_t state)
{
	if (LED_LIGHT_ON == state) {
		GPIOB_ResetBits(PIN_LED_SHOW);
	} else {
		GPIOB_SetBits(PIN_LED_SHOW);
	}
}

void set_4g_power (uint8_t state)
{
	if (ENABLE == state) {
		GPIOB_ResetBits(PIN_4G_CTRL);
	} else {
		GPIOB_SetBits(PIN_4G_CTRL);
	}
}

#ifdef __cplusplus
}
#endif




