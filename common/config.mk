COMMON_SUBDIRS = $(wildcard $(COMMON_DIR)*/)

COMMON_SOURCES = $(wildcard $(addsuffix *.cpp, $(COMMON_SUBDIRS)))
COMMON_OBJECTS = $(addprefix $(BUILD_DIR), $(COMMON_SOURCES:.cpp=.o))

CFLAGS += $(addprefix -I, $(COMMON_SUBDIRS))

