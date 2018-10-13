#include <avr/interrupt.h>
#include "queue.h"
#include "pinio.h"
#include "serial.h"
#include "config.h"

#ifdef INVERT_DIRECTION_ENCODER
	#define direction_encoder()						(READ(ENCODER_PIN_B)?1:0)
#else
	#define direction_encoder()						(READ(ENCODER_PIN_B)?0:1)
#endif

extern int16_t desired_speed;
void stop_dc_motor();

#ifdef __cplusplus
extern "C" {
#endif
void sensing_init();
void update_dc_motor();
 #ifdef __cplusplus
}
#endif
