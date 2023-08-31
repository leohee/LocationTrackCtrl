
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t rxData[128];

void uart1_debug_init (void)
{
	// TX -> PA9
	// RX -> PA8
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);

	UART1_DefInit();

	UART1_ByteTrigCfg( UART_1BYTE_TRIG );
    UART1_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
    NVIC_EnableIRQ( UART1_IRQn );
}

void UART1_IRQHandler (void)
{
	switch (UART1_GetITFlag()) {
	case UART_II_LINE_STAT:
		UART1_GetLinSTA();
		break;

	case UART_II_RECV_RDY:
		rxData[0] = UART1_RecvByte();
		queue_in(&rxShell, &rxData[0]);
	    break;

	case UART_II_RECV_TOUT:
		UART1_RecvString(rxData);
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


