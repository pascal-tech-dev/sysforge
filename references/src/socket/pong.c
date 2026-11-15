#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


#define BUF_SIZE 1024

void cleanup() {
	exit(0);
}

int argtoaddr(char *arg, struct sockaddr *addr, int *len)
{
	if (arg[0] == '/' || arg[0] == '+') {
		struct sockaddr_un *uaddr = (struct sockaddr_un *) addr;
		if (arg[0] == '+') arg++;
		*len = sizeof(struct sockaddr_un);
		uaddr->sun_family = AF_UNIX;
		strncpy(uaddr->sun_path, arg, 108);
		return 0;
	}  else {
		struct sockaddr_in *iaddr = (struct sockaddr_in *) addr;
		struct in_addr ipaddr;
		char *tok;
		int v;

		iaddr->sin_family = AF_INET;

		tok = strtok(arg, ":");
		fprintf(stderr, "ip: %s\n", tok);

		if (! inet_aton(tok, &ipaddr) ) {
			return -3;
		}

		iaddr->sin_addr = ipaddr;

		tok = strtok(NULL,"");
		if (tok == NULL) {
			fprintf(stderr, "no port specified in ip addr\n");
			return -1;
		}
		fprintf(stderr, "port: %s\n", tok);
		iaddr->sin_port = htons(atoi(tok));
		*len = sizeof(struct sockaddr_in);

		return 0;
	}
	
	
}

int main(int argc, char *argv[])
{
    char c;
    FILE *fp;
    int fromlen;
    int i,j, s, nread, len,clen;
    time_t now;
    struct sockaddr *srvaddr=malloc(sizeof (struct sockaddr_un));
    struct sockaddr *claddr=malloc(sizeof (struct sockaddr_un));
    int sadrlen, cadrlen;
    short count = 0;

    char  buf[20];

    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    if (argtoaddr(argv[1], srvaddr, &sadrlen)) 
        return -1;

    if ((s = socket(srvaddr->sa_family, SOCK_DGRAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }

   if (bind(s, srvaddr, sadrlen)) {
	perror("bind");
	exit(2); 
   }

   for (int i = 0 ; i < 10; i++) {
       cadrlen = sadrlen;
       nread = recvfrom(s, buf, sizeof(buf), 0, claddr, &cadrlen);
       if (nread <= 0) {
            perror("recv");
            exit(3);
        }
       printf("C:<%s>\n", buf);
       count = atoi(buf+5);
       count++;

       if (claddr->sa_family == AF_UNIX)
		    printf("peer(%s)\n", ((struct sockaddr_un *) claddr)->sun_path);
        else {
            struct sockaddr_in *cin = (struct sockaddr_in *) claddr;
		    printf("peer(%s:%d)\n", inet_ntoa(cin->sin_addr), ntohs(cin->sin_port));
        }

       snprintf(buf, 20, "ping %5d", count);
       nread = sendto(s, buf, sizeof(buf), 0, claddr, cadrlen);
       if (nread <= 0) {
            perror("sending");
            return -1;
       }
   }

	    


    close(s);
    return 0 ;
}
