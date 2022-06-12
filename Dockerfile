FROM ubuntu:focal
MAINTAINER zjuyzj <1019367509@qq.com>
RUN sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
RUN sed -i 's/security.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y vim curl git
RUN apt-get install -y g++ gdb make
RUN apt-get install -y flex bison
RUN apt-get install -y graphviz
WORKDIR /home