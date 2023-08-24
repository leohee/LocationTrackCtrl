
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

void debug_uart_init (void)
{
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
	UART1_DefInit();
}


#ifdef __cplusplus
}
#endif


