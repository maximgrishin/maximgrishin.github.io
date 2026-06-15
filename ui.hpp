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

void cset(char y, char x, char character, char foreground = COLORS, char background = COLORS);
void cls();
char btnp();
void sfx(char channel, char note, char volume);

} // namespace ui
