CC = gcc
CCFLAGS = -W -Wall

BINARY = apt-uninstaller

all:
	$(CC) $(CCFLAGS) Main.c Parser.c -o $(BINARY)

debug: CCFLAGS += -DDEBUG
debug: all

clean:
	rm -f $(BINARY)

install:
	@if [ $(shell id -u) -ne 0 ]; then printf "\033[31mThis rule must be started as root.\033[0m\n"; false; fi
	cp $(BINARY) /usr/bin

uninstall:
	@if [ $(shell id -u) -ne 0 ]; then printf "\033[31mThis rule must be started as root.\033[0m\n"; false; fi
	rm -f /usr/bin/$(BINARY)
