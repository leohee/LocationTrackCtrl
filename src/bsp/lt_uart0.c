
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif


void uart0_4g_init (void)
{
	// TX -> PB7
	// RX -> PB4
	GPIOB_SetBits(PIN_U0_TX);
	GPIOB_ModeCfg(PIN_U0_TX, GPIO_ModeOut_PP_5mA);
	GPIOB_ModeCfg(PIN_U0_RX, GPIO_ModeIN_PU);

	UART0_DefInit();
}

void UART0_IRQHandler (void)
{
	switch (UART0_GetITFlag()) {
	case UART_II_LINE_STAT:
		break;

	case UART_II_RECV_RDY:
	    break;

	case UART_II_RECV_TOUT:
		break;

	case UART_II_THR_EMPTY:
		break;

	case UART_II_MODEM_CHG:
		break;

	default:
		break;
	}
}

#ifdef __cplusplus
}
#endif


