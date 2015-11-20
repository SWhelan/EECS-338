Sarah Whelan
slw96
Assignment 6
RPC - Remote Procedure Calls

The Finite-Source Cookie Jar Problem

My test config but any comp could run either of the clients or server:
eecslinab3 - server/mother
eecslinab2 - client/Tina
eecslinab7 - client/Judy

Suggested run command: 
make clean && rpcgen cookie.x && make

then on their respective computers: 
./mother
./judy eecslinab3.EECS.CWRU.edu
./tina eecslinab3.EECS.CWRU.edu

Info:    
This runs on linux/ubuntu.
Uses the gcc compiler
No environment settings
No libraries possibly rpcgen counts but that is already on the machines

File breakdown:
cookie.x - the rpc specification used with rpcgen to create cookie_clnt.c, cookie_svc.c, cookie_xdr.c and cookie.h
mother.c - the server/implementation of the rpc methods
client.h - shared file for both tia and judy that is the actual client code
tina.c - the favorite child just calls run_client in client.h
judy.c - the other child just calls run_client in client.h
common.h - shared by all a get time method and imports of standard stuff and cookie.h