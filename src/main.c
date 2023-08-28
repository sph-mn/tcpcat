#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define handle_error(a) if (0 > a) goto error

int main(int argc, char **argv) {
  int address_family, port, remote;
  socklen_t address_length;
  int s = 0;
  struct sockaddr_storage address = {};
  // check and parse arguments
  if (2 > argc) {
    printf("arguments: address [port]\n");
    return(1);
  }
  if ('/'  == argv[1][0]) {
    printf("un\n");
    address_family = AF_UNIX;
    address_length = sizeof(struct sockaddr_un);
    ((struct sockaddr_un*)&address)->sun_family = address_family;
  }
  else {
    // host name resolution with getaddrinfo?
    port = htons(atoi(argv[2]));
    if (strchr(argv[1], ':')) {
      printf("ip6\n");
      address_family = AF_INET6;
      address_length = sizeof(struct sockaddr_in6);
      ((struct sockaddr_in6*)&address)->sin6_family = address_family;
      ((struct sockaddr_in6*)&address)->sin6_port = port;
      inet_pton(AF_INET6, argv[1][0], &addr->sin6_addr);
    }
    else {
      printf("ip4\n");
      address_family = AF_INET;
      address_length = sizeof(struct sockaddr_in);
      ((struct sockaddr_in*)&address)->sin_family = address_family;
      ((struct sockaddr_in*)&address)->sin_port = port;
      inet_pton(AF_INET, argv[1][0], &addr->sin_addr);
    }
  }
  // create and bind socket. socket assumed closed on exit
  s = socket(address_family, SOCK_STREAM, 0);
  handle_error(s);
  handle_error(bind(s, (const struct sockaddr*)&address, address_length));
  if (isatty(0)) {
    // no data on stdin, receive
    handle_error(listen(s, 1));
    remote = accept(s, 0, 0);
    handle_error(remote);
    // copy remote to stdout
  }
  else {
    // data on stdin, send
    printf("connect\n");
    remote = connect(s, 0, 0);
    handle_error(remote);
    // copy stdin to remote
  }
  handle_error(close(s));
  return(0);
error:
  perror(strerror(errno));
  return(1);
}
