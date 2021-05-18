#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include <STDIO.H>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define closesocket close
#endif
int main(int argc, char* argv[])
{
	#ifdef WIN32
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	#endif
	u_int sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == (u_int)~0)
	{
		printf("invalid socket !");
		return 0;
	}
 
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(28345);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	printf("connected\n");
	char *datas=calloc(255,1);
	printf("send:");
	while(scanf("%s",datas)!=EOF)
	{
		// printf("send %s\n",datas);
		send(sclient, datas, strlen(datas), 0);
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		printf("receive size:%d\n",ret);
		if (ret > 0)
		{
			recData[ret] = 0x00;
			//printf(recData);
			printf("receive:%s\n",recData);
		}
		printf("send:");
	}
	closesocket(sclient);
	#ifdef WIN32
	WSACleanup();
	#endif
	return 0;
}