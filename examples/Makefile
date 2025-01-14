CLANG := clang
CC = gcc

ARCH := $(shell uname -m | sed -e 's/x86_64/x86/' -e 's/aarch64/arm64/')

SRC = ${wildcard *.bpf.c}
SRCCC = ${wildcard *.bpf.cc}
OBJ = ${patsubst %.bpf.c, %.bpf.o, $(SRC)}
OBJCC = ${patsubst %.bpf.cc, %.bpf.o, $(SRCCC)}

# From https://github.com/libbpf/libbpf-bootstrap/blob/a7c0f7e4a28/examples/c/Makefile#L34-L43
# Get Clang's default includes on this system. We'll explicitly add these dirs
# to the includes list when compiling with `-target bpf` because otherwise some
# architecture-specific dirs will be "missing" on some architectures/distros -
# headers such as asm/types.h, asm/byteorder.h, asm/socket.h, asm/sockios.h,
# sys/cdefs.h etc. might be missing.
#
# Use '-idirafter': Don't interfere with include mechanics except where the
# build would have failed anyways.
CLANG_BPF_SYS_INCLUDES = $(shell $(CC) -v -E - </dev/null 2>&1 \
	| sed -n '/<...> search starts here:/,/End of search list./{ s| \(/.*\)|-idirafter \1|p }')

$(OBJ): %.bpf.o: %.bpf.c
	$(CLANG) -g -O2 -Wall -D__TARGET_ARCH_$(ARCH) $(CLANG_BPF_SYS_INCLUDES) -I../include/$(ARCH) -c -target bpf $< -o $@

$(OBJCC): %.bpf.o: %.bpf.cc
	$(CC) -g -O2 -Wall -D__TARGET_ARCH_$(ARCH) $(CLANG_BPF_SYS_INCLUDES) -I../include/$(ARCH) -c -target bpf $< -o $@

.PHONY: clean
clean:
	rm -f *.o

.PHONY: build
build: $(OBJ) $(OBJCC)
