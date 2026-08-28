/*
 * FreeRTOSConfig.h  --  STM32L552 (Cortex-M33, no TrustZone) / NUCLEO-L552ZE-Q
 *
 * Kept close to FreeRTOS_Toggle_Led_Example_S32K312/generate/include/FreeRTOSConfig.h.
 * Deltas vs that file: Cortex-M33 (FPU on, MPU/TrustZone off), 110 MHz core,
 * larger heap (L552 has 256 KiB RAM), stack-overflow + malloc-failed hooks on
 * for bring-up.
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*------------------------------------------------------------- core / tick */
#define configCPU_CLOCK_HZ                          ( 110000000UL )
#define configTICK_RATE_HZ                          ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                        5
#define configMINIMAL_STACK_SIZE                    ( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN                     16
#define configUSE_16_BIT_TICKS                      0
#define configIDLE_SHOULD_YIELD                     1
#define configUSE_PREEMPTION                       1
#define configUSE_TIME_SLICING                     1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     0
#define configUSE_TASK_NOTIFICATIONS               1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
#define configUSE_NEWLIB_REENTRANT                  0
#define configENABLE_BACKWARD_COMPATIBILITY         1
#define configUSE_POSIX_ERRNO                       0

/*------------------------------------------------------------- Cortex-M33
 * ARM_CM33_NTZ/non_secure port: no TrustZone. On this standalone image TZEN=0,
 * so the core runs single-state; the NTZ port is the correct choice.
 */
#if defined(TFM_NS)
/* NSPE: match the SPE ABI (CONFIG_TFM_FLOAT_ABI=soft, CP10/CP11 not enabled). */
#define configENABLE_FPU                            0
#else
#define configENABLE_FPU                            1
#endif
#define configENABLE_MPU                            0
#define configENABLE_TRUSTZONE                      0
#define configENABLE_MVE                            0

/*------------------------------------------------------------- assert */
#define configASSERT( x )   if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/*------------------------------------------------------------- memory */
#define configSUPPORT_STATIC_ALLOCATION            0
#define configSUPPORT_DYNAMIC_ALLOCATION           1
#define configTOTAL_HEAP_SIZE                      ( ( size_t ) ( 32U * 1024U ) )
#define configAPPLICATION_ALLOCATED_HEAP          0

/*------------------------------------------------------------- SMP (single core) */
#define configNUMBER_OF_CORES                      1

/*------------------------------------------------------------- hooks */
#define configUSE_IDLE_HOOK                        0
#define configUSE_TICK_HOOK                        0   /* HAL_IncTick runs from SysTick_Handler in stm32l5xx_it.c */
#define configUSE_MALLOC_FAILED_HOOK              1
#define configCHECK_FOR_STACK_OVERFLOW            2
#define configUSE_DAEMON_TASK_STARTUP_HOOK        0

/*------------------------------------------------------------- run-time stats */
#define configGENERATE_RUN_TIME_STATS             0
#define configUSE_TRACE_FACILITY                  1
#define configUSE_STATS_FORMATTING_FUNCTIONS      0

/*------------------------------------------------------------- co-routines */
#define configUSE_CO_ROUTINES                     0
#define configMAX_CO_ROUTINE_PRIORITIES          2

/*------------------------------------------------------------- sync objects */
#define configUSE_MUTEXES                         1
#define configUSE_RECURSIVE_MUTEXES              1
#define configUSE_COUNTING_SEMAPHORES            1
#define configQUEUE_REGISTRY_SIZE                4
#define configUSE_QUEUE_SETS                     0

/*------------------------------------------------------------- software timers */
#define configUSE_TIMERS                          1
#define configTIMER_TASK_PRIORITY               2
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )

#define configUSE_TICKLESS_IDLE                  0

/*------------------------------------------------------------- INCLUDE_* */
#define INCLUDE_vTaskPrioritySet                 1
#define INCLUDE_uxTaskPriorityGet                1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1
#define INCLUDE_xTaskDelayUntil                  1
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_xTaskGetSchedulerState           1
#define INCLUDE_xTaskGetCurrentTaskHandle        1
#define INCLUDE_uxTaskGetStackHighWaterMark      1
#define INCLUDE_eTaskGetState                    1
#define INCLUDE_xTimerPendFunctionCall           1
#define INCLUDE_xTaskAbortDelay                  1
#define INCLUDE_xTaskGetHandle                   1
#define INCLUDE_xTaskResumeFromISR               1
#define INCLUDE_xQueueGetMutexHolder             1

/*------------------------------------------------------------- Cortex-M NVIC */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS                      __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS                      4
#endif

#if defined(TFM_NS)
/* The STM32L5 non-secure NVIC implements only __NVIC_PRIO_BITS = 3
 * (the secure side has 4). Priorities must fit 0..7. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        0x07
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   2
#else
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        0x0F
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   5
#endif

#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/*------------------------------------------------------------- handler names
 * SVC and PendSV go straight to the port. SysTick is NOT remapped: a thin
 * SysTick_Handler in stm32l5xx_it.c keeps HAL_IncTick() alive (needed by ST
 * HAL before the scheduler starts) and forwards to xPortSysTickHandler() once
 * the scheduler is running.
 */
#define vPortSVCHandler                          SVC_Handler
#define xPortPendSVHandler                       PendSV_Handler

#endif /* FREERTOS_CONFIG_H */
