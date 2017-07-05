#include "utask.h"

volatile g_status_t g_status = {0};
volatile g_real_t g_real = {0};

task_table_t *fast_table_ptr = NULL;
uint8_t fast_table_size = 0;
task_table_t *slow_table_ptr = NULL;
uint8_t slow_table_size = 0;
task_table_t *super_table_ptr = NULL;
uint8_t super_table_size = 0;


void register_task_table(task_table_t *fast_task_table, uint8_t fast_size,
		task_table_t *slow_task_table, uint8_t slow_size,
		task_table_t *super_task_table, uint8_t super_size)
{
	if ((NULL == fast_task_table) || (NULL == slow_task_table) || (NULL == super_task_table)
			|| (0 == fast_size) || (0 == slow_size) || (0 == super_size))
	{
		g_status.task_state = 1;
		return;
	}

	fast_table_ptr = fast_task_table;
	fast_table_size = fast_size;
	slow_table_ptr = slow_task_table;
	slow_table_size = slow_size;
	super_table_ptr = super_task_table;
	super_table_size = super_size;
}

void slow_task()
{
	static uint32_t cnt = 0;

	if ((NULL == slow_table_ptr) || (0 == slow_table_size))
	{
		g_status.task_state = 1;
		return;
	}

	for (uint8_t i = 0; i < slow_table_size; i++)
	{
		if (0 == (cnt % slow_table_ptr[i].freq))
		{
			slow_table_ptr[i].handler();
		}
	}

	cnt++;
}

void fast_task()
{
	static uint32_t cnt = 0;

	if ((NULL == fast_table_ptr) || (0 == fast_table_size))
	{
		g_status.task_state = 1;
		return;
	}

	for (uint8_t i = 0; i < fast_table_size; i++)
	{
		if (0 == (cnt % fast_table_ptr[i].freq))
		{
			fast_table_ptr[i].handler();
		}
	}

	cnt++;
}

void super_task()
{
	static uint32_t cnt = 0;

	if ((NULL == super_table_ptr) || (0 == super_table_size))
	{
		g_status.task_state = 1;
		return;
	}

	for (uint8_t i = 0; i < super_table_size; i++)
	{
		if (0 == (cnt % super_table_ptr[i].freq))
		{
			super_table_ptr[i].handler();
		}
	}

	cnt++;
}

/*********************** *****END OF FILE****/
