FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -y qemu qemu-system-x86 qemu-utils ssh openssh-server

EXPOSE 1337

RUN useradd -d /home/ctf -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd
RUN ulimit -c 0

WORKDIR /home/ctf
COPY message.ko .
COPY kernel ./kernel
COPY img ./img
COPY startvm .
COPY copy2vm .
COPY ynetd .

EXPOSE 1337
USER ctf
CMD ./ynetd -p ":1337 localhost:10068" ./startvm