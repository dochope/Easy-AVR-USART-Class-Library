#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usart.hpp"

USART uart0, uart1, serial;

int main(void)
{
	uart0.init(); // default 9600kbps 
	uart1.init(BAUD_CALC(115200), 1);
	
	serial.begin(BAUD_CALC(19200), 3);
	
	sei(); // enable interrupts, library wouldn't work without this
	
	uart0.puts("hello from usart 0\r\n"); // write const string to usart buffer 
	// if you do not have enough SRAM memory space to keep all strings, try to use puts_P instead
	uart1.puts_P("hello from flashed, usart 1\r\n"); // write string to usart buffer from flash memory
	
	serial.puts("fake arduino serial coming soon\r\n");
	serial.putfloat(0.1337f);
	
	char buffer[13];
	
	serial << "say my name : \r\n";
	serial >> buffer;
	serial << "so it's " << buffer << "\r\n you have got dam'n right\r\n" << 1111 << 1.234;
	
	while(1)
	{
		uart0.puts("bytes waiting in buffer : ");
		uart0.putint(uart0.AvailableBytes()); // ask for bytes waiting in receiver buffer
		uart0.getln(buffer, 13); // read 12 bytes or one line from usart buffer
		uart0.puts("/r/n");
		
		uart1.putstr(buffer); // write string array to usart buffer
		_delay_ms(5000);
	}
}