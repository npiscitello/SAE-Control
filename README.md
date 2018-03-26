# Compiling
I'm using the Arduino IDE CLI on arch - the compile command looks like this:
`arduino --board arduino:avr:uno --port /path/to/device --verify /path/to/code.ino`.

* on my Arch installation, `/path/to/device` is `/dev/ttyACM0`. Make sure you have write permission
  (I used a udev rule, see my dotfiles repo)!
* Make sure to run `arduino --install-boards arduino:avr` first!
* On Arch, make sure your user is in the `uucp` and `lock` groups per
  [the wiki](https://wiki.archlinux.org/index.php/Arduino#Accessing_serial)
* to set the baud rate of the port, just run `stty -F /path/to/device <baud rate>`. I like 115200.
