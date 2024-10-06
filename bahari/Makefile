cc = gcc
CFLAGS = -I./include -Wall -Wextra -pthread
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/chat_utils.c
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/chat_utils.c

SERVER_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SERVER_SRC))
CLIENT_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CLIENT_SRC))

SERVER_TARGET = $(BIN_DIR)/server
CLIENT_TARGERT = $(BIN_DIR)/client

all: $(SERVER_TARGET) $(CLIENT_TARGERT)

$(SERVER_TARGET): $(SERVER_OBJ) | $(BIN_DIR)/server
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_TARGERT): $(CLIENT_OBJ) | $(BIN_DIR)/client
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY:
	all clean
