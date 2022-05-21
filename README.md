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
sudo backlight <-l[evel] <level>> [-h[elp] -d[evices] -D[evice] <id>]
```

## Options
| Syntax             | Description                                        |
| ------------------ | -------------------------------------------------- |
| `-h[elp]`          | Display usage information                          |
| `-l[evel] <level>` | Set display brightness (between 1 and 100)         |
| `-d[evices]`       | Display list of available devices                  |
| `-D[evice] <id>`   | Set brightness of specific device from device list |

# Examples
## Set brightness of all displays to 100% (default):
```BASH
sudo backlight -l 100 # -l <%>
```

## Display all available devices:
```BASH
backlight -d
```
Example output:
```
$ backlight -d
0: intel_backlight
```