
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char ** argv)
{
	struct sockaddr_in sa;
	#define BUFSIZE 100
	char buf[BUFSIZE];
	
	int af = strcmp(argv[1], "AF_INET")==0 ? AF_INET : (strcmp(argv[1], "AF_INET6")==0 ? AF_INET6 : atoi(argv[1]));
	int ret = inet_pton(af, argv[2], &(sa.sin_addr));
	fprintf(stderr, "inet_pton(%d, %s, ...) -> %u (errno %d)\n", af, argv[2], ret, errno);
	char * rev = inet_ntop(af, &(sa.sin_addr), buf, BUFSIZE);
	fprintf(stderr, "inet_ntop(%d, %s, ...) -> %s (errno %d)\n", af, argv[2], rev, errno);
	return 0;
}
