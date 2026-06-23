all: demo chin

FLAGS := -std=c++26 --target=wasm32 -Os -Wall -Werror

demo: build/site/demo/compiled.wasm build/site/demo/index.html build/site/index.js

chin: build/site/chin/compiled.wasm build/site/chin/index.html build/site/index.js

build/site/index.js: index.js
	mkdir -p build/site
	cp index.js build/site

build/site/demo/index.html: index.html
	mkdir -p build/site/demo
	cp index.html build/site/demo/index.html

build/site/chin/index.html: index.html
	mkdir -p build/site/chin
	cp index.html build/site/chin/index.html

build/pcm/io.pcm: include/io.cppm
	mkdir -p build/pcm
	clang++ $(FLAGS) include/io.cppm --precompile -o build/pcm/io.pcm

build/pcm: build/pcm/io.pcm

build/site/demo/compiled.wasm: demo/app.cpp build/pcm
	mkdir -p build/site/demo
	clang++ $(FLAGS) -nostdlib -Wl,--no-entry -fprebuilt-module-path=build/pcm build/pcm/io.pcm demo/app.cpp -o build/site/demo/compiled.wasm

build/site/chin/compiled.wasm: chin/app.cpp chin/chr.c build/pcm
	mkdir -p build/site/chin
	clang++ $(FLAGS) -nostdlib -Wl,--no-entry -fprebuilt-module-path=build/pcm build/pcm/io.pcm chin/app.cpp -o build/site/chin/compiled.wasm
