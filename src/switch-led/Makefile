# Usage
# $ make *.MOT
# 	Compile files
# $ make clean
# 	Remove all output files

# Append following files
# 	TARGET = *.MOT		# Mototola S-record format executable file
# 	SRC_C = *.c		# C source file
#	SRC_ASM = *.mar		# Asembly source file
#	SRC_LINK = *.sub	# Linker file

TARGET =
SRC_C =
SRC_ASM =
SRC_LINK =

LIST_ASM = $(SRC_ASM:.MAR=.LIS)
OBJ_ASM = $(SRC_ASM:.MAR=.OBJ)
OBJ_C = $(SRC_C:.c=.obj)
EXE_MAP = $(TARGET:.MOT=.MAP)
EXE_ABS = $(TARGET:.MOT=.ABS)

$(TARGET): $(SRC_C) $(SRC_ASM) $(SRC_LINK)
	a38h $(SRC_ASM)
	cl38h $(SRC_C)
	lk38h $(SRC_LINK)
	c38h $(EXE_ABS)

clean:
	rm -f $(LIST_ASM) $(OBJ_ASM) $(SRC_C) $(EXE_MAP) $(EXE_ABS) $(TARGET)
