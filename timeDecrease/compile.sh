avr-gcc -g -Os -mmcu=atmega32 -c timeDecreasing.c
avr-gcc -g -mmcu=atmega32 -o timeDecreasing.elf timeDecreasing.o
avr-objcopy -j .text -j .data -O ihex timeDecreasing.elf timeDecreasing.hex
avr-size --format=avr --mcu=atmega32 timeDecreasing.elf
