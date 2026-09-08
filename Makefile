CC = cc
CFLAGS = -Wall -Wextra -Werror
OS = $(shell uname)

# Exécutables
SERVER = server_app
CLIENT = client_app
GUI_EXEC = client_gui

RAYLIB_VER = 6.0
INC_DIR = gui/include
LIB_DIR = gui/lib

GUI_CFLAGS = -Wall -Wextra -I$(INC_DIR)
GUI_LDFLAGS = -L$(LIB_DIR) -lraylib -lm -lpthread

# Détection de l'OS pour le linkage de Raylib
ifeq ($(OS), Darwin)
	GUI_LDFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
	GUI_LDFLAGS += -lGL -ldl -lrt -lX11
endif

GUI_SRC = gui/src/main.c gui/src/layout.c gui/src/room_view.c gui/src/map.c gui/src/graphics.c $(LIB_DIR)/cJSON.c
GUI_OBJ = $(GUI_SRC:.c=.o)

all: $(SERVER) $(CLIENT) $(GUI_EXEC)

$(SERVER):
	$(CC) $(CFLAGS) -I server/include -I common/include server/src/server.c server/src/server_utils.c server/src/server_cmd.c server/src/server_cmd_2.c server/src/server_world.c server/src/server_json.c server/src/server_cmd_combat.c server/src/server_cmd_talk.c -o $(SERVER)

$(CLIENT):
	$(CC) $(CFLAGS) -I cli/include -I common/include cli/src/client.c -o $(CLIENT)

$(GUI_EXEC): $(GUI_OBJ)
	$(CC) $(GUI_OBJ) -o $@ $(GUI_LDFLAGS)

gui/%.o: gui/%.c
	$(CC) $(GUI_CFLAGS) -c $< -o $@

install-deps:
	@if [ -f "$(LIB_DIR)/libraylib.a" ] && [ -f "$(LIB_DIR)/cJSON.c" ]; then \
		echo "Dependencies already installed. Use 'make clean-deps' to force reinstallation."; \
	else \
		mkdir -p $(INC_DIR) $(LIB_DIR); \
		echo "Downloading cJSON..."; \
		curl -sf -o $(INC_DIR)/cJSON.h https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.h || exit 1; \
		curl -sf -o $(LIB_DIR)/cJSON.c https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.c || exit 1; \
		echo "Downloading Raylib sources (Version $(RAYLIB_VER))..."; \
		curl -sfL "https://github.com/raysan5/raylib/archive/refs/tags/$(RAYLIB_VER).tar.gz" | tar xz -C $(LIB_DIR) || (echo "Failed to download or extract Raylib." && exit 1); \
		echo "Building Raylib locally..."; \
		make -C $(LIB_DIR)/raylib-$(RAYLIB_VER)/src PLATFORM=PLATFORM_DESKTOP > /dev/null 2>&1 || exit 1; \
		cp $(LIB_DIR)/raylib-$(RAYLIB_VER)/src/libraylib.a $(LIB_DIR)/ || exit 1; \
		cp $(LIB_DIR)/raylib-$(RAYLIB_VER)/src/raylib.h $(INC_DIR)/ || exit 1; \
		rm -rf $(LIB_DIR)/raylib-$(RAYLIB_VER); \
		echo "Dependencies installed."; \
	fi

run-server: $(SERVER)
	./$(SERVER)

run-client: $(CLIENT)
	./$(CLIENT)

run-client-gui: $(GUI_EXEC)
	./$(GUI_EXEC)

lint:
	norminette

clean:
	rm -f $(GUI_OBJ)

fclean: clean
	rm -f $(SERVER) $(CLIENT) $(GUI_EXEC)

re: fclean all

clean-deps: fclean
	rm -rf $(INC_DIR) $(LIB_DIR)

.PHONY: all clean fclean re install-deps run-server run-client run-client-gui clean-deps lint