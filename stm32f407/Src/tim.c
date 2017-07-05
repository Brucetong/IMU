/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//开启 PWM 通道 4
    HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */
  
    /**TIM3 GPIO Configuration    
    PB0     ------> TIM3_CH3
    PB1     ------> TIM3_CH4 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  }
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
	///if (htim == &htim3)
	//{
		//temperature = DS18B20_Get_Temp();
	//}
//}
TIM_HandleTypeDef TIM3_Handler;
//定时器 3PWM 句柄
TIM_OC_InitTypeDef TIM3_CH4Handler;
//定时器 3 通道 4 句柄
//PWM 输出初始化
//arr:自动重装值 psc:时钟预分频数
void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
	TIM3_Handler.Instance=TIM3;
	//定时器 3
	TIM3_Handler.Init.Prescaler=psc;
	//定时器分频
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
	TIM3_Handler.Init.Period=arr;
	//自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&TIM3_Handler); //初始化 PWM
	TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择 PWM1
	TIM3_CH4Handler.Pulse=1500; //设置比较值,此值用来确定占空比
	TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_1); //配置 TIM3 通道 4
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启 PWM 通道 4

	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_2); //配置 TIM3 通道 4
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启 PWM 通道 4

	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_3); //配置 TIM3 通道 4
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启 PWM 通道 4

	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4); //配置 TIM3 通道 4
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启 PWM 通道 4
}
//定时器底层驱动,时钟使能,引脚配置
//此函数会被 HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();
	//使能定时器 3
	__HAL_RCC_GPIOB_CLK_ENABLE(); //开启 GPIOB 时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;//|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;
	GPIO_Initure.Pull=GPIO_PULLDOWN;
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure); //PB1

	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;
	GPIO_Initure.Pull=GPIO_PULLDOWN;
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	//复用推挽输出
	//上拉
	//高速
	//PB1 复用为 TIM3_CH4
}
//设置 TIM 通道 4 的占空比
//compare:比较值
void TIM_SetTIM3Compare4(uint32_t compare)
{
	TIM3->CCR4=(2000 - compare);
}
void TIM_SetTIM3Compare1(uint32_t compare)
{
	TIM3->CCR1=(2000 - compare);
}
void TIM_SetTIM3Compare2(uint32_t compare)
{
	TIM3->CCR2=(2000 - compare);
}
void TIM_SetTIM3Compare3(uint32_t compare)
{
	TIM3->CCR3=(2000 - compare);
}
void set_led_r(uint32_t r)
{
	TIM_SetTIM3Compare3(r);
}
void set_led_g(uint32_t g)
{
	TIM_SetTIM3Compare2(g);
}
void set_led_b(uint32_t b)
{
	TIM_SetTIM3Compare1(b);
}
void set_led_rgb(uint32_t r, uint32_t g, uint32_t b)
{
	if (r <= MAX_PWM_WITH)
	{
		set_led_r(r);
	}
	else
	{
		printf("r:%d is bigger than max:%d, func:%s, line:%d\n",
				r, MAX_PWM_WITH, __FUNCTION__, __LINE__);
	}

	if (g <= MAX_PWM_WITH)
	{
		set_led_g(g);
	}
	else
	{
		printf("g:%d is bigger than max:%d, func:%s, line:%d\n",
						g, MAX_PWM_WITH, __FUNCTION__, __LINE__);
	}

	if (b <= MAX_PWM_WITH)
	{
		set_led_b(b);
	}
	else
	{
		printf("b:%d is bigger than max:%d, func:%s, line:%d\n",
						b, MAX_PWM_WITH, __FUNCTION__, __LINE__);
	}
}
void set_temp(uint32_t temp_ctrl)
{
	TIM_SetTIM3Compare4(temp_ctrl);
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
