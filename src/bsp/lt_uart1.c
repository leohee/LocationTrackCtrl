
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t rxData[128];

void debug_uart_init (void)
{
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
	UART1_DefInit();
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

		//UART1_SendByte(rxData[0]);
	    break;

	case UART_II_RECV_TOUT:
		UART1_RecvString(rxData);
		//UART1_SendString(rxData, i); 
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


