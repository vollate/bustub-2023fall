#!/bin/bash

 docker buildx build --build-arg USER_NAME=$USER --build-arg USER_PASS=123 --build-arg ABS_PATH=$(pwd -P) --force-rm -t bustub_docker .
