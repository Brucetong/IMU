/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
#include "spi.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    /**SPI1 GPIO Configuration    
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

  }
  /* USER CODE BEGIN SPI1_MspDeInit 1 */
  __HAL_SPI_ENABLE(&hspi1);
  SPI1_ReadWrite_Byte(0xff);
  /* USER CODE END SPI1_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */
 void set_ncs()
 {
	 GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	 GPIO_InitStruct.Pin = GPIO_PIN_8;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
 }
SPI_HandleTypeDef SPI1_Handler; //SPI1 句柄
void SPI1_Init(void)
{
	set_ncs();
	SPI1_Handler.Instance=SPI1;
	//SP2
	SPI1_Handler.Init.Mode=SPI_MODE_MASTER; //设置 SPI 工作模式,设置为主模式
	SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;//SPI 设置为双线模式
	SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;
	// SPI 发送接收 8 位帧结构
	SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_LOW; //同步时钟空闲状态为高电平
	SPI1_Handler.Init.CLKPhase=SPI_PHASE_1EDGE; //同步时钟第二个跳变沿采样数据
	SPI1_Handler.Init.NSS=SPI_NSS_SOFT; //内部 NSS 信号有 SSI 位控制
	SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;
	//定义波特率预分频的值:波特率预分频值为 256
	SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB; //数据传输从 MSB 位开始
	SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE; //关闭 TI 模式
	SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
	//关闭硬件 CRC 校验
	SPI1_Handler.Init.CRCPolynomial=7;
	//CRC 值计算的多项式
	HAL_SPI_Init(&SPI1_Handler);
	__HAL_SPI_ENABLE(&SPI1_Handler);
	//使能 SPI1
	SPI1_Read_Write_Byte(0Xff);
	//启动传输
}


//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);
	return Rxdata;
//返回收到的数据
}
/************************************************************************
** 函数名称:uint8_t SPI3_ReadWrite_Byte(uint8_t byte)
** 功能描述:  发送或者接收1个字节
** 输　入:    byte    发送时候,byte传递为发送的数据字节， 接收的时候，则固定为0xff
** 输　出:    SPI3->DR  发送时候，可以忽略, 接收的时候，则为接收数据
***********************************************************************/
uint8_t SPI1_ReadWrite_Byte(uint8_t byte)
{
	/*等待发送寄存器空*/
	while((SPI1->SR & SPI_I2S_FLAG_TXE)==RESET);
	SPI1->DR = byte;  //发送一个字节
	/* 等待接收寄存器有效*/
	while((SPI1->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(SPI1->DR);
}

//写寄存器
void SPI1_writeReg(uint8_t reg ,uint8_t data){
	SPI1_ReadWrite_Byte(reg);
	SPI1_ReadWrite_Byte(data);
}

//读寄存器
uint8_t SPI1_readReg(uint8_t reg){
	SPI1_ReadWrite_Byte(reg|0x80);
	return SPI1_ReadWrite_Byte(0xff);
}

//从寄存器读出多个字节[寄存器地址要自动增加]
void SPI1_readRegs(uint8_t reg, uint8_t length, uint8_t *data){
	uint8_t count = 0;
	SPI1_ReadWrite_Byte(reg|0x80);
	for(count=0;count<length;count++){
		data[count] = SPI1_ReadWrite_Byte(0xff);
	}
}


/*
 * 函数名：MPU6500_Write_Reg
 * 描述  ：SPI写入寄存器
 * 输入  ：reg:指定的寄存器地址；value：写入的值
 * 输出  ：status：返回状态值
 */
uint8_t MPU6500_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU6500_CS(0);  										//使能SPI传输
	status = SPI1_Read_Write_Byte(reg); //发送写命令+寄存器号
	SPI1_Read_Write_Byte(value);				//写入寄存器值
	MPU6500_CS(1);  										//禁止MPU9500
	return(status);											//返回状态值
}


/*
 * 函数名：MPU6500_Read_Reg
 * 描述  ：SPI读取寄存器
 * 输入  ：reg:指定的寄存器地址
 * 输出  ：reg_val：reg寄存器地址对应的值
 */
uint8_t MPU6500_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	MPU6500_CS(0);  										//使能SPI传输
	//SPI1_Read_Write_Byte(reg | 0x80); 	//发送读命令+寄存器号SPI1_ReadWriteByte
	//reg_val = SPI1_Read_Write_Byte(0xff); //读取寄存器值
	SPI1_ReadWrite_Byte(reg | 0x80); 	//发送读命令+寄存器号
	reg_val = SPI1_ReadWrite_Byte(0xff); //读取寄存器值
	MPU6500_CS(1);  									  //禁止MPU9250
	return(reg_val);
}

/*
 * 函数名：SPI1_Read_Write_Byte
 * 描述  ：读写一个字节
 * 输入  ：TxData:要写入的字节
 * 输出  ：读取到的字节
 */
uint8_t SPI1_Read_Write_Byte(uint8_t TxData)
{
	uint8_t retry = 0;
	while ((SPI1->SR & SPI_I2S_FLAG_TXE)==RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry > 250)	return 0;
		}
	SPI1->DR = TxData; 																//通过外设SPIx发送一个数据
	retry = 0;

	while ((SPI1->SR & SPI_I2S_FLAG_RXNE)==RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry > 250) return 0;
	}
	return (SPI1->DR); 															//返回通过SPIx最近接收的数据
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
