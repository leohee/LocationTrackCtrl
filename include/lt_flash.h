#ifndef __LT_FLASH_H__
#define __LT_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#define OFFSET_ENABLE_LOG		0x00



extern int flash_write (uint32_t addr, uint32_t value);

extern int flash_init (void);

#ifdef __cplusplus
}
#endif


#endif
