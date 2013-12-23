#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>


#include <arpa/inet.h> /* inet_aton */

int main(int argc, char **argv )
{
/*       struct addrinfo {
           int     ai_flags;
           int     ai_family;
           int     ai_socktype;
           int     ai_protocol;
           size_t  ai_addrlen;
           struct sockaddr *ai_addr;
           char   *ai_canonname;
           struct addrinfo *ai_next;
       }; */


struct addrinfo hints, *res, *ptr;
        int rc, fd, nread;
        char buf[1024],nombre[80];
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_UNSPEC;

        rc = getaddrinfo(argv[1], "www", &hints, &res);
        if(rc)
        {
                fprintf(stderr, "%s: %s\n", argv[0], gai_strerror(rc));
                return EXIT_FAILURE;
        }
        for (ptr=res; ptr!=NULL; ptr=ptr->ai_next) {
                fd=socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
                if (fd<0) continue;
                if (connect(fd, ptr->ai_addr, ptr->ai_addrlen) == 0) break;
                }
    
        write (fd,"GET / \r\n",8);
        while ((nread=read(fd, buf, sizeof (buf)))){
                write (1,buf,nread);
       }
    
        return(0);
    
}


