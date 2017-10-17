#include <LPD6803.h>
#include <TimerOne.h>
//		LED SUIT V0.5 - Spawn of Adrian Brink

//SETUP
/*****************************************************************************/
//Don't forget to also connect the GND wire to your arduino GND.

#pragma region Configuration

//pin config

//pin config - spi
const int dataPin = 16;       // 'blue' wire
const int clockPin = 15;      // 'green' wire

//pin config - input buttons
const int buttonInputPin = 8;

//suit config
int SuitMode = 0;
const int STANDBY_MODE = 0;
const int PARTY_WITH_COLOUR = 1;
const int LOW_POWER_MODE = 2;

//LED strip config 
const int totalLEDs = 50;
LPD6803 strip = LPD6803(totalLEDs, dataPin, clockPin);

//colour config
const int MaxWheelValue = 96;
const int MaxHueValue = 360;
int RandomWheelColour;
int RandomHueColour;

//input config
bool awaitingInput = true;
//input config - analog readings
const int button1_MIN = 40;
const int button1_MAX = 155;
const int button2_MIN = 200;
const int button2_MAX = 245;

//grid config
const int chestGridColumns = 6;
const int chestGridRows = 7;
int chestGridTotal = chestGridColumns * chestGridRows;
int chestGrid[chestGridRows][chestGridColumns] = { //Unfortunately, being an idiot, i started implimenting with [Y:X] instead of [X:Y], too late now :/
	{ 41, 40, 39, 38, 37, 36 },
	{ 30, 31, 32, 33, 34, 35 },
	{ 29, 28, 27, 26, 25, 24 },
	{ 18, 19, 20, 21, 22, 23 },
	{ 17, 16, 15, 14, 13, 12 },
	{ 6, 7, 8, 9, 10, 11 },
	{ 5, 4, 3, 2, 1, 0 },
};

#pragma endregion

void setup() {
	//pin config
	pinMode(buttonInputPin, INPUT);

	//NOTE: Serial implicitly started at 115200 baud

	// The Arduino needs to clock out the data to the pixels, this happens in interrupt timer 1, we can change how often to call the interrupt.
	// Setting CPUmax to 100% will take nearly all all the time to do the pixel updates and a nicer/faster display, especially with strands of over 100 dots.
	// (Note that the max is 'pessimistic', its probably 10% or 20% less in reality)

	strip.setCPUmax(80); // start with 50% CPU usage. up this if the strand flickers or is slow, decrease if timing seems inconsistant - stable at 80% for me
	strip.begin();

	//Set the strip to OFF
	SetChestGridColour(0, 1);

	RandomWheelColour = GetRandomWheelColour();
	RandomHueColour = GetRandomHueColour();
}

const bool testing = false;
void loop() {
	if (testing == true) {

	}
	else {
		int hueColour;
		int wheelColour = random(0, MaxWheelValue);
		while (awaitingInput == true || SuitMode == STANDBY_MODE) {
			hueColour = random(0, MaxHueValue);
			awaitingInput = PleaseSirCanIHaveSomeMore(80, 8, &hueColour);
		}
		PlayVisualSet(SuitMode, &hueColour, wheelColour);
	}
}

#pragma region Visual Sets

void ColourfulViiibes(int startingHueColour, int startingWheelColour) {
	//intro
	//RectractingInverseLines(38);
	////things get trippy
	//for (int i = 0; i < 15; i++) {
	//	BouncingLine(80, startingWheelColour, false);
	//}
	//for (int j = 0; j < MaxWheelValue; j = j + 3) {
	//	GrowingBox(70, j, true, false);
	//}
	//for (int i = 0; i < 15; i++) {
	//	BouncingLine(80, startingWheelColour, false);
	//}


	//intro
	//pixel flash builds up bottom to top
	RandomRainbowPixelBuildUp(1);
	RandomRainbowPixel(80, 1);
	//replace with primitive spin
	PrimitiveGlowSpin(4, 30, startingHueColour, true);
	GlowingHorizontalBar(30, 7, startingHueColour);
	//things get trippy
	for (int i = 0; i < 12; i++) {
		BouncingLine(70, startingWheelColour, false);
	}
	RandomWarpedStar(startingWheelColour, false);
	RandomWarpedStar(startingWheelColour, false);
	for (int j = 0; j < MaxWheelValue; j = j + 6) {
		GrowingBox(70, j, true, false);
	}
	for (int i = 0; i < 8; i++) {
		GrowingBox(110, RandomWheelColour, false, false);
	}
	GrowingBox(70, RandomWheelColour, true, true);
	SetChestGridColour(0, 1);
	delay(70);
	BouncingLine(70, RandomWheelColour, true);
	RandomRainbowPixelBuildUp(0);
	for (int i = 0; i < 4; i++) {
		BouncingLine(70, RandomWheelColour, false);
	}
	BouncingLine(70, RandomWheelColour, true);
	int difference = abs(RandomWheelColour - MaxWheelValue);
	for (int i = RandomWheelColour; i < MaxWheelValue; i++) {
		SetChestGridColour(Wheel(i), 60);
	}
	for (int i = 0; i < difference; i++) {
		SetChestGridColour(Wheel(i), 60);
	}
	RainbowWaves(70, difference);
	//hardcore blade spinning
	RotatingBlade(8, 30, startingWheelColour);
	RotateAntiClockWiseVerticalLine(30, startingWheelColour);

	TheColourfulStripe(5, 3, true, startingWheelColour);

	RotateAntiClockWiseHorizontalLine(30, startingWheelColour);
	for (int i = 0; i < 8; i++) {
		startingWheelColour = random(0, 96);
		RotatingBlade(1, 30, startingWheelColour);
	}
	//strobe build up
	SetChestGridColour(0, 0);
	WhiteFlashBuildUp();
	//full rainbow 
	for (int i = 0; i < MaxWheelValue; i++) {
		SetChestGridColour(Wheel(i), 60);
	}
	BouncingLine(70, MaxWheelValue, false);
	RectractingInverseLines(38, false);
	for (int i = 0; i < 8; i++) {
		GlowingHorizontalBar(30, 8, startingHueColour);
		startingHueColour = random(0, MaxHueValue);
	}
	RectractingLines(38, false);
	//alternating lines
	for (int i = 0; i < 4; i++) {
		GlowingVerticalBar(30, 8, startingHueColour);
		startingHueColour = random(0, MaxHueValue);
		GlowingHorizontalBar(30, 8, startingHueColour);
		startingHueColour = random(0, MaxHueValue);
	}
	//warp drive
	RotatingBlade(4, 30, startingWheelColour);
	for (int i = 0; i < 4; i++) {
		RandomWarpedStar(startingWheelColour, false);
	}
	RotatingBlade(4, 30, startingWheelColour);
	for (int i = 0; i < 4; i++) {
		RandomWarpedStar(startingWheelColour, false);
	}
	//interlude
	for (int i = 0; i < 2; i++) {
		PrimitiveGlowSpin(1, 30, startingHueColour, true);
		PrimitiveGlowSpin(1, 30, startingHueColour, false);
	}
	GlowingHorizontalBar(30, 7, startingHueColour);
	//super nova
	for (int j = 0; j < MaxWheelValue; j = j + 2) {
		if (j % 10 == 0) {
			GrowingBox(80, j, false, false);
		}
		else {
			GrowingBox(75, j, true, false);
		}
	}
	for (int j = 0; j < 24; j++) {
		GrowingBox(80, j, false, false);
	}
	//finale
	BouncingLine(70, RandomWheelColour, true);
	for (int i = 0; i < 3; i++) {
		RainbowWavesColourShuffle();
	}
	for (int i = 0; i < 4; i++) {
		CrissCrossInitialise(60, 1);
		CrissCross(60, 1);
	}
	CrissCrossDemo();
	//the comedown
	RandomRainbowPixel(60, 1);
	RandomRainbowPixelBuildUp(0);
}

void Spontanious(int startingHueColour, int startingWheelColour) {
	int wait = random(1000, 10000);

	for (int i = 0; i < wait; i++) {
		delay(1);
	}
	if (wait >= 8000) {
		if (wait > 9000) {
			RectractingLines(38, true);
		}
		else {
			RectractingInverseLines(38, true);
		}
	} else if (wait > 4000 && wait < 8000) {
		RandomWarpedStar(GetRandomWheelColour(), true);
	}
	else
	{
		strip.setPixelColor(random(0, totalLEDs), Wheel(GetRandomWheelColour()));
		strip.show();
		delay(80);
		SetChestGridColour(0, 1);
	}
}

void RainbowColourEverywherreeee() {
	//full rainbow
	int difference = abs(RandomWheelColour - MaxWheelValue);
	for (int i = RandomWheelColour; i < MaxWheelValue; i++) {
		SetChestGridColour(Wheel(i), 60);
	}
	for (int i = 0; i < difference; i++) {
		SetChestGridColour(Wheel(i), 60);
	}
	RainbowWaves(70, RandomWheelColour = difference);
	RainbowWavesColourShuffle();
	BouncingLine(70, RandomWheelColour, true);
}

#pragma endregion

#pragma region Visual Methods

bool PleaseSirCanIHaveSomeMore(int wait, int speed, int *hueColour) {
	int i, j;
	int buttonInput = 0;
	int suitMode = 0;
	//fade in
	for (i = 0; i < speed; i++) {
		//chest
		for (j = 0; j < 6; j++) {
			strip.setPixelColor(chestGrid[6][j], GetColorFromHSB(*hueColour, i * 100 / speed));
			strip.setPixelColor(chestGrid[6][j], GetColorFromHSB(*hueColour, i * 100 / speed));
		}
		strip.show();
		buttonInput = analogRead(buttonInputPin);
		if (CanResolveSuitMode(buttonInput)) {
			return false;
		}
		delay(wait);
	}
	//fade out
	for (i = speed; i >= 0; i--) {
		//chest
		for (j = 5; j >= 0; j--) {
			strip.setPixelColor(chestGrid[6][j], GetColorFromHSB(*hueColour, i * 100 / speed));
			strip.setPixelColor(chestGrid[6][j], GetColorFromHSB(*hueColour, i * 100 / speed));
		}
		strip.show();
		buttonInput = analogRead(buttonInputPin);
		if (CanResolveSuitMode(buttonInput)) {
			return false;
		}
		delay(wait);
	}
	SetChestGridColour(0, 60);

	for (int t = 0; t < 200; t++) {
		buttonInput = analogRead(buttonInputPin);
		if (CanResolveSuitMode(buttonInput)) {
			return false;
		}
		delay(10);
	}

	return true;
}

void PrimitiveGlowSpin(int spins, int wait, int hueColour, bool clockwise) {
	for (int i = 0; i < spins; i++) {
		if (clockwise == false) {
			GlowingHorizontalBar(wait, 7, hueColour);
			GlowingDiagonalPosGradientBar(wait, 7, hueColour);
			GlowingVerticalBar(wait, 7, hueColour);
			GlowingDiagonalNegGradientBar(wait, 7, hueColour);
		}
		else {
			GlowingHorizontalBar(wait, 7, hueColour);
			GlowingDiagonalNegGradientBar(wait, 7, hueColour);
			GlowingVerticalBar(wait, 7, hueColour);
			GlowingDiagonalPosGradientBar(wait, 7, hueColour);
		}
	}

}

void CrissCrossDemo() {
	CrissCross(60, 8);
	CrissCrossInitialise(60, 1);
	CrissCrossFlash(800, 6);
	GrowingBox(100, GetRandomWheelColour(), false, false);
	CrissCross(60, 8);
}

void CrissCrossFlash(int wait, int cycles) {
	int c = GetRandomWheelColour();
	for (int j = 0; j < cycles; j++) {
		for (int i = 0; i < 7; i++) {
			strip.setPixelColor(chestGrid[i][i], Wheel(c));
			if (i < 6) {
				strip.setPixelColor(chestGrid[i][5 - i], Wheel(c));
			}
		}

		strip.show();
		delay(wait);
		SetChestGridColour(0, 0);
		c = GetRandomWheelColour();
	}
}

void CrissCross(int wait, int cycles) {
	for (int j = 0; j < cycles; j++) {
		int c = random(0, 96);
		for (int i = 0; i < 7; i++) {
			strip.setPixelColor(chestGrid[i][i], Wheel(c));
			strip.show();
			delay(wait);
			strip.setPixelColor(chestGrid[i][i], Color(0, 0, 0));
		}
		for (int i = 0; i < 6; i++) {
			strip.setPixelColor(chestGrid[i][5 - i], Wheel(c));
			strip.show();
			delay(wait);
			strip.setPixelColor(chestGrid[i][5 - i], Color(0, 0, 0));
		}
	}
}

void CrissCrossInitialise(int wait, int cycles) {
	for (int j = 0; j < cycles; j++) {
		int c = random(0, 96);
		for (int i = 0; i < 7; i++) {
			strip.setPixelColor(chestGrid[i][i], Wheel(c));
			strip.show();
			delay(wait);
		}
		for (int i = 0; i < 6; i++) {
			strip.setPixelColor(chestGrid[i][5 - i], Wheel(c));
			strip.show();
			delay(wait);
		}
	}
}

void RainbowWaves(int wait, int wheelColour){
	int colour = wheelColour;

	for (int c = 0; c < chestGridColumns; c++) {
		strip.setPixelColor(chestGrid[0][c], Wheel(colour));
	}

	strip.show();
	delay(wait);

	for (int i = 0; i < chestGridRows; i++) {
		if (i != chestGridRows - 1){
			for (int c = 0; c < chestGridColumns; c++) {
				strip.setPixelColor(chestGrid[i + 1][c], Wheel(colour));
			}
		}
		strip.show();
		delay(wait);
		for (int c = 0; c < chestGridColumns; c++) {
			strip.setPixelColor(chestGrid[i][c], Color(0, 0, 0));
		}
	}
	SetChestGridColour(0, 60);
}

int TheColourfulStripe(int claps, int probabilitySpin, bool pauseInto, int startColour) {
	//VERTICAL CLAPS OF LINES
	bool pause = pauseInto;
	int colour = startColour;

	for (int j = 0; j < claps; +j++) {
		//for (int i = 0; i < 3; i++) {
		for (int y = 0; y < 7; y++) {
			strip.setPixelColor(chestGrid[y][2], Wheel(colour));
			strip.setPixelColor(chestGrid[y][3], Wheel(colour));
		}
		strip.show();
		delay(67 + (pauseInto ? 40 : 0));
		for (int y = 0; y < 7; y++) {
			strip.setPixelColor(chestGrid[y][1], Wheel(colour));
			strip.setPixelColor(chestGrid[y][4], Wheel(colour));
			strip.setPixelColor(chestGrid[y][2], 0);
			strip.setPixelColor(chestGrid[y][3], 0);
		}
		strip.show();
		delay(80);
		for (int y = 0; y < 7; y++) {
			strip.setPixelColor(chestGrid[y][0], Wheel(colour));
			strip.setPixelColor(chestGrid[y][5], Wheel(colour));
			strip.setPixelColor(chestGrid[y][1], 0);
			strip.setPixelColor(chestGrid[y][4], 0);
		}
		strip.show();
		delay(95);
		for (int y = 0; y < 7; y++) {
			strip.setPixelColor(chestGrid[y][1], Wheel(colour));
			strip.setPixelColor(chestGrid[y][4], Wheel(colour));
			strip.setPixelColor(chestGrid[y][0], 0);
			strip.setPixelColor(chestGrid[y][5], 0);
		}
		strip.show();
		delay(80);
		for (int y = 0; y < 7; y++) {
			strip.setPixelColor(chestGrid[y][2], Wheel(colour));
			strip.setPixelColor(chestGrid[y][3], Wheel(colour));
			strip.setPixelColor(chestGrid[y][1], 0);
			strip.setPixelColor(chestGrid[y][4], 0);

		}
		strip.show();
		delay(67);
		if (pauseInto == true) {
			pauseInto = false;
		}
		//}
		SetChestGridColour(0, 0);
	}
	//ROTATION 90 degress anti-clockwise
	RotateAntiClockWiseHorizontalLine(10, colour);

	while (1 == random(0, probabilitySpin))
	{
		RotateAntiClockWiseVerticalLine(10, colour);
		RotateAntiClockWiseHorizontalLine(10, colour);
	};

	//HORIZONTAL CLAPS
	for (int i = 0; i < claps; i++) {
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[3][x], Wheel(colour));
		}
		strip.show();
		delay(40);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[2][x], Wheel(colour));
			strip.setPixelColor(chestGrid[4][x], Wheel(colour));
			strip.setPixelColor(chestGrid[3][x], 0);
		}
		strip.show();
		delay(60);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[1][x], Wheel(colour));
			strip.setPixelColor(chestGrid[5][x], Wheel(colour));
			strip.setPixelColor(chestGrid[2][x], 0);
			strip.setPixelColor(chestGrid[4][x], 0);
		}
		strip.show();
		delay(75);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[0][x], Wheel(colour));
			strip.setPixelColor(chestGrid[6][x], Wheel(colour));
			strip.setPixelColor(chestGrid[1][x], 0);
			strip.setPixelColor(chestGrid[5][x], 0);
		}
		strip.show();
		delay(85);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[1][x], Wheel(colour));
			strip.setPixelColor(chestGrid[5][x], Wheel(colour));
			strip.setPixelColor(chestGrid[0][x], 0);
			strip.setPixelColor(chestGrid[6][x], 0);
		}
		strip.show();
		delay(75);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[2][x], Wheel(colour));
			strip.setPixelColor(chestGrid[4][x], Wheel(colour));
			strip.setPixelColor(chestGrid[1][x], 0);
			strip.setPixelColor(chestGrid[5][x], 0);
		}
		strip.show();
		delay(60);
		for (int x = 0; x < 6; x++) {
			strip.setPixelColor(chestGrid[3][x], Wheel(colour));
			strip.setPixelColor(chestGrid[2][x], 0);
			strip.setPixelColor(chestGrid[4][x], 0);
		}
		strip.show();
		delay(40);
	}
	SetChestGridColour(0, 0);

	RotateAntiClockWiseVerticalLine(10, colour);
}

void RotateAntiClockWiseHorizontalLine(int subtractFromDelay, int colourWheelPos) {
	for (int y = 0; y < 7; y++) {
		strip.setPixelColor(chestGrid[y][2], Wheel(colourWheelPos));
		strip.setPixelColor(chestGrid[y][3], Wheel(colourWheelPos));
	}
	strip.show();
	delay(85 - subtractFromDelay);
	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[0][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[6][4], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);
	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[1][0], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][5], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);
	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[2][0], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][5], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
	for (int x = 0; x < 6; x++) {
		strip.setPixelColor(chestGrid[3][x], Wheel(colourWheelPos));
	}
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
}

void RotateAntiClockWiseVerticalLine(int subtractFromDelay, int colourWheelPos) {
	for (int x = 0; x < 6; x++) {
		strip.setPixelColor(chestGrid[3][x], Wheel(colourWheelPos));
	}
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[2][5], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][0], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[1][5], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][1], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
	strip.setPixelColor(chestGrid[0][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][4], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[1][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[2][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[3][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[4][3], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][2], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[5][1], Wheel(colourWheelPos));
	strip.setPixelColor(chestGrid[6][1], Wheel(colourWheelPos));
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
	for (int y = 0; y < 7; y++) {
		strip.setPixelColor(chestGrid[y][2], Wheel(colourWheelPos));
		strip.setPixelColor(chestGrid[y][3], Wheel(colourWheelPos));

	}
	strip.show();
	delay(85 - subtractFromDelay);

	SetChestGridColour(0, 0);
}

void RotatingBlade(int rotations, int speed, int colourWheelPos) {
	for (int i = 0; i < rotations; i++) {
		RotateAntiClockWiseVerticalLine(speed, colourWheelPos);
		RotateAntiClockWiseHorizontalLine(speed, colourWheelPos);
	}
}

void RandomRainbowPixelBuildUp(int startPosition) {
	int iIncrement = 10;

	if (startPosition == 1) {
		int currentRow = 6;
		int rowRange = random(currentRow, 6);
		for (int j = 0; j < 7; j++) {
			for (int i = 0; i < MaxHueValue; i = i + iIncrement) {
				int column = random(0, 6);
				strip.setPixelColor(chestGrid[rowRange][column], GetColorFromHSB(i, (j * 8) + 10));
				strip.show();
				delay(85);
				strip.setPixelColor(chestGrid[rowRange][column], 0);
				rowRange = random(currentRow, 6);
			}
			iIncrement = iIncrement + 20;
			currentRow = currentRow - 1;
			rowRange = random(currentRow, 7);
		}

	}
	else if (startPosition == 0) {
		int currentRow = 0;
		int rowRange = random(currentRow, 6);
		for (int j = 0; j < 7; j++) {

			for (int i = 0; i < MaxHueValue; i = i + iIncrement) {
				int column = random(0, 6);
				strip.setPixelColor(chestGrid[rowRange][column], GetColorFromHSB(i, 100));
				strip.show();
				delay(85);
				strip.setPixelColor(chestGrid[rowRange][column], 0);
				rowRange = random(currentRow, 6);
			}
			iIncrement = iIncrement + 35;
			currentRow = currentRow + 1;
			rowRange = random(currentRow, 7);
		}
	}
	delay(60);
}

void WhiteFlashBuildUp(){
	int white = Color(30, 30, 30);


	//SetChestGridColour(white, 150);
	//SetChestGridColour(0, 170);

	/*SetChestGridColour(0, 0);
	SetChestGridColour(white, 360);

	SetChestGridColour(0, 250);*/
	SetChestGridColour(white, 200);

	SetChestGridColour(0, 170);
	SetChestGridColour(white, 150);
	SetChestGridColour(0, 0);

	SetChestGridColour(0, 90);
	SetChestGridColour(white, 70);

	SetChestGridColour(0, 60);
	SetChestGridColour(white, 55);
	SetChestGridColour(0, 35);
	SetChestGridColour(white, 35);
	SetChestGridColour(0, 35);
	SetChestGridColour(white, 35);
	//SetChestGridColour(0, 35);
	//SetChestGridColour(white, 35);

	SetChestGridColour(0, 0);
}

void SetChestGridColour(int colour, int wait) {
	for (int j = 0; j < chestGridRows; j++) {
		for (int i = 0; i < chestGridColumns; i++) {
			strip.setPixelColor(chestGrid[j][i], colour);
		}
	}
	if (wait != 0) {
		strip.show();
		delay(wait);
	}
}

void RectractingLines(int wait, bool singleLine) {
	int i;
	int startingColumn = random(0, chestGridColumns);
	int colour = GetRandomWheelColour();

	//DEFINE COLUMNS
	bool HasHitAllColumns = false;
	bool first = false;
	bool second = false;
	bool third = false;
	bool fourth = false;
	bool fifth = false;
	bool sixth = false;

	while (HasHitAllColumns == false) {

		//DOWN
		for (i = 0; i < chestGridRows; i++) {
			strip.setPixelColor(chestGrid[i][startingColumn], Wheel(colour));
			strip.show();
			delay(wait);
		}

		//UP
		for (i = chestGridRows - 1; i >= 0; i--) {
			strip.setPixelColor(chestGrid[i][startingColumn], 0);
			strip.show();
			delay(wait - 13);
		}

		delay(10);

		//CHECK IF ALL COLUMNS HAVE BEEN SET
		switch (startingColumn) {
		case 0: first = true;
			break;
		case 1: second = true;
			break;
		case 2: third = true;
			break;
		case 3: fourth = true;
			break;
		case 4: fifth = true;
			break;
		case 5: sixth = true;
			break;
		}
		if ((first == true && second == true && third == true && fourth == true && fifth == true && sixth == true) || singleLine == true) {
			HasHitAllColumns = true;
		}
		colour = GetRandomWheelColour();
		startingColumn = random(0, chestGridColumns);
	}
	SetChestGridColour(0, 60);
}

void RectractingInverseLines(int wait, bool singleLine) {
	int i;
	int startingColumn = random(0, chestGridColumns);
	int colour = GetRandomWheelColour();

	//DEFINE COLUMNS
	bool HasHitAllColumns = false;
	bool first = false;
	bool second = false;
	bool third = false;
	bool fourth = false;
	bool fifth = false;
	bool sixth = false;

	while (HasHitAllColumns == false) {

		//DOWN
		for (i = chestGridRows - 1; i >= 0; i--) {
			strip.setPixelColor(chestGrid[i][startingColumn], Wheel(colour));
			strip.show();
			delay(wait);
		}

		//UP
		for (i = 0; i < chestGridRows; i++) {
			strip.setPixelColor(chestGrid[i][startingColumn], 0);
			strip.show();
			delay(wait - 13);
		}
		delay(10);

		//CHECK IF ALL COLUMNS HAVE BEEN SET
		switch (startingColumn) {
		case 0: first = true;
			break;
		case 1: second = true;
			break;
		case 2: third = true;
			break;
		case 3: fourth = true;
			break;
		case 4: fifth = true;
			break;
		case 5: sixth = true;
			break;
		}

		if ((first == true && second == true && third == true && fourth == true && fifth == true && sixth == true) || singleLine == true) {
			HasHitAllColumns = true;
		}
		colour = GetRandomWheelColour();
		startingColumn = random(0, chestGridColumns);
	}
	SetChestGridColour(0, 60);
}

void RainbowWavesColourShuffle() {

	int minWait = 26;
	int wait = 80;
	int increment = 6;

	int rows = 7;
	int columns = 6;

	for (int i = wait; i > minWait; i = i - increment) {
		RainbowWaves(i, RandomWheelColour);
	}

	RainbowWaves(minWait, RandomWheelColour);
	RainbowWaves(minWait - 3, RandomWheelColour);

	SetChestGridColour(0, 100);
	SetChestGridColour(Wheel(RandomWheelColour), 40);
	SetChestGridColour(0, 100);

	RandomWheelColour = GetRandomWheelColour();

	SetChestGridColour(Wheel(RandomWheelColour), 40);
	SetChestGridColour(0, 100);

	for (int i = minWait; i <= wait; i = i + increment) {
		RainbowWaves(i, RandomWheelColour);
		if (i < 42) {
			RainbowWaves(i, RandomWheelColour);
		}
		delay(10);
	}
	SetChestGridColour(0, 60);
}

void RandomRainbowPixel(int wait, int cycles) {
	int j;
	int randomChestRow = random(0, 6);
	int randomChestColumn = random(0, 7);

	int gridPositionCurrent;
	int rainbowPositionCurrent;

	for (j = 0; j < MaxWheelValue * cycles; j++) { // cycles of all 96 colors in the wheel * - MaxWheelValue

		rainbowPositionCurrent = Wheel(((MaxWheelValue / (strip.numPixels())) + j) % MaxWheelValue);
		gridPositionCurrent = chestGrid[randomChestRow][randomChestColumn];

		//SET COLOUR
		strip.setPixelColor(gridPositionCurrent, rainbowPositionCurrent);
		strip.show();
		delay(wait);

		//SET BLANK
		strip.setPixelColor(gridPositionCurrent, Color(0, 0, 0));
		strip.show();
		delay(wait);

		randomChestColumn = random(0, 6);
		randomChestRow = random(0, 7);
	}
	SetChestGridColour(0, 60);
}

void BouncingLine(int wait, int colour, bool wipeColour) {
	int i, j;
	int rows = 7;
	int columns = 6;
	int speedCounter = 5;

	//DOWN
	for (j = 0; j < rows; j++) {
		speedCounter = 7;
		for (int i = 0; i < columns; i++) {
			strip.setPixelColor(chestGrid[j][i], Wheel(colour));
			if (j > 0) {
				strip.setPixelColor(chestGrid[j - 1][i], Color(0, 0, 0));
			}
		}
		if (j > 2) {
			wait = wait - speedCounter;
			speedCounter + 5;
		}
		strip.show();
		delay(wait);
	}

	//UP
	for (j = rows - 2; j >= 0; j--) {
		speedCounter = 5;
		for (i = 0; i < columns; i++) {
			strip.setPixelColor(chestGrid[j][i], Wheel(colour));
			if (wipeColour == false) {
				strip.setPixelColor(chestGrid[j + 1][i], Color(0, 0, 0));
			}
		}
		if (j > 2) {
			wait = wait - speedCounter;
			speedCounter + 5;
		}
		delay(wait);
		strip.show();
	}
	if (wipeColour == false) {
		for (i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Color(0, 0, 0));
		}
	}
}

void GrowingBox(int wait, int colour, bool shrink, bool outline) {
	int i;
	//1st frame
	strip.setPixelColor(chestGrid[3][2], Wheel(colour));
	strip.setPixelColor(chestGrid[3][3], Wheel(colour));
	strip.show();
	delay(wait - 5);
	strip.setPixelColor(chestGrid[3][2], Color(0, 0, 0));
	strip.setPixelColor(chestGrid[3][3], Color(0, 0, 0));
	//2nd frame
	for (i = 1; i < 5; i++) {
		strip.setPixelColor(chestGrid[2][i], Wheel(colour));
		strip.setPixelColor(chestGrid[4][i], Wheel(colour));
	}
	strip.setPixelColor(chestGrid[3][1], Wheel(colour));
	strip.setPixelColor(chestGrid[3][4], Wheel(colour));
	strip.show();
	delay(wait - 12);
	for (i = 1; i < 5; i++) {
		strip.setPixelColor(chestGrid[2][i], Color(0, 0, 0));
		strip.setPixelColor(chestGrid[4][i], Color(0, 0, 0));
	}
	strip.setPixelColor(chestGrid[3][1], Color(0, 0, 0));
	strip.setPixelColor(chestGrid[3][4], Color(0, 0, 0));
	//3rd frame
	for (i = 0; i < 6; i++) {
		strip.setPixelColor(chestGrid[1][i], Wheel(colour));
		strip.setPixelColor(chestGrid[5][i], Wheel(colour));
	}
	for (i = 1; i < 5; i++) {
		strip.setPixelColor(chestGrid[i][0], Wheel(colour));
		strip.setPixelColor(chestGrid[i][5], Wheel(colour));
	}
	strip.show();
	delay(wait - 22);
	for (i = 0; i < 6; i++) {
		strip.setPixelColor(chestGrid[1][i], Color(0, 0, 0));
		strip.setPixelColor(chestGrid[5][i], Color(0, 0, 0));
	}
	for (i = 1; i < 5; i++) {
		strip.setPixelColor(chestGrid[i][0], Color(0, 0, 0));
		strip.setPixelColor(chestGrid[i][5], Color(0, 0, 0));
	}
	//4th frame
	for (i = 0; i < 6; i++) {
		strip.setPixelColor(chestGrid[0][i], Wheel(colour));
		strip.setPixelColor(chestGrid[6][i], Wheel(colour));
	}
	strip.show();
	delay(wait - 35);
	//SHRINK
	if (shrink == true) {
		delay(wait + 5);
		//5th frame
		if (outline == false) {
			for (i = 0; i < 6; i++) {
				strip.setPixelColor(chestGrid[0][i], Color(0, 0, 0));
				strip.setPixelColor(chestGrid[6][i], Color(0, 0, 0));
			}
		}
		for (i = 0; i < 6; i++) {
			strip.setPixelColor(chestGrid[1][i], Wheel(colour));
			strip.setPixelColor(chestGrid[5][i], Wheel(colour));
		}
		for (i = 1; i < 5; i++) {
			strip.setPixelColor(chestGrid[i][0], Wheel(colour));
			strip.setPixelColor(chestGrid[i][5], Wheel(colour));
		}
		strip.show();
		delay(wait + 10);
		for (i = 0; i < 6; i++) {
			strip.setPixelColor(chestGrid[1][i], Color(0, 0, 0));
			strip.setPixelColor(chestGrid[5][i], Color(0, 0, 0));
		}
		for (i = 1; i < 5; i++) {
			strip.setPixelColor(chestGrid[i][0], Color(0, 0, 0));
			strip.setPixelColor(chestGrid[i][5], Color(0, 0, 0));
		}
		//6th frame
		for (i = 1; i < 5; i++) {
			strip.setPixelColor(chestGrid[i][0], Color(0, 0, 0));
			strip.setPixelColor(chestGrid[i][5], Color(0, 0, 0));
			strip.setPixelColor(chestGrid[2][i], Wheel(colour));
			strip.setPixelColor(chestGrid[4][i], Wheel(colour));
		}
		strip.setPixelColor(chestGrid[3][1], Wheel(colour));
		strip.setPixelColor(chestGrid[3][4], Wheel(colour));
		strip.show();
		delay(wait + 15);
		for (i = 1; i < 5; i++) {
			strip.setPixelColor(chestGrid[2][i], Color(0, 0, 0));
			strip.setPixelColor(chestGrid[4][i], Color(0, 0, 0));
		}
		strip.setPixelColor(chestGrid[3][1], Color(0, 0, 0));
		strip.setPixelColor(chestGrid[3][4], Color(0, 0, 0));
		//7th frame
		strip.setPixelColor(chestGrid[3][2], Wheel(colour));
		strip.setPixelColor(chestGrid[3][3], Wheel(colour));
		strip.show();
		delay(wait);
	}
	else {
		if (outline == false) {
			for (i = 0; i < 6; i++) {
				strip.setPixelColor(chestGrid[0][i], Color(0, 0, 0));
				strip.setPixelColor(chestGrid[6][i], Color(0, 0, 0));
			}
		}
		strip.show();
		delay(wait);
	}
	//SetChestGridColour(0, wait);
}

void RandomWarpedStar(int colour, bool fadeOut) {
	int rows = 6;
	int columns = 5;

	int y = random(1, rows);
	int x = random(1, columns);

	int randomStar = random(0, 2);

	//FIRST DOT
	strip.setPixelColor(chestGrid[y][x], Wheel(colour));
	strip.show();
	delay(45);
	//FIRST STAR

	if (randomStar == 0) {
		//y = x
		strip.setPixelColor(chestGrid[y + 1][x + 1], Wheel(colour));
		strip.setPixelColor(chestGrid[y - 1][x - 1], Wheel(colour));
	}
	else if (randomStar == 1)  {
		//vertical line
		strip.setPixelColor(chestGrid[y + 1][x], Wheel(colour));
		strip.setPixelColor(chestGrid[y - 1][x], Wheel(colour));
	}
	strip.show();
	delay(30);


	if (randomStar == 0) {
		//y = -x
		strip.setPixelColor(chestGrid[y + 1][x - 1], Wheel(colour));
		strip.setPixelColor(chestGrid[y - 1][x + 1], Wheel(colour));
	}
	else if (randomStar == 1)  {
		//horizontal line
		strip.setPixelColor(chestGrid[y][x - 1], Wheel(colour));
		strip.setPixelColor(chestGrid[y][x + 1], Wheel(colour));
	}
	strip.show();
	delay(30);
	//SECOND STAR
	//y = -x || horizontal line
	if (randomStar == 0) {
		strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x - 2, 0, columns)], Wheel(colour));
		strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x + 2, 0, columns)], Wheel(colour));
	}
	else if (randomStar == 1)  {
		strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x - 2, 0, columns)], Wheel(colour));
		strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x + 2, 0, columns)], Wheel(colour));
	}

	strip.show();
	delay(35);

	//y = x || vertical line
	if (randomStar == 0) {
		strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x + 2, 0, columns)], Wheel(colour));
		strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x - 2, 0, columns)], Wheel(colour));
	}
	else if (randomStar == 1)  {
		strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x, 0, columns)], Wheel(colour));
		strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x, 0, columns)], Wheel(colour));
	}
	strip.show();
	delay(100);
	//SWITCH OFF
	if (fadeOut == false) {
		//SECOND AND THIRD STAR
		if (randomStar == 0) {
			strip.setPixelColor(chestGrid[y + 1][x - 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x - 1], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
		}
		else if (randomStar == 1)  {
			strip.setPixelColor(chestGrid[y][x - 1], 0);
			strip.setPixelColor(chestGrid[y][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x], 0);
			strip.setPixelColor(chestGrid[y - 1][x], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x, 0, columns)], 0);
		}

		strip.show();
		delay(35);
		//FIRST STAR
		if (randomStar == 0) {
			strip.setPixelColor(chestGrid[y + 1][x - 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x - 1], 0);
			/*strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x + 2, 0, columns)], 0);*/
		}
		else if (randomStar == 1)  {
			strip.setPixelColor(chestGrid[y][x - 1], 0);
			strip.setPixelColor(chestGrid[y][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x], 0);
			strip.setPixelColor(chestGrid[y - 1][x], 0);
			/*strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x + 2, 0, columns)], 0);*/
		}
		strip.show();
		delay(35);
		//FIRST DOT
		strip.setPixelColor(chestGrid[y][x], 0);
		strip.show();
		delay(120);
	}
	else {
		//FIRST DOT
		strip.setPixelColor(chestGrid[y][x], 0);
		strip.show();
		delay(120);
		//FIRST STAR
		if (randomStar == 0) {
			strip.setPixelColor(chestGrid[y + 1][x - 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x - 1], 0);
			/*strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x + 2, 0, columns)], 0);*/
		}
		else if (randomStar == 1)  {
			strip.setPixelColor(chestGrid[y][x - 1], 0);
			strip.setPixelColor(chestGrid[y][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x], 0);
			strip.setPixelColor(chestGrid[y - 1][x], 0);
			/*strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x + 2, 0, columns)], 0);*/
		}
		strip.show();
		delay(35);
		//SECOND AND THIRD STAR
		if (randomStar == 0) {
			strip.setPixelColor(chestGrid[y + 1][x - 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x + 1], 0);
			strip.setPixelColor(chestGrid[y - 1][x - 1], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x - 2, 0, columns)], 0);
		}
		else if (randomStar == 1)  {
			strip.setPixelColor(chestGrid[y][x - 1], 0);
			strip.setPixelColor(chestGrid[y][x + 1], 0);
			strip.setPixelColor(chestGrid[y + 1][x], 0);
			strip.setPixelColor(chestGrid[y - 1][x], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x - 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y, 0, rows)][constrain(x + 2, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y - 2, 0, rows)][constrain(x, 0, columns)], 0);
			strip.setPixelColor(chestGrid[constrain(y + 2, 0, rows)][constrain(x, 0, columns)], 0);
		}

		strip.show();
		delay(35);
	}
	SetChestGridColour(0, 60);
}

void GlowingHorizontalBar(int wait, int speed, int hueColour) {
	int i, j;
	//fade in
	for (i = 0; i < speed; i++) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[3][j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	//fade out
	for (i = speed; i >= 0; i--) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[3][j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	SetChestGridColour(0, 60);
}

void GlowingVerticalBar(int wait, int speed, int hueColour) {
	int i, j;
	//fade in
	for (i = 0; i < speed; i++) {
		for (j = 0; j < 7; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][2], GetColorFromHSB(hueColour, i * 100 / speed));
			strip.setPixelColor(chestGrid[j][3], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	//fade out
	for (i = speed; i >= 0; i--) {
		for (j = 0; j < 7; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][2], GetColorFromHSB(hueColour, i * 100 / speed));
			strip.setPixelColor(chestGrid[j][3], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	SetChestGridColour(0, 60);
}

void GlowingDiagonalNegGradientBar(int wait, int speed, int hueColour) {
	int i, j;
	//fade in
	for (i = 0; i < speed; i++) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	//fade out
	for (i = speed; i >= 0; i--) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	SetChestGridColour(0, 60);
}

void GlowingDiagonalPosGradientBar(int wait, int speed, int hueColour) {
	int i, j;
	//fade in
	for (i = 0; i < speed; i++) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][5 - j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	//fade out
	for (i = speed; i >= 0; i--) {
		for (j = 0; j < 6; j++) {
			//chest
			strip.setPixelColor(chestGrid[j][5 - j], GetColorFromHSB(hueColour, i * 100 / speed));
		}
		strip.show();
		delay(wait);
	}
	SetChestGridColour(0, 60);
}

#pragma endregion

#pragma region Helper Methods

void PlayVisualSet(int suitMode, int *startingHueColour, int wheelColour) {
	SetChestGridColour(0, 0);
	switch (suitMode) {
	case LOW_POWER_MODE:
		//Serial.println("Low Power Mode engaged!");
		//Spontanious(*startingHueColour, wheelColour);
		RainbowColourEverywherreeee();
		break;
	case PARTY_WITH_COLOUR:
		//Serial.println("Party Mode engaged!");
		ColourfulViiibes(*startingHueColour, wheelColour);
		break;
	}
}

unsigned int Color(unsigned char r, unsigned char g, unsigned char b)
{
	//31 IS THE MAX VALUE, 32 WILL DISPLAY 0
	r = constrain(r, 0, 31);
	g = constrain(g, 0, 31);
	b = constrain(b, 0, 31);
	//Take the lowest 5 bits of each value and append them end to end
	return(((int)r & 0x1F) << 10 | ((int)g & 0x1F) << 5 | (int)b & 0x1F);
}

unsigned int Wheel(int WheelPos)
{
	int r = 0, g = 0, b = 0;
	switch (WheelPos >> 5)
	{
	case 0:
		r = 31 - WheelPos % 32;   //Red down
		g = WheelPos % 32;      // Green up
		b = 0;                  //blue off
		break;
	case 1:
		g = 31 - WheelPos % 32;  //green down
		b = WheelPos % 32;      //blue up
		r = 0;                  //red off
		break;
	case 2:
		b = 31 - WheelPos % 32;  //blue down 
		r = WheelPos % 32;      //red up
		g = 0;                  //green off
		break;
	}
	return(Color(r, g, b));
}

unsigned int GetRandomWheelColour() {
	GetRandomColour(RandomWheelColour, 1);
}

unsigned int GetRandomHueColour() {
	GetRandomColour(RandomWheelColour, 2);
}

unsigned int GetRandomColour(int colour, int type) {
	int randomColour, maxRange;
	int range = 0; // ensures the next colour will not match the first colour within the colour wheel range specified

	//error handling
	if (type != 1 && type != 2) {
		return 0;
	}

	if (type == 1) { //Wheel Colour
		range = 20;
		randomColour = RandomWheelColour;
		maxRange = MaxWheelValue;
	}
	if (type == 2) { //Hue Colour
		range = 40;
		randomColour = RandomHueColour;
		maxRange = MaxWheelValue;
	}
	while (abs(colour - randomColour) <= range) {
		randomColour = random(0, maxRange);
	}
	//set values to new values... figure out how to do this with pointers
	if (type == 1) { //Wheel Colour
		RandomWheelColour = randomColour;
	}
	if (type == 2) { //Hue Colour
		RandomHueColour = randomColour;
	}
	return RandomWheelColour;
}

int GetColorFromHSB(int hue, int bright) {
	//set default saturation as there is no need for it within LED pixels
	int sat = 100;
	//constrain all input variables to expected range
	hue = constrain(hue, 0, MaxHueValue);
	bright = constrain(bright, 0, 100);


	// define maximum value for RGB array elements
	float max_rgb_val = 31.0;

	// convert saturation and brightness value to decimals and init r, g, b variables
	float sat_f = float(sat) / 100.0;
	float bright_f = float(bright) / 100.0;

	int r, g, b;

	// If brightness is 0 then color is black (achromatic)
	if (bright <= 0) {
		r = 0;
		b = 0;
		g = 0;
	}

	// if saturation and brightness are greater than 0 then calculate 
	// R, G and B values based on the current hue and brightness
	else {
		float hue_primary;
		float hue_secondary;
		float sat_primary;
		float sat_secondary;
		float sat_tertiary;

		if (hue >= 0 && hue < 120) {
			hue_primary = 1.0 - (float(hue) / 120.0);
			hue_secondary = float(hue) / 120.0;
			sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
			sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
			sat_tertiary = 1.0 - sat_f;
			r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
			b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
			g = (bright_f * max_rgb_val) * sat_tertiary;
		}

		else if (hue >= 120 && hue < 240) {
			hue_primary = 1.0 - ((float(hue) - 120.0) / 120.0);
			hue_secondary = (float(hue) - 120.0) / 120.0;
			sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
			sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
			sat_tertiary = 1.0 - sat_f;
			r = (bright_f * max_rgb_val) * sat_tertiary;
			b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
			g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
		}

		else if (hue >= 240 && hue <= MaxHueValue) {
			float hue_primary = 1.0 - ((float(hue) - 240.0) / 120.0);
			float hue_secondary = (float(hue) - 240.0) / 120.0;
			float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
			float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
			float sat_tertiary = 1.0 - sat_f;
			r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
			b = (bright_f * max_rgb_val) * sat_tertiary;
			g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
		}
	}

	return Color(r, b, g);
}

bool CanResolveSuitMode(int inputValue) {
	/*if (inputValue < button1_MIN) {
		Serial.print(".");
		}
		else {
		Serial.println();
		Serial.print("Input value is: ");
		Serial.println(inputValue);
		}*/
	if (inputValue > button1_MIN && inputValue < button1_MAX) {
		SuitMode = LOW_POWER_MODE;
		return true;
	}
	if (inputValue > button2_MIN && inputValue < button2_MAX) {
		SuitMode = PARTY_WITH_COLOUR;
		return true;
	}
	SuitMode = STANDBY_MODE;
	return false;
}

#pragma endregion

#pragma region Miscellaneous (Commented out)

//int GetColorFromHSBFloat(float hue, float bright) {
//	if (hue > 1) hue = 1.0;
//	if (bright > 1) bright = 1.0;
//	return GetColorFromHSB(hue*MaxHueValue.0, bright*100.0); 
//}

/*BACKUP HSB CONVERTER (READY FOR CFIDDLE - http://cfiddle.net/)*/
//#include<stdio.h>
//
//void H2R_HSBtoRGB(int hue, int bright);
//
//void main() {
//    H2R_HSBtoRGB(0, 100);
//}
//
//void H2R_HSBtoRGB(int hue, int bright) {
//    float R, B, G;
//    float sat = 100;
//    //NB: PUT IN FLOAT CASTS
//
//    //constrain all input variables to expected range
//    //hue = constrain(hue, 0, MaxHueValue);
//    //bright = constrain(bright, 0, 100);
//
//    // define maximum value for RGB array elements
//    float max_rgb_val = 255.0;
//
//    // convert saturation and brightness value to decimals and init r, g, b variables
//    float sat_f;
//    sat_f = sat / 100.0;
//
//    float bright_f;// = float(bright) / 100.0;
//    bright_f = bright / 100.0;
//        
//    int r, g, b;
//    
//    // If brightness is 0 then color is black (achromatic)
//    // therefore, R, G and B values will all equal to 0
//    if (bright <= 0) {      
//        R = 0;
//        B = 0;
//        G = 0;
//    }
//
//    // if saturation and brightness are greater than 0 then calculate 
//    // R, G and B values based on the current hue and brightness
//    else {
//        float hue_primary;
//	float hue_secondary;
//	float sat_primary;
//	float sat_secondary;
//	float sat_tertiary;
//
//        if (hue >= 0 && hue < 120) {
//			hue_primary = 1.0 - (hue / 120.0);
//			hue_secondary = hue / 120.0;
//			sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
//			sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
//			sat_tertiary = 1.0 - sat_f;
//			r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
//			b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
//			g = (bright_f * max_rgb_val) * sat_tertiary;  
//        }
//
//        else if (hue >= 120 && hue < 240) {
//			hue_primary = 1.0 - ((hue-120.0) / 120.0);
//			hue_secondary = (hue-120.0) / 120.0;
//			sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
//			sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
//			sat_tertiary = 1.0 - sat_f;
//			r = (bright_f * max_rgb_val) * sat_tertiary;  
//			b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
//			g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
//        }
//
//        else if (hue >= 240 && hue <= MaxHueValue) {
//			float hue_primary = 1.0 - ((hue-240.0) / 120.0);
//			float hue_secondary = (hue-240.0) / 120.0;
//			float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
//			float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
//			float sat_tertiary = 1.0 - sat_f;
//			r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
//			b = (bright_f * max_rgb_val) * sat_tertiary;  
//			g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
//        }
//        
//      R=r;
//      B=g;
//      G=b;
//
//		printf("%g\n",R);
//		printf("%g\n",B);
//		printf("%g",G);
//    }
//}
//
//void H2R_HSBtoRGBfloat(float hue, float sat, float bright) {
//	if (hue > 1) hue = 1.0;
//	if (sat > 1) sat = 1.0;
//	if (bright > 1) bright = 1.0;
//    H2R_HSBtoRGB(hue*MaxHueValue.0, bright*100.0); 
//}

//Deprecated - Grids are positioned inversely so this makes text useless as text on the back will be mirrored and illegible, USE THIS METHOD FOR LOGIC
//void Letters(int wait){
//
//	int rows = 7;
//	int columns = 6;
//	int colour = random(0, MaxWheelValue);
//	int multiplier = 1;
//
//	for (int j = 0; j < MaxWheelValue * multiplier; j++) { 
//
//		//LETTER: "P"
//		if (j < ((MaxWheelValue * multiplier) / 3)) {
//			for (int i = 0; i < columns - 1; i++) {
//				strip.setPixelColor(chestGrid[0][i], Wheel(j));
//			}
//			for (int i = 1; i < 3; i++) {
//				strip.setPixelColor(chestGrid[i][4], Wheel(j));
//			}
//			for (int i = 0; i < columns - 1; i++) {
//				strip.setPixelColor(chestGrid[3][i], Wheel(j));
//			}
//			for (int i = 0; i < rows; i++) {
//				strip.setPixelColor(chestGrid[i][0], Wheel(j));
//			}
//			strip.show();
//			delay(wait);
//
//			for (int i=0; i < strip.numPixels(); i++) {
//				strip.setPixelColor(i, Color(0,0,0));
//			}
//		}
//
//		if ((j > ((MaxWheelValue * multiplier) / 3)) && (j < ((MaxWheelValue * multiplier) / 3) * 2)) {
//			//LETTER: "S"
//			for (int i = 0; i < columns - 1; i++) {
//				strip.setPixelColor(chestGrid[0][i], Wheel(j));
//			}
//			for (int i = 1; i < 3; i++) {
//				strip.setPixelColor(chestGrid[i][0], Wheel(j));
//			}
//			for (int i = 0; i < columns - 1; i++) {
//				strip.setPixelColor(chestGrid[3][i], Wheel(j));
//			}
//			for (int i = 4; i < rows - 1; i++) {
//				strip.setPixelColor(chestGrid[i][4], Wheel(j));
//			}
//			for (int i = 0; i < columns - 1; i++) {
//				strip.setPixelColor(chestGrid[6][i], Wheel(j));
//			}
//			strip.show();
//			delay(wait);
//
//			for (int i=0; i < strip.numPixels(); i++) {
//				strip.setPixelColor(i, Color(0,0,0));
//			}
//		}
//
//		if (j > ((MaxWheelValue * multiplier) / 3) * 2) {
//			//LETTER: "Y"
//			strip.setPixelColor(chestGrid[0][0], Wheel(j));
//			strip.setPixelColor(chestGrid[0][4], Wheel(j));
//			strip.setPixelColor(chestGrid[1][1], Wheel(j));
//			strip.setPixelColor(chestGrid[1][3], Wheel(j));
//			for (int i = 2; i < rows; i++) {
//				strip.setPixelColor(chestGrid[i][2], Wheel(j));
//			}
//			strip.show();
//			delay(wait);
//
//			for (int i=0; i < strip.numPixels(); i++) {
//				strip.setPixelColor(i, Color(0,0,0));
//			}
//		}
//	}
//}

//DEPRECATED FULL SET
//void PartyMode(int startingHueColour, int wheelColour) {
//	int colour = wheelColour;
//	//intro
//	//pixel flash builds up bottom to top
//	RandomRainbowPixelBuildUp(1);
//	RandomRainbowPixel(80, 1, 12);
//	for (int i = 0; i < 4; i++) {
//		GlowingHorizontalBar(30, 8, startingHueColour);
//		startingHueColour = GetRandomColour(startingHueColour);
//	}
//	//things get trippy
//	for (int i = 0; i < 15; i++) {
//		BouncingLine(80, wheelColour, false);
//	}
//	for (int j = 0; j < MaxWheelValue; j = j + 3) {
//		GrowingBox(70, j, true, false);
//	}
//	for (int i = 0; i < 15; i++) {
//		BouncingLine(80, wheelColour, false);
//	}
//	//strobe build up
//	SetChestGridColour(0, 0);
//	WhiteFlashBuildUp();
//	//full rainbow wheelColour 
//	for (int i = 0; i < MaxWheelValue; i++) {
//		SetChestGridColour(Wheel(i), 60);
//	}
//	SetChestGridColour(0, 60);
//	//bouncy
//	RectractingLines(38);
//	//full wheelColour
//	for (int i = 0; i < MaxWheelValue; i++) {
//		SetChestGridColour(Wheel(i), 60);
//	}
//	SetChestGridColour(0, 60);
//	//bouncy
//	RectractingInverseLines(38);
//	for (int i = 0; i < 10; i++) {
//		GlowingHorizontalBar(30, 8, startingHueColour);
//		startingHueColour = random(0, MaxHueValue);
//	}
//	//alternating lines
//	for (int i = 0; i < 10; i++) {
//		GlowingVerticalBar(30, 8, startingHueColour);
//		startingHueColour = random(0, MaxHueValue);
//		GlowingHorizontalBar(30, 8, startingHueColour);
//		startingHueColour = random(0, MaxHueValue);
//	}
//
//	//interlude
//	PrimitiveGlowSpin(8, 30, startingHueColour);
//	CrissCrossDemo();
//
//	//hardcore blade spinning
//	RotatingBlade(8, 30, wheelColour);
//	RotateAntiClockWiseVerticalLine(30, wheelColour);
//
//	TheColourfulStripe(5, 3, true, wheelColour);
//
//	RotateAntiClockWiseHorizontalLine(30, wheelColour);
//	wheelColour = random(0, 96);
//	RotatingBlade(8, 30, wheelColour);
//	//warp drive
//	for (int i = 0; i < 5; i++) {
//		RandomWarpedStar(wheelColour, false);
//	}
//	RotatingBlade(8, 30, wheelColour);
//	for (int i = 0; i < 5; i++) {
//		RandomWarpedStar(wheelColour, false);
//	}
//	RotatingBlade(8, 30, wheelColour);
//	//comedown
//	PrimitiveGlowSpin(8, 30, startingHueColour);
//	for (int i = 0; i < 15; i++) {
//		BouncingLine(80, wheelColour, false);
//	}
//	for (int i = 0; i < 3; i++) {
//		RainbowWavesColourShuffle(true);
//	}
//	//warp drive extreme
//	CrissCross(60, 4);
//	for (int i = 0; i < MaxWheelValue; i = i + 2) {
//		if (i % 16 == 0) {
//			CrissCross(60, 8);
//		}
//		RandomWarpedStar(wheelColour, false);
//	}
//	//the comedown
//	RandomRainbowPixel(60, 1, 12);
//	RandomRainbowPixelBuildUp(0);
//	//super nova
//	for (int j = 0; j < MaxWheelValue; j = j + 2) {
//		if (j % 10 == 0) {
//			GrowingBox(80, j, false, false);
//		}
//		else {
//			GrowingBox(75, j, true, false);
//		}
//	}
//	for (int j = 0; j < 24; j++) {
//		GrowingBox(80, j, false, false);
//	}
//	//all in one
//	BouncingLine(100, wheelColour, true);
//	SetChestGridColour(0, 0);
//
//	RotatingBlade(12, 30, wheelColour);
//	RotateAntiClockWiseVerticalLine(30, wheelColour);
//	for (int i = 0; i < 10; i++) {
//		wheelColour = random(0, 96);
//		TheColourfulStripe(1, 3, true, wheelColour);
//		TheColourfulStripe(1, 3, true, wheelColour);
//	}
//	RotateAntiClockWiseHorizontalLine(30, wheelColour);
//	wheelColour = random(0, 96);
//
//	RotatingBlade(6, 30, wheelColour);
//	RotateAntiClockWiseVerticalLine(30, wheelColour);
//	TheColourfulStripe(1, 3, true, wheelColour);
//	for (int i = 0; i < 5; i++) {
//		RandomWarpedStar(wheelColour, false);
//	}
//	RotatingBlade(4, 30, wheelColour);
//	for (int i = 0; i < 5; i++) {
//		RandomWarpedStar(wheelColour, false);
//	}
//	RotatingBlade(4, 30, wheelColour);
//
//
//	for (int i = 0; i < 7; i++){
//		BouncingLine(70, wheelColour, false);
//	}
//	BouncingLine(70, wheelColour, true);
//	for (int i = 0; i < MaxWheelValue; i++) {
//		SetChestGridColour(Wheel(i), 80);
//	}
//	WhiteFlashBuildUp();
//	RectractingLines(38);
//	RectractingInverseLines(38);
//	for (int i = 0; i < MaxWheelValue; i = i + 2) {
//		RandomWarpedStar(i, false);
//	}
//	RandomRainbowPixelBuildUp(0);
//
//}

#pragma endregion