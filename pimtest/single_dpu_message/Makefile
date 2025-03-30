CC = g++
GEM5_PATH = /home/weichu/my_gem5/gem5
UPMEM_RUNTIME_PATH = $(GEM5_PATH)/pimtest/upmem-runtime
TARGET_ISA = x86


CFLAGS = -Wall -g
CFLAGS += -I$(GEM5_PATH)/include -I$(GEM5_PATH)/src -I$(UPMEM_RUNTIME_PATH)/src/

LDFLAGS += $(UPMEM_RUNTIME_PATH)/bin/libupmem_runtime.a

LDFLAGS += -L$(GEM5_PATH)/util/m5/build/$(TARGET_ISA)/out -lm5


TARGET = single_dpu_message.out

ASM_TARGET = single_dpu_message.s

SOURCES = single_dpu_message.cpp


all: $(TARGET) $(ASM_TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

$(ASM_TARGET): $(SOURCES)
	$(CC) $(SOURCES) -S -o $(ASM_TARGET) $(CFLAGS)


clean:
	rm -f $(OBJECTS) $(TARGET) $(ASM_TARGET)


.PHONY: all clean
