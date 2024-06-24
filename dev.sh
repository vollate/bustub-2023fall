#!/bin/bash

docker run --name ops-docker -v ./project:/home/$USER/workspace --rm -it bustub_docker:latest
