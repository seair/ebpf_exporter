// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/* Copyright (c) 2022 Meta Platforms, Inc. */
#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include "bits.bpf.h"
#include "maps.bpf.h"

#include "stack_trace.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

struct key_t {
    u64 cgroup;
    char comm[TASK_COMM_LEN];
    u64 stack;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 102400);
    __type(key, struct key_t);
    __type(value, u64);
} hgame_cpu_sample_count SEC(".maps");

SEC("perf_event")
int profile(struct bpf_perf_event_data *ctx)
{
	struct key_t key;
    key.cgroup = bpf_get_current_cgroup_id();
    u64 bp = ctx->regs.bp;
    if (bpf_get_current_comm(key.comm, sizeof(key.comm)))
		key.comm[0] = 0;

    bpf_printk("comm:%s", key.comm);

    int index = 0;

repeat:
    key.stack = (u64)get_frame(&bp);
    if(!key.stack)
        goto out;
    if(key.stack > 10)
        increment_map(&hgame_cpu_sample_count, &key, 1);
    ++index;
    if(index < MAX_STACK_DEPTH)
        goto repeat;
out:
	return 0;
}