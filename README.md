# backlight
The purpose of this program is to provide a simple and Xorg independent way of setting the backlight brightness via the [sysfs backlight interface](https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-class-backlight) - this way the backlight can be set even in a virtual console (tty).

# Prerequisites
- `g++` >=10.2.1

# Installation
```BASH
# build
make build

# install
sudo make install
```
# Usage
```BASH
# must be run as root
# use -h for more options
sudo backlight <-l[evel]> <level> [-h[elp]]
```