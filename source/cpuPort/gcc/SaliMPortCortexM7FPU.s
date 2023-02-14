        .syntax unified
        .cpu cortex-m7
        .thumb

        .global  smPortInitStack
        .global  smPortBuildStack
        .global  smPortSwitchContext

        .section .text.smPortSwitchContext
smPortSwitchContext:
        push  {r4-r12}
        push  {lr}
        vpush {s16-s31}
        ldr   r0,=smCurrentTask @ r0 = &smCurrentTask
        ldr   r4,[r0]           @ r4 = smCurrentTask
        str   sp,[r4]           @ *smCurrentTask = sp
        ldr   r5,=smNextTask    @ r5 = &smNextTask
        ldr   r6,[r5]           @ r6 = smNextTask
        str   r6,[r0]           @ smCurrentTask =  smNextTask
        ldr   sp,[r6]           @ sp = *smNextTask
        vpop  {s16-s31}
        pop   {lr}
        pop   {r4-r12}
        bx    lr

        .word smCurrentTask
        .word smNextTask


        .section .text.smPortInitStack
smPortInitStack:
        ldr  r1,=smTopStack @ r1 = &smTopStack
        mov  r0,sp          @ r0 = sp
        str  r0,[r1]        @ smTopStack = sp
        bx   lr

        .word smTopStack




        .section .text.smPortBuildStack
smPortBuildStack:
        ldr  r1,=smTaskEntry @ r1 = proc
        mov  r0,sp           @ r0 = sp
        ldr  r2,=smNextTask  @ r2 = &smNextTask
        ldr  r2,[r2]         @ r2 = smNextTask
        ldr  sp,[r2]         @ sp = *smNextTask
        push {r4-r12}
        push {r1}
        vpush {s16-s31}
        str  sp,[r2]         @ *smNextTask = sp
        mov  sp,r0           @ sp = r0
        bx   lr

        .word smTaskEntry
        .word smNextTask
