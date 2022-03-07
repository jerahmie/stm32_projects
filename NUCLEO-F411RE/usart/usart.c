/*
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void)
{
  // enable gpio clock for LED indicator
  rcc_periph_clock_enable(RCC_GPIOA);

  // enable clocks for USART2
  rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void)
{
  // Setup USART2
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

  // enable USART
  usart_enable(USART2);
}

static void gpio_setup(void)
{
  // setup LED GPIO
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

  // setup GPIO pins for usart transmit
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

  // setup USART2 Tx pin as alternate function
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

int main(void)
{
  int i, j = 0, c = 0;
  clock_setup();
  gpio_setup();
  usart_setup();

  /* main loop */
  while (1)
    {
      gpio_toggle(GPIOA, GPIO5);  /* LED on/off */
      usart_send_blocking(USART2, c + '0');
      c = ( c == 9 ) ? 0 : c + 1;
      if (( j++ % 80) == 0) {
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2, '\n');
      }
      for (i = 0; i < 300000; i++) {
	__asm__("nop");
      }
      
    }
}
