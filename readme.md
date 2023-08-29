# tcpcat
send and receive data as is via tcp.

supports one protocol:
* tcp

supports three socket types:
* unix
* ipv6
* ipv4

command-line interface:
~~~
tcpcat address [port]
~~~

port is only optional when using unix domain sockets.

without data on standard input:
* wait for connection on address and port
* copy all data from client to standard output
* exit

with data on standard input:
* establish connection to host with address and port
* copy all data from standard input to server
* exit

# setup
~~~
./exe/compile
~~~

the provided compile script uses gcc and musl-libc or glibc. it creates a statically compiled binary at ``exe/tcpcat``.
it can be run directly from the project directory with ``./exe/tcpcat``. alternatively, it can be linked or copied anywhere else. it can be copied into one of the directories listed in the $PATH environment variable to be available as a shell command.

# examples
## 1: file transfer
receiver:
~~~
tcpcat ::1 7000 > myfile
~~~

sender:
~~~
cat myfile | tcpcat ::1 7000
~~~

## 2: transmission statistics and multiple files
additional programs used:
* tar: converts multiple files into a single data stream and the inverse
* pv: displays pipe transmission statistics

receiver:
~~~
tcpcat address port | tar x
~~~

sender:
~~~
tar cf - myfiles | pv | tcpcat address port
~~~

## 3: unix socket
receiver:
~~~
tcpcat /tmp/socket
~~~

sender:
~~~
echo test | tcpcat /tmp/socket
~~~

# configuration
in src/main.c, the value of buffer_size is in bytes and can be modified before compilation to define the amount of data that will be attempted to be transferred in one read/write cycle. the same amount of free memory is required when the program is run. the default buffer size is 10MB.

use_stack_buffer can be set to 1 to allocate the buffer on the stack instead of the heap. in this case, buffer_size can not be greater than the compiler dependent stack size minus a few hundred bytes required for executing the program.

# possible enhancements
* run-time configurable buffer size
* hostname dns resolution. this can also be done with ``getent hosts example.com | cut -d' ' -f1``

# technical
tcpcat is written in one c function. can it be less?
information about some features used: [bind(2)](https://man7.org/linux/man-pages/man2/bind.2.html), [ipv6(7)](https://man7.org/linux/man-pages/man7/ipv6.7.html), [isatty(3)](https://man7.org/linux/man-pages/man3/isatty.3.html), [perror(3)](https://man7.org/linux/man-pages/man3/perror.3.html), [socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html).
