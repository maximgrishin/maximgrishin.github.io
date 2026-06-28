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

__attribute__((export_name("flip")))
extern "C"
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

#include "io.hpp"

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
	nbuf[channel] = note;
	vbuf[channel] = volume;
}

} // namespace io

__attribute__((export_name("call_btnp")))
extern "C"
void call_btnp(io::Button b) {
	io::btnp(b);
}

__attribute__((export_name("call_update")))
extern "C"
void call_update() {
	io::update();
}
