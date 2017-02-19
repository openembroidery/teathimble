#include "serial.h"
#include <avr/interrupt.h>
#include "timer.h"
#include "pinio.h"

#include    <stdarg.h>

#include "msg.h"

/** \def BUFSIZE

  Size of TX and RX buffers. MUST be a \f$2^n\f$ value.

  Unlike ARM MCUs, which come with a hardware buffer, AVRs require a read and
  transmit buffer implemented in software. This buffer not only raises
  reliability, it also allows transmitting characters from interrupt context.
*/
#define BUFSIZE     64

/** \def ASCII_XOFF

  ASCII XOFF character.
*/
#define ASCII_XOFF  19

/** \def ASCII_XON

  ASCII XON character.
*/
#define ASCII_XON   17

#ifndef USB_SERIAL

#if __SIZEOF_INT__ == 2
  #define GET_ARG(T) (va_arg(args, T))
#elif __SIZEOF_INT__ >= 4
  #define GET_ARG(T) ((T)va_arg(args, int))
#endif

/** RX buffer.

  rxhead is the head pointer and points to the next available space.

  rxtail is the tail pointer and points to last character in the buffer.
*/
volatile uint8_t rxhead = 0;
volatile uint8_t rxtail = 0;
volatile uint8_t rxbuf[BUFSIZE];

/** TX buffer.

  Same mechanism as RX buffer.
*/
volatile uint8_t txhead = 0;
volatile uint8_t txtail = 0;
volatile uint8_t txbuf[BUFSIZE];

#include "ringbuffer.h"

#ifdef XONXOFF
#define FLOWFLAG_STATE_XOFF 0
#define FLOWFLAG_SEND_XON   1
#define FLOWFLAG_SEND_XOFF  2
#define FLOWFLAG_STATE_XON  4
// initially, send an XON
volatile uint8_t flowflags = FLOWFLAG_SEND_XON;
#endif


/** Initialise serial subsystem.

  Set up baud generator and interrupts, clear buffers.
*/
void serial_init() {

  #if BAUD > 38401
    UCSR0A = MASK(U2X0);
    UBRR0 = (((F_CPU / 8) / BAUD) - 0.5);
  #else
    UCSR0A = 0;
    UBRR0 = (((F_CPU / 16) / BAUD) - 0.5);
  #endif

  UCSR0B = MASK(RXEN0) | MASK(TXEN0);
  UCSR0C = MASK(URSEL) | MASK(UCSZ01) | MASK(UCSZ00) |(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2);

  UCSR0B |= MASK(RXCIE0) | MASK(UDRIE0);
}

/** Receive interrupt.

  We have received a character, stuff it in the RX buffer if we can, or drop
  it if we can't. Using the pragma inside the function is incompatible with
  Arduinos' gcc.
*/
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#ifdef USART_RXC_vect
ISR(USART_RXC_vect)
#else
ISR(USART0_RX_vect)
#endif
{
  if (buf_canwrite(rx))
    buf_push(rx, UDR0);
  else {
    // Not reading the character makes the interrupt logic to swamp us with
    // retries, so better read it and throw it away.
    //#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    uint8_t trash;
    //#pragma GCC diagnostic pop

    trash = UDR0;
  }

  #ifdef XONXOFF
    if (flowflags & FLOWFLAG_STATE_XON && buf_canwrite(rx) <= 16) {
      // The buffer has only 16 free characters left, so send an XOFF.
      // More characters might come in until the XOFF takes effect.
      flowflags = FLOWFLAG_SEND_XOFF | FLOWFLAG_STATE_XON;
      // Enable TX interrupt so we can send this character.
      UCSR0B |= MASK(UDRIE0);
    }
  #endif
}
#pragma GCC diagnostic pop

/** Transmit buffer ready interrupt.

  Provide the next character to transmit if we can, otherwise disable this
  interrupt.
*/
#ifdef USART_UDRE_vect
ISR(USART_UDRE_vect)
#else
ISR(USART0_UDRE_vect)
#endif
{
  #ifdef XONXOFF
    if (flowflags & FLOWFLAG_SEND_XON) {
      UDR0 = ASCII_XON;
      flowflags = FLOWFLAG_STATE_XON;
    }
    else if (flowflags & FLOWFLAG_SEND_XOFF) {
      UDR0 = ASCII_XOFF;
      flowflags = FLOWFLAG_STATE_XOFF;
    }
    else
  #endif

  if (buf_canread(tx))
    buf_pop(tx, UDR0);
  else
    UCSR0B &= ~MASK(UDRIE0);
}

/** Check how many characters can be read.
*/
uint8_t serial_rxchars() {
  return buf_canread(rx);
}

/** Read one character.
*/
uint8_t serial_popchar() {
  uint8_t c = 0;

  // It's imperative that we check, because if the buffer is empty and we
  // pop, we'll go through the whole buffer again.
  if (buf_canread(rx))
    buf_pop(rx, c);

  #ifdef XONXOFF
    if ((flowflags & FLOWFLAG_STATE_XON) == 0 && buf_canread(rx) <= 16) {
      // The buffer has (BUFSIZE - 16) free characters again, so send an XON.
      flowflags = FLOWFLAG_SEND_XON;
      UCSR0B |= MASK(UDRIE0);
    }
  #endif

  return c;
}

/**
  Write string from FLASH.

  Extensions to output flash memory pointers. This prevents the data to
  become part of the .data segment instead of the .code segment. That means
  less memory is consumed for multi-character writes.

  For single character writes (i.e. '\n' instead of "\n"), using
  serial_writechar() directly is the better choice.
*/
void serial_writestr_P(PGM_P data_P)
{
    uint8_t r, i = 0;
    // yes, this is *supposed* to be assignment rather than comparison, so we break when r is assigned zero
    while ((r = pgm_read_byte(&data_P[i++])))
        serial_writechar(r);
}


/** Send one character.
*/
void serial_writechar(uint8_t data) {

  // Check if interrupts are enabled.
  if (SREG & MASK(SREG_I)) {
    // If they are, we should be ok to block since the tx buffer is emptied
    // from an interrupt.
    for ( ; buf_canwrite(tx) == 0; ) ;
    buf_push(tx, data);
  }
  else {
    // Interrupts are disabled -- maybe we're in one?
    // Anyway, instead of blocking, only write if we have room.
    if (buf_canwrite(tx))
      buf_push(tx, data);
  }

  // Enable TX interrupt so we can send this character.
  UCSR0B |= MASK(UDRIE0);
}
#endif /* USB_SERIAL */

void sendf_P(void (*writechar)(uint8_t), PGM_P format_P, ...) { 
    va_list args;
    va_start(args, format_P);

    uint16_t i = 0;
    uint8_t c = 1, j = 0;
    while ((c = pgm_read_byte(&format_P[i++]))) {
        if (j) {
            switch(c) {
                case 's':
                    j = 1;
                    break;
                case 'l':
                    j = 4;
                    break;
                case 'u':
          if (j == 1)
            write_uint8(writechar, (uint8_t)GET_ARG(uint16_t));
          else if (j == 2)
            write_uint16(writechar, (uint16_t)GET_ARG(uint16_t));
                    else
            write_uint32(writechar, GET_ARG(uint32_t));
                    j = 0;
                    break;
                case 'd':
          if (j == 1)
            write_int8(writechar, (int8_t)GET_ARG(int16_t));
          else if (j == 2)
            write_int16(writechar, (int16_t)GET_ARG(int16_t));
                    else
            write_int32(writechar, GET_ARG(int32_t));
                    j = 0;
                    break;
                case 'c':
          writechar((uint8_t)GET_ARG(uint16_t));
                    j = 0;
                    break;
                case 'x':
          writechar('0');
          writechar('x');
          if (j == 1)
            write_hex8(writechar, (uint8_t)GET_ARG(uint16_t));
          else if (j == 2)
            write_hex16(writechar, (uint16_t)GET_ARG(uint16_t));
                    else
            write_hex32(writechar, GET_ARG(uint32_t));
                    j = 0;
                    break;
/*              case 'p':
          serwrite_hex16(writechar, GET_ARG(uint16_t));*/
                case 'q':
          write_int32_vf(writechar, GET_ARG(uint32_t), 3);
                    j = 0;
                    break;
                default:
          writechar(c);
                    j = 0;
                    break;
            }
        }
        else {
            if (c == '%') {
                j = 2;
            }
            else {
        writechar(c);
            }
        }
    }
    va_end(args);
}