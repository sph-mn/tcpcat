#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define handle_error(a) if (0 > a) goto error
#define buffer_size 10000000
#define use_stack_buffer 0

int main(int argc, char **argv) {
  int address_family, port, s, source, target;
  socklen_t address_length;
  ssize_t count;
  struct sockaddr_storage address = {};
  if (3 == argc) {
    port = htons(atoi(argv[2]));
    if (strchr(argv[1], ':')) {
      address_family = AF_INET6;
      address_length = sizeof(struct sockaddr_in6);
      ((struct sockaddr_in6*)&address)->sin6_family = address_family;
      ((struct sockaddr_in6*)&address)->sin6_port = port;
      inet_pton(address_family, argv[1], &((struct sockaddr_in6*)&address)->sin6_addr.s6_addr);
    }
    else {
      address_family = AF_INET;
      address_length = sizeof(struct sockaddr_in);
      ((struct sockaddr_in*)&address)->sin_family = address_family;
      ((struct sockaddr_in*)&address)->sin_port = port;
      inet_pton(address_family, argv[1], &((struct sockaddr_in*)&address)->sin_addr.s_addr);
    }
  }
  else if (2 == argc) {
    address_family = AF_UNIX;
    address_length = sizeof(struct sockaddr_un);
    ((struct sockaddr_un*)&address)->sun_family = address_family;
    strcpy(((struct sockaddr_un*)&address)->sun_path, argv[1]);
  }
  else {
    printf("arguments: address [port]\n");
    return(1);
  }
  s = socket(address_family, SOCK_STREAM, 0); // socket assumed closed on exit
  handle_error(s);
#if use_stack_buffer
  unsigned char buffer[buffer_size];
#else
  unsigned char* buffer;
  buffer = malloc(buffer_size);
  if (!buffer) goto error;
#endif
  if (isatty(0)) {
    handle_error(bind(s, (const struct sockaddr*)&address, address_length));
    handle_error(listen(s, 1));
    source = accept(s, 0, 0);
    handle_error(source);
    target = 1;
  }
  else {
    handle_error(connect(s, (const struct sockaddr*)&address, address_length));
    source = 0;
    target = s;
  }
  // copy
  while (0 < (count = read(source, buffer, buffer_size))) {
    handle_error(write(target, buffer, count));
  }
  handle_error(count);
  return(0);
error:
  perror(0);
  return(1);
}
