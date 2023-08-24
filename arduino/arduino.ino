#include "FastLED.h"

// -------------- КОНФИГ
#define NUM_LEDS 48    // Кол-во светодиодов
#define LED_PIN 4      // Пин ленты
#define BAUD_RATE 9600 // Скорость передачи информации по USB

// -------------- КОД
enum
{
	MODE_RED,
	MODE_GREEN,
	MODE_BLUE,
	MODE_WHITE,
	MODE_RAINBOW,
	MODE_CUSTOM,
	NUM_MODES
};

byte bright = 50;
byte speed = 5;
byte mode = MODE_WHITE;
byte temperature = 0;
byte custom_r = 255;
byte custom_g = 255;
byte custom_b = 255;

CRGB leds[NUM_LEDS];

void setup()
{
	FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	Serial.begin(9600);
}

CRGB get_led(int i)
{
	int x = millis() / speed;

	switch (mode)
	{
		case MODE_RED: return CRGB(255, 0, 0);
		case MODE_GREEN: return CRGB(0, 255, 0);
		case MODE_BLUE: return CRGB(0, 0, 255);
		case MODE_WHITE: return CRGB(255, 255, 255 - temperature);
		case MODE_RAINBOW: return CHSV(x + (i * 10) % 255, 255, 255);
		case MODE_CUSTOM: return CRGB(custom_r, custom_g, custom_b);
	}
}

void read_serial()
{
	uint8_t buf[16];
	Serial.readBytes(buf, 16);

	mode = buf[0];
	bright = buf[1];
	speed = buf[2];
	temperature = buf[3];
	custom_r = buf[4];
	custom_g = buf[5];
	custom_b = buf[6];
}

void loop()
{
	if(Serial.available())
		read_serial();

	for (int i = 0; i < NUM_LEDS; i++)
		leds[i] = get_led(i);

	FastLED.setBrightness(bright);
	FastLED.show();
	delay(1);
}
