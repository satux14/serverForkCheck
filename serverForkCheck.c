/* 
 * Program to check socket behaviour during fork
 *  - Sathish Kumar R
 * 
 * Open a socket and listen on it. 
 * Before doing accept, do a fork().
 * Now both process listens on the same listen socket queue
 * This is quite different from REUSE_PORT behaviour where 2 sockets 
 * created and LBalanced.
 *
 * Result: Either of one will get the connection at any given point of time
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
serverForkCheck() {
	int fd;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int ret;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket:");
		exit(-1);
	}

	// Used web server port so that I can use wget command
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret < 0) {
		perror("bind:");
		exit(-1);
	}
	printf("Bind success\n");
	listen(fd, 1000);
	
	ret = fork();
	if (ret == 0) {
retryChild:
		// child
		ret = accept(fd, (struct sockaddr*) &addr, &addrlen);
		if (ret < 0) {
			perror("Child process accept:");
			exit(-1);
		}
		printf("Got a connection in  child process\n");
		goto retryChild;
	} else {
retry:
		// Parent
		ret = accept(fd, (struct sockaddr*) &addr, &addrlen);
		if (ret < 0) {
			perror("Parent process accept:");
			exit(-1);
		}
		printf("Got a connection in parent process\n");
		goto retry;
	}
	return 0;
}

int main() {
	serverForkCheck();
	return 0;
}
