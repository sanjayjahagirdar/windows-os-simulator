/*
 * ChromeOS OS Concepts Simulator
 * Academic simulation - NOT the real operating-system kernel.
 * ChromeOS model. Focus: process/resource management, scheduling, memory, files, permissions, and the security idea of restricted application execution.
 */

#include <stdio.h>
#include <string.h>

#define MAX 20
#define MEM_SIZE 1024

typedef struct {
    int pid;
    char name[32];
    int burst;
    int remaining;
    char state[16];
} Process;

typedef struct {
    int start, size, free;
} Block;

typedef struct {
    char name[40];
    int size;
    char owner[32];
    int read, write;
} FileEntry;

Process processes[MAX];
int process_count = 0;
Block memory[50];
int block_count = 1;
FileEntry files[MAX];
int file_count = 0;

void init_memory(void) {
    memory[0].start = 0;
    memory[0].size = MEM_SIZE;
    memory[0].free = 1;
}

void create_process(void) {
    if (process_count >= MAX) { printf("Process table full.\n"); return; }
    Process *p = &processes[process_count];
    p->pid = 1001 + process_count;
    printf("Process name: "); scanf("%31s", p->name);
    printf("CPU burst time: "); scanf("%d", &p->burst);
    if (p->burst <= 0) { printf("Burst must be positive.\n"); return; }
    p->remaining = p->burst;
    strcpy(p->state, "READY");
    process_count++;
    printf("Created PID %d (%s).\n", p->pid, p->name);
}

void list_processes(void) {
    if (!process_count) { printf("No processes.\n"); return; }
    printf("\nPID\tName\tBurst\tRemaining\tState\n");
    for (int i=0;i<process_count;i++)
        printf("%d\t%s\t%d\t%d\t\t%s\n", processes[i].pid, processes[i].name,
               processes[i].burst, processes[i].remaining, processes[i].state);
}

void fcfs(void) {
    if (!process_count) { printf("Create processes first.\n"); return; }
    int total=0;
    printf("\nFCFS execution order:\n");
    for (int i=0;i<process_count;i++) {
        strcpy(processes[i].state, "RUNNING");
        printf("P%d(%s) -> ", processes[i].pid, processes[i].name);
        total += processes[i].burst;
        processes[i].remaining=0;
        strcpy(processes[i].state, "TERMINATED");
    }
    printf("END\nTotal CPU time = %d units\n", total);
}

void round_robin(void) {
    if (!process_count) { printf("Create processes first.\n"); return; }
    int q, done=0, total=0;
    printf("Time quantum: "); scanf("%d",&q);
    if (q<=0) { printf("Quantum must be positive.\n"); return; }
    for(int i=0;i<process_count;i++) {
        processes[i].remaining=processes[i].burst;
        strcpy(processes[i].state,"READY");
    }
    printf("\nRound-Robin execution:\n");
    while(done < process_count) {
        for(int i=0;i<process_count;i++) {
            if(processes[i].remaining > 0) {
                int slice = processes[i].remaining < q ? processes[i].remaining : q;
                strcpy(processes[i].state,"RUNNING");
                printf("P%d(%d) ", processes[i].pid, slice);
                processes[i].remaining -= slice;
                total += slice;
                if(processes[i].remaining==0) {
                    strcpy(processes[i].state,"TERMINATED");
                    done++;
                } else strcpy(processes[i].state,"READY");
            }
        }
        printf("\n");
    }
    printf("Total CPU time = %d units\n", total);
}

void show_memory(void) {
    printf("\nMemory map (0-%d):\n", MEM_SIZE);
    for(int i=0;i<block_count;i++)
        printf("Start=%d Size=%d Status=%s\n", memory[i].start,memory[i].size,
               memory[i].free ? "FREE" : "USED");
}

void allocate_memory(void) {
    int size; printf("Memory to allocate: "); scanf("%d",&size);
    if(size<=0) { printf("Invalid size.\n"); return; }
    for(int i=0;i<block_count;i++) if(memory[i].free && memory[i].size>=size) {
        if(memory[i].size > size) {
            for(int j=block_count;j>i;j--) memory[j]=memory[j-1];
            memory[i+1].start=memory[i].start+size;
            memory[i+1].size=memory[i].size-size;
            memory[i+1].free=1;
            memory[i].size=size;
            block_count++;
        }
        memory[i].free=0;
        printf("Allocated %d units.\n",size);
        return;
    }
    printf("Allocation failed.\n");
}

void free_memory(void) {
    int start; printf("Start address to free: "); scanf("%d",&start);
    for(int i=0;i<block_count;i++) if(memory[i].start==start && !memory[i].free) {
        memory[i].free=1;
        if(i+1<block_count && memory[i+1].free) {
            memory[i].size+=memory[i+1].size;
            for(int j=i+1;j<block_count-1;j++) memory[j]=memory[j+1];
            block_count--;
        }
        if(i>0 && memory[i-1].free) {
            memory[i-1].size+=memory[i].size;
            for(int j=i;j<block_count-1;j++) memory[j]=memory[j+1];
            block_count--;
        }
        printf("Memory block freed.\n"); return;
    }
    printf("Used block not found.\n");
}

void create_file(void) {
    if(file_count>=MAX) { printf("File table full.\n"); return; }
    FileEntry *f=&files[file_count];
    printf("File name: "); scanf("%39s",f->name);
    printf("File size: "); scanf("%d",&f->size);
    printf("Owner: "); scanf("%31s",f->owner);
    f->read=1; f->write=1; file_count++;
    printf("File created with read/write permission.\n");
}

void list_files(void) {
    if(!file_count){printf("No files.\n");return;}
    printf("\nName\tSize\tOwner\tPermissions\n");
    for(int i=0;i<file_count;i++)
        printf("%s\t%d\t%s\t%c%c\n",files[i].name,files[i].size,files[i].owner,
               files[i].read?'r':'-',files[i].write?'w':'-');
}

void toggle_write(void) {
    char name[40]; printf("File name: "); scanf("%39s",name);
    for(int i=0;i<file_count;i++) if(strcmp(files[i].name,name)==0) {
        files[i].write=!files[i].write;
        printf("Write permission is now %s.\n",files[i].write?"ON":"OFF");
        return;
    }
    printf("File not found.\n");
}

void mode_demo(void) {
    int c; printf("\n1. User mode\n2. Kernel mode\nChoice: "); scanf("%d",&c);
    if(c==1) printf("USER MODE: application code runs with restricted access to protected resources.\n");
    else if(c==2) printf("KERNEL MODE: privileged OS code can manage protected resources.\n");
    else printf("Invalid choice.\n");
}

int main(void) {
    init_memory();
    int choice;
    do {
        printf("\n========== OS CONCEPTS SIMULATOR ==========\n");
        printf("1.Create process  2.List processes  3.FCFS  4.Round-Robin\n");
        printf("5.Allocate memory 6.Free memory 7.Show memory\n");
        printf("8.Create file      9.List files       10.Toggle write\n");
        printf("11.User/Kernel mode  0.Exit\nChoice: ");
        scanf("%d",&choice);
        switch(choice) {
            case 1:create_process();break; case 2:list_processes();break;
            case 3:fcfs();break; case 4:round_robin();break;
            case 5:allocate_memory();break; case 6:free_memory();break;
            case 7:show_memory();break; case 8:create_file();break;
            case 9:list_files();break; case 10:toggle_write();break;
            case 11:mode_demo();break; case 0:printf("Exiting simulator.\n");break;
            default:printf("Invalid choice.\n");
        }
    } while(choice!=0);
    return 0;
}

