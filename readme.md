# tcpcat - simple file transfer

transfers data as is via tcp.

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

port is only optional when using using unix sockets.

without data on standard input:
* listen for a new connection on given address and port
* if connection received, read all data from remote and write it to standard output

with data on standard input:
* establish a new connection to host with given address and port
* if connection successful, write all data from standard input to remote

# examples
on the receiver host:
~~~
tcpcat 2606:2800:220:1:248:1893:25c8:1946 7000 > myfile
~~~

on the sender host:
~~~
cat myfile | tcpcat 2606:2800:220:1:248:1893:25c8:1946 7000
~~~

## advanced examples
additional programs used:
* tar: converts multiple files into a single data stream and the inverse
* pv: displays pipe transmission statistics

receiver:
~~~
tcpcat address port > tar x
~~~

sender:
~~~
cat myfile | pv | tar cf - | tcpcat address port
~~~

# technical
tcpcat is written in c in one function. can it be less?

information about used features:
* [socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html)
* [bind(2)](https://man7.org/linux/man-pages/man2/bind.2.html)
* [perror(3)](https://man7.org/linux/man-pages/man3/perror.3.html)