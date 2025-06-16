/******************************************************************************
 * @file    main.c
 * @author  S. Ganesh Kumar Reddy
 * @date    2025-06-16
 * @brief   GPIO Interrupt-based Button Toggle with UART Debug Print
 *
 * @details
 * This application configures a GPIO pin to detect button press interrupts 
 * on the MSPM0 microcontroller. When the button is pressed, an LED is toggled
 * and a debug message is sent over UART. The implementation uses:
 *   - External GPIO interrupt for real-time response
 *   - UART interface for debug.
 *   - Simple software flag for ISR-to-main communication
 *
 * @platform MSPM0 (tested on MSPM0l2228)
 * @peripherals Used:
 *   - GPIO: Input (button), Output (LED)
 *   - UART: For debug output
 *
 * 
 ******************************************************************************/


#include "ti_msp_dl_config.h"

bool button_pressed = false;
void print_string(char *text) {

  unsigned int i = 0;
  int length = strlen((const char *)text);
  for (i = 0; i < length; i++) {
    uint8_t datap = text[i];
    DL_UART_Main_transmitData(DEBUG_UART_INST, datap);
    delay_cycles(160000);
  }
}

int main(void) {
  SYSCFG_DL_init();

  NVIC_ClearPendingIRQ(EXT_IN_INT_IIDX);
  NVIC_EnableIRQ(EXT_IN_INT_IIDX);

  NVIC_ClearPendingIRQ(DEBUG_UART_INST_INT_IRQN);
  NVIC_EnableIRQ(DEBUG_UART_INST_INT_IRQN);

  while (1) {

    if (button_pressed) {
      button_pressed = false;
      print_string("button is pressed\n\r");
    }
  }
}

void GROUP1_IRQHandler(void) {
  uint32_t pending_interrupt_ext_ina = DL_GPIO_getPendingInterrupt(GPIOA);
  // printf("%d\n",pending_interrupt_ext_ina);
  if (pending_interrupt_ext_ina == 19) // 19 got from pending_interrupt_ext_ina
  {
    button_pressed = true;
    DL_GPIO_togglePins(EXT_OUT_PORT, EXT_OUT_LED_3_PIN);
  }
  DL_GPIO_clearInterruptStatus(EXT_IN_PORT, 19);
}