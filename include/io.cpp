#include "io.hpp"

namespace {

__attribute__((import_name("cset")))
extern "C"
void cset(int y, int x, int c);

__attribute__((import_name("bset")))
extern "C"
void bset(int y, int x, int b);

__attribute__((import_name("fset")))
extern "C"
void fset(int y, int x, int f);

__attribute__((import_name("nset")))
extern "C"
void nset(int c, int n);

__attribute__((import_name("vset")))
extern "C"
void vset(int c, int v);

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
				cset(y, x, cbuf[y][x]);
				cscr[y][x] = cbuf[y][x];
			}
			if (bbuf[y][x] != bscr[y][x]) {
				bset(y, x, bbuf[y][x]);
				bscr[y][x] = bbuf[y][x];
			}
			if (fbuf[y][x] != fscr[y][x]) {
				fset(y, x, fbuf[y][x]);
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

void print(int y, int x, char const *msg) {
	int i = 0;
	while (msg[i] != 0) {
		io::cset(y, x + i, msg[i], 6);
		++i;
	}
}

void (*update)();
void (*btnp)(io::Button);

__attribute__((export_name("call_onbutton")))
extern "C"
void call_onbutton(io::Button b) {
	if (btnp) {
		btnp(b);
	}
}

bool started = false;

__attribute__((export_name("call_onframe")))
extern "C"
void call_onframe() {
	if (update && started) {
		update();
		flip();
	}
}

__attribute__((export_name("call_oninit")))
extern "C"
void call_oninit() {
	io::cls();
	for (int i = 0; i < 7; ++i) {
		io::cset(5 + i, 10, '|', 1 + i);
	}
	for (int i = 0; i < 4; ++i) {
		io::cset(5 + i, 13 + 3*i, '-', 8 + i);
	}
	for (int i = 0; i < 4; ++i) {
		io::cset(11 - i, 13 + 3*i, '-', 14 - i);
	}
	io::cset(8, 13, 'P', 12);
	io::cset(8, 14, 'L', 13);
	io::cset(8, 15, 'A', 14);
	io::cset(8, 16, 'Y', 15);
	print(14, 6, "controls: WASD, O, P");
	io::init();
	flip();
}

__attribute__((export_name("call_onclick")))
extern "C"
void call_onclick([[maybe_unused]] int x, [[maybe_unused]] int y) {
	started = true;
}

} // namespace

namespace io {

void cset(int y, int x, int character, int foreground, int background) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
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
			cset(y, x, ' ', White, Black);
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

void registerUpdate(void (*func)()) {
	update = func;
}

void registerBtnp(void (*func)(Button)) {
	btnp = func;
}

extern "C++" void init();
} // namespace io
