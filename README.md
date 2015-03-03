# Easy-AVR-USART-Class-Library
easy to use C++ avr uart class library

# Features
-todo

int main(void)
{
	USART uart0;
    sei();
    while(1)
    {
		uart0.puts("hello world\r\n");
		_delay_ms(1000);
    }
}



gives

         text      data       bss       dec       hex   filename
          502        14         2       518       206   avrtpp.elf
   
Program Memory Usage    :   516 bytes   1,6 % Full
Data Memory Usage       :   16 bytes   0,5 % Full

defined  NO_USART_RX flag gives

         text      data       bss       dec       hex   filename
          412        14         2       428       1ac   avrtpp.elf

Program Memory Usage    :   426 bytes   1,3 % Full
Data Memory Usage       :   16 bytes   0,5 % Full

Of course the real memory usage is higher by buffers sizes and some variables.



