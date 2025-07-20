#include <stddef.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <runtime.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <types.h>
#include <argv.h>
#include <stdlib.h>
#include <sys/time.h>
#include <main.h>

void callback(mypriv_t * priv, args_option_t * option, int arg) {
	switch (option->type) {
		case TYPE_STRING:
			priv->address = (char *) arg;
			return;
		case TYPE_INT:
			priv->port = arg;
			return;
	}
}

int main(int argc, char * argv[]) {
	args_progspec_t spec[] = {
		"LemonOS coreutils", "1.0", "Lemon", NULL,
		"unlicense: <https://unlicense.org>",
		"This is public domain software: you are free to change and redistribute it.",
		"There is NO WARRANTY, to the extent permitted by law.",
		"Probe UDP ports."
	};
	args_option_t options[] = {
		{'p', "port",	1, TYPE_INT,	ARG_REQUIRED, callback, .help="UDP port to probe"},
		{'\1',NULL,	1, TYPE_STRING,	ARG_REQUIRED, callback, .help="ADDRESS"}
	};
	int options_count = sizeof(options) / sizeof(options[0]);
	mypriv_t priv;
	memset(&priv, 0, sizeof(priv));
	priv.address = "0.0.0.0";
	priv.port = 0;

	args_setup(ARG_DEFAULT_TO_HELP | ARG_STACK_POSITIONALS);
	args_load_spec(spec);
	if (args_parse(argc, argv, options_count, options, &priv)) {
		return -1;
	}

	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) { // old LemonOS kernels
		printf("Networking unsuported, please use a supported kernel!\n");
		exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(priv.address);
	servaddr.sin_port = htons(priv.port);

	struct timeval timev;
	timev.tv_sec = 1;
	timev.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timev, sizeof(timev)) < 0) {
		printf("Unknown setsockopt error!\n");
		exit(0);
	}

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("Unknown connection error!\n");
		exit(0);
	}
	write(sockfd, "ping", 4);

	char buf;
	ssize_t stat = recvfrom(sockfd, &buf, 1, 0, NULL, NULL);
	switch (stat) {
		default:
			if (stat < 0) {
				printf("recvfrom() = %d\n", stat);
				break;
			}
			printf("Port open!\n");
			break;
		case 0:
		case -111:
			printf("Port closed!\n");
			break;
		case -11:
			printf("Port open!\n");
			break;
		case -1:
			printf("recvfrom() = %d\n", stat);
	}

	args_unsetup();
	return 0;
}
