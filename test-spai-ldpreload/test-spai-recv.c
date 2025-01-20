#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create the UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Clear and set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port and IP
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", PORT);

    // Prepare to receive a message
    struct iovec iov[1];
    struct msghdr msg;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Set up the buffer for the iovec
    iov[0].iov_base = buffer;

    while(1)
    {
	    iov[0].iov_len = BUFFER_SIZE;
        
        // Clear and set up the message header structure
        memset(&msg, 0, sizeof(msg));
        msg.msg_name = &client_addr;
        msg.msg_namelen = client_addr_len;
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
    
        // Receive a message
        ssize_t num_bytes = recvmsg(sockfd, &msg, 0);
        if (num_bytes < 0) {
            perror("recvmsg failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    
        // Null-terminate the received data (if needed)
        if (num_bytes < BUFFER_SIZE) {
            buffer[num_bytes] = '\0';
        }

        // Print the received message
        printf("Received message from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Message: %s\n", buffer);

	    iov[0].iov_len = num_bytes;
        num_bytes = sendmsg(sockfd, &msg, 0);
        if (num_bytes < 0) {
            perror("sendmsg failed");
        }
    }
    
    // Close the socket
    close(sockfd);

    return 0;
}
