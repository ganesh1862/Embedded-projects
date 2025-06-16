/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0L222X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0L222X
#define CONFIG_MSPM0L2228

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for DEBUG_UART */
#define DEBUG_UART_INST                                                    UART3
#define DEBUG_UART_INST_FREQUENCY                                       32000000
#define DEBUG_UART_INST_IRQHandler                              UART3_IRQHandler
#define DEBUG_UART_INST_INT_IRQN                                  UART3_INT_IRQn
#define GPIO_DEBUG_UART_RX_PORT                                            GPIOB
#define GPIO_DEBUG_UART_TX_PORT                                            GPIOB
#define GPIO_DEBUG_UART_RX_PIN                                    DL_GPIO_PIN_13
#define GPIO_DEBUG_UART_TX_PIN                                    DL_GPIO_PIN_12
#define GPIO_DEBUG_UART_IOMUX_RX                                 (IOMUX_PINCM34)
#define GPIO_DEBUG_UART_IOMUX_TX                                 (IOMUX_PINCM33)
#define GPIO_DEBUG_UART_IOMUX_RX_FUNC                  IOMUX_PINCM34_PF_UART3_RX
#define GPIO_DEBUG_UART_IOMUX_TX_FUNC                  IOMUX_PINCM33_PF_UART3_TX
#define DEBUG_UART_BAUD_RATE                                            (115200)
#define DEBUG_UART_IBRD_32_MHZ_115200_BAUD                                  (17)
#define DEBUG_UART_FBRD_32_MHZ_115200_BAUD                                  (23)





/* Port definition for Pin Group EXT_OUT */
#define EXT_OUT_PORT                                                     (GPIOA)

/* Defines for LED_3: GPIOA.0 with pinCMx 1 on package pin 1 */
#define EXT_OUT_LED_3_PIN                                        (DL_GPIO_PIN_0)
#define EXT_OUT_LED_3_IOMUX                                       (IOMUX_PINCM1)
/* Defines for RGB_LED: GPIOA.1 with pinCMx 2 on package pin 2 */
#define EXT_OUT_RGB_LED_PIN                                      (DL_GPIO_PIN_1)
#define EXT_OUT_RGB_LED_IOMUX                                     (IOMUX_PINCM2)
/* Port definition for Pin Group EXT_IN */
#define EXT_IN_PORT                                                      (GPIOA)

/* Defines for SWITCH: GPIOA.8 with pinCMx 19 on package pin 22 */
// pins affected by this interrupt request:["SWITCH","SWITCH_2"]
#define EXT_IN_INT_IRQN                                         (GPIOA_INT_IRQn)
#define EXT_IN_INT_IIDX                         (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define EXT_IN_SWITCH_IIDX                                   (DL_GPIO_IIDX_DIO8)
#define EXT_IN_SWITCH_PIN                                        (DL_GPIO_PIN_8)
#define EXT_IN_SWITCH_IOMUX                                      (IOMUX_PINCM19)
/* Defines for SWITCH_2: GPIOA.18 with pinCMx 50 on package pin 55 */
#define EXT_IN_SWITCH_2_IIDX                                (DL_GPIO_IIDX_DIO18)
#define EXT_IN_SWITCH_2_PIN                                     (DL_GPIO_PIN_18)
#define EXT_IN_SWITCH_2_IOMUX                                    (IOMUX_PINCM50)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_DEBUG_UART_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
