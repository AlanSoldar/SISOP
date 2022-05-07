CC=g++ -std=c++11 -c
CFLAGS=-I./libraries/ -pthread

BIN_FOLDER=./bin/
SRC_FOLDER=./src/

DBFLAGS=-ggdb3 -O0
RELEASEFLAGS=-O2


SERVER_SRC=$(SRC_FOLDER)Notification.cpp $(SRC_FOLDER)Client.cpp $(SRC_FOLDER)Router.cpp $(SRC_FOLDER)Database.cpp $(SRC_FOLDER)Packet.cpp $(SRC_FOLDER)Server.cpp $(SRC_FOLDER)Socket.cpp $(SRC_FOLDER)ServerApp.cpp
CLIENT_SRC=$(SRC_FOLDER)Notification.cpp $(SRC_FOLDER)Client.cpp $(SRC_FOLDER)Router.cpp $(SRC_FOLDER)Database.cpp $(SRC_FOLDER)Packet.cpp $(SRC_FOLDER)Server.cpp $(SRC_FOLDER)Socket.cpp $(SRC_FOLDER)ClientApp.cpp
ROUTER_SRC=$(SRC_FOLDER)Notification.cpp $(SRC_FOLDER)Client.cpp $(SRC_FOLDER)Router.cpp $(SRC_FOLDER)Database.cpp $(SRC_FOLDER)Packet.cpp $(SRC_FOLDER)Server.cpp $(SRC_FOLDER)Socket.cpp $(SRC_FOLDER)RouterApp.cpp

SERVER_OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(SERVER_SRC:.cpp=.o)))
CLIENT_OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(CLIENT_SRC:.cpp=.o)))
ROUTER_OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(ROUTER_SRC:.cpp=.o)))

SERVER_EXE=./bin/server
CLIENT_EXE=./bin/client
ROUTER_EXE=./bin/router

build:
	make server
	make client
	make router

server: $(SERVER_OBJ)
	g++ -pthread -g -std=c++11 $(SERVER_SRC) -o $(SERVER_EXE) -luuid

client: $(CLIENT_OBJ)
	g++ -pthread -g -std=c++11 $(CLIENT_SRC) -o $(CLIENT_EXE) -luuid

router: $(ROUTER_OBJ)
	g++ -pthread -g -std=c++11 $(ROUTER_SRC) -o $(ROUTER_EXE) -luuid

./bin/%.o: ./src/%.cpp
	@mkdir -p $(BIN_FOLDER)
	$(CC) -o $@ $< $(CFLAGS) $(RELEASEFLAGS)

clean:
	rm -f $(BIN_FOLDER)*.o $(BIN_FOLDER)client $(BIN_FOLDER)server $(BIN_FOLDER)router

serverrun:
	./bin/server

routerrun:
	./bin/router

clientrun:
	./bin/client @gab localhost 4040

clientrungab:
	./bin/client @gab localhost 4040

clientrunalan:
	./bin/client @alan localhost 4040

clientruncarlos:
	./bin/client @carlos localhost 4040

clientrunleo:
	./bin/client @leo localhost 4040