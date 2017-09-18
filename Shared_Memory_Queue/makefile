make:
	gcc main_A_0.c -o server
	gcc main_B_0.c -o client
	gcc main_host_A_1.c -o scanner
	gcc main_host_B_1.c -o presenter
	gnome-terminal -x sh -c "./scanner"
	gnome-terminal -x sh -c "./server; bash"
	gnome-terminal -x sh -c "./client; bash"
	gnome-terminal -x sh -c "./presenter; bash"
