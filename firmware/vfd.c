#define F_CPU 8000000UL //8 MHz

#define CONTROLPORT PORTB
#define CONTROL_DDR DDRB

#define DATAPORT PORTD
#define DATA_DDR DDRD

/*
ARDUINO DUEMILANOVE MAPPING FOR THE VFD:

VFD pin #:		VFD pin name:	ATMEGA 328 pin name:	Arduino pin name:
1,3,5,7,9,11,13,15	D7-0		PD7-PD0			Digital 7-0

2			GND		GND			GND

4,6,8,10,12,14,16,18,	GND		NC			-
22,24,2

17			!WR		PB5			Digital 13
19			A0		PB4			Digital 12
20			RESET		PB3			Digital 11
21			!RD		PB2			Digital 10
23			!CS		PB1			Digital 9
25			T0		NC				-
*/


#include <avr/io.h>
#include <util/delay.h>

void write_data(char,char); //1st char=0 for ASCII data, and !=0 for a command to the VFD , 2. char is data/command
void setup_io();
void setup_vfd();

int main(void)
{
	const char swears[]="horsefucker kasper donkey anal anus arse ass ballsack balls bastard bitch biatch bloody blowjob bollock boner boob bugger bum butt buttplug clitoris cock coon crap cunt damn dick dildo dyke fag feck fellate fellatio felching fuck fudgepacker fudge packer flange Goddamn God damn hell jerk jizz knobend labia muff penis piss poop prick pube pussy scrotum sex shit slut smegma spunk tits tosser turd twat vagina wank whore \0";

	_delay_ms(500);

	setup_io();
	_delay_ms(10);
	setup_vfd();

	int n=0;
	int i=0;
	int z=0;

	while (1)
	{
	n=i+1;
	z=i+41;
	while(i<n && n<z)
		{
		write_data(0,swears[n]);
		_delay_us(1000);
		n++;
		}
	i++;
	_delay_ms(250);

	if (i > 80)
		{
		i=0;
		}
	}
}

void setup_io()
{
//OUTPUT ALL THE THINGS!!!!
CONTROL_DDR = 0xff;
DATA_DDR = 0xff;

DATAPORT = 0x00;
CONTROLPORT = 0x00;

CONTROLPORT &= ~(1 << PB1); //Dig9 <-> VFD23 = !CS
}

void setup_vfd()
{
	CONTROLPORT |= 1 << PB3; //dig11 <-> VFD20 = reset

	_delay_ms(100);

	CONTROLPORT &=~(1 << PB3);

	_delay_ms(500);

	CONTROLPORT |= 1 << PB2; //dig10 <-> VFD21 = !RD 


//	write_data(1,0x13); //DC3 - Horizontal Scroll mode
//	write_data(1,0x17); //DC7 - carriage is flashing underline
//	write_data(1,0x1c); //danish character set :D

}

void write_data(char command,char data)
{
	if(command) CONTROLPORT |=1 << PB4; //dig12 <-> VFD19 = A0  (command/!data)
	else CONTROLPORT &=~(1 << PB4); //dig12 <-> VFD19 = A0  (command/!data)

	CONTROLPORT &=~(1 << PB5); //dig13 <-> VFD17 = Write enable pin toggle.

	DATAPORT = data;

	_delay_us(10);

	CONTROLPORT |= 1 << PB5; //dig13 <-> VFD17 = Write enable pin toggle.

	_delay_us(10);


}


