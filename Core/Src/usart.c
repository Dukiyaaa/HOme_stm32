/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "sg90.h"
#include "step_motor.h"
#include "dc_motor.h"

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit (&huart2 ,(uint8_t *)&ch,1,HAL_MAX_DELAY );
	return ch;
}

char usart1_receive_buffer[32] = {0};  // USART1 接收缓冲区
uint8_t usart1_receive_pointer = 0;

uint8_t usart2_receive_buffer[4];  // 存储4字节数据  
uint8_t usart2_receive_pointer = 0;

uint8_t usart1_rx_data;  // USART1 临时接收字节
uint8_t usart2_rx_data;  // USART2 临时接收字节

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)  // 判断是否是 USART1
    {
        usart1_receive_buffer[usart1_receive_pointer++] = usart1_rx_data;
        if (usart1_receive_pointer >= sizeof(usart1_receive_buffer)) {
            usart1_receive_pointer = 0;  // 防止溢出
        }
        HAL_UART_Receive_IT(&huart1, &usart1_rx_data, 1); // 继续接收
    }
    else if (huart->Instance == USART2) // 判断是否是 USART2
    {
        HAL_UART_Receive_IT(&huart2, usart2_receive_buffer, 4); // 继续接收
    }
}

uint32_t CMD_MASK         = 0xf000000f;

uint32_t CMD_DOOR_MASK 		= 0xb0000001;
uint32_t CMD_DOOR_OPEN_STATE_MASK = 0x000000f0;
uint32_t CMD_CURTAIN_MASK = 0xb0000002;

void usart2_rx_process(void)
{			
    uint32_t received_cmd = 0;

    // 解析 32 位数据
    received_cmd = (usart2_receive_buffer[0] << 24) |
                   (usart2_receive_buffer[1] << 16) |
                   (usart2_receive_buffer[2] << 8)  |
                   (usart2_receive_buffer[3]);

		memset(usart2_receive_buffer, 0, sizeof(usart2_receive_buffer));
	  usart2_receive_pointer = 0;
    uint8_t device_id = received_cmd & 0xF;            // 最低 4 位（设备 ID）
    uint8_t switch_state = (received_cmd >> 4) & 0xF; // 倒数第 2 组 4 位（开关）
    uint8_t param1 = (received_cmd >> 8) & 0xF;       // 倒数第 3 组 4 位（窗帘角度 / 空调档位）
    uint8_t param2 = (received_cmd >> 12) & 0xF;      // 倒数第 4 组 4 位（空调冷热）
		
    // 确保帧头为 `0xB`
    if (((received_cmd >> 28) & 0xF) != 0xB) {
        printf("Invalid command!\n");
        return;
    }

		printf("Received: %02X %02X %02X %02X\n", 
            device_id, 
            switch_state, 
            param1, 
            param2);
    if (device_id == 0x1)  // 门
    {
        if (switch_state == 1) 
        {
            door_open();
        }
        else 
        {
            door_close();
        }
        return;
    }

    if (device_id == 0x2) // 窗帘
    {
        if (switch_state == 1) 
        {
            curtain_open_angle(param1 * 10);  // 角度范围 0~150（每 10 度一档）
        }
        else 
        {
            curtain_close_angle(param1 * 10);  // 角度范围 0~150（每 10 度一档）
        }
        return;
    }

    if (device_id == 0x3) // 空调
    {
				printf("airConditioner 309\n");
        if (switch_state == 1) // 1 = 开启空调
        {
					printf("airConditioner 312\n");
            if (param2 == 1) // 热模式
            {
							printf("airConditioner 315\n");
                airConditioner_heat(param1);
            }
            else // 冷模式
            {
                airConditioner_cool(param1);
            }
        }
        else  // 0 = 关闭空调
        {
            airConditioner_stop();
        }
        return;
    }
}


void usart2_func(void)
{
	if(usart2_receive_pointer > 0)
	{
		int temp = usart2_receive_pointer;
		HAL_Delay(1);
		if(temp == usart2_receive_pointer)
		{
			usart2_rx_process();
			usart2_receive_pointer = 0;
			memset(usart2_receive_buffer, 0, sizeof(usart2_receive_buffer));
		}
	}
}

void usart_init(void)
{
	HAL_UART_Receive_IT(&huart1, &usart1_rx_data, 1);
	HAL_UART_Receive_IT(&huart2, usart2_receive_buffer, 4); // 继续接收
}
/* USER CODE END 1 */
