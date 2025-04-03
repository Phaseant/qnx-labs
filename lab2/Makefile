# Makefile

# Имя выходного файла
TARGET = threads

# Компилятор
CC = gcc

# Исходный файл
SRC = threads.c

# Цель по умолчанию
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET)
