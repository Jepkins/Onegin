CC = g++
CFLAGS = -c -Wall -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual\
-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wsuggest-override\
-Wlong-long -fopenmp -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
-Wlarger-than=8192 -fPIE -Werror=vla -MP -MMD
LDFLAGS =
BUILD_DIR = build

ONEGIN_DIR = onegin
NORM_DIR = normalizer
COMMON_DIR = common

ONEGIN_OBJECTS  = $(shell $(MAKE) -s -C $(ONEGIN_DIR) curr=$(ONEGIN_DIR) build='$(BUILD_DIR)' echo_objects)
NORM_OBJECTS    = $(shell $(MAKE) -s -C $(NORM_DIR)   curr=$(NORM_DIR)   build='$(BUILD_DIR)' echo_objects)
COMMON_OBJECTS  = $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) build='$(BUILD_DIR)' echo_objects)
COMMON_INCLUDES = $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) echo_includes)

LDFLAGS += $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) echo_libs)
EXECUTABLE_DIR = $(BUILD_DIR)/executable

ONEGIN_EXEC = $(EXECUTABLE_DIR)/onegin.exe
NORM_EXEC   = $(EXECUTABLE_DIR)/normalizer.exe

.PHONY: all onegin common run

all: onegin norm

onegin: $(ONEGIN_EXEC)
	@echo Successfully remade $<

norm: $(NORM_EXEC)
	@echo Successfully remade $<

common: $(COMMON_OBJECTS)

$(ONEGIN_EXEC): make_common make_onegin
	@echo --making $@--
	@mkdir -p $(@D)
	@sleep 0.5
	@$(CC) $(COMMON_OBJECTS) $(ONEGIN_OBJECTS) -o $@ $(LDFLAGS)

$(NORM_EXEC): make_common make_norm
	@echo --making $@--
	@mkdir -p $(@D)
	@sleep 0.5
	@$(CC) $(COMMON_OBJECTS) $(NORM_OBJECTS) -o $@ $(LDFLAGS)

make_common:
	@echo $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)')

make_onegin:
	@echo $(shell $(MAKE) -s -C $(ONEGIN_DIR) curr=$(ONEGIN_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)' common_incs='$(COMMON_INCLUDES)')

make_norm:
	@echo $(shell $(MAKE) -s -C $(NORM_DIR)   curr=$(NORM_DIR)   comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)' common_incs='$(COMMON_INCLUDES)')

run_norm:
	./$(NORM_EXEC) -i data/text_rus.txt -o data/text_rus_norm.txt

run_onegin:
	./$(ONEGIN_EXEC) -i data/text_rus_norm.txt -o data/output.txt -s beg_ascend -s end_ascend -s unsorted

clean:
	@rm -rf -d $(BUILD_DIR)
