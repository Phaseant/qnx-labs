# Makefile for client-server QNX app using Neutrino IPC

# Компилятор
CC = gcc

# Цели
CLIENT_TARGET = client
SERVER_TARGET = server

# Исходные файлы
CLIENT_SRC = client.c
SERVER_SRC = server.c

# Сборка всех целей
all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CC) -o $(CLIENT_TARGET) $(CLIENT_SRC)

$(SERVER_TARGET): $(SERVER_SRC)
	$(CC) -o $(SERVER_TARGET) $(SERVER_SRC)

# Очистка
clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET)
