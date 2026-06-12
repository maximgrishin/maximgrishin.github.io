#include "../lib.hpp"

#include <vector>

std::vector<std::vector<char>> char_buffer(16, std::vector<char>(32, ' '));
std::vector<std::vector<int>> bg_buffer(16, std::vector<int>(32, 0));
std::vector<std::vector<int>> fg_buffer(16, std::vector<int>(32, 7));

std::vector<std::vector<char>> char_buffer2(16, std::vector<char>(32, ' '));
std::vector<std::vector<int>> bg_buffer2(16, std::vector<int>(32, 0));
std::vector<std::vector<int>> fg_buffer2(16, std::vector<int>(32, 7));

void set(int i, int j, char c, int bg, int fg) {
	//cset(i,j,c,bg,fg);
	if (i < 0 || 16 <= i || j < 0 || 32 <= j) {
		return;
	}
	char_buffer[i][j] = c;
	bg_buffer[i][j] = bg;
	fg_buffer[i][j] = fg;
}

void flip() {
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			chset(i, j, char_buffer[i][j]);
			bgset(i, j, bg_buffer[i][j]);
			fgset(i, j, fg_buffer[i][j]);
		}
	}
}

void cls() {
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			set(i, j, ' ', 0, 7);
		}
	}
}

int y = 8;
int x = 16;

std::vector<int> xs{0,0,0,1,2,2,2,1};
std::vector<int> ys{0,1,2,2,2,1,0,0};
int i = 0;

int j = 0;
int fy = 3;
int explosion = 0;

extern "C" void update() {
	cls();
	set(ys[i], xs[i], 'U', 1, 8);
	i += 1;
	i %= 8;
	while (int b = btnp()) {
		if (b == 1) {
			y -= 1;
		}
		if (b == 2) {
			y += 1;
		}
		if (b == 3) {
			x -= 1;
		}
		if (b == 4) {
			x += 1;
		}
	}
	set(y, x, 'B', 2, 9);
	if (explosion==0&&y == fy && x == j) {
		explosion = 1;
	}
	if (explosion == 0) {
		++j;
		j %= 32;
		set(fy,j,'X', 0, 10);
	} else {
		if (explosion <= 4) {
			set(fy,j+1,'/', 0, 10);
			set(fy,j-1,'/', 0, 10);
			set(fy+1,j,'.', 0, 10);
				++explosion;
			}
		else if (explosion <= 8) {
			set(fy,j+2,'-', 0, 10);
			set(fy,j-2,'/', 0, 10);
			set(fy+2,j+1,'|', 0, 10);
				++explosion;
			}
		else if (explosion <= 12) {
			set(fy-2,j+2,'/', 0, 10);
			set(fy,j-3,'.', 0, 10);
			set(fy+3,j+1,'.', 0, 10);
				++explosion;
			}
		else {
			j = 0;
			fy += 6;
			fy %= 16;
			explosion = 0;
		}
	}
	flip();
}
