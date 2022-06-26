SYSCALL_DEFINE4(my_top, int __user *, pid ,char __user *, temp ,int __user * , isrunning ,unsigned long long __user * , cputime){
    struct task_struct* task;
    int i = 0;
    int j = 0;
    int id[256] = {0};
    char name[256*16] = {0};
    unsigned long long cpu[256] = {0};
    int isrun[256] = {0};
    printk("[Syscall] my_top\n");
    for_each_process(task){
        id[i] = task->pid;
        int flag = 0;
        for(j = 0; j < 16; j++){
            if(task->comm[j] == '\0')flag = 1;
            if(!flag){
                name[16*i+j] = task->comm[j];
            }else name[16*i+j] = 32;
        }
        cpu[i] = task->se.sum_exec_runtime;
        //printk("cpu[%d] = %llu\n",cpu[i]);//debug
        isrun[i] = task->state == 0;
        i++;
        if(i > 255){
            printk("Process num overflow\n");
            break;
        }
    } 
    copy_to_user(pid, id, 256*sizeof(int));
    copy_to_user(temp,name,256*16*sizeof(char));
    copy_to_user(cputime, cpu, 256*sizeof(unsigned long long));
    copy_to_user(isrunning, isrun, 256*sizeof(int));
    copy_to_user(pid, id, 256*sizeof(int));
    return 0;
}