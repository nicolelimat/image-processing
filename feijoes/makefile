CC=gcc
#CCFLAGS=-Wall
CCFLAGS=-g
LDFLAGS=-lm 
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=contafeijao

ifeq ($(OS), Windows_NT)
   LIB = ..\\utils\\imagelib.c
else
   LIB = ../utils/imagelib.c
endif


all: $(TARGET)

$(TARGET): $(OBJECTS);\
    $(CC) -g -o $@ $(LIB) $^ $(LDFLAGS) 

%.o: %.c %.h;\
    $(CC) $(CCFLAGS) -c $<

%.o: %.c;\
    $(CC) $(CCFLAGS) -c $<

clean || limpa:;\
    rm -f *.o *-result.p*m *.pbm $(TARGET);\
