#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include "bits.bpf.h"
#include "maps.bpf.h"
#include <string.h>
#include "../libbpfgo/libbpf/src/bpf_tracing.h"
#include "stack_trace.h"

#define MAXDEPTH    10

struct reload_key_t {
    char process[32];
    stack_trace_t stack;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct reload_key_t);
    __type(value, u64);
} hgame_bigworld_reload_stack_track SEC(".maps");


SEC("uprobe//data/home/zzhijie/heroes-server-core-proj/build/bin/BigWorldServer:_ZN14BigWorldServer6reloadERKSsS1_RSs")
int do_count(struct pt_regs *ctx)
{
    struct reload_key_t key;
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
    increment_map(&hgame_bigworld_reload_stack_track, &key, 1);

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
