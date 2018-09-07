#if defined __AVR__

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || \
    defined (__AVR_ATmega328P__)
    #include    "arduino_168_328p.h"
#endif

#if defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__) || \
    defined (__AVR_ATmega644PA__) || defined (__AVR_ATmega1284__) || \
    defined (__AVR_ATmega1284P__)
    #include    "arduino_644.h"
#endif

#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
    #include    "arduino_1280.h"
#endif

#if defined (__AVR_AT90USB1286__)
  #include "arduino_usb1286.h"
#endif

#if defined (__AVR_AT90USB1287__)
  #include "arduino_usb1287.h"
#endif

#if defined (__AVR_ATmega32U4__) || defined (__AVR_ATmega32__) || \
    defined (__AVR_ATmega16__)
    #include    "arduino_32U4.h"
#endif 

#elif defined __ARMEL__

  #if defined (__ARM_LPC1114__)
    #include "arduino_lpc1114.h"
  #endif

  #if defined (__ARM_STM32F411__)
    #include "arduino_stm32f411.h"
  #endif

#endif
