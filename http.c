#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

int main(void){
	struct hostent *hostinfo = NULL;
	SOCKADDR_IN sin = { 0 };
	char var[50] = {0};


	char request[8192] = {0};
	SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}

	hostinfo = gethostbyname("192.168.6.49");
	if(hostinfo ==NULL)
	{
		printf("UNKNOW host \n");
		exit(EXIT_FAILURE);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
	sin.sin_port = htons(8080);
	sin.sin_family =  AF_INET;

	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
		exit(errno);
	}
	else
	{
	printf("connect success\n");
	}
	#if 0
	/*Get Request*/
	bzero(var,50);
	printf("Enter Get Request\n");
	scanf("%s",var);
	printf("%s\n",var);
	bzero(request, 10000);
	sprintf(request,"Get /%s\r\nHost: %s\r\n\r\n",var,hostname);
	printf("%s\n",request);
	#else
	/*Post Request*/
	char body[4096] = {0};
	bzero(var,50);
	bzero(request,8192);
	strcat(request,"POST /idm/v1/account/sign HTTP/1.1\n");
	strcat(request,"HOST: 192.168.6.49\n");
	strcat(request,"Content-Type: text/json;charset=utf-8\n");

	bzero(body,4096);
	strcat(body,"{\n\"appkey\":\"1214d112\",\n\"devsn\":\"354257056991746\",\n\"imei1\":\"000039495642710\",\n\"imei2\":\"000039495642728\",\n\"devname\":\"My phone\",\n\"devmodel\":\"Honer 3\",\n\"androidversion\":\"4.1.1\",\n\"romversion\":\"4.1.1\",\n\"source\":\"Gigaset\",\n\"rand\":\"NXRTEJii0PNUDAM43bgX3vq/xexfVS8U3aosPjq3Pm9eNRbYcvK3M4mSBPTbyGKJJR4/CbmkrpI6a4Ypqvg4RA==\"\n}");
	int length = strlen(body);
	sprintf(var,"%d",length);
	strcat(request,"Content-Length: ");
	strcat(request,var);
	strcat(request,"\n\n");
	strcat(request,body);
	strcat(request,"\n\n");

	printf("Send:\n%s\n",request);
	#endif

	if(send(sock,request,strlen(request),0)<0)
	{
		perror("send()");
		exit(errno);
	}
	else
	{
		printf("send succes\n");

	}
	bzero(request,8192);
	read(sock,request,8192);
	printf("%s\n",request);

	close(sock);

}
