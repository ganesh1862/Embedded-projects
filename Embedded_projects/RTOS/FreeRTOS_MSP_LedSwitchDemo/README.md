# MSP FreeRTOS LED and Switch Demo

This project demonstrates a simple multitasking application using FreeRTOS on a Texas Instruments MSPM0G3507
Two tasks are created:
- A blinking LED task (`LED_TASK`)
- A switch/button monitoring task (`switch_task`)

Both tasks run concurrently using FreeRTOS, with GPIO access

---

## Requirements

- **Hardware**: MSPM0G3507 Launchpad
  - At least 2 GPIO-connected LEDs
  - One push-button input
- **IDE**: Code Composer Studio (CCS)
- **RTOS**: FreeRTOS



---

## 🚦 Features

### 🔁 LED_TASK

- Blinks the **User LED 1** every 1 second (500ms ON, 500ms OFF)
- Outputs status via `printf()` to console screen

### switch_task

- Polls **Switch 2 (SW2)** every 100ms
- When pressed (active-low), toggles a **green LED**
- Logs switch status via `printf()`  console screen

---

##  GPIO Mapping

| Signal         | Port/Pin                                                 |
|----------------|----------------------------------------------------------|
| User LED 1     | `GPIO_LEDS_USER_LED_1_PORT`, `GPIO_LEDS_USER_LED_1_PIN`  |
| Green LED      | `GPIO_LEDS_GREEN_PORT`, `GPIO_LEDS_GREEN_PIN`            |
| Switch 2 (SW2) | `GPIO_INPUT_PORT`, `GPIO_INPUT_SW_2_PIN`                 |


---

## Task Summary

| Task Name     | Stack Size | Priority | Description                          |
|---------------|------------|----------|--------------------------------------|
| `LED_TASK`    | 256        | 1        | Blinks User LED 1 every 1 second     |
| `switch_task` | 256        | 2        | Monitors Switch 2, toggles green LED |


- **CPU Load Balancing**: `vTaskDelay()` is used in both tasks to allow cooperative multitasking.


## Example Console Output



CORTEX_M0P: LED task running...

CORTEX_M0P: switch 2 is pressed

CORTEX_M0P: An lED is blinking at 1 sec delay in RTOS

CORTEX_M0P: switch 2 is pressed

CORTEX_M0P: LED task running...

CORTEX_M0P: switch 2 is pressed

CORTEX_M0P: An lED is blinking at 1 sec delay in RTOS