apps = demo

all: build/index.js $(apps)

build/index.js: index.js
	mkdir -p build
	cp index.js build

define GROUP
$(app): build/$(app)/compiled.wasm
endef

define BUILD
build/$(app)/compiled.wasm: $(app)/app.cpp ui.cpp index.html ui.hpp
	mkdir -p build/$(app)
	clang++ --target=wasm32 -nostdlib -Wl,--no-entry ui.cpp $(app)/app.cpp -o build/$(app)/compiled.wasm
	cp index.html build/$(app)/index.html
endef

$(foreach app, $(apps), $(eval $(GROUP)))
$(foreach app, $(apps), $(eval $(BUILD)))
