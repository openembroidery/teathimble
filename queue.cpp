#include "queue.h"

/// movebuffer head pointer. Points to the last move in the queue.
/// this variable is used both in and out of interrupts, but is
/// only written outside of interrupts.
uint8_t mb_head = 0;

/// movebuffer tail pointer. Points to the currently executing move
/// this variable is read/written both in and out of interrupts.
uint8_t mb_tail = 0;

/// move buffer.
/// holds move queue
/// contents are read/written both in and out of interrupts, but
/// once writing starts in interrupts on a specific slot, the
/// slot will only be modified in interrupts until the slot is
/// is no longer live.
/// The size does not need to be a power of 2 anymore!
DDA BSS movebuffer[MOVEBUFFER_SIZE]; 

/// check if the queue is completely full
uint8_t queue_full() {
    MEMORY_BARRIER();
  return MB_NEXT(mb_head) == mb_tail;
}

/// check if the queue is completely empty
uint8_t queue_empty() {
  uint8_t result;

  ATOMIC_START
    result = (mb_tail == mb_head && movebuffer[mb_tail].live == 0);
  ATOMIC_END

    return result;
}


DDA *queue_current_movement() {
  DDA* current;

  ATOMIC_START
    current = &movebuffer[mb_tail];

    if ( ! current->live || current->waitfor_temp || current->nullmove)
      current = NULL;
  ATOMIC_END

  return current;
}

/// add a move to the movebuffer
/// \note this function waits for space to be available if necessary, check queue_full() first if waiting is a problem
/// This is the only function that modifies mb_head and it always called from outside an interrupt.
void enqueue_home(TARGET *t, uint8_t endstop_check, uint8_t endstop_stop_cond) {
    // don't call this function when the queue is full, but just in case, wait for a move to complete and free up the space for the passed target
    /*while (queue_full())
        delay_us(100);
    */
    uint8_t h = MB_NEXT(mb_head);;

    DDA* new_movebuffer = &(movebuffer[h]);

    // Initialise queue entry to a known state. This also clears flags like
    // dda->live, dda->done and dda->wait_for_temp.
    new_movebuffer->allflags = 0;

    if (t != NULL) {
            new_movebuffer->endstop_check = endstop_check;
            new_movebuffer->endstop_stop_cond = endstop_stop_cond;
        }
        else {
            // it's a wait for temp
            new_movebuffer->waitfor_temp = 1;
        }
    dda_create(new_movebuffer, t);


    mb_head = h;

  uint8_t isdead;

  ATOMIC_START
    isdead = (movebuffer[mb_tail].live == 0);
  ATOMIC_END

    if (isdead) {
    timer_reset();
        next_move();
    // Compensate for the cli() in timer_set().
        sei();
    }
}

// -------------------------------------------------------
// This is the one function called by the timer interrupt.
// It calls a few other functions, though.
// -------------------------------------------------------
/// Take a step or go to the next move.
void queue_step() {
    // do our next step
    DDA* current_movebuffer = &movebuffer[mb_tail];
    if (current_movebuffer->live) {
        
            dda_step(current_movebuffer);
    }
  // Start the next move if this one is done.
    if (current_movebuffer->live == 0)
        next_move();
}

// called from step timer when current move is complete
void next_move(void)
{ 
    while ((queue_empty() == 0) && (movebuffer[mb_tail].live == 0)) {
        // next item
        uint8_t t = MB_NEXT(mb_tail);
        DDA* current_movebuffer = &movebuffer[t];
        // Tail must be set before calling timer_set(), as timer_set() reenables
        // the timer interrupt, potentially exposing mb_tail to the timer
        // interrupt routine.
        mb_tail = t;
        dda_start(current_movebuffer);
    }
}