#include "chr.hpp"

#include <io/io.hpp>

constexpr int animation_tick = 3;

constexpr int TOP_MARGIN = 0;
constexpr int LEFT_MARGIN = 2;

constexpr int CELL_WIDTH = 4;
constexpr int CELL_HEIGHT = 2;

void pset(int x, int y, int col) {
	for (int i = 0; i < CELL_WIDTH; ++i) {
		for (int j = 0; j < CELL_HEIGHT; ++j) {
			io::cset(LEFT_MARGIN + x * CELL_WIDTH + i, TOP_MARGIN + y * CELL_HEIGHT + j, ' ', io::Black, col);
		}
	}
}

constexpr int SIDE = 7;

bool canvas[SIDE][SIDE];
bool fresh[SIDE][SIDE];
bool stale[SIDE][SIDE];
bool done[SIDE][SIDE];

int ch;
int strokedot = 1;

int blink_fuse;
int victory_fuse;

void clear_canvas(bool canvas[SIDE][SIDE]) {
	for (int y = 0; y < SIDE; ++y) {
		for (int x = 0; x < SIDE; ++x) {
			canvas[y][x] = false;
		}
	}
}

void copy_canvas(bool to[SIDE][SIDE], bool from[SIDE][SIDE]) {
	for (int y = 0; y < SIDE; ++y) {
		for (int x = 0; x < SIDE; ++x) {
			if (from[y][x]) {
				to[y][x] = from[y][x];
			}
		}
	}
	clear_canvas(from);
}

void resetch() {
	strokedot = 1;
	clear_canvas(fresh);
	clear_canvas(stale);
	clear_canvas(done);
}

void prevch() {
	if (ch > 0) {
		--ch;
		resetch();
	}
}

void nextch() {
	if (chars[ch + 1].unicode_point) {
		++ch;
		resetch();
	}
}

char getCurrentChar() {
	return chars[ch].strokes[strokedot];
}

int getCurrentX() {
	return getCurrentChar()-'0';
}

int getCurrentY() {
	return chars[ch].strokes[strokedot + 1]-'0';
}

void plusplusdot() {
	strokedot += 2;
}

bool isdot() {
	return '1' <= getCurrentChar() && getCurrentChar() <= ';';
}

bool nextdot() {
	if (isdot()) {
		plusplusdot();
		return true;
	}
	return false;
}

bool nextstroke() {
	while (getCurrentChar() != 0) {
		++strokedot;
		if (isdot()) {
			return true;
		}
	}
	return false;
}

void dd() {
	if (isdot()) {
		fresh[getCurrentY() - 1][getCurrentX() - 1] = true;
		nextdot();
	}
	else if (nextstroke()) {
		blink_fuse = 1;
	}
	else {
		blink_fuse = 1;
		victory_fuse = 1;
	}
}

enum Mode {
	Demo,
	Exam,
	Free,
};
int mode;
void prevmode() {
	if (mode > 0) {
		--mode;
		resetch();
	}
}
void nextmode() {
	if (mode < 3 - 1) {
		++mode;
		resetch();
	}
}

Dot getCurrentDot() {
	return {getCurrentY(), getCurrentX()};
}

void handleExamPress(int x, int y) {
	Dot d = getCurrentDot();
	if (x == d.x - 1 && y == d.y - 1) {
		dd();
		if (!isdot()) {
			dd();
			blink_fuse = 4*animation_tick-1;
			if (!isdot()) {
				dd();
				blink_fuse = 4*animation_tick-1;
				victory_fuse = 4*animation_tick-1;
			}
		}
	}
}

void draw_grid() {
	for (int i = 0; i < SIDE; ++i) {
		pset(i, SIDE/2, io::DarkBlue);
	}
	for (int i = 0; i < SIDE; ++i) {
		pset(SIDE/2, i, io::DarkBlue);
	}
}

void draw_canvas(bool canvas[SIDE][SIDE], int col) {
	for (int y = 0; y < SIDE; ++y) {
		for (int x = 0; x < SIDE; ++x) {
			if (canvas[y][x]) {
				pset(x, y, col);
			}
		}
	}
}

int animation_fuse = 2*animation_tick;

void print(int x, int y, char const *str, io::Color color = io::White) {
	for (int i = 0; str[i] != 0 && x + i < 32; ++i) {
		io::cset(x + i, y, str[i], color);
	}
}

int lastMouseX;
int lastMouseY;

constexpr int INFO_UI_LINE_Y = 14;
constexpr int INFO_UI_LINE_X = 18;
constexpr int FIRST_UI_LINE_Y = 14;
constexpr int SECOND_UI_LINE_Y = 15;

void onframe() {
	io::cls();
	draw_grid();
	if (mode == Demo) {
		--animation_fuse;
		if (animation_fuse <= 0) {
			dd();
			animation_fuse = animation_tick;
		}
	}
	if (mode == Free) {
		draw_canvas(canvas, io::LightGray);
	}
	else {
		if (blink_fuse) {
			io::sfx(io::Square, (int[]){0,57,54,50,47}[(blink_fuse + animation_tick - 2)/animation_tick], io::Forte);
			--blink_fuse;
			if (blink_fuse == 0) {
				copy_canvas(stale, fresh);
			}
		}
		if (victory_fuse && !blink_fuse) {
			io::sfx(io::Square, (int[]){0,74,69,66,62}[(victory_fuse + animation_tick - 2)/animation_tick], io::Forte);
			--victory_fuse;
			if (victory_fuse == 0) {
				copy_canvas(done, stale);
			}
		}
		draw_canvas(done, io::Blue);
		draw_canvas(stale, io::Blue + (blink_fuse ? 0 : victory_fuse/animation_tick));
		draw_canvas(fresh, io::Green + blink_fuse/animation_tick);
	}
	switch (mode) {
	case Free:
		print(LEFT_MARGIN + 4, FIRST_UI_LINE_Y, "mode: canvas");
		break;
	case Demo:
		print(LEFT_MARGIN + 4, FIRST_UI_LINE_Y, "mode: demo");
		break;
	case Exam:
		print(LEFT_MARGIN + 4, FIRST_UI_LINE_Y, "mode: exam");
		break;
	}
	if (mode != Free) {
		int up = chars[ch].unicode_point;
		char const *hex = "0123456789abcdef";
		print(LEFT_MARGIN + 4, SECOND_UI_LINE_Y, "char: ");
		print(LEFT_MARGIN + 10, SECOND_UI_LINE_Y, chars[ch].name);
		print(LEFT_MARGIN + INFO_UI_LINE_X + 4, INFO_UI_LINE_Y, "U+", io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 6, INFO_UI_LINE_Y, hex[up%0x10000/0x1000], io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 7, INFO_UI_LINE_Y, hex[up%0x1000/0x100], io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 8, INFO_UI_LINE_Y, hex[up%0x100/0x10], io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 9, INFO_UI_LINE_Y, hex[up%0x10], io::DarkGray);
	}
	if (mode == Free) {
		print(LEFT_MARGIN + INFO_UI_LINE_X + 4, INFO_UI_LINE_Y, "xy:", io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 8, INFO_UI_LINE_Y, '0' + (lastMouseX+1)%10, io::DarkGray);
		io::cset(LEFT_MARGIN + INFO_UI_LINE_X + 9, INFO_UI_LINE_Y, '0' + (lastMouseY+1)%10, io::DarkGray);
	}
	io::cset(LEFT_MARGIN, FIRST_UI_LINE_Y, '<', io::Red);
	io::cset(LEFT_MARGIN + 2, FIRST_UI_LINE_Y, '>', io::Red);
	if (mode != Free) {
		io::cset(LEFT_MARGIN, SECOND_UI_LINE_Y, '<', io::Red);
		io::cset(LEFT_MARGIN + 2, SECOND_UI_LINE_Y, '>', io::Red);
	}
}

int fromYToCanvasY(int y) {
	y -= TOP_MARGIN;
	y /= CELL_HEIGHT;
	if (0 <= y && y < SIDE) {
		return y;
	}
	return -1;
}

int fromXToCanvasX(int x) {
	x -= LEFT_MARGIN;
	x /= CELL_WIDTH;
	if (0 <= x && x < SIDE) {
		return x;
	}
	return -1;
}

bool mouseDown;
bool freeModePaintValue;
void onmouse(int x, int y, io::Mouse m) {
	if (m == io::MouseDown) {
		if (LEFT_MARGIN - 2 <= x && x <= LEFT_MARGIN) {
			if (y == FIRST_UI_LINE_Y) {
				prevmode();
			}
			if (y == SECOND_UI_LINE_Y && mode != Free) {
				prevch();
			}
		}
		if (LEFT_MARGIN + 2 <= x && x <= LEFT_MARGIN + 4) {
			if (y == FIRST_UI_LINE_Y) {
				nextmode();
			}
			if (y == SECOND_UI_LINE_Y && mode != Free) {
				nextch();
			}
		}
	}
	x = fromXToCanvasX(x);
	y = fromYToCanvasY(y);
	if (x == -1 || y == -1) {
		return;
	}
	if (m == io::MouseDown) {
		mouseDown = true;
		if (mode == Free) {
			freeModePaintValue = !canvas[y][x];
		}
	}
	else if (m == io::MouseUp) {
		mouseDown = false;
	}
	if (mouseDown) {
		if (m != io::MouseMove || lastMouseX != x || lastMouseY != y) {
			if (mode == Free) {
				canvas[y][x] = freeModePaintValue;
			}
			else if (mode == Exam && !blink_fuse && !victory_fuse) {
				handleExamPress(x, y);
			}
		}
	}
	lastMouseX = x;
	lastMouseY = y;
}

void init() {
	io::onframe(onframe);
	io::onmouse(onmouse);
}
