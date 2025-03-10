CC = g++
GEM5_PATH = /home/weichu/my_gem5/gem5
TARGET_ISA = x86


CFLAGS = -Wall -g
CFLAGS += -I$(GEM5_PATH)/include

LDFLAGS += -L$(GEM5_PATH)/util/m5/build/$(TARGET_ISA)/out -lm5

TARGET = daxpy_O3

ASM_TARGET = daxpy_O3.s

SOURCES = daxpy.cpp

OBJECTS = $(SOURCES:.cpp=.o)


ASSEMBLY = $(SOURCES:.cpp=.s)


all: $(TARGET) $(ASM_TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(ASM_TARGET): $(ASSEMBLY)
	$(CC) $(ASSEMBLY) -o $(ASM_TARGET) $(LDFLAGS)

%.s: %.cpp
	$(CC) $(CFLAGS) -S -o $@ $<


%.o: %.cpp
	$(CC)  -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(ASM_TARGET) $(ASSEMBLY)

-include $(OBJECTS:.o=.d)

.PHONY: all clean