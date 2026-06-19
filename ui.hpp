namespace ui {

enum {
	BLACK,
	DARK_BLUE,
	DARK_PURPLE,
	DARK_GREEN,
	BROWN,
	DARK_GRAY,
	LIGHT_GRAY,
	WHITE,
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	INDIGO,
	PINK,
	PEACH,
	COLORS,
};

enum {
	SINE,
	SQUARE,
	TRIANGLE,
	SAWTOOTH,
};

enum {
	SILENT,
	PIANO,
	MEZZO,
	FORTE,
};

void cset(int y, int x, int character, int foreground = COLORS, int background = COLORS);
void cls();
int btnp();
void sfx(int channel, int note, int volume);

} // namespace ui
