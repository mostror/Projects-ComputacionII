#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
int main(){
        signal (SIGCHLD,SIG_IGN);
        int sd, csd ,leido;
        char buffer[128];
        struct sockaddr_in6 dir = {};
        sd = socket(AF_INET6,SOCK_STREAM,0);
        if (sd < 0){
                perror ("socket");
                return -1;
        }
        dir.sin6_family = AF_INET6;
        dir.sin6_port = htons( 2500);
        inet_pton(AF_INET6, "::", &dir.sin6_addr);
	dir.sin6_addr = in6addr_loopback; //esta linea hace lo mismo que la de arriba, pero mas legible
        if (bind(sd , (struct sockaddr *)&dir , sizeof dir) < 0 ){
                perror ("bind");
                return -1;
        }
        listen(sd,5);
        while (csd = accept(sd,NULL,NULL)) {
                switch (fork()){
                case 0:
                                while ((leido = read (csd,buffer,sizeof buffer)) > 0){
                                        buffer[leido]=0;
                                        printf ("leido %s\n",buffer);
                                        write(csd,buffer,leido);
                                }
                                return 0; //del hijo    
                }
        }
        close (csd);
        return 0;
}

