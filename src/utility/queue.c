#include "inc_files.h"

#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USE_SHELL

/* inits the contents of the queue to zeros */
uint8_t queue_init (struct queue_t *queue)
{
	queue->Front = queue->Rear = 0;

    memset(queue->PBase, 0, QUEUE_LENGTH);

    return true;
}

/* checks if the queue is full */
uint8_t queue_full (struct queue_t *queue)
{
    if((((queue->Rear) + 1) % QUEUE_LENGTH) == queue->Front) {
        return true;
    } else {
        return false;
    }
}

/* checks if the queue is empty */
uint8_t queue_empty (struct queue_t *queue)
{
    if(queue->Front == queue->Rear) {
        return true;
    } else {
        return false;
    }
}

/* inserts a byte in the queue */
uint8_t queue_in (struct queue_t *queue, uint8_t *PData)
{

    if(queue_full(queue)) {
        return false;
    }

    queue->PBase[queue->Rear] = *PData;
    queue->Rear = ((queue->Rear) + 1) % QUEUE_LENGTH;

    return true;
}

/* output bytes from queue */
uint8_t queue_out (struct queue_t *queue, uint8_t *PData)
{
    if(queue_empty(queue)) {
        return false;
    }

    *PData = queue->PBase[queue->Front];
    queue->Front = ((queue->Front) + 1) % QUEUE_LENGTH;

    return true;
}

#endif


#ifdef __cplusplus
}
#endif
