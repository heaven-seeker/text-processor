CC = gcc
CFLAGS = -o
SRC = main.c io.c utility.c text_processing.c
OBJ = $(SRC:.c=.o)
TARGET = text-processor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(TARGET) $(OBJ)

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
