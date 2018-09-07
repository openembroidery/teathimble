#ifndef _MATHS_H
#define _MATHS_H

#include "motor.h"

/*! Preprocessor square root.

  (uint32_t)(SQRT(i) + .5)
  equals
  (uint32_t)(sqrt(i) + .5)

  These two provide identical results for all tested numbers across the
  uint32 range. Casting to other sizes is also possible.

  Can principally be used for calculations at runtime, too, but its compiled
  size is prohibitively large (more than 20kB per instance).

  Initial version found on pl.comp.lang.c, posted by Jean-Louis PATANE.
*/
#define SQR00(x) (((x) > 65535) ? (double)65535 : (double)(x) / 2)
#define SQR01(x) ((SQR00(x) + ((x) / SQR00(x))) / 2)
#define SQR02(x) ((SQR01(x) + ((x) / SQR01(x))) / 2)
#define SQR03(x) ((SQR02(x) + ((x) / SQR02(x))) / 2)
#define SQR04(x) ((SQR03(x) + ((x) / SQR03(x))) / 2)
#define SQR05(x) ((SQR04(x) + ((x) / SQR04(x))) / 2)
#define SQR06(x) ((SQR05(x) + ((x) / SQR05(x))) / 2)
#define SQR07(x) ((SQR06(x) + ((x) / SQR06(x))) / 2)
#define SQR08(x) ((SQR07(x) + ((x) / SQR07(x))) / 2)
#define SQR09(x) ((SQR08(x) + ((x) / SQR08(x))) / 2)
#define SQR10(x) ((SQR09(x) + ((x) / SQR09(x))) / 2)
#define SQR11(x) ((SQR10(x) + ((x) / SQR10(x))) / 2)
#define SQR12(x) ((SQR11(x) + ((x) / SQR11(x))) / 2)
// We use 9 iterations, note how SQR10() and up get ignored. You can add more
// iterations here, but beware, the length of the preprocessed term
// explodes, leading to several seconds compile time above about SQR10().
#define SQRT(x) ((SQR09(x) + ((x) / SQR09(x))) / 2)


/*!
  Micrometer distance <=> motor step distance conversions.
*/

#define UM_PER_METER (1000000UL)


extern const axes_uint32_t PROGMEM axis_qn_P;
extern const axes_uint32_t PROGMEM axis_qr_P;

// return rounded result of multiplicand * multiplier / divisor
// this version is with quotient and remainder precalculated elsewhere
int32_t muldivQR(int32_t multiplicand, uint32_t qn, uint32_t rn,
                       uint32_t divisor);

// return rounded result of multiplicand * multiplier / divisor
static int32_t muldiv(int32_t, uint32_t, uint32_t) __attribute__ ((always_inline));
inline int32_t muldiv(int32_t multiplicand, uint32_t multiplier,
                      uint32_t divisor) {
  return muldivQR(multiplicand, multiplier / divisor,
                  multiplier % divisor, divisor);
}


static int32_t um_to_steps(int32_t, uint8_t ) __attribute__ ((always_inline));
inline int32_t um_to_steps(int32_t distance, uint8_t a) {
  return muldivQR(distance, pgm_read_dword(&axis_qn_P[a]),
                  pgm_read_dword(&axis_qr_P[a]), UM_PER_METER);
}

// approximate 2D distance
uint32_t approx_distance(uint32_t dx, uint32_t dy);

// approximate 3D distance
uint32_t approx_distance_3(uint32_t dx, uint32_t dy, uint32_t dz);

// integer inverse square root, 12bits precision
uint16_t int_inv_sqrt(uint16_t a);

// Calculates acceleration ramp length in steps.
uint32_t acc_ramp_len(uint32_t feedrate, uint32_t steps_per_m);

#endif  /* _MATHS_H */