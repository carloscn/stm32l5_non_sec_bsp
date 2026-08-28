/**
 ******************************************************************************
 * @file    main.c
 * @brief   FreeRTOS "toggle LED" bring-up for NUCLEO-L552ZE-Q.
 *
 * Structure mirrors FreeRTOS_Toggle_Led_Example_S32K312: HAL + OSAL layers,
 * two tasks (LED blink + heartbeat log), started via osal_sched_start().
 ******************************************************************************
 */
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hal_gpio.h"
#include "hal_uart.h"
#include "osal_log.h"
#include "osal_sched.h"
#include "osal_task.h"

/* Task priorities (idle = 0). LogTask is created at 1 by osal_log_init(). */
#define PRIO_LED        (2U)
#define PRIO_HEARTBEAT  (2U)

static HAL_UART s_console = { .num = HAL_UART_0, .irq = 0U };

void SystemClock_Config(void);
static void MX_ICACHE_Init(void);

/* ------------------------------------------------------------------ tasks */

static void led_task(void *arg)
{
    (void)arg;
    for (;;) {
        hal_gpio_toggle(HAL_GPIO_LED_GREEN);
        osal_task_delay_ms(250U);
    }
}

static void heartbeat_task(void *arg)
{
    uint32_t n = 0U;
    (void)arg;
    for (;;) {
        osal_log_printf("heartbeat %lu  (heap free %u)",
                        (unsigned long)n++,
                        (unsigned)xPortGetFreeHeapSize());
        osal_task_delay_ms(1000U);
    }
}

/* ------------------------------------------------------------------ main */

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_ICACHE_Init();

    (void)hal_gpio_init();
    (void)hal_uart_init(&s_console);
    osal_log_init(&s_console);

    osal_log_info("stm32l5_non_sec_bsp: FreeRTOS " tskKERNEL_VERSION_NUMBER " up");

    (void)osal_task_create(led_task,       "led", 128U, NULL, PRIO_LED,       NULL);
    (void)osal_task_create(heartbeat_task, "hb",  256U, NULL, PRIO_HEARTBEAT, NULL);

    osal_sched_start();      /* does not return */

    for (;;) {
    }
}

/* ------------------------------------------------------------ FreeRTOS hooks
 * (HAL_IncTick runs from SysTick_Handler in stm32l5xx_it.c, not a tick hook.)
 */

void vApplicationMallocFailedHook(void)
{
    osal_log_info("FATAL: pvPortMalloc failed");
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
    (void)task;
    osal_log_printf("FATAL: stack overflow in %s", name ? name : "?");
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

/* ------------------------------------------------------------ clock / icache */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;              /* 4 MHz */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 55;                              /* 4*55/1/2 = 110 MHz */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_ICACHE_Init(void)
{
    if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_ICACHE_Enable() != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    for (;;) {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif
