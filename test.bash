#!/bin/bash

# *** Set path and files ***
THIS_DIR=$(dirname "$(realpath "$0")")
OBJ_FILE="$THIS_DIR/input.o"
ASM_FILE="$THIS_DIR/input.asm"
EXE_FILE="$THIS_DIR/input"
MSK_FILE="$THIS_DIR/input.msk"

# Navigate to the script's directory
cd "$THIS_DIR" || exit

# *** Compile ***
clear
if [ -x "$MSK_FILE" ]; then
    "$MSK_FILE"
    if [ $? -ne 0 ]; then
        echo "!!!Error executing $MSK_FILE"
        exit 1
    fi
else
    echo "$MSK_FILE not found or not executable"
    exit 1
fi

# *** Assemble and link ***
rm -f "$OBJ_FILE" "$EXE_FILE"

nasm -f elf32 "$ASM_FILE" -o "$OBJ_FILE"
if [ $? -ne 0 ]; then
    echo "!!!Assembly Error"
    exit 1
fi

ld -m elf_i386 "$OBJ_FILE" -o "$EXE_FILE"
if [ $? -ne 0 ]; then
    echo "!!!Link error"
    exit 1
fi

# *** Run the executable ***
rm -f "$OBJ_FILE"
echo "Executing..."
"$EXE_FILE"
