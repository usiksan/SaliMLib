        .syntax unified
        .cpu cortex-m0
        .thumb

        .global  smPortInitStack
        .global  smPortBuildStack
        .global  smPortSwitchContext

        .section .text.smPortSwitchContext
smPortSwitchContext:
        push  {r4-r7}           @ push r4-r12
        mov   r3,r8
        mov   r4,r9
        mov   r5,r10
        mov   r6,r11
        mov   r7,r12
        push  {r3-r7}
        push  {lr}
        ldr   r0,=smCurrentTask @ r0 = &smCurrentTask
        ldr   r4,[r0]           @ r4 = smCurrentTask
        mov   r5,sp
        str   r5,[r4]           @ *smCurrentTask = sp
        ldr   r5,=smNextTask    @ r5 = &smNextTask
        ldr   r6,[r5]           @ r6 = smNextTask
        str   r6,[r0]           @ smCurrentTask =  smNextTask
        ldr   r5,[r6]           @ sp = *smNextTask
        mov   sp,r5
        pop   {r5}              @ pop lr
        mov   lr,r5
        pop   {r3-r7}           @ pop r4-r12
        mov   r8,r3
        mov   r9,r4
        mov   r10,r5
        mov   r11,r6
        mov   r12,r7
        pop   {r4-r7}
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
        mov  r0,sp           @ r0 = sp (save temporary)
        ldr  r2,=smNextTask  @ r2 = &smNextTask
        ldr  r2,[r2]         @ r2 = smNextTask
        ldr  r3,[r2]         @ sp = *smNextTask
        mov  sp,r3
        push {r4-r7}         @ push r4-r12
        mov   r3,r12
        push  {r3}
        mov   r3,r11
        push  {r3}
        mov   r3,r10
        push  {r3}
        mov   r3,r9
        push  {r3}
        mov   r3,r8
        push  {r3}
        push {r1}            @ push proc (it will be poped into lr)
        mov  r1,sp           @ *smNextTask = sp
        str  r1,[r2]
        mov  sp,r0           @ sp = r0 (restore temporary saved)
        bx   lr

        .word smTaskEntry
        .word smNextTask
