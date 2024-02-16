FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    && rm -rf /var/lib/apt/lists/*

CMD ["uname", "-a"]
