#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SEGMENTS 5
#define LEDS 37

unsigned char segmentnums[SEGMENTS] = { PD0,PD1,PD3,PD4,PD5 };

void Delay(unsigned int delay) {
	for (unsigned int i = delay; 0 < i; i--) {
		_delay_ms(1);
	}
}

void CloseLeds(void) {
	unsigned char segmentptr = 0;
	PORTB = 0xff;
	for (; segmentptr < SEGMENTS; segmentptr++) {
		PORTD &= ~(1 << segmentnums[segmentptr]);
	}
}

void open_segment(unsigned char segmentnum) {
	PORTD |= (1 << segmentnums[segmentnum]);
}

void open_a_led(unsigned char lednum, unsigned short sound_delay) {
	PORTD |= (1 << PD6);
	PORTB = 1 << (lednum - ((lednum / 8) * 8));
	open_segment(lednum / 8);
	Delay(sound_delay);
	PORTD &= ~(1 << PD6);
}

#define BUTTON !(PIND&(1 << PORTD2))

void MainLoop(unsigned char lednumber);

void start_spin(unsigned char lednumber) {
	unsigned short delay = 10;
	unsigned short delayus = 0;
	while (delay < 350) {
		open_a_led(lednumber, 1);
		Delay(delay);
		CloseLeds();
		lednumber++;
		if (lednumber == LEDS)
		lednumber -= LEDS;
		delayus += 100;
		if (delayus >= 1000) {
			delayus = 0;
			delay++;
		}
		delay += (delay - 10) / 10;
	}
	Delay(400);
	delay = 0;
	while (delay < 4) {
		Delay(200);
		open_a_led(lednumber, 10);
		Delay(200);
		CloseLeds();
		delay++;
	}
	MainLoop(lednumber);
}

void MainLoop(unsigned char lednumber) {
	open_a_led(0, 0);
	unsigned short total_loop = 0;
	while (1) {
		if (BUTTON)
		start_spin(lednumber);
		lednumber++;
		if (lednumber == LEDS) {
			lednumber -= LEDS;
			total_loop++;
			if (total_loop > 810)
			CloseLeds();
		}
		Delay(1);
	}
}

int main(void) {
	DDRB = 0xff;
	PORTB = 0x01;
	DDRD = 0x7B;
	PORTD = 0x05;
	MainLoop(0);
}
