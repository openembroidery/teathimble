#include	"pinio.h"

static char ps_is_on = 0;

/// step/psu timeout
volatile uint8_t	psu_timeout = 0;

/** Initialise all I/O.

  This sets pins as input or output, appropriate for their usage.
*/
void pinio_init(void) {
  /// X Stepper.
  SET_OUTPUT(X_STEP_PIN); WRITE(X_STEP_PIN, 0);
  SET_OUTPUT(X_DIR_PIN); WRITE(X_DIR_PIN, 0);
  #ifdef X_MIN_PIN
    SET_INPUT(X_MIN_PIN);
    PULL_OFF(X_MIN_PIN);
  #endif
  #ifdef X_MAX_PIN
    SET_INPUT(X_MAX_PIN);
    PULL_OFF(X_MAX_PIN);
  #endif

  /// Y Stepper.
  SET_OUTPUT(Y_STEP_PIN); WRITE(Y_STEP_PIN, 0);
  SET_OUTPUT(Y_DIR_PIN); WRITE(Y_DIR_PIN, 0);
  #ifdef Y_MIN_PIN
    SET_INPUT(Y_MIN_PIN);
    PULL_OFF(Y_MIN_PIN);
  #endif
  #ifdef Y_MAX_PIN
    SET_INPUT(Y_MAX_PIN);
    PULL_OFF(Y_MAX_PIN);
  #endif

  /// Z Stepper.
  #if defined Z_STEP_PIN && defined Z_DIR_PIN
    SET_OUTPUT(Z_STEP_PIN); WRITE(Z_STEP_PIN, 0);
    SET_OUTPUT(Z_DIR_PIN); WRITE(Z_DIR_PIN, 0);
  #endif
  #ifdef Z_MIN_PIN
    SET_INPUT(Z_MIN_PIN);
    PULL_OFF(Z_MIN_PIN);
  #endif
  #ifdef Z_MAX_PIN
    SET_INPUT(Z_MAX_PIN);
    PULL_OFF(Z_MAX_PIN);
  #endif

  #if defined E_STEP_PIN && defined E_DIR_PIN
    SET_OUTPUT(E_STEP_PIN); WRITE(E_STEP_PIN, 0);
    SET_OUTPUT(E_DIR_PIN); WRITE(E_DIR_PIN, 0);
  #endif

  /// Common Stepper Enable.
  #ifdef STEPPER_ENABLE_PIN
    SET_OUTPUT(STEPPER_ENABLE_PIN);
    #ifdef STEPPER_INVERT_ENABLE
      WRITE(STEPPER_ENABLE_PIN, 0);
    #else
      WRITE(STEPPER_ENABLE_PIN, 1);
    #endif
  #endif

  /// X Stepper Enable.
  #ifdef X_ENABLE_PIN
    SET_OUTPUT(X_ENABLE_PIN);
    #ifdef X_INVERT_ENABLE
      WRITE(X_ENABLE_PIN, 0);
    #else
      WRITE(X_ENABLE_PIN, 1);
    #endif
  #endif

  /// Y Stepper Enable.
  #ifdef Y_ENABLE_PIN
    SET_OUTPUT(Y_ENABLE_PIN);
    #ifdef Y_INVERT_ENABLE
      WRITE(Y_ENABLE_PIN, 0);
    #else
      WRITE(Y_ENABLE_PIN, 1);
    #endif
  #endif

  /// Z Stepper Enable.
  #ifdef Z_ENABLE_PIN
    SET_OUTPUT(Z_ENABLE_PIN);
    #ifdef Z_INVERT_ENABLE
      WRITE(Z_ENABLE_PIN, 0);
    #else
      WRITE(Z_ENABLE_PIN, 1);
    #endif
  #endif

  /// E Stepper Enable.
  #ifdef E_ENABLE_PIN
    SET_OUTPUT(E_ENABLE_PIN);
    #ifdef E_INVERT_ENABLE
      WRITE(E_ENABLE_PIN, 0);
    #else
      WRITE(E_ENABLE_PIN, 1);
    #endif
  #endif

  #ifdef  STEPPER_ENABLE_PIN
    power_off();
  #endif

  #ifdef DEBUG_LED_PIN
    SET_OUTPUT(DEBUG_LED_PIN);
    WRITE(DEBUG_LED_PIN, 0);
  #endif
}

