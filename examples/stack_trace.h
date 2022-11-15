/* SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause) */
/* Copyright (c) 2022 Meta Platforms, Inc. */
#ifndef __STACK_TRACE_H_
#define __STACK_TRACE_H_

#ifndef TASK_COMM_LEN
#define TASK_COMM_LEN 16
#endif

#ifndef MAX_STACK_DEPTH
#define MAX_STACK_DEPTH         16
#endif

#ifndef __START_KERNEL_map
    #define __START_KERNEL_map 0xffffffff80000000
#endif

typedef __u64 stack_trace_t[MAX_STACK_DEPTH];

#define STACKID_FLAGS (0 | BPF_F_USER_STACK)

u64 get_frame(u64 *bp) {
    if (*bp) {
        // The following stack walker is x86_64 specific
        u64 stack = 0;
        if (bpf_probe_read(&stack, sizeof(stack), (void *)(*bp+8)))
            return 0;
        if (bpf_probe_read(bp, sizeof(*bp), (void *)*bp))
            *bp = 0;
        if (stack >= __START_KERNEL_map)
        {
            //bpf_printk("in kernel");
            return 1;
        }
        //bpf_printk("stack %x\n", stack);
        return stack;
    }
    return 0;
}

#endif /* __PROFILE_H_ */