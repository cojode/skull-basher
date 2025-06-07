CC ?= gcc
CFLAGS += -Wall -Wextra -std=c11 -Ihl
LDFLAGS +=

SRC_DIR = hl
OBJ_DIR = build/obj
MKDIR=mkdir -p
RM=rm -rf

TARGETS = hldetector hlgenerator

COMMON_SRCS = $(SRC_DIR)/common.c $(SRC_DIR)/hlparser.c $(SRC_DIR)/strlcat.c
DETECTOR_SRCS = $(SRC_DIR)/hldetector.c
GENERATOR_SRCS = $(SRC_DIR)/hlgenerator.c

COMMON_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(COMMON_SRCS))
DETECTOR_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(DETECTOR_SRCS))
GENERATOR_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(GENERATOR_SRCS))

.PHONY: all clean directories

all: directories $(TARGETS)

directories:
	@$(MKDIR) $(OBJ_DIR)

hldetector: $(DETECTOR_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

hlgenerator: $(GENERATOR_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	$(RM) $(OBJ_DIR) $(TARGETS)

-include $(COMMON_OBJS:.o=.d) $(DETECTOR_OBJS:.o=.d) $(GENERATOR_OBJS:.o=.d)