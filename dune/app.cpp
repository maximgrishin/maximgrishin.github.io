#include "io.hpp"

constexpr int speed = 2;

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

		43, 43, 46,  0, 50, 	0, 50, 53,
		 0, 53, 55,  0, 58,  0, 55,  0,
		53, 53, 55,  0, 50,  0, 53, 50,
		 0, 50, 48,  0, 46,  0, 48,  0,
	}[step/2];
	note = transpose(note, shift);
	io::sfx(2, note, volume);
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
	io::sfx(2, note, volume);
}

int t;

void onframe() {
	++t;
	t %= 768*speed;
	if (t < 128*speed) {
		io::sfx(1, bassA(t), 2);
		bridge(t);
	}
	else if (t < 384*speed) {
		io::sfx(1, bassA(t-128*speed), 2);
		io::sfx(2, melodyA(t-128*speed), 2);
	}
	else if (t < 512*speed) {
		io::sfx(1, bassB(t-384*speed), 2);
		bridge(t-384*speed);
	}
	else {
		io::sfx(1, bassB(t-512*speed), 2);
		melodyB(t-512*speed);
	}
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			io::cset(j, i, ' ', 15, 15);
		}
	}
	for (int i = 5; i < 16; ++i) {
		for (int j = 0; j < 26; ++j) {
			io::cset(j, i, ' ', 4, 4);
		}
	}
	for (int j = 0; j < 32; ++j) {
		io::cset(j, 4 + j/19, ' ', 4, (bool[]){
			1,1,1,1,0,0,1,1,
			1,1,1,0,0,0,1,1,
			1,0,0,0,0,1,1,1,
			0,0,0,0,0,0,0,0,
		}[j] ? 4 : 15);
	}
	io::cset(26,2,' ',7,9);
	io::cset(27,2,' ',7,9);
	io::cset(28,2,' ',7,9);
	io::cset(29,2,' ',7,10);
	io::cset(29,1,'7',7,9);
	io::cset(30,1,'9',7,15);
	io::cset(28,1,' ',7,10);
	io::cset(30,3,' ',7,10);
	for (int i = 7; i < 10; ++i) {
		for (int j = 10; j < 16; ++j) {
			io::cset(j, i, ' ', 12, 1);
		}
	}
	for (int i = 7; i < 10; ++i) {
		for (int j = 16; j < 22; ++j) {
			io::cset(j, i, ' ', 12, 6);
		}
	}
	char spin = (int[]){'-','\\','|','/'}[t / 8 % 4];
	io::cset(10,9,spin,12,1);
	io::cset(16,9,spin,12,6);
	io::cset(4,12,' ',5,5);
	io::cset(5,12,' ',5,5);
	io::cset(4,13,' ',5,5);
	io::cset(5,13,' ',5,5);
	io::cset(3,13,' ',5,5);
	io::cset(2,13,' ',5,5);
	io::cset(0,3,' ',4,4);
	io::cset(26,15,' ',4,4);
	io::cset(27,15,' ',4,4);
}

void init() {
	io::onframe(onframe);
}
