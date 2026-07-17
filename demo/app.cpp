#include <io/io.hpp>

int y = 8;
int x = 16;

int xs[] {0,0,0,1,2,2,2,1};
int ys[] {0,1,2,2,2,1,0,0};
int i = 0;
int ii = 0;

int j = 0;
int fy = 3;
int explosion = 0;

bool move;
int targetX;
int targetY;
int moveFuse;

void onmouse(int x, int y, io::Mouse m) {
	if (m == io::MouseDown) {
		move = true;
	}
	if (m == io::MouseUp) {
		move = false;
	}
	if (m == io::MouseMove) {
		targetX = x;
		targetY = y;
	}
}

void onframe() {
	if (move) {
		--moveFuse;
		if (moveFuse <= 0) {
			moveFuse = 4;
			if (targetY < y) {
				--y;
			}
			if (y < targetY) {
				++y;
			}
		}
		if (moveFuse == 2 || moveFuse == 4) {
			if (targetX < x) {
				--x;
			}
			if (x < targetX) {
				++x;
			}
		}
	}

	if (y == 8 && 8 <= x && x <= 10) {
		io::sfx(60+x-8, io::SquareOne);
		io::sfx(64+x-8, io::SquareTwo);
	}
	io::cls();
	io::cset(8,8,' ',9,9);
	io::cset(9,8,' ',9,9);
	io::cset(10,8,' ',9,9);
	bool in = x == 1 && y == 1;
	ii += 1;
	if (!in) {
		if (ii >= 1) {
			i += 1;
			i %= 8;
			ii = 0;
		}
	} else {
		if (ii >= 4) {
			i += 1;
			i %= 8;
			ii = 0;
		}
		int ns[] {61,63,66,68,70,68,66,63};
		io::sfx(ns[i]);
	}
	io::cset(xs[i], ys[i], 'U', 8, 1);
	if (explosion == 0 && y == fy && x == j) {
		explosion = 1;
	}
	if (explosion == 0) {
		++j;
		j %= 32;
		io::cset(j, fy, 'X', 10, 0);
	} else {
		if (explosion <= 2) {
			io::cset(j+1, fy, '/', 10, 0);
			io::cset(j-1, fy, '/', 10, 0);
			io::cset(j, fy+1, '.', 10, 0);
				++explosion;
			io::sfx(84);
			}
		else if (explosion <= 4) {
			io::cset(j+2,fy,  '-', 10, 0);
			io::cset(j-2,fy,  '/', 10, 0);
			io::cset(j+1,fy+2,'|', 10, 0);
				++explosion;
			io::sfx(82);
			}
		else if (explosion <= 6) {
			io::cset(j+2,fy-2,'/', 10, 0);
			io::cset(j-3,fy,  '.', 10, 0);
			io::cset(j+1,fy+3,'.', 10, 0);
				++explosion;
			io::sfx(80);
			}
		else {
			j = 0;
			fy += 6;
			fy %= 16;
			explosion = 0;
		}
	}
	io::cset(x, y, 'B', io::Blue, io::Green);
}

void init() {
	io::onframe(onframe);
	io::onmouse(onmouse);
}
