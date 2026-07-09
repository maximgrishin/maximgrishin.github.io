#include "io.hpp"

namespace {

__attribute__((import_name("cset")))
extern "C"
void cset(int x, int y, int c);

__attribute__((import_name("bset")))
extern "C"
void bset(int x, int y, int b);

__attribute__((import_name("fset")))
extern "C"
void fset(int x, int y, int f);

__attribute__((import_name("nset")))
extern "C"
void nset(int c, int n);

__attribute__((import_name("vset")))
extern "C"
void vset(int c, int v);

__attribute__((import_name("focus")))
extern "C"
void focus();

constexpr int HEIGHT = 16;
constexpr int WIDTH = 32;
constexpr int CHANNELS = 4;

int cbuf[HEIGHT][WIDTH];
int bbuf[HEIGHT][WIDTH];
int fbuf[HEIGHT][WIDTH];
int nbuf[CHANNELS];
int vbuf[CHANNELS];

int cscr[HEIGHT][WIDTH];
int bscr[HEIGHT][WIDTH];
int fscr[HEIGHT][WIDTH];
int nsnd[CHANNELS];
int vsnd[CHANNELS];

void flip() {
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (cbuf[y][x] != cscr[y][x]) {
				cset(x, y, cbuf[y][x]);
				cscr[y][x] = cbuf[y][x];
			}
			if (bbuf[y][x] != bscr[y][x]) {
				bset(x, y, bbuf[y][x]);
				bscr[y][x] = bbuf[y][x];
			}
			if (fbuf[y][x] != fscr[y][x]) {
				fset(x, y, fbuf[y][x]);
				fscr[y][x] = fbuf[y][x];
			}
		}
	}

	for (int c = 0; c < CHANNELS; ++c) {
		if (nbuf[c] != nsnd[c]) {
			nset(c, nbuf[c]);
			nsnd[c] = nbuf[c];
		}
		if (vbuf[c] != vsnd[c]) {
			vset(c, vbuf[c]);
			vsnd[c] = vbuf[c];
		}
		vbuf[c] = 0;
	}
}

void print(int x, int y, char const *msg) {
	int i = 0;
	while (msg[i] != 0) {
		io::cset(x + i, y, msg[i], 6);
		++i;
	}
}

void (*frameCallback)();
void (*buttonCallback)(io::Button);
void (*mouseCallback)(int, int, io::Mouse);

bool started = false;

__attribute__((export_name("oninit")))
extern "C"
void oninit() {
	io::cls();
	for (int i = 0; i < 7; ++i) {
		io::cset(10, 5 + i, '|', 1 + i);
	}
	for (int i = 0; i < 4; ++i) {
		io::cset(13 + 3*i, 5 + i, '-', 8 + i);
	}
	for (int i = 0; i < 4; ++i) {
		io::cset(13 + 3*i, 11 - i, '-', 14 - i);
	}
	io::cset(13, 8, 'P', 12);
	io::cset(14, 8, 'L', 13);
	io::cset(15, 8, 'A', 14);
	io::cset(16, 8, 'Y', 15);
	print(6, 14, "controls: WASD, O, P");
	init();
	flip();
}

__attribute__((export_name("onframe")))
extern "C"
void onframe() {
	if (frameCallback && started) {
		frameCallback();
		flip();
	}
}

__attribute__((export_name("onchar")))
extern "C"
void onchar(int c) {
	if (buttonCallback) {
		if (c == 'w' || c == 'W') {
			buttonCallback(io::Up);
		}
		if (c == 's' || c == 'S') {
			buttonCallback(io::Down);
		}
		if (c == 'a' || c == 'A') {
			buttonCallback(io::Left);
		}
		if (c == 'd' || c == 'D') {
			buttonCallback(io::Right);
		}
		if (c == 'p' || c == 'P') {
			buttonCallback(io::ButtonA);
		}
		if (c == 'o' || c == 'O') {
			buttonCallback(io::ButtonB);
		}
	}
}

__attribute__((export_name("onmouse")))
extern "C"
void onmouse(int x, int y, io::Mouse m) {
	if (m == io::MouseDown) {
		started = true;
	}
	if (m == io::MouseUp) {
		focus();
	}
	if (mouseCallback) {
		mouseCallback(x, y, m);
	}
}

} // namespace

namespace io {

void cset(int x, int y, int character, int foreground, int background) {
	if (x < 0 || WIDTH <= x || y < 0 || HEIGHT <= y) {
		return;
	}
	cbuf[y][x] = character;
	if (0 <= background && background < Colors) {
		bbuf[y][x] = background;
	}
	if (0 <= foreground && foreground < Colors) {
		fbuf[y][x] = foreground;
	}
}

void cls() {
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			cset(x, y, ' ', White, Black);
		}
	}
}

void sfx(int channel, int note, int volume) {
	if (note == 0) {
		vbuf[channel] = 0;
	} else {
		nbuf[channel] = note;
		vbuf[channel] = volume;
	}
}

void onframe(void (*func)()) {
	frameCallback = func;
}

void onbutton(void (*func)(Button)) {
	buttonCallback = func;
}

void onmouse(void (*func)(int, int, Mouse)) {
	mouseCallback = func;
}

} // namespace io
