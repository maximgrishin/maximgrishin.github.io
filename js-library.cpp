#include <emscripten.h>

extern "C" void update();

extern "C" EMSCRIPTEN_KEEPALIVE void call_update() {
	update();
}
