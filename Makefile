build:
	g++ bl.cpp -o backlight
install:
	cp backlight /bin/backlight
	chmod +x /bin/backlight
uninstall:
	rm -rf /bin/backlight