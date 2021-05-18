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
int main(int argc, char *argv[])
{
	#ifdef WIN32
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	#endif
    u_int fd, new_fd, struct_len, numbytes,i;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buff[BUFSIZ];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(28345);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);
    struct_len = sizeof(struct sockaddr_in);

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    while(bind(fd, (struct sockaddr *)&server_addr, struct_len) == -1);
    printf("Bind Success!\n");
    while(listen(fd, 10) == -1);
    printf("Ready for Accept,Waitting...\n");
    while((new_fd = accept(fd, (struct sockaddr *)&client_addr, &struct_len))>0){
        printf("Get the Client.IP is %d\n",client_addr.sin_addr.s_addr);
        while((numbytes = recv(new_fd, buff, BUFSIZ, 0)) > 0)
        {
            buff[numbytes] = '\0';
            printf("receive %s,",buff);
            int k=0;
            while(buff[k++]){
                if(buff[k-1] >= 'a' && buff[k-1] <='z') buff[k-1]=buff[k-1]-'a'+'A';
            }
            printf("will send %s\n",buff);
            if(send(new_fd,buff,numbytes,0)<0)
            {
                perror("write");
                return 1;
            }
        }
        closesocket(new_fd);
        printf("connection died\n");
    }
    closesocket(fd);
	#ifdef WIN32
	WSACleanup();
	#endif
    return 0;
}