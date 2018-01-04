#include "sockets.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <event2/util.h>

int sockets::createListeningSocket(const char *hostname, const char *service)
{
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo *servinfo;
    int status = getaddrinfo(hostname, service, &hints, &servinfo);
    if(status != 0)
    {
        return -1;
    }

    int sockfd;
    struct addrinfo *ptr;
    for(ptr = servinfo; ptr != nullptr; ptr = ptr->ai_next)
    {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd != -1)
        {
            evutil_make_socket_nonblocking(sockfd);
            
            if(::bind(sockfd, ptr->ai_addr, ptr->ai_addrlen) == 0)
            {
                break;
            }
            
            ::close(sockfd);
            sockfd = -1;
        }        
    }
    
    freeaddrinfo(servinfo);
    
    return sockfd;    
}
