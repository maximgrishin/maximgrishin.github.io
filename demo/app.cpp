#include "../lib.hpp"

#include <vector>

void cls() {
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			cset(i, j, ' ', 0, 7);
		}
	}
}

int y = 8;
int x = 16;

std::vector<int> xs{0,0,0,1,2,2,2,1};
std::vector<int> ys{0,1,2,2,2,1,0,0};
int i = 0;

int j = 0;

extern "C" void update() {
	cls();
	cset(ys[i], xs[i], 'U', 1, 8);
	cset(3,j,'F', 3, 10);
	i += 1;
	++j;
	j %= 32;
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
	cset(y, x, 'B', 2, 9);
}
