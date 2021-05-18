## Online Chatting service
------
### Network Programming HW3
This is a C/C++ program that can provide user-to-user chatting service.
### How my code works:
The "chat" command allows you to send messages to one or more users
```
$ chat user1 user2 user3 "Hello World"
```
If user1 is online, he will recieve the message
```
User Admin has sent you a message “Hello World” at 03:00PM 2021/4/28.
```
If user2 doesn't exist, client will recieve
```
User user1 does not exist.
```
If user3 is offline , client will recieve
```
User user3 is offline. The message will be passed when he comes back.
```
When user3 logins back, he will reveive
```
User Admin has sent you a message “Hello World” at 03:00PM 2021/4/28.
```

The "bye" command will logout the server, and other online users will recieve the message:
```
User Admin is off-line.
```

