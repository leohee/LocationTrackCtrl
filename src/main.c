
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lt_info_t *gLT;

void sysclock_init (void)
{
	DelayMs(2); 
	SetSysClock(CLK_SOURCE_HSE_32MHz);	// 设置主频为外部 32MHz

#if defined(CONFIG_USE_SYSTICK)
	while(SysTick_Config(GetSysClock()/COUNT_PER_SECOND)){;}	// tick 1ms
#endif

	GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
	GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
}

// 0x2C ...  -> "2C..."
void u8v_to_hexstr (uint8_t *data, uint16_t len, char *buf)
{
	int i = 0;

	for(i = 0; i < len; i++) {
		buf[2*i] = Hex2Ascii[(data[i]>>4)&0x0F];
		buf[2*i+1] = Hex2Ascii[data[i]&0x0F];
	}
}

void lt_get_uid (void)
{
	GetUniqueID(gLT->rawUID);
	u8v_to_hexstr(gLT->rawUID, 6, gLT->UID);
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

	flash_init();

	lt_get_uid();

	log_info("Chip id : 0x%02X : %s", gLT->chipID, gLT->UID);
	log_info("%s built @ %s.", gLT->ver, gLT->built);

	shell_init();

}

int main ()
{
	int ret = 0, cnt = 0;

	gLT = lt_malloc();
	if (NULL == gLT) {
		return -1;
	}

	gLT->enLog = true;

	lt_init();

    while (1) {
        ret = shell_run();
		if (ret < 0) {
			break;
		}

		DelayMs(1);

#if !defined(CONFIG_USE_SYSTICK)
	gLT->ticks++;
	if (++cnt == 1) {
		GPIOB_SetBits(GPIO_Pin_18);
	} else if (cnt == 499) {
		GPIOB_ResetBits(GPIO_Pin_18);
	} else if (cnt == 998) {
		cnt = 0;
		gLT->lifetime++;
	}
#endif

    }

	// crash run to here

	return 0;
}


#ifdef __cplusplus
}
#endif

