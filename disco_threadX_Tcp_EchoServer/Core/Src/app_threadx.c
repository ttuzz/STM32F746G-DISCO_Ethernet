/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
#include "tx_api.h"
#include "stdio.h"

#define THREAD_STACK_SIZE 1024

// Stack for the thread
UCHAR thread_stack[THREAD_STACK_SIZE];

// Thread control block
TX_THREAD my_thread_control_block;

// Thread function
void my_thread_entry(ULONG thread_input)
{
    while (1)
    {
        // Perform the task
        printf("Thread is running!\n");

        // Sleep for a while (simulate work)
        tx_thread_sleep(1000); // Sleep for 100 ticks
    }
}

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */

 /* ret = tx_thread_create(&my_thread_control_block,       // Thread control block
                         "My Thread",                    // Thread name
                         my_thread_entry,                 // Entry function
                         0,                               // Input parameter for thread (not used here)
                         thread_stack,                    // Stack
                         THREAD_STACK_SIZE,               // Stack size
                         1,                               // Priority
                         1,                               // Preemption threshold
                         TX_NO_TIME_SLICE,                // Time slice
                         TX_AUTO_START);                  // Auto start thread

  if (ret != TX_SUCCESS)
  {
      // Handle error
      printf("Thread creation failed!\n");
  }*/
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */



/* USER CODE END 1 */
