
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USE_SHELL

uint8_t rxData[16];

void uart1_debug_init (void)
{
	// TX -> PA9
	// RX -> PA8
	GPIOA_SetBits(PIN_DBG_TX);
	GPIOA_ModeCfg(PIN_DBG_TX, GPIO_ModeOut_PP_5mA);
	GPIOA_ModeCfg(PIN_DBG_RX, GPIO_ModeIN_PU);

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

#else
void uart1_debug_init (void)
{

}

#endif


#ifdef __cplusplus
}
#endif


