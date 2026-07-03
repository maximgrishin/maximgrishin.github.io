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

void cset(int y, int x, int character, int foreground = Colors, int background = Colors);
void cls();
void sfx(int channel, int note, int volume);

void registerUpdate(void (*func)());
void registerBtnp(void (*func)(Button ));

extern "C++" void init();

}
