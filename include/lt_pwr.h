#ifndef __LT_PWR_H__
#define __LT_PWR_H__

#ifdef __cplusplus
extern "C" {
#endif

enum {
	PWR_NORMAL						= (0x00),		// 正常工作
	PWR_IDLE						= (0x01),		// 1.15~1.5mA 空闲模式
	PWR_HALT_1						= (0x02),		// 420~470uA 暂停模式1
	PWR_HALT_2						= (0x03),		// 420~470uA 暂停模式2
	PWR_nSLP_CORE_RAM2K				= (0x10),		// 0.6~0.8uA 睡眠保留核和RAM2K
	PWR_nSLP_CORE_RAM2K_CK32K		= (0x11),		// 0.9~1.2uA 睡眠保留核，RAM2K和RTC唤醒
	PWR_nSLP_CORE_RAM14K			= (0x12),		// 1.1~1.5uA 睡眠保留核和RAM14K
	PWR_nSLP_CORE_RAM14K_CK32K		= (0x13),		// 1.4~1.9uA 睡眠保留核，RAM14K和RTC唤醒
	PWR_SLP_SYS						= (0x14),		// 1.8~2.6uA 仅睡眠系统电源
	PWR_STDW_ALL					= (0x20),		// 0.2uA 下电所有外设
	PWR_nSTDW_RAM2K					= (0x21),		// 0.5~0.7uA 下电保留RAM2K外设
	PWR_nSTDW_CK32K					= (0x22),		// 0.5~0.6uA 下电保留RTC外设
	PWR_nSTDW_RAM2K_CK32K			= (0x23),		// 0.8~1.1uA 下电保留RAM2K和RTC外设
};


extern void pwr_mode (uint8_t mode);


#ifdef __cplusplus
}
#endif


#endif
