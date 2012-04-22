#define F_CPU 8000000UL //8 MHz

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define CONTROLPORT PORTB
#define CONTROL_DDR DDRB

#define DATAPORT1 PORTD
#define DATA_DDR1 DDRD

#define DATAPORT2 PORTC
#define DATA_DDR2 DDRC


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "uart.c"

void write_data(char,char); //1st char=0 for ASCII data, and !=0 for a command to the VFD , 2. char is data/comm
void setup_io();
void setup_uart();
void setup_vfd();
void parse_cmd(char);

char mode = 1;
char display_array[39];
char *write_pointer;


int main(void)
{
	while(1)
	{
		sei;
	}
}

void setup_io()
{
//OUTPUT ALL THE THINGS!!!!
CONTROL_DDR = 0xff;
DATA_DDR1 = 0xff;
DATA_DDR2 = 0xff;

DATAPORT1 = 0x00;
DATAPORT2 = 0x00;
CONTROLPORT = 0x00;

CONTROLPORT &= ~(1 << PB1); //Dig9 <-> VFD23 = !CS
}

void setup_uart()
{
        UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

        UBRR0H = (BAUD_PRESCALE >> 8);
        UBRR0L = BAUD_PRESCALE;

	stdout = stdin = &uart_str;
}

void setup_vfd()
{
        CONTROLPORT |= 1 << PB3; //dig11 <-> VFD20 = reset

        _delay_ms(100);

        CONTROLPORT &=~(1 << PB3);

        _delay_ms(500);

        CONTROLPORT |= 1 << PB2; //dig10 <-> VFD21 = !RD
}

void write_data(char command,char data)
{
        if(command) CONTROLPORT |=1 << PB4; //dig12 <-> VFD19 = A0  (command/!data)
        else CONTROLPORT &=~(1 << PB4); //dig12 <-> VFD19 = A0  (command/!data)

        CONTROLPORT &=~(1 << PB5); //dig13 <-> VFD17 = Write enable pin toggle.

        DATAPORT1 = (0xfc & data);
        DATAPORT2 = (0x03 & data);

        _delay_us(10);

        CONTROLPORT |= 1 << PB5; //dig13 <-> VFD17 = Write enable pin toggle.

        _delay_us(10);


}

ISR(USART_RX_vect)
{
	char newchar;

	newchar = UDR0;

	UDR0 = newchar;

	if(mode == '1')
	{
		parse_cmd(newchar);
	} else if(mode == '2') {
		if(newchar == 0x0d) // if CR recived
		{
			printf("\nRecived CR, saving data and exiting");
			mode = '1';
		} else {
			*write_pointer = newchar;
			write_pointer++;
			if(write_pointer == (void *)&display_array[39])
			{
				printf("\nDisplay array full, saving data and exiting");
				mode = '1';
			}
		}
	} else if(mode == '3') {
		// grab new settings
	}
}

void parse_cmd(char cmd)
{
	if(cmd == 'd')
	{
		write_pointer = &display_array[0];
		memset(display_array, 0, sizeof(display_array));
		mode = '2';
	} else if(cmd == 'w') {
		//save data to eeprom
		printf("\nWrote display array to EEPROM");
	} else if(cmd == 's') {
		printf("\nSettings:");
		// Output all settings
		mode = '3';
	} else {
		printf("\nUnkown command");
	}
}
