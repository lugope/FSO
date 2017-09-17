make:
	gcc main_servidor.c -o servidor
	gcc main_cliente.c -o cliente
	./servidor& 
	gnome-terminal -x sh -c "./cliente; bash"
