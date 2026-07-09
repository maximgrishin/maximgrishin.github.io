#pragma once

namespace io {

enum {
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

enum {
	Sine,
	Square,
	Triangle,
	Sawtooth,
};

enum {
	Silent,
	Piano,
	Forte,
};

enum Button {
	NoInput,
	Up,
	Down,
	Left,
	Right,
	ButtonA,
	ButtonB,
};

enum Mouse {
	MouseDown = 1,
	MouseMove,
	MouseUp,
};

void cset(int x, int y, int character, int foreground = Colors, int background = Colors);
void cls();
void sfx(int channel, int note, int volume);

void onframe(void (*func)());
void onbutton(void (*func)(Button));
void onmouse(void (*func)(int, int, Mouse));

}

void init();
