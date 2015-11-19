CC = gcc

MOTHER_SOURCE = mother.c
TINA_SOURCE = tina.c
JUDY_SOURCE = judy.c

CLIENT_SOURCE = cookie_clnt.c cookie_xdr.c
SERVER_SOURCE = cookie_svc.c cookie_xdr.c

MOTHER = mother
JUDY = judy
TINA = tina

OUTPUT_FLAG = -o
RPCGEN_FLAG = -lnsl

all: $(MOTHER) $(TINA) $(JUDY)

mother: $(MOTHER_SOURCE)
	$(CC) $(OUTPUT_FLAG) $(MOTHER) $(SERVER_SOURCE) $(MOTHER_SOURCE) $(RPCGEN_FLAG)

judy: $(JUDY_SOURCE)
	$(CC) $(OUTPUT_FLAG) $(JUDY) $(CLIENT_SOURCE) $(JUDY_SOURCE) $(RPCGEN_FLAG)

tina: $(TINA_SOURCE)
	$(CC) $(OUTPUT_FLAG) $(TINA) $(CLIENT_SOURCE) $(TINA_SOURCE) $(RPCGEN_FLAG)

clean:
	rm -f $(CLIENT_SOURCE) $(SERVER_SOURCE) $(MOTHER) $(TINA) $(JUDY)