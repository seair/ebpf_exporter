#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include "bits.bpf.h"
#include "maps.bpf.h"
#include <string.h>
#include "../libbpfgo/libbpf/src/bpf_tracing.h"

#define MAXDEPTH    10
#ifndef __START_KERNEL_map
    #define __START_KERNEL_map 0xffffffff80000000
#endif

struct reload_key_t {
    char process[32];
    u64 stack[MAXDEPTH];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct reload_key_t);
    __type(value, u64);
} hgame_bigworld_reload_total SEC(".maps");

#define STACKID_FLAGS (0 | BPF_F_USER_STACK)

static u64 get_frame(u64 *bp) {
    if (*bp) {
        // The following stack walker is x86_64 specific
        u64 stack = 0;
        if (bpf_probe_read(&stack, sizeof(stack), (void *)(*bp+8)))
            return 0;
        if (bpf_probe_read(bp, sizeof(*bp), (void *)*bp))
            *bp = 0;
        if (stack >= __START_KERNEL_map)
        {
            bpf_printk("in kernel");
            return 1;
        }
        bpf_printk("stack %x\n", stack);
        return stack;
    }
    return 0;
}

SEC("uprobe//data/home/zzhijie/heroes-server-core-proj/build/bin/BigWorldServer:_ZN14BigWorldServer6reloadERKSsS1_RSs")
int do_count(struct pt_regs *ctx)
{
    struct reload_key_t key = {};
    bpf_probe_read_user_str(&key.process, sizeof(key.process), "BigWorldServer");
    //std::string str = PT_REGS_PARM1(ctx);
    //bpf_probe_read_user_str(&key.cmd, sizeof(key.cmd), PT_REGS_PARM1(ctx));
    //bpf_probe_read_user_str(&key.param, sizeof(key.param), (void *)PT_REGS_PARM2(ctx));
    //bpf_probe_read_user_str(&key.result, sizeof(key.result), (void *)PT_REGS_PARM3(ctx));

    u64 bp = ctx->bp;
    int depth = 0;
    // unrolled loop, 10 (MAXDEPTH) frames deep:
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    if (!(key.stack[depth++] = get_frame(&bp))) goto out;
    
out:
    increment_map(&hgame_bigworld_reload_total, &key, 1);

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
