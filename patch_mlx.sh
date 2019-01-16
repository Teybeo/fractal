#!/usr/bin/env bash

rm libmlx.a
cp /usr/local/lib/libmlx.a .

echo "Displaying 2 doubles (2 * 8 bytes) at offset 0x44d0"
xxd -s 0x44d0 -l 16 libmlx.a
printf "%032x" 0 | xxd -s 0x44d0 -l 16 -r -p - libmlx.a
echo "After patch: "
xxd -s 0x44d0 -l 16 libmlx.a
