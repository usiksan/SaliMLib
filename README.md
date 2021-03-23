# SaliMLib
SaliLab cooperative Minimal Multitasking Library for 32-bit single-core Microcontrollers


SaliMLib is a library for use as a multitasking framework for embedded systems based on 32-bit single-core microcontrollers. This library uses cooperative multitasking. This means that each task is not allocated a pre-defined time quantum, but the task itself determines the appropriate time to transfer control to another task. This approach greatly simplifies task synchronization and eliminates the need to use critical sections.

The main disadvantages of cooperative multitasking compared to preemptive multitasking are the uncertainty of the response time to events, which is why such systems are not used in real-time systems. However, this limitation can be easily circumvented by using hardware interrupts, thus obtaining a complete analog of "hard time systems", and the actual "real time" can be achieved by configuring the microcontroller peripherals to automatically work in parallel with the microcontroller core.

This library is an excellent alternative or addition to the traditional "state machines"for embedded systems. With this multitasking library, the implementation of many algorithms is radically simplified.

Features
• minimalism. This library consumes less resources than other RTOS
• multitasking. The library provides cooperative multitasking
• a two-level priority. In the library, all tasks are divided into critical and other tasks. Tasks with a critical priority level are managed first
• a model without priorities can be used
• no task scheduler. Based on the principle of cooperation all tasks of the same priority are performed sequentially
• computer language. It uses C++ 11 for the interface and platform-independent part, as well as Assembler language for the platform-dependent part
• simplicity. The entire library consists of three files: a kernel header file, a file with kernel source codes, and an assembler file with platform-dependent code
• documentation. The source codes of the library are provided with comprehensive comments, as well as a guide
• examples. There are usage examples for all parts of the library
