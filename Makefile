CC=gcc
CFLAGS=-O2 -Wall 

SYMBOLS= -DBOARD_M37SIGMA -D__TMPM_373__ -DTERMIOS_CONFLICT -DUSE_GTK

MOTORMIND_FW_DIR = /home/marcelw/Projects/Fanuc/src/arm/MotorControlFW
INCL_DIRS  = -I $(MOTORMIND_FW_DIR)/Projects/MotorControlM37x
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Projects/MotorControlM37x/GNU
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Source/M37xSWModules/include
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Source/M37xHWGeneric/include
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Source/Board/CPU_Board/M37Sigma/include/
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Source/CMSIS/TX03_CMSIS
INCL_DIRS += -I $(MOTORMIND_FW_DIR)/Source/CMSIS/TX03_Periph_Driver/inc
INCL_DIRS += `xml2-config --cflags` 
LD_FLAGS = `xml2-config --libs` 



TARGET=motormind MotorMind_gtk
OBJS= main.o commands.o crc8.o xml_parse.o
TMPM373= ../../arm/MotorControlFW/Source/CMSIS/TX03_CMSIS/TMPM373

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(SYMBOLS) $(CFLAGS) $(LD_FLAGS) $(OBJS) -o $@

.c.o:
	cp $(TMPM373).h $(TMPM373).h.orig 
	sed -e '/__IO uint32_t CR[12];/i \#if 0' -e '/__IO uint32_t CR[12];/a \#endif' $(TMPM373).h.orig > $(TMPM373).h 
	$(CC) $(SYMBOLS) $(INCL_DIRS) -c $(CFLAGS) $<
	mv $(TMPM373).h.orig $(TMPM373).h

MotorMind_gtk:
	$(CC) -Wno-attributes -Wno-unused-variable $(SYMBOLS) $(INCL_DIRS) $(CFLAGS) $(LD_FLAGS) `pkg-config --cflags gtk+-3.0` -o MotorMind_gtk MotorMind_gtk.c commands.o xml_parse.o crc8.o  `pkg-config --libs gtk+-3.0`

clean:
	-@rm $(OBJS) $(TARGET)
