#include <stdio.h>
#include <stdint.h>

#define SERIAL_PORT_BASE     0xffffc000

#define SERP_RX_ST_REG_o           0x00
#define SERP_RX_ST_REG_READY_m      0x1
#define SERP_RX_ST_REG_IE_m         0x2

#define SERP_RX_DATA_REG_o         0x04

#define SERP_TX_ST_REG_o           0x08
#define SERP_TX_ST_REG_READY_m      0x1
#define SERP_TX_ST_REG_IE_m         0x2

#define SERP_TX_DATA_REG_o         0x0c

#define SPILED_REG_BASE    0xffffc100

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024


static inline void serp_write_reg(uint32_t base, uint32_t reg, uint32_t val)
{
  *(volatile uint32_t *)(base + reg) = val;
}
static inline uint32_t serp_read_reg(uint32_t base, uint32_t reg)
{
  return *(volatile uint32_t *)(base + reg);
}

void serp_tx_byte(int data)
{
  while (!(serp_read_reg(SERIAL_PORT_BASE, SERP_TX_ST_REG_o) &
                SERP_TX_ST_REG_READY_m));
  serp_write_reg(SERIAL_PORT_BASE, SERP_TX_DATA_REG_o, data);
}

static inline void spi_led_write_reg(uint32_t base, uint32_t reg, uint32_t val)
{
  *(volatile uint32_t *)(base + reg) = val;
}
static inline uint32_t spi_led_read_reg(uint32_t base, uint32_t reg)
{
  return *(volatile uint32_t *)(base + reg);
}


void serp_send_str(const char * str)
{
  const char *s;

  for (s = str; *s; s++)
    serp_tx_byte(*s);
}

int main(void)
{

  const char *text = "Dial knobs test\n";
  char buff[30];
  const char *s;

  serp_send_str(text);

  while (1) {
    unsigned int val = spi_led_read_reg(SPILED_REG_BASE, SPILED_REG_KNOBS_8BIT_o);
    sprintf(buff, "0x%08x\n", val);
    serp_send_str(buff);
    spi_led_write_reg(SPILED_REG_BASE, SPILED_REG_LED_RGB1_o, val);
    spi_led_write_reg(SPILED_REG_BASE, SPILED_REG_LED_RGB2_o, ~val);
    spi_led_write_reg(SPILED_REG_BASE, SPILED_REG_LED_LINE_o, val);
  }
  return 0;
}
