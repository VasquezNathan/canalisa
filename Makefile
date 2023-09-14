CPU=atmega328p
CPU_HZ=16000000

BUILD_DIR=./build
GCC_FLAGS= -mmcu=${CPU} -DF_CPU=${CPU_HZ}UL

default:
	avr-gcc ${GCC_FLAGS} -c -o ${BUILD_DIR}/main.o main.cc

	avr-gcc ${GCC_FLAGS} -o ${BUILD_DIR}/main.elf ${BUILD_DIR}/main.o
	
burn:
	sudo avrdude \
		-p ${CPU} \
		-c arduino \
		-P /dev/ttyUSB0 \
		-U flash:w:${BUILD_DIR}/main.elf

clean:
	rm  ${BUILD_DIR}/*
	rm elf.text

text:
	avr-objdump -D ${BUILD_DIR}/main.elf > elf.txt

hex:
	avr-objcopy -O ihex -R .eeprom ${BUILD_DIR}/main.elf ${BUILD_DIR}/main.hex
