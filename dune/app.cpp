#include <io/io.hpp>

constexpr int speed = 2;

constexpr io::Channel BassChannel = io::Square;
constexpr io::Channel MelodyChannel = io::Triangle;

int transpose(int note, int shift) {
	return note != 0 ? note + shift : 0;
}

int bassA(int tick) {
	int step = tick/speed;
	int volume = (bool[]){
		0, 0, 0, 0, 1, 0, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 0,
		1, 0, 1, 0, 1, 0, 0, 0,
		1, 0, 1, 0, 1, 0, 0, 0,
	}[step % 32];
	int note = (int[]){
		50, 50, 51, 48,
		50, 50, 56, 55,
	}[step / 8 % 8];
	return transpose(volume ? note : 0, 0);
}

int bassB(int tick) {
	int step = tick/speed;
	int volume = (bool[]){
		1, 1, 0, 0, 0, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 1, 1,
		1, 1, 0, 0, 0, 0, 1, 1,
		0, 0, 0, 0, 1, 1, 1, 1,
	}[step/2 % 32];
	int note = (int[]){
		43, 43, 43, 41,
		43, 43, 43, 46,
	}[step / 8 % 8];
	return transpose(volume ? note : 0, 0);
}

int melodyA(int tick) {
	constexpr int shift = 12;
	int step = tick/speed % 256;
	if (176 <= step && step < 192) {
		int note = (int[]){
			43, 45, 46, 48, 46, 48, 49, 51,
			50, 51, 53, 55, 53, 55, 57, 58,
		}[step-176];
		return transpose(note, shift);
	}
	if (224 <= step && step < 256) {
		int note = (int[]){
			67, 65, 63, 62, 60, 58, 57, 55,
			65, 63, 62, 60, 58, 57, 55, 53,
			63, 62, 60, 58, 57, 55, 53, 51,
			62, 60, 58, 57, 55, 53, 51, 50,
		}[step-224];
		return transpose(note, shift);
	}
	int note = (int[]){
		58, 58, 58, 58, 58, 58, 58, 58,
		58, 58, 58, 58, 58, 58, 57, 55,
		50, 50, 50, 50, 50, 50, 50, 50,
		50, 50, 50, 50, 50, 50, 50, 50,

		51, 51, 51, 51, 51, 51, 51, 51,
		51, 51, 51, 51, 55, 55, 51, 51,
		50, 51, 50, 48, 50, 50, 50, 50,
		50, 50, 50, 50, 50, 50, 50, 50,

		50, 50, 50, 50, 43, 43, 45, 45,
		46, 46, 46, 46, 50, 50, 50, 50,
		49, 49, 49, 49, 49, 49, 49, 49,
		 0,  0,  0,  0,  0,  0,  0,  0,

		60, 60, 60, 60, 60, 60, 60, 60,
		60, 60, 60, 60, 63, 63, 63, 63,
		 0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,
	}[step/2];
	return transpose(note, shift);
}

void melodyB(int tick) {
	constexpr int shift = 24;
	int step = tick/speed % 128;
	int volume = 0;
	if ((step < 48 && step % 8 >= 5) || step == 55 || step == 67 || (68 <= step && step % 2 == 1)) {
		volume = 1;
	} else {
		volume = 2;
	}
	int note = (int[]){
		50, 50, 50, 50, 46, 46, 46, 46,
		 0,  0,  0,  0,  0,  0,  0,  0,
		48, 48, 48, 48, 45, 45, 45, 45,
		45, 45, 45, 45,  0,  0,  0,  0,

		43, 43, 46,  0, 50,  0, 50, 53,
		 0, 53, 55,  0, 58,  0, 55,  0,
		53, 53, 55,  0, 50,  0, 53, 50,
		 0, 50, 48,  0, 46,  0, 48,  0,
	}[step/2];
	note = transpose(note, shift);
	io::sfx(note, MelodyChannel, volume);
}

void bridge(int tick) {
	int shift = 24;
	int step = tick/speed % 128;
	int volume = 0;
	if (step % 8 == 6) {
		volume = 1;
	} else {
		volume = 2;
	}
	int note = (int[]){
		43, 43, 43, 43,  0,  0, 41, 43,
		43, 41, 43, 43, 44, 44, 44, 44,
	}[step/2 % 16];
	if (step % 2 == 1) {
		note = 0;
	}
	if (step / 64 == 1) {
		shift += 7;
	}
	note = transpose(note, shift);
	io::sfx(note, MelodyChannel, volume);
}

int cameraX;
int cameraY = 16;

void mset(int x, int y, int ch, int fg, int bg) {
	io::cset(2*x - cameraX, y - cameraY, ch, fg, bg);
	io::cset(2*x+1 - cameraX, y - cameraY, ch, fg, bg);
}

void sset(int x, int y, int ch, int fg, int bg) {
	io::cset(x - cameraX, y - cameraY, ch, fg, bg);
}

constexpr int MAP_WIDTH = 32;
constexpr int MAP_HEIGHT = 32;

char const* mapConfig = R"(
44333000000000000111111111111222
44300000000000000011111111111112
43030000000000000001111111111111
33300000000000000001111111111121
00000000000000000001111111111221
00000000000000300011111111111111
00000000000000300001111111111111
00000000000000340001121111111111
00000000000000030000122111111111
00000000000000000001111111111111
00000000000000000000011000111000
00000000000000000000000000000000
00000000000000001110000000003000
00000000000000011111000000044300
00000033000000012211000000000030
00000330000000012111000000000000
00000000000000001110000000000000
00000000000000000000000000000000
00000000000000000000000000000000
10000000000000000000000000000033
11011001000000000000000000000334
11111111101100000033000000000034
11111111111110000343300000000003
11111111111110003434433000000003
11111111111110003433330000000000
11111111111110000343300000000000
11111111111110000330000000000000
11111111111110000000000000000000
11211111111110000000000000330000
12211111111110000000000000030000
11111111111110000000000000000000
11111111111111000000000000000000
)";

char map[MAP_HEIGHT][MAP_WIDTH];

void readMap() {
	int x = 0;
	int y = -1;
	char const* mapReader = mapConfig;
	while (*mapReader != 0) {
		if (*mapReader == '\n') {
			++y;
			x = 0;
		}
		else {
			map[y][x] = *mapReader;
			++x;
		}
		++mapReader;
	}
}

void printint(int y, int num) {
	int x = 31;
	if (num == 0) {
		cset(x, y, '0', io::White, io::Black);
	}
	bool isNegative = num < 0;
	if (isNegative) {
		num = -num;
	}
	while (num != 0) {
		cset(x, y, '0' + (num % 10), io::White, io::Black);
		num /= 10;
		--x;
	}
	if (isNegative) {
		cset(x, y, '-', io::White, io::Black);
	}
}

int t;

void onframe() {
	++t;
	t %= 768*speed;
	if (t < 128*speed) {
		io::sfx(bassA(t), BassChannel, 2);
		bridge(t);
	}
	else if (t < 384*speed) {
		io::sfx(bassA(t-128*speed), BassChannel, 2);
		io::sfx(melodyA(t-128*speed), MelodyChannel, 2);
	}
	else if (t < 512*speed) {
		io::sfx(bassB(t-384*speed), BassChannel, 2);
		bridge(t-384*speed);
	}
	else {
		io::sfx(bassB(t-512*speed), BassChannel, 2);
		melodyB(t-512*speed);
	}

	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			int color = (int[]){15,4,5,10,9}[map[i][j] - '0'];
			mset(j, i, ' ', color, color);
		}
	}

	for (int i = 24; i < 26; ++i) {
		for (int j = 6; j < 8; ++j) {
			mset(j, i, ' ', 12, 1);
		}
	}
	for (int i = 24; i < 26; ++i) {
		for (int j = 8; j < 10; ++j) {
			mset(j, i, ' ', 12, 6);
		}
	}
	for (int i = 2; i < 4; ++i) {
		for (int j = 23; j < 25; ++j) {
			mset(j, i, ' ', 12, 13);
		}
	}
	for (int i = 2; i < 4; ++i) {
		for (int j = 25; j < 27; ++j) {
			mset(j, i, ' ', 12, 1);
		}
	}
	for (int i = 4; i < 6; ++i) {
		for (int j = 24; j < 27; ++j) {
			mset(j, i, ' ', 12, 6);
		}
	}
	char spin = (int[]){'-','\\','|','/'}[t / 8 % 4];
	sset(12,25,spin,12,1);
	sset(16,25,spin,12,6);
	sset(46,3,spin,8,13);
	sset(50,3,spin,8,1);
	sset(48,5,spin,8,6);
	io::cset(28,1,'8',7);
	io::cset(29,1,'3',7);
	io::cset(30,1,'2',7);
	//printint(14, cameraX);
	//printint(15, cameraY);
}

int dragStartX;
int dragStartY;
int lastMouseX;
int lastMouseY;
bool isMouseDown;

void clamp(int* val, int lo, int hi) {
	if (*val < lo) {
		*val = lo;
	}
	if (*val > hi) {
		*val = hi;
	}
}

void onmouse(int x, int y, io::Mouse m) {
	if (m == io::MouseDown) {
		dragStartX = x;
		dragStartY = y;
		isMouseDown = true;
	}
	if (m == io::MouseUp) {
		isMouseDown = false;
	}
	if (isMouseDown && m == io::MouseMove) {
		const int dragShiftX = lastMouseX - x;
		const int dragShiftY = lastMouseY - y;
		cameraX += dragShiftX;
		cameraY += dragShiftY;
		clamp(&cameraX, 0, 32);
		clamp(&cameraY, 0, 16);
	}
	lastMouseX = x;
	lastMouseY = y;
}

void init() {
	readMap();
	io::onframe(onframe);
	io::onmouse(onmouse);
}
