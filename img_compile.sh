#!/bin/sh

rm client
rm server
gcc img_client.c -o client
gcc img_server.c -o server
./server
