## Description 

Simple remote Windows command terminal based on UDP protocol. Communication protocol include command packet and data packet.

Command packet:\
1 byte - command\
2 bytes - quantaty data packets\
last 2 bytes - control sum.\
Data packet:\
2 bytes- count packet\
38 bytes - user data

The server executes a command in the terminal based on the number of data packets. The response is sent to the client. The work is terminated by the 'exit' command.

## Useful reference
[Execute a command on console](https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po)\
[Boost asio UDP](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/tutorial/tutdaytime5.html)
