CC := gcc

PROGRAM := main

SOURCE_DIR := ./src
OUTPUT_DIR := ./build

SOURCE_FILES = $(SOURCE_DIR)/main.c
SOURCE_FILES += $(SOURCE_DIR)/volterno_http.c
SOURCE_FILES += $(SOURCE_DIR)/timerino/timerino.c

CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -I$(SOURCE_DIR)

#LDFLAGS += -lm

OBJS = $(SOURCE_FILES:%.c=%.o)
OBJS_OUTPUT = $(OBJS:$(SOURCE_DIR)/%=$(OUTPUT_DIR)/%)

all: $(PROGRAM)

debug: CFLAGS += -g3
debug: clean all

$(PROGRAM): $(OBJS_OUTPUT) makefile
	$(CC) $(OBJS_OUTPUT) -o $(PROGRAM) $(LDFLAGS)

$(OUTPUT_DIR)/%.o : $(SOURCE_DIR)/%.c | $(OUTPUT_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(PROGRAM) $(OUTPUT_DIR)

.PHONY: all clean debug
