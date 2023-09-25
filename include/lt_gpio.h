#ifndef __LT_GPIO_H__
#define __LT_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_LIGHT_DET		GPIO_Pin_5		// i PA5
#define PIN_KEY_PWR			GPIO_Pin_8		// i PB8

#define PIN_DBG_RX			GPIO_Pin_8		// i PA8
#define PIN_DBG_TX			GPIO_Pin_9		// o PA9

#define PIN_U0_RX			GPIO_Pin_4		// i PB4
#define PIN_U0_TX			GPIO_Pin_7		// o PB7

#define PIN_LED_SHOW		GPIO_Pin_18		// o PB18
#define PIN_4G_CTRL			GPIO_Pin_9		// o PB9
#define PIN_4G_RST			GPIO_Pin_12		// o PA12
#define PIN_4G_PWR			GPIO_Pin_7		// o PA7

enum {
	LED_LIGHT_OFF		= (0),
	LED_LIGHT_ON		= (1)
};

enum {
	GPIO_TYPE_IN		= (1),
	GPIO_TYPE_HIGH		= (2),
	GPIO_TYPE_LOW		= (3)
};

extern void gpio_init (void);

extern void set_led (uint8_t state);

extern void set_4g_power (uint8_t state);


#ifdef __cplusplus
}
#endif


#endif
