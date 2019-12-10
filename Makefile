CC = gcc
CCFLAGS = -W -Wall

BINARY = apt-uninstaller

all:
	$(CC) $(CCFLAGS) Main.c Parser.c -o $(BINARY)

debug: CCFLAGS += -DDEBUG
debug: all

clean:
	rm -f $(BINARY)
