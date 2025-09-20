CC ?= gcc
CFLAGS += -Wall -Wextra -std=c99 -Ihl
LDFLAGS +=

MKDIR = mkdir -p
RM = rm -rf

TARGETS = hldetector hlgenerator

.PHONY: all clean directories

all: directories $(TARGETS)

directories:
	@$(MKDIR) build/obj

hldetector: build/obj/hldetector.o build/obj/common.o build/obj/hlparser.o build/obj/strlcat.o
	$(CC) $(CFLAGS) -o hldetector build/obj/hldetector.o build/obj/common.o build/obj/hlparser.o build/obj/strlcat.o $(LDFLAGS)

hlgenerator: build/obj/hlgenerator.o build/obj/common.o build/obj/hlparser.o build/obj/strlcat.o
	$(CC) $(CFLAGS) -o hlgenerator build/obj/hlgenerator.o build/obj/common.o build/obj/hlparser.o build/obj/strlcat.o $(LDFLAGS)

# Explicit rules for each object file
build/obj/common.o: hl/common.c
	$(CC) $(CFLAGS) -c hl/common.c -o build/obj/common.o

build/obj/hlparser.o: hl/hlparser.c
	$(CC) $(CFLAGS) -c hl/hlparser.c -o build/obj/hlparser.o

build/obj/strlcat.o: hl/strlcat.c
	$(CC) $(CFLAGS) -c hl/strlcat.c -o build/obj/strlcat.o

build/obj/hldetector.o: hl/hldetector.c
	$(CC) $(CFLAGS) -c hl/hldetector.c -o build/obj/hldetector.o

build/obj/hlgenerator.o: hl/hlgenerator.c
	$(CC) $(CFLAGS) -c hl/hlgenerator.c -o build/obj/hlgenerator.o

clean:
	$(RM) build/obj $(TARGETS)