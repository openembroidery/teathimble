#ifndef _GCODE_PARSER_H
#define _GCODE_PARSER_H

#include "motor.h"

typedef struct {
    uint8_t name;
    uint32_t value;
    uint8_t exponent;
    uint8_t is_negative;
} GCODE_PARAM;

/// the command being processed
//extern GCODE_COMMAND next_target;

extern const uint32_t powers[];  // defined in msg.c

/// accept the next character and process it
uint8_t gcode_parse_char(uint8_t c);

void parser_init();

#endif  /* _GCODE_PARSE_H */