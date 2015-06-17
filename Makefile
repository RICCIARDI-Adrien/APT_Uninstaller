all:
	gcc -W -Wall Main.c Parser.c -o APT_Uninstaller

clean:
	rm -f APT_Uninstaller

