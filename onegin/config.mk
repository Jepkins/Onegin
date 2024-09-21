ONEGIN_SRC_DIR = $(ONEGIN_DIR)source/
ONEGIN_INC_DIR = $(ONEGIN_DIR)includes/

ONEGIN_SOURCES = $(wildcard $(addsuffix *.cpp, $(ONEGIN_SRC_DIR)))
ONEGIN_OBJECTS = $(addprefix $(BUILD_DIR), $(ONEGIN_SOURCES:.cpp=.o))

CFLAGS += $(addprefix -I, $(ONEGIN_INC_DIR))

