CC = g++
GEM5_PATH = /home/weichu/my_gem5/gem5
TARGET_ISA = x86


CFLAGS = -Wall -g -fPIC
CFLAGS += -I$(GEM5_PATH)/include -I$(GEM5_PATH)/src

LDFLAGS += -L$(GEM5_PATH)/util/m5/build/$(TARGET_ISA)/out -lm5
LDFLAGS += -shared

SOURCES = basic.cc byte_stream.cc

LIB_NAME=libupmem_runtime
LIB_PATH=../bin

SHARED_LIB = $(LIB_PATH)/$(LIB_NAME).so

STATIC_LIB = $(LIB_PATH)/$(LIB_NAME).a


all: $(SHARED_LIB) $(STATIC_LIB)

$(SHARED_LIB): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(STATIC_LIB): $(SOURCES)
	$(CC) $(CFLAGS) -c $^
	ar rcs $@ $(SOURCES:.cc=.o)


clean:
	rm -f $(OBJECTS) $(SHARED_LIB)


.PHONY: all clean
