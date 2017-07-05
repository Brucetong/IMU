#ifndef __LED_H
#define __LED_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "utask.h"

#define LIGHT_ON 20
#define LIGHT_OFF MAX_PWM_WITH
#define BRIGHTNESS_REVERSE 100
#define LED_MAX_ACTION 8


typedef enum
{
	LED_OFF,
	LED_ON,
	LED_WAIT_ON
}LED_STATE;

typedef enum
{
   CODE_ERROR,
   SYS_INIT,
   IMU_HEATING,

   NORMAL_LED = 255
}LED_ID;

typedef enum
{
    RED,
    GREEN,
	BLUE,
	YELLOW,
	PURPLE,
	CYAN,
	WHITE,
	BLACK
}LED_COLOR;

typedef enum
{
    RED_ALWAYS_ON,//0
	RED_QUICK_FLASH,//1
	RED_SLOW_FLASH,//2
	RED_QUICK_DOUBLE_FLASH,//3
	RED_QUICK_TRIPLE_FLASH,//4
	RED_BLN,//5

    GREEN_ALWAYS_ON,//6
	GREEN_SLOW_FLASH,//7
	GREEN_QUICK_FLASH,//8
	GREEN_QUICK_DOUBLE_FLASH,//9
	GREEN_QUICK_TRIPLE_FLASH,//10
	GREEN_BLN,//11

	BLUE_ALWAYS_ON,//11
	BLUE_QUICK_FLASH,//12
	BLUE_SLOW_FLASH,//13
	BLUE_QUICK_DOUBLE_FLASH,//14
	BLUE_QUICK_TRIPLE_FLASH,//15
	BLUE_BLN,//16

	WHITE_ALWAYS_ON,//17
	WHITE_QUICK_FLASH,//18
	WHITE_SLOW_FLASH,//19
	WHITE_QUICK_DOUBLE_FLASH,//20
	WHITE_QUICK_TRIPLE_FLASH,//21
	WHITE_BLN,//22

	YELLOW_ALWAYS_ON,//23
	YELLOW_QUICK_FLASH,//24
	YELLOW_SLOW_FLASH,//25
	YELLOW_QUICK_DOUBLE_FLASH,//26
	YELLOW_QUICK_TRIPLE_FLASH,//27
	YELLOW_BLN,//28

	PURPLE_ALWAYS_ON,//29
	PURPLE_QUICK_FLASH,//30
	PURPLE_SLOW_FLASH,//31
	PURPLE_QUICK_DOUBLE_FLASH,//31
	PURPLE_QUICK_TRIPLE_FLASH,//32
	PURPLE_BLN,//33

	CYAN_ALWAYS_ON,//34
	CYAN_QUICK_FLASH,//35
	CYAN_SLOW_FLASH,//36
	CYAN_QUICK_DOUBLE_FLASH,//37
	CYAN_QUICK_TRIPLE_FLASH,//38
	CYAN_BLN,//39

	R_G_B_Y_P_FLASH//40
}LED_FLASH_TYPE;

typedef struct
{
	uint8_t color;
	int8_t brightness;
	uint8_t ticks;
	int8_t slope;
}led_single_action;

typedef struct
{
	uint8_t flash_type;
	led_single_action action[LED_MAX_ACTION];
}led_action;

typedef struct
{
	uint8_t id;
	uint8_t enable;
	uint8_t times;
	uint8_t priori;
	uint8_t flash_type;
	char * desc;
}led_priv;

const static led_action led_flash_table[] =
{
	{RED_ALWAYS_ON, {{RED, 100, 50, 0},}},
	{RED_QUICK_FLASH, {{RED, 100, 10, 0},
						{BLACK, 0, 10, 0},}},
	{RED_SLOW_FLASH, {{RED, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{RED_QUICK_DOUBLE_FLASH, {{RED, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{RED, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{RED_QUICK_TRIPLE_FLASH, {{RED, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{RED, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{RED, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{RED_BLN, {{RED, 0, 100, 1},
				{RED, 100, 100, -1},}},

	{GREEN_ALWAYS_ON, {{GREEN, 100, 50, 0},}},
	{GREEN_QUICK_FLASH, {{GREEN, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{GREEN_SLOW_FLASH, {{GREEN, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{GREEN_QUICK_DOUBLE_FLASH, {{GREEN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{GREEN, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{GREEN_QUICK_TRIPLE_FLASH, {{GREEN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{GREEN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{GREEN, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{GREEN_BLN, {{GREEN, 0, 100, 1},
				{GREEN, 100, 100, -1},}},

	{BLUE_ALWAYS_ON, {{BLUE, 100, 50, 0},}},
	{BLUE_QUICK_FLASH, {{BLUE, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{BLUE_SLOW_FLASH, {{BLUE, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{BLUE_QUICK_DOUBLE_FLASH, {{BLUE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{BLUE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{BLUE_QUICK_TRIPLE_FLASH, {{BLUE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{BLUE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{BLUE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{BLUE_BLN, {{BLUE, 0, 100, 1},
				{BLUE, 100, 100, -1},}},

	{WHITE_ALWAYS_ON, {{WHITE, 100, 50, 0},}},
	{WHITE_QUICK_FLASH, {{WHITE, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{WHITE_SLOW_FLASH, {{WHITE, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{WHITE_QUICK_DOUBLE_FLASH, {{WHITE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{WHITE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{WHITE_QUICK_TRIPLE_FLASH, {{WHITE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{WHITE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{WHITE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{WHITE_BLN, {{WHITE, 0, 100, 1},
				{WHITE, 100, 100, -1},}},

	{YELLOW_ALWAYS_ON, {{YELLOW, 100, 50, 0},}},
	{YELLOW_QUICK_FLASH, {{YELLOW, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{YELLOW_SLOW_FLASH, {{YELLOW, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{YELLOW_QUICK_DOUBLE_FLASH, {{YELLOW, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{YELLOW, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{YELLOW_QUICK_TRIPLE_FLASH, {{YELLOW, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{YELLOW, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{YELLOW, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{YELLOW_BLN, {{YELLOW, 0, 100, 1},
				{YELLOW, 100, 100, -1},}},

	{PURPLE_ALWAYS_ON, {{PURPLE, 100, 50, 0},}},
	{PURPLE_QUICK_FLASH, {{PURPLE, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{PURPLE_SLOW_FLASH, {{PURPLE, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{PURPLE_QUICK_DOUBLE_FLASH, {{PURPLE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{PURPLE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{PURPLE_QUICK_TRIPLE_FLASH, {{PURPLE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{PURPLE, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{PURPLE, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{PURPLE_BLN, {{PURPLE, 0, 100, 1},
				{PURPLE, 100, 100, -1},}},

	{CYAN_ALWAYS_ON, {{CYAN, 100, 50, 0},}},
	{CYAN_QUICK_FLASH, {{CYAN, 100, 10, 0},
							{BLACK, 0, 10, 0},}},
	{CYAN_SLOW_FLASH, {{CYAN, 100, 50, 0},
						{BLACK, 0, 50, 0},}},
	{CYAN_QUICK_DOUBLE_FLASH, {{CYAN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{CYAN, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{CYAN_QUICK_TRIPLE_FLASH, {{CYAN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{CYAN, 100, 10, 0},
							{BLACK, 0, 10, 0},
							{CYAN, 100, 10, 0},
							{BLACK, 0, 80, 0},}},
	{CYAN_BLN, {{CYAN, 0, 100, 1},
				{CYAN, 100, 100, -1},}},

	{R_G_B_Y_P_FLASH, {{RED, 100, 25, 0},
						{GREEN, 100, 25, 0},
						{BLUE, 100, 25, 0},
						{YELLOW, 100, 25, 0},
						{PURPLE, 100, 25, 0},}},
};

static led_priv led_table[] =
{
	{CODE_ERROR, 0, 1, 0, RED_ALWAYS_ON, "code error"},
	{SYS_INIT, 0, 3, 1, R_G_B_Y_P_FLASH, "system initializing"},
	{IMU_HEATING, 0, 2, 2, RED_BLN, "imu_heating"},

	{3, 0, 1, 100, RED_ALWAYS_ON, "TEST"},
	{4, 0, 1, 101, RED_QUICK_FLASH, "TEST"},
	{5, 0, 1, 102, RED_SLOW_FLASH, "TEST"},
	{6, 0, 1, 103, RED_QUICK_DOUBLE_FLASH, "TEST"},
	{7, 0, 1, 104, RED_QUICK_TRIPLE_FLASH, "TEST"},
	{8, 0, 1, 105, RED_BLN, "TEST"},

	{9, 0, 1, 100, GREEN_ALWAYS_ON, "TEST"},
	{10, 0, 1, 100, GREEN_SLOW_FLASH, "TEST"},
	{11, 0, 1, 100, GREEN_QUICK_FLASH, "TEST"},
	{12, 0, 1, 100, GREEN_QUICK_DOUBLE_FLASH, "TEST"},
	{13, 0, 1, 100, GREEN_QUICK_TRIPLE_FLASH, "TEST"},
	{14, 0, 1, 100, GREEN_BLN, "TEST"},

	{15, 0, 1, 100, BLUE_ALWAYS_ON, "TEST"},
	{16, 0, 1, 100, BLUE_QUICK_FLASH, "TEST"},
	{17, 0, 1, 100, BLUE_SLOW_FLASH, "TEST"},
	{18, 0, 1, 100, BLUE_QUICK_DOUBLE_FLASH, "TEST"},
	{19, 0, 1, 100, BLUE_QUICK_TRIPLE_FLASH, "TEST"},
	{20, 0, 1, 100, BLUE_BLN, "TEST"},

	{21, 0, 1, 100, WHITE_ALWAYS_ON, "TEST"},
	{22, 0, 1, 100, WHITE_QUICK_FLASH, "TEST"},
	{23, 0, 1, 100, WHITE_SLOW_FLASH, "TEST"},
	{24, 0, 1, 100, WHITE_QUICK_DOUBLE_FLASH, "TEST"},
	{25, 0, 1, 100, WHITE_QUICK_TRIPLE_FLASH, "TEST"},
	{26, 0, 1, 100, WHITE_BLN, "TEST"},

	{27, 0, 1, 100, YELLOW_ALWAYS_ON, "TEST"},
	{28, 0, 1, 100, YELLOW_QUICK_FLASH, "TEST"},
	{29, 0, 1, 100, YELLOW_SLOW_FLASH, "TEST"},
	{30, 0, 1, 100, YELLOW_QUICK_DOUBLE_FLASH, "TEST"},
	{31, 0, 1, 100, YELLOW_QUICK_TRIPLE_FLASH, "TEST"},
	{32, 0, 1, 100, YELLOW_BLN, "TEST"},

	{33, 0, 1, 100, PURPLE_ALWAYS_ON, "TEST"},
	{34, 0, 1, 100, PURPLE_QUICK_FLASH, "TEST"},
	{35, 0, 1, 100, PURPLE_SLOW_FLASH, "TEST"},
	{36, 0, 1, 100, PURPLE_QUICK_DOUBLE_FLASH, "TEST"},
	{37, 0, 1, 100, PURPLE_QUICK_TRIPLE_FLASH, "TEST"},
	{38, 0, 1, 100, PURPLE_BLN, "TEST"},

	{39, 0, 1, 100, CYAN_ALWAYS_ON, "TEST"},
	{40, 0, 1, 100, CYAN_QUICK_FLASH, "TEST"},
	{41, 0, 1, 100, CYAN_SLOW_FLASH, "TEST"},
	{42, 0, 1, 100, CYAN_QUICK_DOUBLE_FLASH, "TEST"},
	{43, 0, 1, 100, CYAN_QUICK_TRIPLE_FLASH, "TEST"},
	{44, 0, 1, 100, CYAN_BLN, "TEST"},

	{45, 0, 3, 100, R_G_B_Y_P_FLASH, "TEST"},

	{NORMAL_LED, 1,  1, 250, GREEN_QUICK_DOUBLE_FLASH, "normal led"}
};

void set_led_status(uint8_t enable_id, uint8_t enable);
void set_rgb_led_color(uint8_t color, uint8_t brightness);
void register_led_table(led_priv *led_table, uint8_t size);
void register_led_flash_table(led_action *led_flash_table, uint8_t size);
void led_control(void);
void test_led_table(void);
void led_event_check(void);

#endif
/************************ (C) COPYRIGHT  *****END OF FILE****/
