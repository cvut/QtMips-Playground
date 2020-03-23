/*******************************************************************
  Simple program to demostrate binary reprezentation
  on QtMips emulator developed by Karel Koci and Pavel Pisa.

  qrmips_binrep.c       - main and only file

  (C) Copyright 2004 - 2019 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>

/*
 * Next macros provides location of knobs and LEDs peripherals
 * implemented on QtMips simulator.
 *
 * More information can be found on page
 *   https://github.com/ppisa/QtMips
 */


/*
 * Base address of the region where simple serial port (UART)
 * implementation is mapped in emulated MIPS address space
 */
#define SERIAL_PORT_BASE   0xffffc000
/*
 * Byte offset of the 32-bit transition status register
 * of the serial port
 */
#define SERP_TX_ST_REG_o         0x08

/*
 * Mask of the bit which inform that peripheral is busy by
 * sending previous character. If clean, peripheral is ready
 * to accept next character.
 */
#define SERP_TX_ST_REG_READY_m    0x1
/*
 * Byte offset of the UART transmit register.
 * When the 32-bit word is written the least-significant (LSB)
 * eight bits are send to the terminal.
 */
#define SERP_TX_DATA_REG_o        0x0c

/*
 * Base address of the region where knobs and LEDs peripherals
 * are mapped in the emulated MIPS physical memory address space.
 */
#define SPILED_REG_BASE      0xffffc100

/* Valid address range for the region */
#define SPILED_REG_SIZE      0x00000100

/*
 * Byte offset of the register which controls individual LEDs
 * in the row of 32 yellow LEDs. When the corresponding bit
 * is set (value 1) then the LED is lit.
 */
#define SPILED_REG_LED_LINE_o           0x004

/*
 * The register to control 8 bit RGB components of brightness
 * of the first RGB LED
 */
#define SPILED_REG_LED_RGB1_o           0x010

/*
 * The register to control 8 bit RGB components of brightness
 * of the second RGB LED
 */
#define SPILED_REG_LED_RGB2_o           0x014

/*
 * The register which combines direct write to RGB signals
 * of the RGB LEDs, write to the keyboard scan register
 * and control of the two additional individual LEDs.
 * The direct write to RGB signals is orred with PWM
 * signal generated according to the values in previous
 * registers.
 */
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

/*
 * Register providing access to unfiltered encoder channels
 * and keyboard return signals.
 */
#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020

/*
 * The register representing knobs positions as three
 * 8-bit values where each value is incremented
 * and decremented by the knob relative turning.
 */
#define SPILED_REG_KNOBS_8BIT_o         0x024


static inline void serp_write_reg(uint32_t base, uint32_t reg, uint32_t val)
{
  *(volatile uint32_t *)(base + reg) = val;
}
static inline uint32_t serp_read_reg(uint32_t base, uint32_t reg)
{
  return *(volatile uint32_t *)(base + reg);
}

/*
 * Send single byte by UART port to a terminal, wait for ready
 * to send first.
 */
void serp_tx_byte(int data)
{
  while (!(serp_read_reg(SERIAL_PORT_BASE, SERP_TX_ST_REG_o) &
                SERP_TX_ST_REG_READY_m));
  serp_write_reg(SERIAL_PORT_BASE, SERP_TX_DATA_REG_o, data);
}

/*
 * Write 32-bit hexadecimal number to the terminal.
 */
void serp_send_hex(unsigned int val)
{
  int i;
  for (i = 8; i > 0; i--) {
    char c = (val >> 28) & 0xf;
    if (c < 10 )
      c += '0';
    else
      c += 'A' - 10;
    serp_tx_byte(c);
    val <<= 4;
  }
}

/*
 * The main entry into example program
 */
int main(int argc, char *argv[])
{

  unsigned char *mem_base = (unsigned char *)SPILED_REG_BASE;

  while (1) {
     uint32_t rgb_knobs_value;
     unsigned int uint_val;

     /*
      * Access register holding 8 bit relative knobs position
      * The type "(volatile uint32_t*)" casts address obtained
      * as a sum of base address and register offset to the
      * pointer type which target in memory type is 32-bit unsigned
      * integer. The "volatile" keyword ensures that compiler
      * cannot reuse previously read value of the location.
      */
     rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);

     /* Store the read value to the register controlling individual LEDs */
     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;

     /*
      * Store RGB knobs values to the corersponding components controlling
      * a color/brightness of the RGB LEDs
      */
     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = rgb_knobs_value;

     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = ~rgb_knobs_value;

     /* Assign value read from knobs to the basic signed and unsigned types */
     uint_val = rgb_knobs_value;

     /* Print values */
     serp_send_hex(uint_val);
     serp_tx_byte('\n');
  }

  return 0;
}
