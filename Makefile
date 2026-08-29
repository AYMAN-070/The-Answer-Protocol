CC = cc
CFLAGS = -Wall -Wextra -Werror

SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): src/server.c
	$(CC) $(CFLAGS) src/server.c -o $(SERVER)

$(CLIENT): src/client.c
	$(CC) $(CFLAGS) src/client.c -o $(CLIENT)

run-server: $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	./$(CLIENT)

clean:
	rm -f $(SERVER) $(CLIENT)

lint:
	norminette src/

.PHONY: all clean run-server run-client lint