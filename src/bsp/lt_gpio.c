#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init (void)
{
	// PA5 light_det
	GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PU);
	GPIOA_ITModeCfg(GPIO_Pin_5, GPIO_ITMode_FallEdge);        // 下降沿唤醒

	GPIOB_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
	GPIOB_ITModeCfg(GPIO_Pin_8, GPIO_ITMode_FallEdge);        // 下降沿唤醒

	NVIC_EnableIRQ(GPIO_IRQn);
	PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE);

}

void GPIO_IRQHandler (void)
{

}



#ifdef __cplusplus
}
#endif




