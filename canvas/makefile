CC=clang -Wall -std=gnu17
CFLAGS=-lX11 -lm

ifeq ($(build),release)
	CC+=-O3
else ifeq ($(build),debug)
	CC+=-Og -g
endif

all: basic random
basic: basic.c
random: random.c

.PHONY+=clean
clean:
	rm -rfv random basic

