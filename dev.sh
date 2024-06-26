#!/bin/bash
CONTAINER_NAME=bustub_foo

if [ -z "$(docker container ls -a |grep $CONTAINER_NAME)" ]; then
    docker run --name $CONTAINER_NAME -v .:$(pwd -P) --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it bustub_docker:latest
else
    docker start -i $CONTAINER_NAME
fi
