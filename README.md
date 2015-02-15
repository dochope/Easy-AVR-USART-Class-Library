# Easy-AVR-USART-Class-Library
easy to use C++ avr uart class library


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
          514         8         2       524       20c   avrtpp.elf
   
Program Memory Usage    :   522 bytes   1,6 % Full
Data Memory Usage       :   10 bytes   0,5 % Full

defined  NO_USART_RX flag gives

         text      data       bss       dec       hex   filename
          424         8         2       434       1b2   avrtpp.elf

Program Memory Usage    :   432 bytes   1,3 % Full
Data Memory Usage       :   10 bytes   0,5 % Full

Of course the real memory usage is higher by buffers sizes and some variables.

next step: rewrite it into C

