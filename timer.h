#ifndef	_TIMER_H
#define	_TIMER_H

#include	<stdint.h>
//#include "arduino.h"  // For F_CPU on ARM.

// time-related constants
#define	US	* (F_CPU / 1000000)
#define	MS	* (F_CPU / 1000)

/// How often we overflow and update our clock.
/// With F_CPU = 16MHz, max is < 4.096ms (TICK_TIME = 65535).
#define TICK_TIME (2 MS)

/// Convert back to ms from cpu ticks so our system clock runs
/// properly if you change TICK_TIME.
#define TICK_TIME_MS (TICK_TIME / (F_CPU / 1000))

#if defined __AVR__

#include <avr/version.h>

// Provide a memory barrier to the compiler. This informs
// the compiler that is should write any cached values that
// are destined for a global variable and discard any other
// cached values from global variables.
//
// Note that this behavior does apply to all global variables,
// not just volatile ones. However, cached local variables
// are not affected as they are not externally visible.

#define MEMORY_BARRIER() __asm volatile( "" ::: "memory" )

// There is a bug in the CLI/SEI functions in older versions of
// avr-libc - they should be defined to include a memory barrier.
// This macro is used to define the barrier in the code so that
// it will be easy to remove once the bug has become ancient history.
// At the moment the bug is included in most of the distributed
// compilers.

#if __AVR_LIBC_VERSION__ < 10700UL
    #define CLI_SEI_BUG_MEMORY_BARRIER() MEMORY_BARRIER()
#else
    #define CLI_SEI_BUG_MEMORY_BARRIER()
#endif

#define ATOMIC_START { \
                       uint8_t save_reg = SREG; \
                       cli(); \
                       CLI_SEI_BUG_MEMORY_BARRIER();

#define ATOMIC_END   MEMORY_BARRIER(); \
                     SREG = save_reg; \
                   }

#elif defined __ARMEL__

  #define ATOMIC_START cli();
  #define ATOMIC_END sei();
  #define MEMORY_BARRIER()

#elif defined SIMULATOR

  #define CLI_SEI_BUG_MEMORY_BARRIER()
  #define MEMORY_BARRIER()

  #define ATOMIC_START { \
                         uint8_t save_reg = sim_interrupts; \
                         cli();

  #define ATOMIC_END     MEMORY_BARRIER(); \
                         if (save_reg) sei(); \
                       }

#endif /* __AVR__, __ARMEL__, SIMULATOR */

#ifdef __cplusplus
extern "C" {
#endif

void timer_init(void);

void cpu_init();

#ifdef __cplusplus
}
#endif

uint8_t timer_set(int32_t delay, uint8_t check_short);

void timer_reset(void);

void timer_stop(void);

// Should be called every TICK_TIME (currently 2 ms).
//void clock_tick(void);

//void clock(void);

// TIMER
// microsecond delay, does NOT reset WDT if feature enabled
void delay_us(uint16_t delay);

#endif	/* _TIMER_H */
