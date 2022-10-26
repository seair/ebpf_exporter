#!/bin/bash
docker run --rm -it --privileged -p 9435:9435 \
  -v $(pwd)/examples:/examples \
  -v /sys/kernel/debug:/sys/kernel/debug:ro \
  -v /sys/fs/cgroup:/sys/fs/cgroup:ro \
  ebpf_exporter /bin/bash
