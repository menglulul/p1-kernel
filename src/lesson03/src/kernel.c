#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "mini_uart.h"
#include "utils.h"


void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	printf("kernel boots...\n");

	irq_vector_init();
	//generic_timer_init();

	enable_interrupt_controller();
	enable_irq();
	
	asm volatile("HVC #3"   "\n\t");
	handle_irq();

	while (1){
		uart_send(uart_recv());
	}	
}
