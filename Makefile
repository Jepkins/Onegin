CC = g++
CFLAGS = -c -Wall -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual\
-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -Wsuggest-override\
-Wlong-long -fopenmp -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
-Wlarger-than=8192 -Wstack-protector -fPIE -Werror=vla -MP -MMD

BUILD_DIR = build/

ONEGIN_DIR = onegin/
NORM_DIR = normalizer/
COMMON_DIR = common/

COMMON_OBJECTS = $(shell $(MAKE) -s -C $(COMMON_DIR) build='$(BUILD_DIR)' echo_objects)
ONEGIN_OBJECTS = $(shell $(MAKE) -s -C $(ONEGIN_DIR) build='$(BUILD_DIR)' echo_objects)
NORM_OBJECTS   = $(shell $(MAKE) -s -C $(NORM_DIR)   build='$(BUILD_DIR)' echo_objects)
COMMON_INCLUDES = $(shell $(MAKE) -s -C $(COMMON_DIR) echo_includes)

EXECUTABLE_DIR = $(BUILD_DIR)executable/
LDFLAGS =

ONEGIN_EXEC = $(EXECUTABLE_DIR)onegin.exe
NORM_EXEC = $(EXECUTABLE_DIR)normalizer.exe

.PHONY: all common onegin normalizer

all: onegin normalizer

onegin: $(ONEGIN_EXEC)
	@echo Successfully remade $<

normalizer: $(NORM_EXEC)
	@echo Successfully remade $<

common: $(COMMON_OBJECTS)

$(ONEGIN_EXEC): $(COMMON_OBJECTS) $(ONEGIN_OBJECTS)
	@echo making $@
	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) $^ -o $@

$(NORM_EXEC): $(COMMON_OBJECTS) $(NORM_OBJECTS)
	@echo making $@
	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) $^ -o $@

$(COMMON_OBJECTS):
	@echo $(shell $(MAKE) -s -C $(COMMON_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)')

$(ONEGIN_OBJECTS):
	@echo $(shell $(MAKE) -s -C $(ONEGIN_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)' common_incs='$(COMMON_INCLUDES)')

$(NORM_OBJECTS):
	@echo $(shell $(MAKE) -s -C $(NORM_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)' common_incs='$(COMMON_INCLUDES)')

clean:
	@rm -rf -d $(BUILD_DIR)

echo:
	@echo $(COMMON_INCLUDES)
