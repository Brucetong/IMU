#include "led.h"


led_priv *led_table_ptr = NULL;
uint8_t led_table_size = 0;

led_action *led_flash_table_ptr = NULL;
uint8_t led_flash_table_size = 0;


void register_led_flash_table(led_action *led_flash_table, uint8_t size)
{
	if ((NULL == led_flash_table) || (0 == size))
	{
		printf("error,led table is null or size:%d, func:%s, line:%d\n",
				size, __FUNCTION__, __LINE__);
		return;
	}

	led_flash_table_ptr = led_flash_table;
	led_flash_table_size = size;
}

void register_led_table(led_priv *led_table, uint8_t size)
{
	if ((NULL == led_table) || (0 == size))
	{
		printf("error,led table is null or size:%d, func:%s, line:%d\n",
				size, __FUNCTION__, __LINE__);
		return;
	}

	led_table_ptr = led_table;
	led_table_size = size;
}

void set_rgb_led_color(uint8_t color, uint8_t brightness)
{
	uint32_t r;
	uint32_t g;
	uint32_t b;

	brightness = BRIGHTNESS_REVERSE - brightness;

	switch(color)
	{
	case RED:
		r = LIGHT_ON * brightness;
		g = LIGHT_OFF;
		b = LIGHT_OFF;
		set_led_rgb(r, g, b);
		break;
	case GREEN:
		r = LIGHT_OFF;
		g = LIGHT_ON * brightness;
		b = LIGHT_OFF;
		set_led_rgb(r, g, b);
		break;
	case BLUE:
		r = LIGHT_OFF;
		g = LIGHT_OFF;
		b = LIGHT_ON * brightness;
		set_led_rgb(r, g, b);
		break;
	case WHITE:
		r = LIGHT_ON * brightness;
		g = LIGHT_ON * brightness;
		b = LIGHT_ON * brightness;
		set_led_rgb(r, g, b);
		break;
	case YELLOW:
		r = LIGHT_ON * brightness;
		g = LIGHT_ON * brightness;
		b = LIGHT_OFF;
		set_led_rgb(r, g, b);
		break;
	case PURPLE:
		r = LIGHT_ON * brightness;
		g = LIGHT_OFF;
		b = LIGHT_ON * brightness;
		set_led_rgb(r, g, b);
		break;
	case CYAN:
		r = LIGHT_OFF;
		g = LIGHT_ON * brightness;
		b = LIGHT_ON * brightness;
		set_led_rgb(r, g, b);
		break;
	case BLACK:
		r = LIGHT_OFF;
		g = LIGHT_OFF;
		b = LIGHT_OFF;
		set_led_rgb(r, g, b);
		break;
	default:
		//printf("bug\n");
		break;
	}

}

void set_led_status(uint8_t enable_id, uint8_t enable)
{
	if ((NULL == led_table_ptr) || (0 == led_table_size) || (enable_id >= led_table_size))
	{
		printf("error,led table is null or size:%d, en_id:%d, func:%s, line:%d\n",
				led_table_size, enable_id, __FUNCTION__, __LINE__);
		return;
	}

	led_table_ptr[enable_id].enable = enable;
	if (enable)
	g_status.led_state = enable_id;
}

uint8_t find_highest_priori_index()
{
	uint8_t index = led_table_size;
	uint8_t priori = 255;

	for (uint8_t i = 0; i < led_table_size; i++)
	{
		if (((LED_ON == led_table_ptr[i].enable) || (LED_WAIT_ON == led_table_ptr[i].enable))
				&& (priori > led_table_ptr[i].priori))
		{
			priori = led_table_ptr[i].priori;
			index = i;
		}
	}

	return index;
}

uint8_t find_action_index(uint8_t color)
{
	uint8_t index = led_flash_table_size;

	for (uint8_t i = 0; i < led_flash_table_size; i++)
	{
		if (color == led_flash_table_ptr[i].flash_type)
		{
			index = i;
			break;
		}
	}
	return index;
}

void test_led_table()
{/*
	for (uint8_t i = 0; i < led_flash_table_size; i++)
	{
		printf("/n/nindex is :%d\n", i);
		for (uint8_t j = 0; j < 8; j++)
		printf("color:%d,brightness:%d,ticks:%d,slope:%d\n",led_flash_table_ptr[i].action[j].color,
				led_flash_table_ptr[i].action[j].brightness,
				led_flash_table_ptr[i].action[j].ticks,
				led_flash_table_ptr[i].action[j].slope);
	}

	for (uint8_t i = 0; i < led_table_size; i++)
	{
		printf("id:%d,en:%d,times:%d,pri:%d,flash:%d,desc:%s\n",
				led_table_ptr[i].id, led_table_ptr[i].enable,
				led_table_ptr[i].times, led_table_ptr[i].priori,
				led_table_ptr[i].flash_type, led_table_ptr[i].desc);
	}*/
	static uint8_t led_id = 2;

	set_led_status(led_id++, LED_OFF);
	set_led_status(led_id, LED_ON);

	if (led_id == 45)
		led_id = 2;
}

void led_event_check()
{
	if (!g_status.task_state)
	{
		set_led_status(SYS_INIT, LED_WAIT_ON);
		g_status.task_state = 2;
	}
	else if (1 == g_status.task_state)
	{
		set_led_status(CODE_ERROR, LED_ON);
	}

	if (!g_status.imu_state)
	{
		set_led_status(IMU_HEATING, LED_ON);
	}
	else
	{
		set_led_status(IMU_HEATING, LED_OFF);
	}
}

void led_control()
{
	static uint8_t index = 0;
	static uint8_t flash_index = 0;
	static uint8_t action_state = 1;
	static uint8_t cur_sigle_flash = 0;
	static uint8_t cur_times = 0;
	static uint8_t cur_index = 0;
	static int8_t brightness = 0;

	if ((NULL == led_table_ptr) || (0 == led_table_size) || (NULL == led_flash_table_ptr) || (0 == led_flash_table_size))
	{
		printf("error,table is null or size1:%d, size2:%d, func:%s, line:%d\n",
				led_table_size, led_flash_table_size, __FUNCTION__, __LINE__);
		return;
	}

	if (1 == action_state)
	{
		action_state = 0;

		index = find_highest_priori_index();

		flash_index = find_action_index(led_table_ptr[index].flash_type);
	}

	if ((led_table_size == index) || (led_flash_table_size == flash_index))
	{
		printf("error  index:%d,flash_index:%d, func:%s,line:%d\n",
				index, flash_index, __FUNCTION__, __LINE__);
		return;
	}

	if (cur_times < led_table_ptr[index].times)
	{
		if (cur_sigle_flash < LED_MAX_ACTION)
		{
			if (((0 == led_flash_table_ptr[flash_index].action[cur_sigle_flash].color)
					&& (0 == led_flash_table_ptr[flash_index].action[cur_sigle_flash].brightness)
					&& (0 == led_flash_table_ptr[flash_index].action[cur_sigle_flash].ticks)
					&& (0 == led_flash_table_ptr[flash_index].action[cur_sigle_flash].slope))
					|| (LED_MAX_ACTION -1 == cur_times))
			{
				cur_times++;
				cur_sigle_flash = 0;
			}

			if (cur_index < led_flash_table_ptr[flash_index].action[cur_sigle_flash].ticks)
			{
				cur_index++;
				brightness += led_flash_table_ptr[flash_index].action[cur_sigle_flash].slope;
				set_rgb_led_color(led_flash_table_ptr[flash_index].action[cur_sigle_flash].color,
						led_flash_table_ptr[flash_index].action[cur_sigle_flash].brightness + brightness);
			}
			else
			{
				cur_sigle_flash++;
				cur_index = 0;
				brightness = 0;
			}
		}
	}
	else
	{
		cur_times = 0;
		action_state = 1;
		cur_sigle_flash = 0;
		brightness = 0;
		cur_index = 0;

		if (LED_WAIT_ON == led_table_ptr[index].enable)
		{
			set_led_status(index, LED_OFF);
		}
	}

}

