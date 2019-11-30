TARGET = imia1.MOT
SOURCE_C = imia1.c
SOURCE_ASM = imia1_startup.mar
SOURCE_LNK = imia1.sub

$(TARGET): $(SOURCE_C) $(SOURCE_ASM) $(SOURCE_LNK)
	a38h $(SOURCE_ASM)
	cl38h $(SOURCE_C)
	lk38h $(SOURCE_LNK)
	c38h $(TARGET:.MOT=.ABS)

clean:
	rm -f $(SOURCE_ASM:.mar=.LIS) $(SOURCE_ASM:.mar=.OBJ) $(SOURCE_C:.c=.obj) $(TARGET:.MOT=.MAP) $(TARGET:.MOT=.ABS) $(TARGET)
