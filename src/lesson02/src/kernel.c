#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	printf("Print something\r\n");
	//int el = 0;
	//printf("Exception level: %d \r\n", el);
	while (1) {
		uart_send(uart_recv());
	}
}
