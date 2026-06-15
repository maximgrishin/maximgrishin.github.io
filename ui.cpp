#include "ui.hpp"

#include <emscripten.h>

#include <array>

extern "C" void cset(char y, char x, char c);
extern "C" void bset(char y, char x, char b);
extern "C" void fset(char y, char x, char f);
extern "C" char btnp();
extern "C" void nset(char c, char n);
extern "C" void vset(char c, char v);

constexpr char HEIGHT = 16;
constexpr char WIDTH = 32;
constexpr char CHANNELS = 4;

std::array<std::array<char, WIDTH>, HEIGHT> cbuf;
std::array<std::array<char, WIDTH>, HEIGHT> bbuf;
std::array<std::array<char, WIDTH>, HEIGHT> fbuf;
std::array<char, CHANNELS> nbuf;
std::array<char, CHANNELS> vbuf;

std::array<std::array<char, WIDTH>, HEIGHT> cscr;
std::array<std::array<char, WIDTH>, HEIGHT> bscr;
std::array<std::array<char, WIDTH>, HEIGHT> fscr;
std::array<char, CHANNELS> nsnd;
std::array<char, CHANNELS> vsnd;

extern "C" EMSCRIPTEN_KEEPALIVE void flip() {
	for (char y = 0; y < HEIGHT; ++y) {
		for (char x = 0; x < WIDTH; ++x) {
			if (cbuf[y][x] != cscr[y][x]) {
				cset(y, x, cbuf[y][x]);
			}
			if (bbuf[y][x] != bscr[y][x]) {
				bset(y, x, bbuf[y][x]);
			}
			if (fbuf[y][x] != fscr[y][x]) {
				fset(y, x, fbuf[y][x]);
			}
		}
	}
	cscr = cbuf;
	bscr = bbuf;
	fscr = fbuf;

	for (char c = 0; c < CHANNELS; ++c) {
		if (nbuf[c] != nsnd[c]) {
			nset(c, nbuf[c]);
		}
		if (vbuf[c] != vsnd[c]) {
			vset(c, vbuf[c]);
		}
	}
	nsnd = nbuf;
	vsnd = vbuf;
	vbuf.fill(0);
}

extern "C" void update();

extern "C" EMSCRIPTEN_KEEPALIVE void call_update() {
	update();
}

namespace ui {

void cset(char y, char x, char character, char foreground, char background) {
	if (y < 0 || HEIGHT <= y || x < 0 || WIDTH <= x) {
		return;
	}
	cbuf[y][x] = character;
	if (0 <= background && background < COLORS) {
		bbuf[y][x] = background;
	}
	if (0 <= foreground && foreground < COLORS) {
		fbuf[y][x] = foreground;
	}
}

void cls() {
	for (char y = 0; y < HEIGHT; ++y) {
		for (char x = 0; x < WIDTH; ++x) {
			cset(y, x, ' ', WHITE, BLACK);
		}
	}
}

char btnp() {
	return ::btnp();
}

void sfx(char channel, char note, char volume) {
	nbuf[channel] = note;
	vbuf[channel] = volume;
}

} // namespace ui
