CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
DEPS = my_list.h
OBJFILES = build_file_list.c main.c parse_args.c \
						print.c sort.c destroy.c options.c helpers.c
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJFILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

.PHONY: clean
clean:
	rm -f *.o $(TARGET)