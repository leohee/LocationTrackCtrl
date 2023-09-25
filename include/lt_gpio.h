#ifndef __LT_GPIO_H__
#define __LT_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

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


#ifdef __cplusplus
}
#endif


#endif
