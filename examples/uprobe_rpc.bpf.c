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
    u32 stackid;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct reload_key_t);
    __type(value, u64);
} hgame_bigworld_reload_total SEC(".maps");

struct {
	__uint(type, BPF_MAP_TYPE_STACK_TRACE);
	__uint(key_size, sizeof(u32));
	__uint(value_size, MAXDEPTH * sizeof(u64));
	__uint(max_entries, 10240);
} stackmap SEC(".maps");

#define STACKID_FLAGS (0 | BPF_F_FAST_STACK_CMP)

SEC("uprobe//data/home/zzhijie/heroes-server-core-proj/build/bin/BigWorldServer:_ZN14BigWorldServer6reloadERKSsS1_RSs")
int do_count(struct pt_regs *ctx)
{
    struct reload_key_t key = {};
    bpf_probe_read_user_str(&key.process, sizeof(key.process), "BigWorldServer");
    //std::string str = PT_REGS_PARM1(ctx);
    //bpf_probe_read_user_str(&key.cmd, sizeof(key.cmd), PT_REGS_PARM1(ctx));
    //bpf_probe_read_user_str(&key.param, sizeof(key.param), (void *)PT_REGS_PARM2(ctx));
    //bpf_probe_read_user_str(&key.result, sizeof(key.result), (void *)PT_REGS_PARM3(ctx));

    key.stackid = bpf_get_stackid(ctx, &stackmap, STACKID_FLAGS);
    bpf_printk("stack id:%x \n", key.stackid);

    increment_map(&hgame_bigworld_reload_total, &key, 1);

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
