FROM debian:bookworm

ARG USER_NAME="foo"
ARG USER_PASS="bar"
ARG ABS_PATH="~"

RUN apt-get update && apt-get install -y \
    #build-essential \
    git cmake curl zip unzip \
    clang-format clang-tidy clang lldb lld libc++-dev libc++abi-dev python3-lldb \ 
    doxygen pkg-config zlib1g-dev libelf-dev libdwarf-dev \
    zsh sudo

RUN useradd -m $USER_NAME && \
    echo "$USER_NAME:$USER_PASS" | chpasswd && \
    echo "$USER_NAME ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

RUN ln -s /usr/lib/llvm-14/lib/python3.11/dist-packages/lldb/* /usr/lib/python3/dist-packages/lldb/

USER $USER_NAME

RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

RUN zsh -c 'source ~/.zshrc && omz plugin enable sudo'

RUN echo "export CC=clang\nexport CXX=clang++">> ~/.zshrc

WORKDIR $ABS_PATH

CMD ["/bin/zsh"]
