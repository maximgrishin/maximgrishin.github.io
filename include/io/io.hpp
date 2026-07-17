#pragma once

namespace io {

enum Color {
	Black,
	DarkBlue,
	DarkPurple,
	DarkGreen,
	Brown,
	DarkGray,
	LightGray,
	White,
	Red,
	Orange,
	Yellow,
	Green,
	Blue,
	Indigo,
	Pink,
	Peach,
	Colors,
};

enum Channel {
	SquareOne,
	SquareTwo,
};

enum Volume {
	Silent,
	Piano,
	Forte,
};

enum Mouse {
	MouseDown = 1,
	MouseMove,
	MouseUp,
};

void cset(int x, int y, int character, int foreground = Colors, int background = Colors);
void cls();
void sfx(int note, int channel = SquareOne, int volume = Forte);

void onframe(void (*func)());
void onchar(void (*func)(int));
void onmouse(void (*func)(int, int, Mouse));

}

void init();
