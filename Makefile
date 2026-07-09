CXX := clang++
CXXFLAGS += -std=c++26 --target=wasm32 -Os -Wall -Wextra -Werror -Iinclude
LINKFLAGS += -nostdlib -Wl,--no-entry

BUILD_DIR := build
SITE_DIR := build/site
COMPILED_NAME := compiled.wasm
IO_JS := io.js
IO_HTML := io.html

APP_DIRS := $(shell find -type f -wholename "./*/app.cpp")
APPS := $(patsubst ./%/app.cpp, %, $(APP_DIRS))

COMMON_DIR := include
COMMON_CPP := $(shell find -type f -wholename "./$(COMMON_DIR)/*.cpp")
COMMON_OBJ := $(patsubst ./%.cpp, $(BUILD_DIR)/%.o, $(COMMON_CPP))

.PHONY: all
all: $(APPS) $(SITE_DIR)/$(IO_JS)

$(SITE_DIR)/$(IO_JS): $(IO_JS)
	mkdir -p $(@D)
	cp $(IO_JS) $(SITE_DIR)/$(IO_JS)

define GROUP
$(1)_CPP := $$(shell find -type f -wholename "./$(1)/*.cpp")
$(1)_OBJ := $$(patsubst ./%.cpp, $(BUILD_DIR)/%.o, $$($(1)_CPP))
$(1): $(SITE_DIR)/$(1)/$(COMPILED_NAME)
endef

define BUILD
$(SITE_DIR)/$(1)/$(COMPILED_NAME): $($(1)_OBJ) $(COMMON_OBJ) $(IO_HTML)
	mkdir -p $(SITE_DIR)/$(1)
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) $($(1)_OBJ) $(COMMON_OBJ) -o $(SITE_DIR)/$(1)/$(COMPILED_NAME)
	cp $(IO_HTML) $(SITE_DIR)/$(1)/index.html
endef

$(foreach 1, $(APPS), $(eval $(call GROUP,$(1))))
$(foreach 1, $(APPS), $(eval $(call BUILD,$(1))))

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -c -o $@
