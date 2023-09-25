
#include "inc_files.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_USE_SYSTICK)
void SysTick_Handler (void)
{
	gLT->ticks++;

	if (gLT->ticks%COUNT_PER_SECOND == 0) {
		gLT->lifetime++;
		set_led(LED_LIGHT_OFF);
	} else if (gLT->ticks%COUNT_PER_SECOND == 500) {
		set_led(LED_LIGHT_ON);
	}

}
#endif

void pwr_mode (uint8_t mode)
{
	set_led(LED_LIGHT_OFF);
	set_4g_power(DISABLE);

	switch (mode) {
	case PWR_IDLE:
		log_info("IDLE mode sleep.");
		DelayMs(1);

		LowPower_Idle();

		log_info("wake up from LowPower_Idle.");
		DelayMs(500);
		break;
	case PWR_HALT_1:
		log_info("HALT mode 1 sleep.");
		DelayMs(1);

		LowPower_Halt_1();

		/*
		使用HSI/5=6.4M睡眠，唤醒时间大概需要 2048Tsys≈330us
		HSE起振一般不超过1.2ms(500us-1200us)，所以切换到外部HSE，需要 1.2ms-330us 
		这个时间可以保证HSE足够稳定，一般用于蓝牙
		DelayUs()函数时基于32M时钟的书写，此时主频为6.4M，所以 DelayUs((1200-330)/5)    
		*/
		if (!(R8_HFCK_PWR_CTRL&RB_CLK_XT32M_PON)) {			// 是否HSE上电
			PWR_UnitModCfg( ENABLE, UNIT_SYS_HSE );			// HSE上电
			DelayUs((1200)/5);
		} else if (!(R16_CLK_SYS_CFG&RB_CLK_OSC32M_XT)) {	// 是否选择 HSI/5 做时钟源
			DelayUs((1200-330)/5);
		} 
		HSECFG_Current(HSE_RCur_100);     // 降为额定电流(低功耗函数中提升了HSE偏置电流)
		DelayUs(5/5);                       // 等待稳定 1-5us
		SetSysClock(CLK_SOURCE_HSE_32MHz);
		log_info("wake up from halt mode 1."); 
		DelayMs(500);
		break;

	case PWR_HALT_2:
    	log_info("HALT mode 2 sleep.");   
		DelayMs(1);

		LowPower_Halt_2();
		/*
		使用HSI/5=6.4M睡眠，唤醒时间大概需要 2048Tsys≈330us
		HSE起振一般不超过1.2ms(500us-1200us)，所以切换到外部HSE，需要 1.2ms-330us 
		这个时间可以保证HSE足够稳定，一般用于蓝牙
		DelayUs()函数时基于32M时钟的书写，此时主频为6.4M，所以 DelayUs((1200-330)/5)    
		*/
		if (!(R8_HFCK_PWR_CTRL&RB_CLK_XT32M_PON)) {			// 是否HSE上电
			PWR_UnitModCfg( ENABLE, UNIT_SYS_HSE );			// HSE上电
			DelayUs((1200)/5);
		} else if (!(R16_CLK_SYS_CFG&RB_CLK_OSC32M_XT)){	// 是否选择 HSI/5 做时钟源
			DelayUs((1200-330)/5);
		}
		HSECFG_Current(HSE_RCur_100);     // 降为额定电流(低功耗函数中提升了HSE偏置电流)
		DelayUs(5/5);                       // 等待稳定 1-5us
		SetSysClock(CLK_SOURCE_HSE_32MHz);
		log_info("wake up from halt mode 2."); 
		DelayMs(500); 
		break;

	case PWR_nSLP_CORE_RAM14K:
	    log_info("sleep mode sleep");   
	    DelayMs(1);
	    LowPower_Sleep( RB_PWR_RAM14K|RB_PWR_RAM2K );       //只保留14+2K SRAM 供电
		/*
		使用HSI/5=6.4M睡眠，唤醒时间大概需要 2048Tsys≈330us
		HSE起振一般不超过1.2ms(500us-1200us)，所以切换到外部HSE，需要 1.2ms-330us 
		这个时间可以保证HSE足够稳定，一般用于蓝牙
		DelayUs()函数时基于32M时钟的书写，此时主频为6.4M，所以 DelayUs((1200-330)/5)    
		*/
	    if(!(R8_HFCK_PWR_CTRL&RB_CLK_XT32M_PON)) {     // 是否HSE上电
	        PWR_UnitModCfg( ENABLE, UNIT_SYS_HSE );   // HSE上电
	        DelayUs((1200)/5);
	    }
	    else if(!(R16_CLK_SYS_CFG&RB_CLK_OSC32M_XT)){   // 是否选择 HSI/5 做时钟源
	        DelayUs((1200-330)/5);
	    } 
	    HSECFG_Current( HSE_RCur_100 );     // 降为额定电流(低功耗函数中提升了HSE偏置电流)
	    DelayUs(5/5);                       // 等待稳定 1-5us
	    SetSysClock( CLK_SOURCE_HSE_32MHz );
	    log_info("wake up form nsleep mode ram14k.");      
	    DelayMs(500);
		break;

	case PWR_STDW_ALL:
	    log_info("shut down mode sleep");   
	    DelayMs(1);
	    LowPower_Shutdown( NULL );                          //全部断电，唤醒后复位
	/* 
	   此模式唤醒后会执行复位，所以下面代码不会运行，
	   注意要确保系统睡下去再唤醒才是唤醒复位，否则有可能变成IDLE等级唤醒 
	*/
	/*
	使用HSI/5=6.4M睡眠，唤醒时间大概需要 2048Tsys≈330us
	HSE起振一般不超过1.2ms(500us-1200us)，所以切换到外部HSE，需要 1.2ms-330us 
	这个时间可以保证HSE足够稳定，一般用于蓝牙
	DelayUs()函数时基于32M时钟的书写，此时主频为6.4M，所以 DelayUs((1200-330)/5)    
	*/
	    if(!(R8_HFCK_PWR_CTRL&RB_CLK_XT32M_PON)) {     // 是否HSE上电
	        PWR_UnitModCfg( ENABLE, UNIT_SYS_HSE );   // HSE上电
	        DelayUs((1200)/5);
	    }
	    else if(!(R16_CLK_SYS_CFG&RB_CLK_OSC32M_XT)){   // 是否选择 HSI/5 做时钟源
	        DelayUs((1200-330)/5);
	    } 
	    HSECFG_Current( HSE_RCur_100 );     // 降为额定电流(低功耗函数中提升了HSE偏置电流)
	    DelayUs(5/5);                       // 等待稳定 1-5us
	    SetSysClock( CLK_SOURCE_HSE_32MHz );
	    log_info("wake up from shutdown.");
	    DelayMs(500);
		break;

	}


	set_4g_power(ENABLE);

}


#ifdef __cplusplus
}
#endif


