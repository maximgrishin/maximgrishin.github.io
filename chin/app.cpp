#include "chr.hpp"

#include <io/io.hpp>

constexpr int animation_tick = 2;

void pset(int y, int x, int col) {
	io::cset(2*x+5, y+1, ' ', io::Black, col);
	io::cset(2*x+6, y+1, ' ', io::Black, col);
}

constexpr int SIDE = 11;

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

int getCurrentY() {
	return getCurrentChar()-'0';
}

int getCurrentX() {
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
	for (int y = 3; y < SIDE; y += 4) {
		for (int x = 0; x < SIDE; ++x) {
			pset(y, x, io::DarkBlue);
		}
	}
	for (int x = 3; x < SIDE; x += 4) {
		for (int y = 0; y < SIDE; ++y) {
			pset(y, x, io::DarkBlue);
		}
	}
}

void draw_canvas(bool canvas[SIDE][SIDE], int col) {
	for (int y = 0; y < SIDE; ++y) {
		for (int x = 0; x < SIDE; ++x) {
			if (canvas[y][x]) {
				pset(y, x, col);
			}
		}
	}
}

int animation_fuse = 2*animation_tick;

void print(int y, int x, char const *str) {
	for (int i = 0; str[i] != 0 && x + i < 32; ++i) {
		io::cset(x + i, y, str[i]);
	}
}

int lastMouseX;
int lastMouseY;

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
		print(12, 8, "mode: canvas");
		break;
	case Demo:
		print(12, 8, "mode: demo");
		break;
	case Exam:
		print(12, 8, "mode: exam");
		break;
	}
	if (mode != Free) {
		int up = chars[ch].unicode_point;
		char const *hex = "0123456789abcdef";
		print(13, 8, "char: ");
		print(13, 14, chars[ch].name);
		print(14, 8, "U+");
		io::cset(8, 14, 'U', io::DarkGray);
		io::cset(9, 14, '+', io::DarkGray);
		io::cset(10, 14, hex[up%0x10000/0x1000], io::DarkGray);
		io::cset(11, 14, hex[up%0x1000/0x100], io::DarkGray);
		io::cset(12, 14, hex[up%0x100/0x10], io::DarkGray);
		io::cset(13, 14, hex[up%0x10], io::DarkGray);
	}
	if (mode == Free) {
		io::cset(8, 13, 'y', io::DarkGray);
		io::cset(9, 13, ':', io::DarkGray);
		io::cset(11, 13, '0' + (lastMouseY+1)/10, io::DarkGray);
		io::cset(12, 13, '0' + (lastMouseY+1)%10, io::DarkGray);
		io::cset(8, 14, 'x', io::DarkGray);
		io::cset(9, 14, ':', io::DarkGray);
		io::cset(11, 14, '0' + (lastMouseX+1)/10, io::DarkGray);
		io::cset(12, 14, '0' + (lastMouseX+1)%10, io::DarkGray);
	}
	io::cset(4, 12, '<', io::Red);
	io::cset(6, 12, '>', io::Red);
	if (mode != Free) {
		io::cset(4, 13, '<', io::Red);
		io::cset(6, 13, '>', io::Red);
	}
}

int fromYToCanvasY(int y) {
	y -= 1;
	if (0 <= y && y < 11) {
		return y;
	}
	return -1;
}

int fromXToCanvasX(int x) {
	x -= 5;
	x /= 2;
	if (0 <= x && x < 11) {
		return x;
	}
	return -1;
}

bool mouseDown;
bool freeModePaintValue;
void onmouse(int x, int y, io::Mouse m) {
	if (m == io::MouseDown) {
		if (x == 4) {
			if (y == 12) {
				prevmode();
			}
			if (y == 13 && mode != Free) {
				prevch();
			}
		}
		if (x == 6) {
			if (y == 12) {
				nextmode();
			}
			if (y == 13 && mode != Free) {
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
