/*
*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* TI includes */
#include "ti_msp_dl_config.h"


void LED_TASK(void *pvParameters) {
  while (1) {
    printf("LED task running...\n");
    vTaskDelay(pdMS_TO_TICKS(500));
    DL_GPIO_setPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);

    printf("An lED is blinking at 1 sec delay in RTOS\n");
    vTaskDelay(pdMS_TO_TICKS(500));
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
  }
}

void switch_task(void *pvParameters) {
  while (1) {
    int result = DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_SW_2_PIN);
   // printf("result is :- %d\n");
    if (DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_SW_2_PIN) == 0) {
     DL_GPIO_togglePins(GPIO_LEDS_GREEN_PORT, GPIO_LEDS_GREEN_PIN);
      printf("switch 2 is pressed\n");
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
int main(void) {
  /* Prepare the hardware to run this demo. */
  SYSCFG_DL_init();

  // main_blinky();

  xTaskCreate(LED_TASK, "BLINK_LED", 256, NULL, 1, NULL);
  xTaskCreate(switch_task, "switch", 256, NULL, 2, NULL);

  vTaskStartScheduler();
  while (1) {
    //     delay_cycles(16000000); // blocking delay 1 sec
    //     DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    //     printf("An lED is blinking at 1 sec delay in BM\n");

    //     delay_cycles(64000000); // blocking dekay 2 sec

    //     int result = DL_GPIO_readPins( GPIO_INPUT_PORT,GPIO_INPUT_SW_2_PIN);
    //     // missing this button press......YES printf("result is :- %d\n"); if
    //     if(DL_GPIO_readPins(GPIO_INPUT_PORT, GPIO_INPUT_SW_2_PIN) == 0) {
    //       __BKPT();
    //       printf("switch 2 is pressed\n");
    //     }
    //     DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
  }

  return 0;
}

//if the stack overflow occures 
#if (configCHECK_FOR_STACK_OVERFLOW)
/*
 *  ======== vApplicationStackOverflowHook ========
 *  When stack overflow checking is enabled the application must provide a
 *  stack overflow hook function. This default hook function is declared as
 *  weak, and will be used by default, unless the application specifically
 *  provides its own hook function.
 */
#if defined(__IAR_SYSTEMS_ICC__)
__weak void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
#elif (defined(__TI_COMPILER_VERSION__))
#pragma WEAK(vApplicationStackOverflowHook)
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
#elif (defined(__GNUC__) || defined(__ti_version__))
void __attribute__((weak)) vApplicationStackOverflowHook(TaskHandle_t pxTask,
                                                         char *pcTaskName)
#endif
{
  /* default to spin upon stack overflow */
  while (1) {
  }
}
#endif

/*-----------------------------------------------------------*/
