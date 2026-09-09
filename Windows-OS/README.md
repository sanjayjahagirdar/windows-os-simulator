# Windows 11 OS Concepts Simulator

## Purpose
This is a C-based educational simulation of major Operating Systems concepts associated with **Windows OS**.

It does **not** implement the real OS kernel, bootloader, device drivers, or production filesystem. It demonstrates concepts that can be observed during an OS viva/demo.

## Demonstrated concepts
- Process creation and process states
- FCFS CPU scheduling
- Round-Robin CPU scheduling and time quantum
- Memory allocation and freeing
- Basic memory-map display
- File metadata
- Read/write permissions
- User mode vs kernel mode

## Compile

### GCC / MinGW
```bash
gcc -std=c11 -Wall -Wextra -O2 windows_os_simulator.c -o windows_os_simulator
```

On Windows:
```text
windows_os_simulator.exe
```

## Suggested demo
1. Create three processes.
2. Run FCFS.
3. Run Round-Robin with quantum 2.
4. Allocate and free memory.
5. Create a file and toggle write permission.
6. Demonstrate user mode and kernel mode.

## Academic note
The simulator is intentionally simplified so that the relationship between theory and implementation is easy to understand.
