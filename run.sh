#!/bin/bash

QEMU_DIR='/mnt/c/Program Files/qemu'
CUR_WIN_DIR='C:/Users/Ariel Ben Eliezer/Cyber/micros'
CUR_LINUX_DIR="$pwd"

IMAGE=micros.img

cd "$QEMU_DIR"
# ./qemu-system-x86_64.exe -drive format=raw,file="$CUR_WIN_DIR/$IMAGE",if=floppy -no-reboot \
#     -chardev stdio,id=char0,logfile="$CUR_WIN_DIR/serial.log",signal=off \
#     -serial chardev:char0
./qemu-system-x86_64.exe -s -S "$CUR_WIN_DIR/$IMAGE"
cd "$CUR_LINUX_DIR"
