#!/bin/bash

docker run --name ops-docker -v .:$(pwd) --rm -it bustub_docker:latest
