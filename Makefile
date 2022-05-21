build:
	g++ backlight.cpp -o backlight -Wall -Wextra -Wpedantic
install:
	cp backlight /bin/backlight
	chmod +x /bin/backlight
uninstall:
	rm -rf /bin/backlight