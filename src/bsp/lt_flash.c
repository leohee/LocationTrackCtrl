#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

int flash_write (uint32_t offset, uint32_t value)
{
	uint32_t *p32;

	p32 = (uint32_t *)(DATA_FLASH_ADDR + offset);

	FlashBlockErase((uint32_t)p32);

	if (FlashWriteDW((uint32_t)p32, value) != SUCCESS) {
		printf("write flash offset 0x%08X %d failed.\n\r", offset, value);
		return -1;
	}

	return 0;
}

int flash_init (void)
{
	uint32_t *p32;

	p32 = (uint32_t *)(DATA_FLASH_ADDR + OFFSET_ENABLE_LOG);

	gLT->enLog = *p32;

	printf("0x%08lX = 0x%08lX\n\r", (uint32_t)p32, *p32);


	return 0;
}


#ifdef __cplusplus
}
#endif


