#ifndef	_SERIAL_H
#define	_SERIAL_H

#include "config.h"
#include	<stdint.h>

/**
  Before we had display support, all messages went to the serial link,
  so this destination was hardcoded. This macro avoids changing a whole lot
  of older code.

  Deprecated macro? Convenience macro? Dunno.
*/
#define sersendf_P(...) sendf_P(serial_writechar, __VA_ARGS__)


void sendf_P(void (*writechar)(uint8_t), PGM_P format_P, ...);

#ifdef USB_SERIAL
  #include "usb_serial.h"
  #define serial_init() usb_init()
  #define serial_rxchars() usb_serial_available()
  #define serial_popchar() usb_serial_getchar()
#else
  // initialise serial subsystem
  void serial_init(void);

  // return number of characters in the receive buffer,
  // and number of spaces in the send buffer
  uint8_t serial_rxchars(void);
  // uint8_t serial_txchars(void);

  // read one character
  uint8_t serial_popchar(void);
  // send one character
  void serial_writechar(uint8_t data);
#endif /* USB_SERIAL */

void serial_writestr(uint8_t *data);

// write from flash
void serial_writestr_P(PGM_P data_P);

#endif	/* _SERIAL_H */
