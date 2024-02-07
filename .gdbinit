file out/gba-dev.elf
target extended-remote localhost:2345
load
b main
layout split
