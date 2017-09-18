make:
	gcc main_servidor.c -o server
	gcc main_cliente.c -o client
	gcc main_msg_sender.c -o scanner
	gcc main_msg_receiver.c -o presenter
	gnome-terminal -x sh -c "./scanner"
	gnome-terminal -x sh -c "./server; bash"
	gnome-terminal -x sh -c "./client; bash"
	gnome-terminal -x sh -c "./presenter; bash"
