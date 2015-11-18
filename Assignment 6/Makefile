CC = gcc
CSOURCE_1 = judy.c
CSOURCE_2 = tina.c
SSOURCE = mother.c
RPCSRC = cookie.x
RPCGEN_CALL = rpcgen
RPCGEN_CLIENT = cookie_clnt.c cookie_xdr.c
RPCGEN_SERVER = cookie_svc.c cookie_xdr.c
RPCGEN_HEADER = cookie.h
RPCGEN_FLAG = -lnsl
CLIENT_1 = judy
CLIEN_2 = tina
SERVER = server

all: clean $(CLIENT_1) $(CLIENT_2) $(SERVER)

$(RPCGEN): $(RPCSRC)
    rpcgen $(RPCSRC)

$(CLIENT_1): $(CSOURCE_1) $(RPCGEN)
	$(CC) -o $(CLIENT_1) $(CSOURCE_1) $(RPCGEN_CLIENT) $(RPCGEN_FLAG) 

$(CLIENT_2): $(CSOURCE_2) $(RPCGEN)
	$(CC) -o $(CLIENT_2) $(CSOURCE_2) $(RPCGEN_CLIENT) $(RPCGEN_FLAG) 

$(SERVER): $(SSOURCE) $(RPCGEN)
	$(CC) -o $(SERVER) $(SSOURCE) $(RPCGEN_SERVER) $(RPCGEN_FLAG) 

# 'clean' rule for remove non-source files
# To use, call 'make clean'
# Note: you don't have to call this in between every
#       compilation, it's only in case you need to
#       clean out your directory for some reason.
clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	rm -f $(CLIENT_1) $(CLIENT_2) $(SERVER) $(RPCGEN_CLIENT) $(RPCGEN_SERVER) $(RPCGEN_HEADER)