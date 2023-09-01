#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define COUNT_PER_SECOND		(1000)		// 每秒分为1000份

struct lt_info_t {
	char		*ver;
	char		*built;
	uint8_t		chipID;
	uint32_t	ticks;
	uint32_t	lifetime;
};

extern struct lt_info_t *gLT;


#ifdef __cplusplus
}
#endif


#endif
