# Makefile

# Имя выходного файла
TARGET = timer

# Компилятор
CC = gcc

# Исходный файл
SRC = timer.c
# Цель по умолчанию
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET)
