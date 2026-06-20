#include "../ui.hpp"

int y = 8;
int x = 16;

int xs[] {0,0,0,1,2,2,2,1};
int ys[] {0,1,2,2,2,1,0,0};
int i = 0;
int ii = 0;

int j = 0;
int fy = 3;
int explosion = 0;

__attribute__((export_name("btnp")))
extern "C"
void btnp(int b) {
	if (b == ui::UP) {
		y -= 1;
	}
	if (b == ui::DOWN) {
		y += 1;
	}
	if (b == ui::LEFT) {
		x -= 1;
	}
	if (b == ui::RIGHT) {
		x += 1;
	}
}

__attribute__((export_name("update")))
extern "C"
void update() {
	if (y == 8 && 8 <= x && x <= 10) {
		ui::sfx(ui::SINE, 60+x-8, ui::FORTE);
		ui::sfx(ui::TRIANGLE, 64+x-8, ui::FORTE);
	}
	ui::cls();
	ui::cset(8,8,' ',9,9);
	ui::cset(8,9,' ',9,9);
	ui::cset(8,10,' ',9,9);
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
		ui::sfx(ui::SAWTOOTH, ns[i], ui::FORTE);
	}
	ui::cset(ys[i], xs[i], 'U', 8, 1);
	if (explosion == 0 && y == fy && x == j) {
		explosion = 1;
	}
	if (explosion == 0) {
		++j;
		j %= 32;
		ui::cset(fy, j, 'X', 10, 0);
	} else {
		if (explosion <= 4) {
			ui::cset(fy, j+1, '/', 10, 0);
			ui::cset(fy, j-1, '/', 10, 0);
			ui::cset(fy+1, j, '.', 10, 0);
				++explosion;
			ui::sfx(ui::SQUARE, 84, ui::FORTE);
			}
		else if (explosion <= 8) {
			ui::cset(fy, j+2, '-', 10, 0);
			ui::cset(fy, j-2, '/', 10, 0);
			ui::cset(fy+2,j+1,'|', 10, 0);
				++explosion;
			ui::sfx(ui::SQUARE, 82, ui::FORTE);
			}
		else if (explosion <= 12) {
			ui::cset(fy-2,j+2,'/', 10, 0);
			ui::cset(fy, j-3, '.', 10, 0);
			ui::cset(fy+3,j+1,'.', 10, 0);
				++explosion;
			ui::sfx(ui::SQUARE, 80, ui::FORTE);
			}
		else {
			j = 0;
			fy += 6;
			fy %= 16;
			explosion = 0;
		}
	}
	ui::cset(y, x, 'B', ui::BLUE, ui::GREEN);
}
