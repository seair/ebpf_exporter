FROM mirrors.tencent.com/tlinux/tlinux2.6-minimal:v20220329 as builder

MAINTAINER zzhijie

USER root
WORKDIR /root/

ENV LANG=en_US.UTF-8
# Timezone
ENV TZ=Asia/Shanghai

RUN yum update -y && \
    yum install -y curl git ca-certificates build-essential libelf-dev gnupg2 golang wget tar gcc automake autoconf libtool make elfutils-libelf-devel procps

RUN echo 'install go' && \
    wget https://studygolang.com/dl/golang/go1.19.2.linux-amd64.tar.gz && \
    tar -zvxf go1.19.2.linux-amd64.tar.gz && \
    mv /usr/lib/golang /usr/lib/golangbak && \
    mv go /usr/lib/golang && \
    rm -f go1.19.2.linux-amd64.tar.gz

RUN mkdir /build

RUN git clone https://github.com/seair/libbpf.git /build/libbpf && \
    make -C /build/libbpf/src install

COPY ./ /build/ebpf_exporter

RUN cd /build/ebpf_exporter && \
    PATH="/usr/lib/go-1.19/bin:$PATH" make build && \
    /build/ebpf_exporter/ebpf_exporter --version && \
    cp /build/ebpf_exporter/ebpf_exporter /ebpf_exporter

# copy source
COPY entrypoint.sh /sbin/
COPY wait-for-it.sh /sbin/

ENTRYPOINT [ "/sbin/entrypoint.sh", "start" ]

#Expose ports
EXPOSE 9435
