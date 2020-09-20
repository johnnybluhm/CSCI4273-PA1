all: build_server create_server server_clean build_client create_client client_clean

build_server:
	@echo "Building server"

create_server:
	@gcc -Wall -Wextra -o ./server/server ./server/udp_server.c

server_clean:
	@echo "Server built"

build_client:
	@echo "Building client"

create_client:
	@gcc -Wall -Wextra -o ./client/client ./client/udp_client.c

client_clean:
	@echo "Client built"