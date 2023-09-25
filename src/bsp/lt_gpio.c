#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init (void)
{
	GPIOB_ModeCfg(GPIO_Pin_18, GPIO_ModeOut_PP_20mA);

	// PA5 light_det
	GPIOA_ModeCfg(GPIO_Pin_2|GPIO_Pin_1, GPIO_ModeIN_PU);
	GPIOA_ITModeCfg(GPIO_Pin_2|GPIO_Pin_1, GPIO_ITMode_RiseEdge);

	NVIC_EnableIRQ(GPIO_IRQn);
	PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE);

}

void GPIO_IRQHandler (void)
{

}

void set_led (uint8_t state)
{
	if (LED_LIGHT_ON == state) {
		GPIOB_ResetBits(GPIO_Pin_18);
	} else {
		GPIOB_SetBits(GPIO_Pin_18);
	}
}

#ifdef __cplusplus
}
#endif




