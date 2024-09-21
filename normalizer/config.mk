NORM_SRC_DIR = $(NORM_DIR)source/
NORM_INC_DIR = $(NORM_DIR)includes/

NORM_SOURCES = $(wildcard $(addsuffix *.cpp, $(NORM_SRC_DIR)))
NORM_OBJECTS = $(addprefix $(BUILD_DIR), $(NORM_SOURCES:.cpp=.o))

CFLAGS += $(addprefix -I, $(NORM_INC_DIR))

