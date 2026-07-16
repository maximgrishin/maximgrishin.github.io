struct Dot {
	int y = 0;
	int x = 0;
};

struct Chr {
	int unicode_point;
	char const *name;
	char const *strokes;
};

extern Chr chars[];
