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

uint8_t usart1_receive_buffer[4] = {0};  // USART1 接收缓冲区
uint8_t usart1_receive_pointer = 0;

uint8_t usart2_receive_buffer[4];  // 存储4字节数据  
uint8_t usart2_receive_pointer = 0;

uint8_t usart1_rx_data;  // USART1 临时接收字节
uint8_t usart2_rx_data;  // USART2 临时接收字节

uint8_t usart1_data_ready = 0;  // 标志 USART1 数据是否准备好
uint8_t usart2_data_ready = 0;  // 标志 USART2 数据是否准备好

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)  
    {
        usart1_data_ready = 1;  // 标记 USART1 数据已接收完成
        HAL_UART_Receive_IT(&huart1, usart1_receive_buffer, 4);  // 继续接收
    }
    else if (huart->Instance == USART2)  
    {
        usart2_data_ready = 1;  // 标记 USART2 数据已接收完成
        HAL_UART_Receive_IT(&huart2, usart2_receive_buffer, 4);  // 继续接收
    }
}

// 解析 32 位数据
ParsedData parse_received_data(uint8_t *buffer, uint8_t expected_header)
{
    ParsedData data = {0};

    uint32_t received_cmd = (buffer[0] << 24) |
                            (buffer[1] << 16) |
                            (buffer[2] << 8)  |
                            (buffer[3]);

    data.device_id = received_cmd & 0xF;
    data.switch_state = (received_cmd >> 4) & 0xF;
    data.param1 = (received_cmd >> 8) & 0xF;
    data.param2 = (received_cmd >> 12) & 0xF;

    // 确保帧头正确
    if (((received_cmd >> 28) & 0xF) != expected_header) {
        data.valid = 0;
    } else {
        data.valid = 1;
    }

    return data;
}

uint8_t process_device_command(ParsedData data)
{
    uint8_t response = 0x0;

    switch (data.device_id)
    {
        case 0x1: // 门
            response = data.switch_state ? door_open() : door_close();
            break;

        case 0x2: // 窗帘
            if (data.switch_state) {
                curtain_open_angle(data.param1 * 10);
            } else {
                curtain_close_angle(data.param1 * 10);
            }
            response = 0x1;
            break;

        case 0x3: // 空调
            response = data.switch_state ? 
                        (data.param2 ? airConditioner_heat(data.param1) : airConditioner_cool(data.param1))
                        : airConditioner_stop();
            break;

        default:
            printf("Unknown device ID: %02X\n", data.device_id);
            response = 0x0;
            break;
    }
    return response;
}


void send_response(uint8_t response, ParsedData data, UART_HandleTypeDef *huart)
{
    uint8_t header = (huart == &huart1) ? 0xC : 0xB;  // 适配 USART1 (ASRPRO) 和 USART2 (HI3861)

    uint32_t response_data = ((uint32_t)header << 28) |  
                             (0x1 << 24) |  
                             (response << 20) |  
                             (data.param2 << 12) | 
                             (data.param1 << 8) | 
                             (data.switch_state << 4) | 
                             data.device_id;

    uint8_t response_buffer[4];
    response_buffer[0] = (response_data >> 24) & 0xFF;
    response_buffer[1] = (response_data >> 16) & 0xFF;
    response_buffer[2] = (response_data >> 8) & 0xFF;
    response_buffer[3] = response_data & 0xFF;

//		printf("stm32 sent : %08x\n", *response_buffer);
    HAL_UART_Transmit(huart, response_buffer, 4, 100);
}



void usart1_rx_process(void)
{
    ParsedData data = parse_received_data(usart1_receive_buffer, 0xC);
    memset(usart1_receive_buffer, 0, sizeof(usart1_receive_buffer));

    if (!data.valid) {
        printf("Invalid command from asrpro!\n");
        send_response(0x0, data, &huart1);
        return;
    }

    printf("ASRPRO Received: %02X %02X %02X %02X\n", data.device_id, data.switch_state, data.param1, data.param2);

    uint8_t response = process_device_command(data);
    send_response(response, data, &huart1);
}

void usart2_rx_process(void)
{
    ParsedData data = parse_received_data(usart2_receive_buffer, 0xB);
    memset(usart2_receive_buffer, 0, sizeof(usart2_receive_buffer));

    if (!data.valid) {
        printf("Invalid command from hi3861!\n");
        send_response(0x0, data, &huart2);
        return;
    }

    printf("HI3861 Received: %02X %02X %02X %02X\n", data.device_id, data.switch_state, data.param1, data.param2);

    uint8_t response = process_device_command(data);
    send_response(response, data, &huart2);
}

void usart_init(void)
{
	HAL_UART_Receive_IT(&huart1, usart1_receive_buffer, 4);
	HAL_UART_Receive_IT(&huart2, usart2_receive_buffer, 4); // 继续接收
}
/* USER CODE END 1 */
