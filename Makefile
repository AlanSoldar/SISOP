CC=g++ -std=c++11 -c
CFLAGS=-I./libraries/ -pthread

BIN_FOLDER=./bin/
SRC_FOLDER=./src/

DBFLAGS=-ggdb3 -O0
RELEASEFLAGS=-O2


SERVER_SRC=$(SRC_FOLDER)Notification.cpp $(SRC_FOLDER)Client.cpp $(SRC_FOLDER)Database.cpp $(SRC_FOLDER)Packet.cpp $(SRC_FOLDER)Server.cpp $(SRC_FOLDER)Socket.cpp $(SRC_FOLDER)ServerApp.cpp

CLIENT_SRC=$(SRC_FOLDER)Notification.cpp $(SRC_FOLDER)Client.cpp $(SRC_FOLDER)Database.cpp $(SRC_FOLDER)Packet.cpp $(SRC_FOLDER)Server.cpp $(SRC_FOLDER)Socket.cpp $(SRC_FOLDER)ClientApp.cpp

SERVER_OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(SERVER_SRC:.cpp=.o)))
CLIENT_OBJ=$(addprefix $(BIN_FOLDER),$(notdir $(CLIENT_SRC:.cpp=.o)))

SERVER_EXE=./bin/server
CLIENT_EXE=./bin/client

server: $(SERVER_OBJ)
	g++ -pthread -o $(SERVER_EXE) $(SERVER_OBJ) -luuid

client: $(CLIENT_OBJ)
	g++ -pthread -o $(CLIENT_EXE)  $(CLIENT_OBJ) -luuid

./bin/%.o: ./src/%.cpp
	@mkdir -p $(BIN_FOLDER)
	$(CC) -o $@ $< $(CFLAGS) $(RELEASEFLAGS)

clean:
	rm -f $(BIN_FOLDER)*.o $(BIN_FOLDER)client $(BIN_FOLDER)server

serverrun:
	./bin/server

clientrun:
	./bin/client @gab localhost 4040