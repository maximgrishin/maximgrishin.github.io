apps = demo

all: build/index.js $(apps)

build/index.js:
	mkdir -p build
	cp index.js build

define GROUP
$(app): build/$(app)/compiled.js build/$(app)/compiled.wasm
endef

define BUILD
build/$(app)/compiled.js build/$(app)/compiled.wasm: $(app)/app.cpp js-library.cpp js-library.js index.html
	mkdir -p build/$(app)
	em++ js-library.cpp $(app)/app.cpp -o build/$(app)/compiled.js -s EXPORTED_FUNCTIONS="['cwrap']" --js-library js-library.js
	cp index.html build/$(app)/index.html
endef

$(foreach app, $(apps), $(eval $(GROUP)))
$(foreach app, $(apps), $(eval $(BUILD)))
