#include "ps4.h"

#define STACK_RESOLVE(module, name) do { char n[] = #name; syscall(591, module, n, &name); } while(0)

void _start(void) {
	int libNet = 14;
	
	
	// Resolve functions
	int (*sceNetSocket)(const char *, int, int, int);
	int (*sceNetSocketClose)(int);
	int (*sceNetBind)(int, struct sockaddr_in *, int);
	int (*sceNetListen)(int, int);
	int (*sceNetAccept)(int, struct sockaddr_in *, int *);
	int (*sceNetRecv)(int, void *, size_t, int);
	STACK_RESOLVE(libNet, sceNetSocket);
	STACK_RESOLVE(libNet, sceNetSocketClose);
	STACK_RESOLVE(libNet, sceNetBind);
	STACK_RESOLVE(libNet, sceNetListen);
	STACK_RESOLVE(libNet, sceNetAccept);
	STACK_RESOLVE(libNet, sceNetRecv);
	
	
	// Fetch code
	char socketName[] = "loader";
	
	struct sockaddr_in serverAddress;
	
	serverAddress.sin_len = sizeof(serverAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = IN_ADDR_ANY;
	serverAddress.sin_port = htons(9023);
	serverAddress.sin_zero[0] = 0;
	serverAddress.sin_zero[1] = 0;
	serverAddress.sin_zero[2] = 0;
	serverAddress.sin_zero[3] = 0;
	serverAddress.sin_zero[4] = 0;
	serverAddress.sin_zero[5] = 0;
	
	int server = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetBind(server, &serverAddress, sizeof(serverAddress));
	sceNetListen(server, 10);
	int client = sceNetAccept(server, NULL, NULL);
	
	void *write = (void *)DATA_ADDRESS;
	int length;
	
	while((length = sceNetRecv(client, write, 4096, 0)) > 0) {
		write += length;
	}
	
	
	sceNetSocketClose(server);
	sceNetSocketClose(client);
}
