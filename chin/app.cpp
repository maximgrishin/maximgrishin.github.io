#include "chr.hpp"

#include "io.hpp"

constexpr int animation_tick = 2;

int x;
int y = 12;

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
int stroke;
int dot;

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
	stroke = 0;
	dot = 0;
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

bool isdot() {
	return chars[ch].strokes[stroke][dot].x;
}

bool nextdot() {
	if (chars[ch].strokes[stroke][dot].x) {
		++dot;
		return true;
	}
	return false;
}

bool nextstroke() {
	if (chars[ch].strokes[stroke + 1]) {
		++stroke;
		dot = 0;
		return true;
	}
	return false;
}

void dd() {
	if (isdot()) {
		fresh[chars[ch].strokes[stroke][dot].y - 1][chars[ch].strokes[stroke][dot].x - 1] = true;
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

bool in_canvas() {
	return 0 <= x && x < SIDE && 0 <= y && y < SIDE;
}

void handleExamPress(int x, int y) {
	Dot& d = chars[ch].strokes[stroke][dot];
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

void onbutton(io::Button b) {
	if (b == io::Up) {
		if ((mode == Demo && y == 12) || (mode != Demo && y > 0)) {
			y -= 1;
		}
	}
	if (b == io::Down) {
		if (y < SIDE - 1) {
			y += 1;
		}
		else if (y == SIDE - 1 && mode == Free) {
			y += 1;
		}
		else if (y == SIDE && mode != Free) {
			y += 1;
		}
	}
	if (b == io::Left) {
		if (in_canvas() && x > 0) {
			x -= 1;
		}
		else {
			if (y == SIDE) {
				prevmode();
			}
			if (y == SIDE + 1) {
				prevch();
			}
		}
	}
	if (b == io::Right) {
		if (in_canvas() && x < SIDE - 1) {
			x += 1;
		}
		else {
			if (y == SIDE) {
				nextmode();
			}
			if (y == SIDE + 1) {
				nextch();
			}
		}
	}
	if (b == io::ButtonA && in_canvas()) {
		if (mode == Free) {
			canvas[y][x] = true;
		}
		else if (mode == Exam && !blink_fuse && !victory_fuse) {
			handleExamPress(x, y);
		}
	}
	if (b == io::ButtonB && in_canvas()) {
		if (mode == Free) {
			canvas[y][x] = false;
		} else if (mode == Exam) {
			y = 11;
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
		if (in_canvas()) {
			pset(y, x, io::Red);
		}
		draw_canvas(done, io::Blue);
		draw_canvas(stale, io::Blue + (blink_fuse ? 0 : victory_fuse/animation_tick));
		draw_canvas(fresh, io::Green + blink_fuse/animation_tick);
	}
	if (in_canvas()) {
		if (!blink_fuse && !victory_fuse) {
			if ((mode != Free && (fresh[y][x] || stale[y][x])) || (mode == Free && canvas[y][x])) {
				pset(y, x, io::Pink);
			}
			else {
				pset(y, x, io::Red);
			}
		}
	} else {
		pset(y, 0, io::Red);
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
	if (in_canvas() && mode == Free) {
		io::cset(8, 13, 'y', io::DarkGray);
		io::cset(9, 13, ':', io::DarkGray);
		io::cset(11, 13, '0' + (y+1)/10, io::DarkGray);
		io::cset(12, 13, '0' + (y+1)%10, io::DarkGray);
		io::cset(8, 14, 'x', io::DarkGray);
		io::cset(9, 14, ':', io::DarkGray);
		io::cset(11, 14, '0' + (x+1)/10, io::DarkGray);
		io::cset(12, 14, '0' + (x+1)%10, io::DarkGray);
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
int lastMouseX;
int lastMouseY;
void onmouse(int x, int y, io::Mouse m) {
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
		if (lastMouseX != x || lastMouseY != y) {
			if (mode == Free) {
				canvas[y][x] = freeModePaintValue;
			}
			else if (mode == Exam) {
				handleExamPress(x, y);
			}
			lastMouseX = x;
			lastMouseY = y;
		}
	}
}

void init() {
	io::onframe(onframe);
	io::onbutton(onbutton);
	io::onmouse(onmouse);
}
