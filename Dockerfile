FROM mirrors.tencent.com/g_k_cdp/tlinux2.6-minimal:latest as builder

RUN yum update -y && \
    yum install -y curl git ca-certificates build-essential libelf-dev gnupg2 golang wget tar gcc automake autoconf libtool make elfutils-libelf-devel

RUN echo 'install go' && \
    wget https://studygolang.com/dl/golang/go1.19.2.linux-amd64.tar.gz && \
    tar -zvxf go1.19.2.linux-amd64.tar.gz && \
    mv go /usr/lib/golang && \
    rm -f go1.19.2.linux-amd64.tar.gz

RUN mkdir /build

RUN git clone https://github.com/seair/libbpf.git /build/libbpf && \
    make -C /build/libbpf/src BUILD_STATIC_ONLY=y LIBSUBDIR=lib install

RUN tar -czf /build/libbpf.tar.gz /usr/lib/libbpf.a /usr/lib/pkgconfig/libbpf.pc /usr/include/bpf

COPY ./ /build/ebpf_exporter

RUN cd /build/ebpf_exporter && \
    PATH="/usr/lib/go-1.19/bin:$PATH" make build && \
    /build/ebpf_exporter/ebpf_exporter --version

FROM scratch as ebpf_exporter

COPY --from=builder /build/ebpf_exporter/ebpf_exporter /ebpf_exporter

ENTRYPOINT ["/ebpf_exporter"]
