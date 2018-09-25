#include "sensors_control.h"

uint8_t last_direction = 0;

// Init INT0 and INT1 interrupts for optical sensors
void sensing_init()
{
	// externally trigged interrupts setup    
    MCUCR |= (1<<ISC01)|(1<<ISC11)|(1<<ISC10); /* INT0 - falling edge, INT1 - raising edge */
    //MCUCR |= (1<<ISC00)|(1<<ISC10); /* INT0 - any change, INT1 - any change */
    GICR |= (1<<INT0)|(1<<INT1);        /* enable INT0 and INT1 */
    PULL_OFF(PD2); 
    PULL_OFF(ENCODER_PIN_A);
    PULL_OFF(ENCODER_PIN_B);
    SET_INPUT(PD2);
    SET_INPUT(ENCODER_PIN_A);
    SET_INPUT(ENCODER_PIN_B);
}
 
 // needle interrupt
ISR(INT1_vect)
{
	// check if second sensor is trigged to estimate needle direction
	if(direction_encoder())
	{
		// false alarm, last interrupt was about the same direction
		if(last_direction)
			return;
		last_direction = 1;
		//serial_writestr_P(PSTR("int1 up\n"));
		if(mb_tail_dda &&!mb_tail_dda->live)
		{
			mb_tail_dda->waitfor = 0;
			dda_start(mb_tail_dda);
			sei();
		}
	}
	else // needle goes downwards
	{
		if(!last_direction)
			return;
		//serial_writestr_P(PSTR("int1 down\n"));
		last_direction = 0;
		//TODO: stop any movement currently in progress
	}
}

// motor rotary encoder interrupt
ISR(INT0_vect) 
{ 
	
}
 
 
