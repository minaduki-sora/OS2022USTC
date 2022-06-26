#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<stdlib.h>
#include<string.h>
#define MAX_PROCESS_NUM 256
#define VIEW_NUM 20

int pid[MAX_PROCESS_NUM];
char temp_1[MAX_PROCESS_NUM*16];
char temp_2[MAX_PROCESS_NUM*16];
char *COMM[MAX_PROCESS_NUM];
int isrunning[MAX_PROCESS_NUM];
unsigned long long cputime[MAX_PROCESS_NUM];
int pid_2[MAX_PROCESS_NUM];
char *COMM_2[MAX_PROCESS_NUM];
int isrunning_2[MAX_PROCESS_NUM];
unsigned long long cputime_2[MAX_PROCESS_NUM];
float cpu[MAX_PROCESS_NUM];
int PE[MAX_PROCESS_NUM];

float cpu_cal(int id_src)
{
    int i;
    for(i = 0 ; i < MAX_PROCESS_NUM - 1 && pid[i] > 0; i++){
        if(pid_2[id_src] == pid[i]){
            //if(cputime[i])printf("pid:%d cputime_1:%llu cputime_2:%llu\n",pid_2[id_src],cputime_2[id_src] , cputime[i]);//debug
            unsigned long long delta = (cputime_2[id_src] - cputime[i]);
            //if(delta)printf("pid:%d  %llu\n",pid_2[id_src],delta);//debug
            return delta / 1.0e9;
        }
    }
    return 0;//new process
}

int Print()
{
    int i;
    float m = -2;
    int mid = 0;
    for(i = 0; i < MAX_PROCESS_NUM && pid_2[i] > 0;i++){
        if(PE[i] == 0 && cpu[i] > m){
            mid = i;
            m = cpu[i];
        }
    }
    if(m > -1){
        PE[mid] = 1;
        printf("%d\t%.6f\t\t%d\t%s\n",pid_2[mid],cpu[mid],isrunning_2[mid],COMM_2[mid]);
        return mid;
    }
    return -1;
}

int split_string(char* string, char *sep, char** string_clips) {
    
    string_clips[0] = strtok(string, sep);
    if(string_clips[0] == NULL){
        return 0;
    }
    int clip_num=0;
    
    do {
        char *head, *tail;
        head = string_clips[clip_num];
        tail = head + strlen(string_clips[clip_num]) - 1;
        while(*head == ' ' && head != tail)
            head ++;
        while(*tail == ' ' && tail != head)
            tail --;
        *(tail + 1) = '\0';
        string_clips[clip_num] = head;
        clip_num ++;
    }while(string_clips[clip_num]=strtok(NULL, sep));
    return clip_num;
}

int main(void)
{
    int i = 0;
    while(1){
        syscall(333, pid,temp_1,isrunning,cputime);
        sleep(1);
        syscall(333, pid_2,temp_2,isrunning_2,cputime_2);
        for(i = 0; i < MAX_PROCESS_NUM - 1; i++ ){
            cpu[i] = cpu_cal(i);
        }
        //sleep(3);//debug
        split_string(temp_1," ",COMM);
        split_string(temp_2," ",COMM_2);
        system("clear");
        printf("PID\tCPU\t\tISRUNNING\tCOMM\n");
        memset(PE,0,MAX_PROCESS_NUM*sizeof(int));
        for(i = 0; i < VIEW_NUM ; i++){
            if(Print() == -1){
                printf("ERROR\n");
                exit(255);
            }
        }
        
    }
    return 0;
}