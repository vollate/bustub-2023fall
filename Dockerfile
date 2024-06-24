FROM debian:latest

ARG USER_NAME="foo"
ARG USER_PASS="bar"

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    curl \
    zip unzip \
    clang-14 \
    zsh \
    sudo

RUN useradd -m $USER_NAME && \
    echo "$USER_NAME:$USER_PASS" | chpasswd && \
    echo "$USER_NAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER $USER_NAME

RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

WORKDIR /home/$USER_NAME/workspace

CMD ["/bin/zsh"]
