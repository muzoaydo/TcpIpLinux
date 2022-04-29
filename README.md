# TcpIpLinux
## A TCP-IP messager which has a client and a server that could connect each other. It can send and recieve messages async and also can accept a new client and kill the other.


In order to use the app, you should open up two terminals either from same or different computers. After that you should use;
```
// 1) To start the server
./server PortNumberHere

// 2) To start the client
./client IpAddressHere PortNumberHere

// 3) To fix socket errors
./socketFixer PortNumberHere
```

For an example you can follow these if you are going to run both in the same PC;
```
./server 9999

./client 127.0.0.1 9999
```
Then you should see the client accepted message in prompt.

After that you will see a menu which has a send message or exit command. If you type 2 and hit enter program should ask for a message input. 
When you hit enter again you will see your message at the other side :)

If you hit 9 and enter program will close the sockets and exit with 9.

If you do not close the app like this your port might get stuck and you can fix that with the socketFixer program.

If you change anything in the code you should compile it in this way;
```
gcc -pthread -o server server.c 
/*or*/
gcc -pthread -o client client.c
```
![Screenshot_1](https://user-images.githubusercontent.com/64156908/165906381-12800722-4064-4da6-a785-90f7abc5e4ce.jpg)


