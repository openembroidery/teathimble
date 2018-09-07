#ifndef _GCODE_PARSER_H
#define _GCODE_PARSER_H

#include "motor.h"

#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_LETTER(c) (c >= 'A' && c <= 'Z')
#define IS_WHITECHAR(c) (c == ' ' || c == '\t')
#define IS_ENDING(c) (c == '\n' || c == '\r')
#define ATOI(c)     (c - '0')

#define STATE_ERROR 1

typedef struct {
    uint8_t name;
    uint32_t value;
    uint8_t exponent;
    uint8_t is_negative;
} GCODE_PARAM;

extern GCODE_PARAM gcode_params[8];
extern TARGET next_target;

//a few state functions prototypes
uint8_t start_parsing_parameter(uint8_t );
uint8_t start_parsing_number(uint8_t);

/// the command being processed
//extern GCODE_COMMAND next_target;

extern const uint32_t powers[];  // defined in msg.c

#ifdef __cplusplus
extern "C" {
#endif
/// accept the next character and process it
uint8_t gcode_parse_char(uint8_t c);

/// setup variables
void parser_init();
#ifdef __cplusplus
}
#endif

// help function, home axis position by hitting endstop
void home_pos_y();

#endif  /* _GCODE_PARSE_H */