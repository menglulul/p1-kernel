#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"

void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(5000000);
		}
		schedule(); // yield
	}
}

void process1(char *array)
{
	for (int i = 0; i < 5; i++){
		uart_send(array[i]);
		delay(5000000);
	}
	schedule();
}

void process2(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(5000000);
		}
		change_state(TASK_WAIT);
		//change_state(TASK_RUNNING);
		schedule(); // yield
	}
}

void idle_task()
{
	while (1){
		printf("begin idle\n");
		asm("WFI");
		printf("end idle\n");
		schedule(); // yield
	}
}

void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);

	printf("kernel boots\n");

	irq_vector_init();
	generic_timer_init();
	enable_interrupt_controller();
	enable_irq();

	int res = copy_process((unsigned long)&process1, (unsigned long)"12345");
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}

	res = copy_process((unsigned long)&process2, (unsigned long)"abcde");
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}

	res = copy_process((unsigned long)&idle_task, (unsigned long)"array");
	if (res != 0) {
		printf("error while starting idle task");
		return;
	}


	while (1){
		schedule();
	}	
}
