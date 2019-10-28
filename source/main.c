#include <avr/io.h>
#include "io.h"


int main(void) {
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	LCD_DisplayString(1, "Hello World");
	while(1) {continue;}
    return 1;
}
