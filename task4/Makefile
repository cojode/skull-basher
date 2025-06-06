CC ?= gcc
CFLAGS += -Wall -Wextra -std=c11 -Ihl
LDFLAGS +=

SRC_DIR = hl
OBJ_DIR = build/obj

TARGETS = hldetector hlgenerator

COMMON_SRCS = $(SRC_DIR)/common.c $(SRC_DIR)/hlparser.c $(SRC_DIR)/strlcat.c
DETECTOR_SRCS = $(SRC_DIR)/hldetector.c
GENERATOR_SRCS = $(SRC_DIR)/hlgenerator.c

COMMON_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(COMMON_SRCS))
DETECTOR_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(DETECTOR_SRCS))
GENERATOR_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(GENERATOR_SRCS))

ifeq ($(OS),Windows_NT)
    RM = del /Q /S
    MKDIR = mkdir
    BIN_EXT = .exe
    PATHSEP = \\
else
    RM = rm -rf
    MKDIR = mkdir -p
    BIN_EXT =
    PATHSEP = /
endif

.PHONY: all clean directories

all: directories $(TARGETS)

directories:
	@$(MKDIR) $(subst /,$(PATHSEP),$(OBJ_DIR))

hldetector$(BIN_EXT): $(DETECTOR_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

hlgenerator$(BIN_EXT): $(GENERATOR_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(subst /,$(PATHSEP),$(dir $@))
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	$(RM) $(subst /,$(PATHSEP),$(OBJ_DIR)) $(addsuffix $(BIN_EXT),$(TARGETS))

-include $(COMMON_OBJS:.o=.d) $(DETECTOR_OBJS:.o=.d) $(GENERATOR_OBJS:.o=.d)