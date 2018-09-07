#ifndef _QUEUE_H
#define _QUEUE_H

#include    "motor.h"
#include    "timer.h"

extern uint_fast8_t mb_tail;
extern DDA movebuffer[MOVEBUFFER_SIZE];
extern DDA *mb_tail_dda;

#ifdef __cplusplus
extern "C" {
#endif
// queue status methods
uint8_t queue_full(void);
uint8_t queue_empty(void);
void queue_flush(void);
#ifdef __cplusplus
}
#endif

DDA *queue_current_movement(void);

/// Find the next DDA index after 'x', where 0 <= x < MOVEBUFFER_SIZE
#define MB_NEXT(x) ((x) < MOVEBUFFER_SIZE - 1 ? (x) + 1 : 0)

// add a new target to the queue
// t == NULL means add a wait for target temp to the queue
void enqueue_home(const TARGET *t, uint8_t endstop_check, uint8_t endstop_stop_cond);

// called from step timer when current move is complete
void next_move(void);

// take one step
void queue_step(void);

static void enqueue(const TARGET *) __attribute__ ((always_inline));
inline void enqueue(const TARGET *t) {
  enqueue_home(t, 0, 0);
}


#endif  /* _QUEUE_H */