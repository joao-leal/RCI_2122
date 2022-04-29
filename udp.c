#include "ring.h"

int main(int argc, char * argv[])
{
	int fd, errorcode;
	ssize_t n, nread;
	socklen_t addrlen;
	struct addrinfo hints, *res;
	struct sockaddr addr;
	knot host;
	char buffer[128], message[128];



	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd == -1)
		exit(1);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;				//IPv4
	hints.ai_socktype = SOCK_DGRAM;			//UDP socket

	memset(&host, 0, sizeof host);

	printf("Choose a KEY, IP and Port to send a message\n");
	scanf("%hd %s %s", &host.self_key, host.self_IP, host.self_Port);

	if((errorcode = getaddrinfo(host.self_IP, host.self_Port, &hints, &res)) != 0)
		exit(1);

	while(1)
	{
		printf("Write your message (type q! to exit): ");
		fgets(message, 128, stdin);

		if(!strcmp(message, "q!"))
		{
			if((n = sendto(fd, "close", 5, 0, res->ai_addr, res->ai_addrlen)) == -1)
				exit(1);

			break;
		}
		
		if((n = sendto(fd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen)) == -1)
			exit(1);

		addrlen = sizeof(addr);

		/* if((nread = recvfrom(fd, buffer, 128, 0, &addr, &addrlen)) == -1)
			exit(1);

		write(1, "echo: ", 6);
		write(1, buffer, nread);
		write(1 , "\n", 1);
 */
	}

	freeaddrinfo(res);
	close(fd);

	return 0;
}