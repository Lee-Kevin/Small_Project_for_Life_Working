# Small Project for life and working
**There is all my project for life and working, for a better experience.**

## 20161109
This project is based on Linkit Smart 7688 Duo, Then run a python script from Arduino side and read the result. Acccording to the result light up different color LED Strip. 
> This project will checkout if somebody's K3 Cloud account is online, if so, the LED Stripe will turn into red.

## 20161114

This is a python script that can turn a picture to a ascii text.

## 20161121

This is a project that can indicate if the door is open.

## 20161122

This is a FFT demo based on Arduino FHT library.

## 20170213

This is a project based on python to deal with json file, and change some special values.

## 20170626 - Arduino Music Player Game

This is a demo that use two button and a speaker to play a song, In fact this is a game that not so interesting.

## 20170801 - C Deal with Json

This is the demo shows how to deal with the string to json format.

## 20170808- Using C to Operate SQLite Interface Demo
This is the demo shows how to use C API to create a table for sql and update, insert, delete data from a database, in the demo I use the Sqlite3 as the database.

Execuate:
Take sqldelete.c for example:

```shell
gcc -o delete sqldelete.c -l sqlite3
```
Then you'll find there is a execuatable file named delete, and run the following code.
```shell
./delete
```

## 20170815 TCP/IP Server/Client

This is a demo that shows how to use tcpdump to see the TCP protocol structure, use 3 termials to run tcpdump server and client.

```shell
sudo tcpdump -vvv -X -i lo tcp port 777
```

Run the following code to run the server.

```shell
./server 127.0.0.1
```

And run the code to run the client.

```shell
./client 127.0.0.1
```


to be continue...
