# Makefile

# Имя выходного файла
TARGET = helloworld

# Компилятор
CC = gcc

# Исходный файл
SRC = helloworld.c

# Цель по умолчанию
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET)
