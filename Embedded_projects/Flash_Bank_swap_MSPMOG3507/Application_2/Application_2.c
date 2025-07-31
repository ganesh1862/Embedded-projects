#include "ti_msp_dl_config.h"
#include <string.h>
#define BOOTLOADER 0x00000000

uint8_t DEBUG_INDEX = 0;
unsigned char DEBUG_BUFFER[256] = {0};
bool DEBUG_DATA_RECEIVE_FLAG = false;

void print_string(char *text) {
  unsigned int i = 0;
  int length = strlen((const char *)text);
  for (i = 0; i < length; i++) {
    uint8_t datap = text[i];
    DL_UART_Main_transmitData(CONSOLE_UART_INST, datap);
    delay_cycles(160000);
  }
}

uint32_t *vector_table_backup;
void jump_to_app(uint32_t *vector_table) {
  uint32_t sp = vector_table[0];
  uint32_t reset_handler = vector_table[1];
  // Set vector table offset register
  SCB->VTOR = (uint32_t)vector_table;

  // Jump to reset handler
  void (*app_reset_handler)(void) = (void (*)(void))reset_handler;
  app_reset_handler();
}
bool is_app_valid(uint32_t app_addr) {
    uint32_t *vector_table = (uint32_t *)app_addr;

    uint32_t sp = vector_table[0]; // Initial SP
    uint32_t pc = vector_table[1]; // Reset handler

    // Validate SP — must be within SRAM range of MSPM0G3507 (64KB at 0x20200000)
    if (sp < 0x20200000 || sp >= 0x20300000) {
        print_string("\r\nInvalid SP range\r\n");
        return false;
    }

    // Optional: Validate PC — must be in flash range
    if (pc < 0x00000000 || pc >= 0x00080000) {  // 512KB Flash
        print_string("\r\nInvalid PC range\r\n");
        return false;
    }

    return true;
}


int main(void) {
  SYSCFG_DL_init();

  NVIC_ClearPendingIRQ(CONSOLE_UART_INST_INT_IRQN);
  NVIC_EnableIRQ(CONSOLE_UART_INST_INT_IRQN);

  while (1) {
    print_string("Application_TWO is running Its an RED LED\n");
    delay_cycles(32000000);
    DL_GPIO_togglePins(GPIO_PORT, GPIO_RED_LED_PIN);
   
   /*Jumpig to Boot loader up on commad receive*/
    if (DEBUG_DATA_RECEIVE_FLAG) {
      if (is_app_valid(BOOTLOADER)) {
        print_string("Bootloader address is valid\n");
        print_string("\r\nJumping to Bootloader");
        jump_to_app((uint32_t *)BOOTLOADER);
      } else {
        print_string("\r\nBootloader not found");
      }
      DEBUG_DATA_RECEIVE_FLAG = false;
    }
  }
}

void CONSOLE_UART_INST_IRQHandler(void) {

  switch (DL_UART_Main_getPendingInterrupt(CONSOLE_UART_INST)) {
  case DL_UART_MAIN_IIDX_RX:

    DEBUG_BUFFER[DEBUG_INDEX] = DL_UART_Main_receiveData(CONSOLE_UART_INST);
    if (DEBUG_BUFFER[DEBUG_INDEX] == 0xAA) {
      DEBUG_INDEX++;
      DEBUG_DATA_RECEIVE_FLAG = true;
    } else {
      DEBUG_INDEX++;
    }
    break;
  default:
    break;
  }
}
