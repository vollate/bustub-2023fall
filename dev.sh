#!/bin/bash

docker run --name ops-docker -v .:$(pwd -P) --rm -it bustub_docker:latest
