
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif


void sysclock_init (void)
{
	DelayMs(2); 
	SetSysClock(CLK_SOURCE_HSE_32MHz);	// 设置主频为外部 32MHz

	GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
	GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
}



void lt_init (void)
{
	sysclock_init();
	gpio_init();

	uart1_debug_init();
	uart0_4g_init();

	log_info("chip id 0x%02X", R8_CHIP_ID);
	log_info("built @ %s, %s", __TIME__, __DATE__);

	shell_init();

}

int main ()
{
	int ret = 0;

	lt_init();

	pwr_mode(PWR_IDLE);

    while (1) {
        ret = shell_run();
		if (ret < 0) {
			break;
		}

		DelayMs(1);
    }

	// crash run to here

	return 0;
}


#ifdef __cplusplus
}
#endif

