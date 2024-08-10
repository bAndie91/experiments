
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

typedef int bool_t;
#define TRUE 1
#define FALSE 0

#define IPADDR_MAX_LEN INET6_ADDRSTRLEN


int
extract_ip_port(struct sockaddr * socket_addr, char * out_ipaddr, in_port_t * out_port)
{
	in_port_t * port_p;
	void * addr_p;
	
	if(socket_addr->sa_family == AF_INET)
	{
		port_p = &((struct sockaddr_in*)socket_addr)->sin_port;
		addr_p = &((struct sockaddr_in*)socket_addr)->sin_addr;
	}
	else if(socket_addr->sa_family == AF_INET6)
	{
		port_p = &((struct sockaddr_in6*)socket_addr)->sin6_port;
		addr_p = &((struct sockaddr_in6*)socket_addr)->sin6_addr;
	}
	else return FALSE;
	
	if(out_port) *out_port = ntohs(*port_p);
	if(!out_ipaddr) return TRUE;
	if(inet_ntop(socket_addr->sa_family, addr_p, out_ipaddr, IPADDR_MAX_LEN) == NULL) return FALSE;
	return TRUE;
}

int
update_socket_addr(struct sockaddr * socket_addr, char * ipaddr_str, char * portnum_str)
{
	int ret;
	in_port_t * port_p;
	void * addr_p;
	
	if(socket_addr->sa_family == AF_INET)
	{
		port_p = &(((struct sockaddr_in*)socket_addr)->sin_port);
		addr_p = &(((struct sockaddr_in*)socket_addr)->sin_addr);
	}
	else if(socket_addr->sa_family == AF_INET6)
	{
		port_p = &(((struct sockaddr_in6*)socket_addr)->sin6_port);
		addr_p = &(((struct sockaddr_in6*)socket_addr)->sin6_addr);
	}
	else return FALSE;
	
	if(portnum_str) *port_p = htons(atoi(portnum_str));
	ret = inet_pton(socket_addr->sa_family, ipaddr_str, addr_p);
	return ret == 1 ? TRUE : FALSE;
}

int main(int argc, char ** argv)
{
	struct sockaddr_in sa;
	#define BUFSIZE 100
	char buf[BUFSIZE];
	
	sa.sin_family = strcmp(argv[1], "AF_INET")==0 ? AF_INET : (strcmp(argv[1], "AF_INET6")==0 ? AF_INET6 : atoi(argv[1]));
	update_socket_addr((struct sockaddr*)&sa, argv[2], argv[3]);
	
	char * rev = inet_ntop(sa.sin_family, &(sa.sin_addr), buf, BUFSIZE);
	fprintf(stderr, "inet_ntop(%s),%s -> %s (errno %d), %u\n", argv[2], argv[3], rev, errno, ntohs(sa.sin_port));
	
	strcpy(buf, "qqqqqqqqqqqqq");
	
	in_port_t port;
	extract_ip_port((struct sockaddr*)&sa, buf, &port);
	fprintf(stderr, "extract ip '%s' port '%u'\n", buf, port);
	
	return 0;
}
