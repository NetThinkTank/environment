#include "FastLED.h"

#define NUM_LEDS 300
#define DATA_PIN 5

int firstLED = 0;
int lastLED = 299;

unsigned int RED[76] = {
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	238,221,204,188,171,154,137,119,102,85,68,51,34,17,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	17,35,52,68,86,102,120,136,154,170,188,204,221,238,255
};

unsigned int GREEN[76] = {
	0,17,34,51,68,85,102,119,136,153,170,187,204,221,238,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	238,221,204,188,170,154,136,120,102,86,68,52,34,18,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned int BLUE[76] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	18,34,52,68,86,102,120,136,154,170,188,204,221,238,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

bool debug = false;

int firstColorIndex = 0;
int currentColorIndex = 0;

CRGB leds[NUM_LEDS];

String inputString = "";
int inputLength = 0;
boolean stringComplete = false;

boolean loopedOnce = false;


void setup() {
	Serial.begin(9600);
	delay(1000);
	inputString.reserve(200);

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void blink(int times) {
	for (int i = 0; i < times; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(250);

		digitalWrite(LED_BUILTIN, LOW);
		delay(250);
	}
}


int getNextColor() {
	int color;

	if (currentColorIndex == 0) {
		currentColorIndex = firstColorIndex;
		color = inputString.charAt(currentColorIndex);

		return color;
	}

	currentColorIndex++;

	if (currentColorIndex >= inputLength - 1) {
		currentColorIndex = firstColorIndex;
	}

	color = inputString.charAt(currentColorIndex);
	return color;
}


void clearAll() {
	for (int i = 0; i <= lastLED; i++) {
		leds[i] = CRGB::Black;
	}

	FastLED.show();
}


void setAll() {
	clearAll();

	int color = inputString.charAt(1);

	for (int i = firstLED; i <= lastLED; i++) {
		leds[i].red = RED[color];
		leds[i].green = GREEN[color];
		leds[i].blue = BLUE[color];
	}

	FastLED.show();
}


void led() {
	int color = inputString.charAt(1);
	int ledNumber = inputString.charAt(2);

	ledNumber = lastLED - ledNumber;

	leds[ledNumber].red = RED[color];
	leds[ledNumber].green = GREEN[color];
	leds[ledNumber].blue = BLUE[color];

	FastLED.show();
}


void ledTest(int arg1, int arg2) {
	int color = arg1;
	int ledNumber = arg2;

	ledNumber = lastLED - ledNumber;

	leds[ledNumber].red = RED[color];
	leds[ledNumber].green = GREEN[color];
	leds[ledNumber].blue = BLUE[color];

	FastLED.show();
}


void ledRange() {
	int color;

	int startLED1 = inputString.charAt(1);
	int startLED2 = inputString.charAt(2);
	int startLED3 = inputString.charAt(3);

	int endLED1 = inputString.charAt(4);
	int endLED2 = inputString.charAt(5);
	int endLED3 = inputString.charAt(6);

	int increment = inputString.charAt(7);

	firstColorIndex = 8;

	int startLED = startLED1 * 100 + startLED2 * 10 + startLED3;
	int endLED = endLED1 * 100 + endLED2 * 10 + endLED3;

	if (startLED > endLED) {
		increment = increment * -1;
	}

	currentColorIndex = 0;

	if (startLED <= endLED) {
		for (int i = startLED; i <= endLED; i += increment) {
			color = getNextColor();

			if (color == 100) {
				leds[i].red = 0;
				leds[i].green = 0;
				leds[i].blue = 0;

				continue;
			}

			if (color == 101) {
				leds[i].red = 255;
				leds[i].green = 255;
				leds[i].blue = 255;

				continue;
			}

			leds[i].red = RED[color];
			leds[i].green = GREEN[color];
			leds[i].blue = BLUE[color];
		}
	} else { // startLED > endLED {
		for (int i = startLED; i >= endLED; i += increment) {
			color = getNextColor();

			if (color == 100) {
				leds[i].red = 0;
				leds[i].green = 0;
				leds[i].blue = 0;

				continue;
			}

			if (color == 101) {
				leds[i].red = 255;
				leds[i].green = 255;
				leds[i].blue = 255;

				continue;
			}

			leds[i].red = RED[color];
			leds[i].green = GREEN[color];
			leds[i].blue = BLUE[color];
		}
	} // end if

	FastLED.show();
}


int getNumber(int startIndex) {
	int firstDigit = inputString.charAt(startIndex);
	int secondDigit = inputString.charAt(startIndex + 1);
	int thirdDigit = inputString.charAt(startIndex + 2);

	int number = firstDigit * 100 + secondDigit * 10 + thirdDigit;

	// Serial.println(firstDigit);
	// Serial.println(secondDigit);
	// Serial.println(thirdDigit);
	// Serial.println(number);

	return number;
}


void loop() {

	if (debug && loopedOnce) { return; }

	if (debug) {
		// debug code here

		loopedOnce = true;
		return;
	}

	if (stringComplete) {
		inputLength = inputString.length();
		int command = inputString.charAt(0);

		if (command == 32) {
			clearAll();
		}

		else if (command == 33) {
			setAll();
		}

		else if (command == 34) {
			led();
		}

		else if (command == 35) {
			ledRange();
		}

		loopedOnce = true;
		inputString = "";
		stringComplete = false;
	}

} // end loop


void serialEvent() {
	if (Serial.available()) {
		char c = (char)Serial.read();

		inputString += c;

		if (c == 127) {
			stringComplete = true;
		}
	}
}
