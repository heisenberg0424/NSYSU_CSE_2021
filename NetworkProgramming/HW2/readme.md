## FTP Program with data Compression
-------
### Network Programming HW2
This is a program that can transfer files using tcp connections.
### Commands :
below command will link to server 192.168.0.1 with port 1500
```
$ link 192.168.0.1 1500
```
the send command will compress the file first then send to server
```
$ send sample.txt
```
to switch variable length to fixed length use switch
```
$ switch
```
leave command will exit the client program
```
$ leave
```
HINT :
run ifconfig to check ip address
```
$ ifconfig
```


### TODO :
- Huffman Encoding
    - Function return error signal
    - combine fixed length and variable length together
    - spilt decode fuction to more sub-function

- FTP Program
    - send files between devices (only on local now)
    - send mutifile once
    - comments for functions
