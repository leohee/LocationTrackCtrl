
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lt_info_t *gLT;

void sysclock_init (void)
{
	DelayMs(2); 
	SetSysClock(CLK_SOURCE_HSE_32MHz);	// 设置主频为外部 32MHz

	while(SysTick_Config(GetSysClock()/COUNT_PER_SECOND)){;}	// tick 1ms

	GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
	GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
}

static struct lt_info_t *lt_malloc (void)
{
	struct lt_info_t *pLT = NULL;

	pLT = (struct lt_info_t *)malloc(sizeof(struct lt_info_t));
	if (NULL == pLT) {
		return NULL;
	}

	memset(pLT, 0, sizeof(struct lt_info_t *));

	return pLT;
}

void lt_init (void)
{
	sysclock_init();
	gpio_init();

	uart1_debug_init();
	uart0_4g_init();

	gLT->ver = "V1.0.0";
	gLT->built = __TIME__", "__DATE__;
	gLT->chipID = R8_CHIP_ID;

	log_info("chip id 0x%02X", gLT->chipID);
	log_info("%s built @ %s.", gLT->ver, gLT->built);

	shell_init();

}

int main ()
{
	int ret = 0;

	gLT = lt_malloc();
	if (NULL == gLT) {
		return -1;
	}

	lt_init();

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

