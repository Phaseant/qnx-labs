# Makefile

# Имя выходного файла
TARGET = semaphores

# Компилятор
CC = gcc

# Исходный файл
SRC = semaphores.c

# Цель по умолчанию
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET)
