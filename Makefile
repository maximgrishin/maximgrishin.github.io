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

build/include/io.o: include/io.cpp
	mkdir -p build/include
	clang++ $(FLAGS) include/io.cpp -c -o build/include/io.o

build/site/demo/compiled.wasm: demo/app.cpp build/include/io.o include/memset.cpp
	mkdir -p build/site/demo
	clang++ $(FLAGS) -nostdlib -Iinclude -Wl,--no-entry -fprebuilt-module-path=build/pcm build/include/io.o include/memset.cpp demo/app.cpp -o build/site/demo/compiled.wasm

build/site/chin/compiled.wasm: chin/app.cpp chin/chr.c build/include/io.o include/memset.cpp
	mkdir -p build/site/chin
	clang++ $(FLAGS) -nostdlib -Iinclude -Wl,--no-entry -fprebuilt-module-path=build/pcm build/include/io.o include/memset.cpp chin/app.cpp -o build/site/chin/compiled.wasm
