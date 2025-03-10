CC = g++
GEM5_PATH = /home/weichu/my_gem5/gem5
TARGET_ISA = x86


CFLAGS = -Wall -g
CFLAGS += -I$(GEM5_PATH)/include

LDFLAGS += -L$(GEM5_PATH)/util/m5/build/$(TARGET_ISA)/out -lm5

TARGET = simple_m5op.out

ASM_TARGET = simple_m5op.s

SOURCES = simple_m5op.cpp


all: $(TARGET) $(ASM_TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

$(ASM_TARGET): $(SOURCES)
	$(CC) $(SOURCES) -S -o $(ASM_TARGET) $(CFLAGS)


clean:
	rm -f $(OBJECTS) $(TARGET) $(ASM_TARGET)


.PHONY: all clean
