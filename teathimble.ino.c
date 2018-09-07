#include "config.h"

#ifdef SIMINFO
  #include "simulavr_info.h"
    #define STR(x)  #x
    #define STRINGIZE(x) STR(x)
  SIMINFO_DEVICE(STRINGIZE(MCU));
  SIMINFO_CPUFREQUENCY(F_CPU);
  #ifdef BAUD
    SIMINFO_SERIAL_IN("D0", "-", BAUD);
    SIMINFO_SERIAL_OUT("D1", "-", BAUD);
  #endif
#endif

#ifndef ARDUINO     
#include "queue.h"
#include "timer.h"
#include "serial.h"
#include "pinio.h"
#include "gcode_parser.h"
#include "queue.h"
uint8_t c, line_done, ack_waiting = 0;

void setup(){
  cpu_init();
  serial_init();
  pinio_init();
  timer_init();
  dda_init();
  parser_init();
  sei();
  serial_writestr_P(PSTR("start\nok\n"));
  
  //enqueue_home(&t, 0, 0);
  }

void loop()
{
    if (queue_full() == 0) {
        if (ack_waiting) {
            serial_writestr_P(PSTR("ok\n"));
            ack_waiting = 0;
        }

        if (//( ! gcode_active || gcode_active & GCODE_SOURCE_SERIAL) &&
            serial_rxchars() != 0) {
            //gcode_active = GCODE_SOURCE_SERIAL;
            c = serial_popchar();
            line_done = gcode_parse_char(c);
            if (line_done) {
                //gcode_active = 0;
                if(line_done > 1)
                    serial_writestr_P(PSTR("err\n"));
                else
                    ack_waiting = 1;
            }
        }
    }
} 
#endif