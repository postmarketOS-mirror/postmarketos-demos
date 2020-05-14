postmarketos-demos: main.c
	$(CC) `pkg-config --cflags gtk+-3.0` -o postmarketos-demos \
		main.c `pkg-config --libs gtk+-wayland-3.0 gtk+-3.0` -Werror \
		-Wno-error=deprecated-declarations
