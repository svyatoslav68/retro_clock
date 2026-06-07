# Имя проекта
TARGET = retro_clock
DEST_DIR = Debug

# Компилятор 
CC = avr-gcc
OBJCOPY = avr-objcopy

# Программатор
SOFTPROG	  = avrdude
#HARDPROG      = arduino-ft232r
HARDPROG      = avrftdi
DEVICE        = m16
PROGFLAGS     = #-F 

# Команда для удаления файлов
DEL_FILE      = rm -f

# Исходные файлы
SOURCES=main.c \
		data_to_display.c\
		encoder.c\
		clock.c\
		beeper.c\
		button.c\
		ctrl_timer.c\
		eeprom_values.c\
		RTOS.c\
		timer_queue.c

# Объектные файлы
#OBJECTS=$(DEST_DIR)/main.o $(DEST_DIR)/display_7seg.o #$(SOURCES:.c=.o)
OBJECTS=$(addprefix $(DEST_DIR)/,$(SOURCES:.c=.o))

# Название микроконтроллера, для которого происходит компиляция и сборка
MCU=atmega16

# Флаги компилятора
CFLAGS = -mmcu=$(MCU) -Wall -ggdb -std=c99 -Os #-Werror
LDFLAGS = -mmcu=$(MCU) -ggdb -Os -Wall #-Werror

all:$(TARGET)
	@echo $(OBJECTS)

$(TARGET): $(OBJECTS)
	echo "$(OBJECTS)"
	$(CC) $(LDFLAGS) -o $(DEST_DIR)/$@.elf $(OBJECTS)
	$(OBJCOPY) -O binary $(DEST_DIR)/$@.elf $(DEST_DIR)/$@.bin
	$(OBJCOPY) -j .text -j .data -O ihex $(DEST_DIR)/$@.elf $(DEST_DIR)/$@.hex
	$(OBJCOPY) -j .eeprom -O ihex $(DEST_DIR)/$@.elf $(DEST_DIR)/$@.eep

$(DEST_DIR)/%.o: %.c
	@echo "input files: $<"
	@echo "output files: $@"
	$(CC) $(CFLAGS) -c -o $@ $^

##### Правила прошивки контроллера
program:all
	$(SOFTPROG) $(PROGFLAGS) -c $(HARDPROG) -p $(DEVICE) -U flash:w:$(DEST_DIR)/$(TARGET).hex:i -U eeprom:w:$(DEST_DIR)/$(TARGET).eep:i

# Стирание временных файлов
clean:
	$(DEL_FILE) $(DEST_DIR)/*.elf $(DEST_DIR)/*.bin $(DEST_DIR)/*.hex $(OBJECTS)

