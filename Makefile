clientservermake: client.c server.c
	gcc -o client client.c 
	gcc -o server server.c
runS: server
	./server
runC: client
	./client

