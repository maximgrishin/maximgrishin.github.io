#include "io.hpp"

int y = 8;
int x = 16;

int xs[] {0,0,0,1,2,2,2,1};
int ys[] {0,1,2,2,2,1,0,0};
int i = 0;
int ii = 0;

int j = 0;
int fy = 3;
int explosion = 0;

void io::btnp(io::Button b) {
	if (b == io::Up) {
		y -= 1;
	}
	if (b == io::Down) {
		y += 1;
	}
	if (b == io::Left) {
		x -= 1;
	}
	if (b == io::Right) {
		x += 1;
	}
}

void io::update() {
	if (y == 8 && 8 <= x && x <= 10) {
		io::sfx(io::Sine, 60+x-8, x-7);
		io::sfx(io::Triangle, 64+x-8, x-7);
	}
	io::cls();
	io::cset(8,8,' ',9,9);
	io::cset(8,9,' ',9,9);
	io::cset(8,10,' ',9,9);
	bool in = x == 1 && y == 1;
	ii += 1;
	if (!in) {
		if (ii >= 2) {
			i += 1;
			i %= 8;
			ii = 0;
		}
	} else {
		if (ii >= 8) {
			i += 1;
			i %= 8;
			ii = 0;
		}
		int ns[] {61,63,66,68,70,68,66,63};
		io::sfx(io::Sawtooth, ns[i], io::Forte);
	}
	io::cset(ys[i], xs[i], 'U', 8, 1);
	if (explosion == 0 && y == fy && x == j) {
		explosion = 1;
	}
	if (explosion == 0) {
		++j;
		j %= 32;
		io::cset(fy, j, 'X', 10, 0);
	} else {
		if (explosion <= 4) {
			io::cset(fy, j+1, '/', 10, 0);
			io::cset(fy, j-1, '/', 10, 0);
			io::cset(fy+1, j, '.', 10, 0);
				++explosion;
			io::sfx(io::Square, 84, io::Forte);
			}
		else if (explosion <= 8) {
			io::cset(fy, j+2, '-', 10, 0);
			io::cset(fy, j-2, '/', 10, 0);
			io::cset(fy+2,j+1,'|', 10, 0);
				++explosion;
			io::sfx(io::Square, 82, io::Forte);
			}
		else if (explosion <= 12) {
			io::cset(fy-2,j+2,'/', 10, 0);
			io::cset(fy, j-3, '.', 10, 0);
			io::cset(fy+3,j+1,'.', 10, 0);
				++explosion;
			io::sfx(io::Square, 80, io::Forte);
			}
		else {
			j = 0;
			fy += 6;
			fy %= 16;
			explosion = 0;
		}
	}
	io::cset(y, x, 'B', io::Blue, io::Green);
}
