apps = demo

all: $(apps)

define GROUP
$(app): $(app)/.compiled.js $(app)/.compiled.wasm
endef

define BUILD
$(app)/.compiled.js $(app)/.compiled.wasm: $(app)/app.cpp js-library.cpp js-library.js
	em++ js-library.cpp $(app)/app.cpp -o $(app)/.compiled.js -s EXPORTED_FUNCTIONS="['cwrap']" --js-library js-library.js
endef

$(foreach app, $(apps), $(eval $(GROUP)))
$(foreach app, $(apps), $(eval $(BUILD)))
