# backlight
The purpose of this program is to provide a simple and Xorg independent way of setting the backlight brightness via the [sysfs backlight interface](https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-class-backlight) - this way the backlight can be set even in a virtual console (tty).

Currently only the `intel_backlight` and `acpi_video0` interfaces are supported.

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
sudo backlight <level> # level - integer between 1 and 100 (% of max brightness)
```