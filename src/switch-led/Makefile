# Usage
#
# $ make *.MOT
# 	- Compile files
#
# $ make clean
# 	- Remove all output files

# Append following files
#
# 	TARGET = *.MOT		# Motorola S-record format executable file
# 	SRC_C = *.c		# C source file
#	SRC_ASM = *.mar		# Asembly source file
#	SRC_LINK = *.sub	# Linker file
#	LINK_MAP = *.MAP	# Linker Map file

TARGET = switch_led.MOT
SRC_C = main.c
SRC_ASM = startup.mar
SRC_LINK = link.sub
LINK_MAP = print.MAP

LIST_ASM = $(SRC_ASM:.mar=.LIS)
OBJ_ASM = $(SRC_ASM:.mar=.OBJ)
OBJ_C = $(SRC_C:.c=.obj)
EXE_ABS = $(TARGET:.MOT=.ABS)

$(TARGET): $(SRC_C) $(SRC_ASM) $(SRC_LINK)
	a38h $(SRC_ASM)
	cl38h $(SRC_C)
	lk38h $(SRC_LINK)
	c38h $(EXE_ABS)

clean:
	rm -f $(LIST_ASM) $(OBJ_ASM) $(OBJ_C) $(LINK_MAP) $(EXE_ABS) $(TARGET)
