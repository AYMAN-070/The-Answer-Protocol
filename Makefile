CC = cc
CFLAGS = -Wall -Wextra -Werror

SERVER = server_app
CLIENT = client_app
GUI = gui_app

all: $(SERVER) $(CLIENT)

$(SERVER): server/src/server.c
	$(CC) $(CFLAGS) -I server/include -I common/include server/src/server.c -o $(SERVER)

$(CLIENT): cli/src/client.c
	$(CC) $(CFLAGS) -I cli/include -I common/include cli/src/client.c -o $(CLIENT)

$(GUI):
	@echo "GUI not done yet"

run-server: $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	./$(CLIENT)

run-client-gui: $(GUI)
	./$(GUI)

clean:
	rm -f $(SERVER) $(CLIENT) $(GUI)

lint:
	norminette

.PHONY: all clean run-server run-client run-client-gui lint