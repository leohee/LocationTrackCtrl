#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif


#define COUNT_PER_SECOND		(1000)		// 每秒分为1000份

struct lt_info_t {
	char		*ver;		// 当前版本
	char		*built;		// 编译时间
	uint8_t		chipID;
	uint8_t		rawUID[8];	// 芯片唯一编号 6B+2B
	char		UID[16];
	uint8_t		enLog;		// 日志使能
	uint32_t	ticks;		// us
	uint32_t	lifetime;	// s



};

extern struct lt_info_t *gLT;


#ifdef __cplusplus
}
#endif


#endif
