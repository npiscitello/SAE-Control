# Compiling
I'm using the Arduino IDE CLI on arch - the compile command looks like this:
`arduino --board arduino:avr:uno --port /path/to/device --verify /path/to/code.ino`.

* on my Arch installation, `/path/to/device` is `/dev/ttyACM0`. Make sure you have write permission
  (I used a udev rule, see my dotfiles repo)!
* Make sure to run `arduino --install-boards arduino:avr` first!
* On Arch, make sure your user is in the `uucp` and `lock` groups per
  [the wiki](https://wiki.archlinux.org/index.php/Arduino#Accessing_serial)
* to set the baud rate of the port, just run `stty -F /path/to/device <baud rate>`. I like 115200.

# Interface
The link between the front and rear modules is over a CAN bus, through 
[a SEEED studio breakout board](http://docs.longan-labs.cc/can_bus/). The rear essentially just
streams sensor data to the front, which stores it in the SD card and displays some data on a
dashboard.
* Communication is one way - the rear module only needs to send data to the front module.
* Due to the way the CAN boards work, we must send 8 bytes in every transmission. The byte order
  will be laid out as follows (byte 0 being the first sent, byte 7 being the last):
  * 0:0 - zero byte
  * 1:2 - Engine speed (RPM, 16 bit unsigned integer)
  * 3:3 - Wheel speed (MPH, 8 bit unsigned integer)
  * 4:4 - CVT temperature (deg F, 8 bit unsigned integer)
  * 5:5 - Oil temperature (deg F, 8 bit unsigned integer)
  * 6:7 - two zero bytes
  * Information in the code supercedes format specifications here!
  * The first zero byte is important, the last two are just padding
* There are currently only 2 modules, but in case there are more or more communication is required,
  addressing is as follows:
  * 0x00000000: front module
  * 0x00000001: rear module
