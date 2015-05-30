#!/bin/sh

gcc ~/Desktop/socketlearn/echo_client.c -o client
gcc ~/Desktop/socketlearn/echo_server.c -o server


./server
