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
