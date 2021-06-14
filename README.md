

Multicore testing
=========

Clone this repo and cd into it

```
mkdir build && cd build
cmake ..
make
```

To deploy this using SWD and monitor using minicom

Start following:
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg

Also start minicom to listen for output:
minicom -b 115200 -o -D /dev/serial0

Enter gdb and load file:
gdb-multiarch multicore.elf
target remote localhost:3333
load
monitor reset init
continue



