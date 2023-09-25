#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USE_SHELL

#ifndef QUEUE_LENGTH
  #define QUEUE_LENGTH 32
#endif

struct queue_t {
	size_t		Front;
	size_t 		Rear;
	uint8_t		PBase[QUEUE_LENGTH];
};

extern uint8_t queue_init (struct queue_t *queue);
extern uint8_t queue_full (struct queue_t *queue);
extern uint8_t queue_empty (struct queue_t *queue);
extern uint8_t queue_in (struct queue_t *queue, uint8_t *PData);
extern uint8_t queue_out (struct queue_t *queue, uint8_t *PData);

#endif

#ifdef __cplusplus
}
#endif

#endif
