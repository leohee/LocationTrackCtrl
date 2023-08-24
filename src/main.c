
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif


UINT8 TxBuff[]="This is a tx exam\r\n";
UINT8 RxBuff[100];
UINT8 trigB;

void UART1_IRQHandler(void)
{
    UINT8 i;
    
    switch( UART1_GetITFlag() )
    {
        case UART_II_LINE_STAT:
            UART1_GetLinSTA();
            break;
        
        case UART_II_RECV_RDY:
            for(i=0; i!=trigB; i++)
            {
                RxBuff[i] = UART1_RecvByte();
                UART1_SendByte(RxBuff[i]);
            }
            break;
        
        case UART_II_RECV_TOUT:
            i = UART1_RecvString(RxBuff);
            UART1_SendString( RxBuff, i ); 
            break;
        
        case UART_II_THR_EMPTY:
            break;
        
        case UART_II_MODEM_CHG:
            break;
        
        default:
            break;
    }
}

void sysclock_init (void)
{
	DelayMs(2); 
	SetSysClock( CLK_SOURCE_HSE_32MHz );	// 设置主频为外部 32MHz
}



void lt_init (void)
{
	sysclock_init();
	debug_uart_init();

	log_info("chip id %02X", R8_CHIP_ID);

	shell_init();

}

int main ()
{
	lt_init();


	  unsigned char len = 0;
		int i = 0;

	  UART1_SendString( TxBuff, sizeof(TxBuff) );
	
    while(1) {
        len = UART1_RecvString(RxBuff);
        if( len ) {
            UART1_SendString( RxBuff, len );            
        }

				if (++i >= 10000) {
					i = 0;
					break;
				}
    }
	
		return 0;
}


#ifdef __cplusplus
}
#endif

