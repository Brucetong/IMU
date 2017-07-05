#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

static uint8_t  fac_us=0;//usÑÓÊ±±¶³ËÊý
static uint16_t fac_ms=0;

//初始化延时函数
//当使用 FreeRTOS 的时候,此函数会初始化 FreeRTOS 的时钟节拍
//SYSTICK 的时钟固定为 AHB 时钟
//SYSCLK:系统时钟频率
void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
	//SysTick 频率为 HCLK
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	fac_us=SYSCLK;
	//不论是否使用 OS,fac_us 都需要使用
	reload=SYSCLK;
	//每秒钟的计数次数 单位为 K
	reload*=1000000/configTICK_RATE_HZ;
	//根据 configTICK_RATE_HZ 设定溢出时间
	//reload 为 24 位寄存器,最大值:16777216,
	//在 180M 下,约合 0.745s 左右
	fac_ms=1000/configTICK_RATE_HZ;
	//代表 OS 可以延时的最少单位
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启 SYSTICK 中断
	SysTick->LOAD=reload;
	//每 1/configTICK_RATE_HZ 断一次
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启 SYSTICK
}
//延时 nus
//nus:要延时的 us 数.
//nus:0~190887435(最大值即 2^32/fac_us@fac_us=22.5)
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;
	//LOAD 的值
	ticks=nus*fac_us;
	//需要的节拍数
	told=SysTick->VAL;
	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			//这里注意一下 SYSTICK 是一个递减的计数器就可以了.
			if(tnow<told)tcnt+=told-tnow;
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;
			//时间超过/等于要延迟的时间,则退出.
		}
	};
}
//延时 nms,会引起任务调度
//nms:要延时的 ms 数
//nms:0~65535
void delay_ms(uint32_t nms)
{
	if(xTaskGetSchedulerState()!= taskSCHEDULER_NOT_STARTED)//系统已经运行
	{
		if(nms>=fac_ms)
		//延时的时间大于 OS 的最少时间周期
		{
			vTaskDelay(nms/fac_ms);
			//FreeRTOS 延时
		}
		nms%=fac_ms;
		//OS 已经无法提供这么小的延时了,
		//采用普通方式延时
	}
	delay_us((uint32_t)(nms*1000));
	//普通方式延时
}
//延时 nms,不会引起任务调度
//nms:要延时的 ms 数
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

