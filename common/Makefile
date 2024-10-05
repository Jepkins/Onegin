CC = $(comp)
COMMON_DIR = common/
CFLAGS = $(flags)
BUILD_DIR = $(build)$(COMMON_DIR)

SUBDIRS = $(wildcard */)

SOURCES = $(wildcard $(addsuffix *.cpp, $(SUBDIRS)))
OBJECTS = $(addprefix $(BUILD_DIR), $(SOURCES:.cpp=.o))
DEPS = $(addsuffix .d, $(basename $(OBJECTS)))

LOCAL_INCLUDES = $(addprefix -I, $(SUBDIRS))
GLOBAL_INCLUDES = $(addprefix $(COMMON_DIR), $(SUBDIRS))
CFLAGS += $(LOCAL_INCLUDES)

all: $(addprefix ../, $(OBJECTS))

-include $(DEPS)

../$(BUILD_DIR)%.o: %.cpp
	@echo making $(@:../%=%)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $< -o $@

echo_objects:
	@echo $(OBJECTS)

echo_includes:
	@echo $(GLOBAL_INCLUDES)

echo:
	@echo $(BUILD_DIR)
	@echo $(OBJECTS)
	@echo $(SOURCES)
