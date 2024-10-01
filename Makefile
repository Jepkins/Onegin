CC = g++
CFLAGS = -c -Wall -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual\
-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -Wsuggest-override\
-Wlong-long -Wopenmp -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
-Wlarger-than=8192 -Wstack-protector -fPIE -Werror=vla

BUILD_DIR = build/

ONEGIN_DIR = onegin/
NORM_DIR = normalizer/
COMMON_DIR = common/

EXECUTABLE_DIR = $(BUILD_DIR)executable/
LDFLAGS =
OBJECTS =

ONEGIN_EXEC = $(EXECUTABLE_DIR)onegin.exe
NORM_EXEC = $(EXECUTABLE_DIR)normalizer.exe
EXECUTABLES = $(ONEGIN_EXEC) $(NORM_EXEC)

-include $(ONEGIN_DIR)config.mk
-include $(NORM_DIR)config.mk
-include $(COMMON_DIR)config.mk

.PHONY: common
.PHONY: onegin
.PHONY: normalizer

all: $(EXECUTABLES)

onegin: $(ONEGIN_EXEC)

normalizer: $(NORM_EXEC)

common: $(COMMON_OBJECTS)

$(ONEGIN_EXEC): $(COMMON_OBJECTS) $(ONEGIN_OBJECTS)
	powershell if (! (Test-Path -Path $(@D))) {(md $(@D))}
	$(CC) $(LDFLAGS) $^ -o $@

$(NORM_EXEC): $(COMMON_OBJECTS) $(NORM_OBJECTS)
	powershell if (! (Test-Path -Path $(@D))) {(md $(@D))}
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)%.o: %.cpp
	powershell if (! (Test-Path -Path $(@D))) {md $(@D)}
	$(CC) $(CFLAGS) -MP -MMD $< -o $@


clean:
	del /f /s *.o *.exe *.d

echo:
	
