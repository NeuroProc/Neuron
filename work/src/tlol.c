#include <stdio.h>
#include "tlol.h"
//#include "/usr/include/sys/types.h"
#include <sys/socket.h>
//#include <netdb.h>
#include <netinet/in.h>
//#include <netinet/ip.h>
//#include <arpa/inet.h>
//#include <netinet/tcp.h>

#ifndef LOL

int main() {

	u_char a;
	u_short b;

	printf("%d\n",IP_MULTICAST_LOOP);

	return 0;
}
