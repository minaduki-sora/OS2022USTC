## 思考

1.<img src="C:\Users\99644\AppData\Roaming\Typora\typora-user-images\image-20220401205144789.png" alt="image-20220401205144789" style="zoom:50%;" />



## caution

(1)ctrl+D结束为正常结束，带有后续逻辑，而ctrl+C为cansel，强制终止，因此在执行如wc -l时，输出终止时按下ctrl+d有结果而ctrl+d无

## 实验内容

测试样例

```c
//多命令
ls ; touch 1.txt ; ls
//单管道 & 重定向-覆盖 ; 重定向-追加 ; <
ps aux | grep mahiru > 1.txt
ps aux | grep root >> 1.txt
grep root < 1.txt
//多管道
ps aux | grep mahiru | wc -l
//内建命令
cd ..
```



1. wc，即word count，计算文件的字数行数等 ；grep，查找与参数对应的字符串
2. 计算出当前进程列表中含firefox的字符串行数，也就是进程数量
3. 不适合。因为echo命令本身不接受输入，所以前两个echo结果不显示，最终结果始终与`echo ccc`相同，不能验证管道正确性
4. 读端读完，一直阻塞，直到有数据输入才继续
5. 写进程收到信号SIGPIPE，通常导致进程异常中止
6. 写端不关闭，读端会一直阻塞
7. 不是，复制的是相同的pipefd数组，即同一根管道的读写端口地址
8. 见下方函数说明
9. 内置命令即shell本身自带的命令/功能，执行的代码在shell代码中；因为子进程exec cd后改变的是子进程的工作路径，而父进程的工作路径不变
10. ps aux和wc -l本身也是进程，执行后会算入进程数
11. 最大长度16，在sched.h中，宏定义TASK_COMM_LEN 
12. sched.h
13. 已删除
14. printk

15.因为read等已经封装，利用glibc库调用，而ps_counter没有，所以要用syscall



![img](file:///C:\Users\99644\AppData\Roaming\Tencent\Users\996449408\QQ\WinTemp\RichOle\_[09O4PDWLAV$IED$G[~35N.png)

ps aux | wc -l 本身是两个进程，同时ps aux输出的第一行不是进程，所以实际进程数 49

get_ps_num本身是个进程，实际进程数49

两者相等



## Note

### 常用函数

- **strtok**

  ```c
  #include<string.h>
  char *strtok(char s[], const char *delim);
  ```

  strtok()用来将字符串分割成一个个片段。参数s指向欲分割的字符串，参数delim则为分割字符串中包含的所有字符。**当strtok()在参数s的字符串中发现参数delim中包含的分割字符时,则会将该字符改为\0 字符**。在第一次调用时，strtok()必需给予参数s字符串，往后的调用则将参数s设置成NULL。每次调用成功则返回指向被分割出片段的指针。

  返回值：

  从s开头开始的一个个被分割的串。当s中的字符查找到末尾时，返回NULL。

  如果查找不到delim中的字符时，返回当前strtok的字符串的指针。

  所有delim中包含的字符都会被滤掉，并将被滤掉的地方设为一处分割的节点。
  
  **注意**：strtok会分割原字符串，为在原串上操作
  
- **fflush**

  ```c
  #include<stdio.h>
  int fflush(FILE *stream);
  ```

  清除读写缓冲区，在需要立即把输出缓冲区的数据进行物理写入时

- 


### 常用系统调用
- **read**——从一个文件描述符中读取内容
  
  ```c
  #include <unistd.h>
  ssize_t read(int fd, void *buf, size_t count);
  ```
  
  read从文件描述符fd指向的文件读取count个字节存放在buf缓冲区中。read从当前
  文件指针指向的位置读取，如果在文件末尾，返回0，表示没有读取字节。
  执行成功返回读取的字节个数，错误情况下返回-1，errno被设置。
  
- **write**——向一个文件描述符中写入内容
  
    ```c
    #include <unistd.h>
    ssize_t write(int fd, const void *buf, size_t count);
    ```
    
    write向文件描述符fd指向的文件写入buf缓冲区指向的count个字节。
    写入的字节数可能会少于count，如存储空间不足或系统调用setrlimit设置了RLIMIT_FSIZE资源限制，或者write被信号中断。
    执行成功返回写入的字节个数，0表示没有写入，否则返回-1，errno被设置。
    
- **open**——打开或创建文件
  
  ```c
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  int open(const char *pathname, int flags);
  int open(const char *pathname, int flags, mode_t mode);
  int creat(const char *pathname, mode_t mode);
  ```
  
  函数说明
  
  参数pathname 指向欲打开的文件路径字符串。下列是参数flags 所能使用的旗标:
  
  O_RDONLY 以只读方式打开文件
  
  O_WRONLY 以只写方式打开文件
  
  O_RDWR 以可读写方式打开文件。
  
  上述三种旗标是互斥的，也就是不可同时使用，但可与下列的旗标利用OR(|)运算符组合。
  
  O_CREAT 若欲打开的文件不存在则自动建立该文件。
  
  O_EXCL 如果O_CREAT 也被设置，此指令会去检查文件是否存在。文件若不存在则建立该文件，否则将导致打开文件错误。此外，若O_CREAT与O_EXCL同时设置，并且欲打开的文件为符号连接，则会打开文件失败。
  
  O_NOCTTY 如果欲打开的文件为终端机设备时，则不会将该终端机当成进程控制终端机。
  
  O_TRUNC 若文件存在并且以可写的方式打开时，此旗标会令文件长度清为0，而原来存于该文件的 资料也会消失。
  
  O_APPEND 当读写文件时会从文件尾开始移动，也就是所写入的数据会以附加的方式加入到文件后面。
  
  O_NONBLOCK 以不可阻断的方式打开文件，也就是无论有无数据读取或等待，都会立即返回进程之中。
  
  O_NDELAY 同O_NONBLOCK。
  
  O_SYNC 以同步的方式打开文件。
  
  O_NOFOLLOW 如果参数pathname 所指的文件为一符号连接，则会令打开文件失败。
  
  O_DIRECTORY 如果参数pathname 所指的文件并非为一目录，则会令打开文件失败。
  
  参数mode 组合
  
  此为Linux2.2以后特有的旗标，以避免一些系统安全问题。参数mode 则有下列数种组合，只有在建立新文件时才会生效，此外真正建文件时的权限会受到umask值所影响，因此该文件权限应该为(mode-umaks)。
  
  S_IRWXU 00700 权限，代表该文件所有者具有可读、可写及可执行的权限。
  
  S_IRUSR 或S_IREAD， 00400权限，代表该文件所有者具有可读取的权限。
  
  S_IWUSR 或S_IWRITE，00200 权限，代表该文件所有者具有可写入的权限。
  
  S_IXUSR 或S_IEXEC， 00100 权限，代表该文件所有者具有可执行的权限。
  
  S_IRWXG 00070权限，代表该文件用户组具有可读、可写及可执行的权限。
  
  S_IRGRP 00040 权限，代表该文件用户组具有可读的权限。
  
  S_IWGRP 00020权限，代表该文件用户组具有可写入的权限。
  
  S_IXGRP 00010 权限，代表该文件用户组具有可执行的权限。
  
  S_IRWXO 00007权限，代表其他用户具有可读、可写及可执行的权限。
  
  S_IROTH 00004 权限，代表其他用户具有可读的权限
  
  S_IWOTH 00002权限，代表其他用户具有可写入的权限。
  
  S_IXOTH 00001 权限，代表其他用户具有可执行的权限。
  
- **pipe**——创建管道
  
  ```c
  #include <unistd.h>
  int pipe(int pipefd[2]);
  ```

  创建一个无向的管道用于进程间通信。数组pipefd返回两个文件描述符指向管道的两端。
  pipefd[0]指向读端，pipefd[1]指向写端。
  执行成功返回0，否则返回-1，errno被设置。
  
- **dup**

  函数原型

  ```c
  #include <unistd.h>
  int dup(int oldfd);
  ```
  dup用来复制参数oldfd所指的文件描述符。当复制成功是，返回最小的尚未被使用过的文件描述符，若有错误则返回-1. 错误代码存入errno中，返回的新文件描述符和参数oldfd指向同一个文件，这两个描述符共享同一个数据结构，共享所有的锁定，读写指针和各项全现或标志位。

- **dup2**

    函数原型

    ```c
     #include <unistd.h>
     int dup2(int oldfd, int newfd);
    ```

    oldfd必须已打开

    dup2与dup区别是dup2可以用参数newfd指定新文件描述符的数值。若参数newfd已经被程序使用，则系统就会将newfd所指的文件关闭，若newfd等于oldfd，则返回newfd,而不关闭newfd所指的文件。dup2所复制的文件描述符与原来的文件描述符共享各种文件状态。共享所有的锁定，读写位置和各项权限或flags等.

    返回值：若成功调用返回新的文件描述符，反之返回-1
    
    **注**：`dup2(oldfd, newfd)`等效于  `close(oldfd)；fcntl(oldfd, F_DUPFD, newfd)；`
    在shell的重定向功能中，(输入重定向”<”和输出重定向”>”)就是通过调用dup或dup2函数对标准输入和标准输出的操作来实现的。
    
    eg:
    
    ```c
    int fd = open("test.txt", O_WRONLY ,0666);
    dup2(fd,STDOUT_FILENO);//从而printf会打印到test.txt，即使在这之前printf (本质上缓冲区还未写入)
    ```
    
- **getpid**——获取进程识别码

    函数原型：

    ```c
    #include<unistd.h>
    pid_t getpid(void);			//int _getpid(void);
    ```

    getpid函数用来取得目前进程的进程ID，许多程序利用取到的此值来建立[临时文件](https://baike.baidu.com/item/临时文件)，以避免临时文件相同带来的问题。

- **getcwd**——获取当前工作目录

    函数原型：

    ```c
    #include<unistd.h>
    char *getcwd( char *buffer, int maxlen );
    ```

    getcwd()会将当前工作目录的绝对路径复制到参数buffer所指的内存空间中,参数maxlen为buffer的空间大小。

    成功则返回当前工作目录，失败返回 **FALSE**。

    注：windows下为direct.h

- **fork**——创建子进程

    ```c
    #include<unistd.h>
    #include<sys/types.h>
    pid_t fork( void);
    ```

    父进程返回子进程id，子进程返回0，失败返回-1

    注：子进程是父进程的副本，它将获得父进程数据空间、堆、栈等资源的副本。注意，子进程持有的是上述存储空间的“副本”，这意味着父子进程间不共享这些存储空间。

    调用fork之后，数据、堆、栈有两份，代码仍然为一份但是这个[代码段](https://baike.baidu.com/item/代码段)成为两个进程的共享代码段都从fork函数中返回，箭头表示各自的执行处。当父子进程有一个想要修改数据或者堆栈时，两个进程真正分裂。

- **vfork**——创建子进程，目的在于调用exec

    ```c
    #include<unistd.h>
    pid_t vfork( void);
    ```

    与fork()区别：

    1. fork():子进程拷贝父进程的数据段，代码段. vfork():子进程与父进程共享数据段.
    
    2. fork():父子进程的执行次序不确定.
        vfork():保证子进程先运行，在调用exec或exit之前与父进程数据是共享的,在它调用exec
        或exit之后父进程才可能被调度运行。
    
    3. vfork()保证**子进程先运行**，在她调用exec或_exit之后父进程才可能被调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。
    4. 当vfork()需要改变共享数据段中变量的值，则拷贝父进程。


- **clone**

  ```c
  #include <sched.h>
  int clone(int (*fn)(void *), void *child_stack, int flags, void *arg);
  ```

  <img src="C:\Users\99644\AppData\Roaming\Typora\typora-user-images\image-20220404100130137.png" alt="image-20220404100130137" style="zoom:50%;" />

- **wait**

  ```c
  #include <sys/types.h>
  #include <wait.h>
  int wait(int *status)
  ```

  父进程一旦调用了wait就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。

- **exec**

  exec函数族共有6种不同形式的函数。这6个函数可以划分为两组：

  (1)execl、execle和execlp。

  (2)execv、execve和execvp。

  这两组函数的不同在于exec后的第一个字符，第一组是l，在此称，为execl系列；第二组是v，在此称为execv系列。这里的l是list(列表)的意思，表示execl系列函数需要将每个[命令行参数](https://baike.baidu.com/item/命令行参数/3206082)作为函数的参数进行传递；而v是vector(矢量)的意思，表示execv系列函数将所有函数包装到一个矢量数组中传递即可。

  exec函数的原型如下：

  ```c
  int execl(const char * path，const char * arg，…)；
  int execle(const char * path，const char * arg，char * const envp[])；
  int execlp(const char * file，const char * arg，…)；
  int execv(const char * path，char * const argv[])；
  int execve(const char * path，char * const argv[]，char * const envp[])；
  int execvp(const char * file，char * const argv[])；
  //execute
  ```

  参数说明：

  path：要执行的程序路径。可以是绝对路径或者是相对路径。在execv、execve、execl和execle这4个函数中，使用带路径名的文件名作为参数。

  file：要执行的程序名称。如果该参数中包含“/”字符，则视为路径名直接执行；否则视为单独的文件名，系统将根据PATH环境变量指定的路径顺序搜索指定的文件。

  argv：命令行参数的矢量数组。

  envp：带有该参数的exec函数可以在调用时指定一个环境变量数组。其他不带该参数的exec函数则使用调用进程的环境变量。

  arg：程序的第0个参数，即程序名自身。相当于argv[O]。

  …：命令行参数列表。调用相应程序时有多少命令行参数，就需要有多少个输入参数项。注意：在使用此类函数时，在所有命令行参数的最后应该增加一个空的参数项(NULL)，表明命令行参数结束。

  返回值：-1表明调用exec失败，无返回表明调用成功。

- **mkdir**

  ```c
  #include <sys/stat.h>
  #include <sys/types.h>
  int mkdir(const char *pathname, mode_t mode);//make directory
  ```

  mkdir()函数以mode方式创建一个以参数pathname命名的目录，mode定义新创建目录的权限。

  若目录创建成功，则返回0；否则返回-1，并将错误记录到全局变量errno中。

  mode方式：可多个权限相或，如0755表示S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

  <img src="C:\Users\99644\AppData\Roaming\Typora\typora-user-images\image-20220404102129324.png" alt="image-20220404102129324" style="zoom:50%;" />

- **rmdir**

  ```c
  #include<unistd.h>
  int rmdir( const char *dirname );//remove directory
  ```

  删除一个目录，若成功则返回0，否则返回-1

- **chdir**——更改当前工作目录

  ```c
  #include<unistd.h>
  int chdir(const char *path);
  ```

  Path 为目标目录，可以是绝对目录或相对目录。

  成功返回0，失败返回-1

- **exit**

  ```c
  #include<stdlib.h>
  void exit(int status);		//一般正常传0，出错传1
  ```

- **kill**——给指定进程发送指定信号

  ```c
  #include<sys/types.h>
  #include<signal.h>
  int kill(pid_t pid,int sig);
  ```

  **参数说明**

  pid：接收信号的进程（组）的进程号

  pid>0：发送给进程号为pid的进程

  pid=0：发送给当前进程所属进程组里的所有进程

  pid=-1：发送给除1号进程和自身以外的所有进程

  pid<-1：发送给属于进程组-pid的所有进程

  signo：发送的信号值

  Signo = 0：不发送信号,可用于检查目标进程是否存在，以及当前进程是否具有向目标进程发送信号的权限（root权限)的进程可以向任何进程发送信号，非root权限的进程只能向属于同一个session或者同一个用户的进程发送信号）。

  信号值 1 - 31叫做不可靠信号 [3] 不支持排队, 信号可能会丢失, 也叫做非实时信号。

  34 - 64 叫做可靠信号，支持排队, 信号不会丢失, 也叫做实时信号

  比较常用的一些信号值

  SIGHUP 1 A 终端挂起或者控制进程终止

  SIGINT 2 A 键盘中断（如break键被按下）

  SIGQUIT 3 C 键盘的退出键被按下

  SIGILL 4 C 非法指令

  SIGABRT 6 C 由abort(3)发出的退出指令

  SIGFPE 8 C 浮点异常

  **SIGKILL 9 AEF Kill信号**

  SIGSEGV 11 C 无效的内存引用

  SIGPIPE 13 A 管道破裂: 写一个没有读端口的管道

  SIGALRM 14 A 由alarm(2)发出的信号

  SIGTERM 15 A 终止信号

  SIGUSR1 30,10,16 A 用户自定义信号1

  SIGUSR2 31,12,17 A 用户自定义信号2

  SIGCHLD 20,17,18 B 子进程结束信号

  SIGCONT 19,18,25 进程继续（曾被停止的进程）

  SIGSTOP 17,19,23 DEF 终止进程

  SIGTSTP 18,20,24 D 控制终端（tty）上按下停止键

  SIGTTIN 21,21,26 D 后台进程企图从控制终端读

  SIGTTOU 22,22,27 D 后台进程企图从控制终端写

- **shutdown**

  ```c
  #include<sys/socket.h>
  int shutdown(int sockfd,int how);
  ```

  linux下成功则返回0，错误返回-1，错误码errno：EBADF表示sockfd不是一个有效描述符；ENOTCONN表示sockfd未连接；ENOTSOCK表示sockfd是一个描述符而不是socket描述符。

  how的方式有三种分别是

  SHUT_RD（0）：关闭sockfd上的读功能，此选项将不允许sockfd进行读操作。

  SHUT_WR（1）：关闭sockfd的写功能，此选项将不允许sockfd进行写操作。

  SHUT_RDWR（2）：关闭sockfd的读写功能。

- **chmod**

  ```c
  #include <sys/types.h> 
  #include <sys/stat.h>
  int chmod(const char * path, mode_t mode);
  ```

  改变文件的读写许可设置，如果改变成功返回0，否则返回-1

  参数 mode 有下列数种组合：

  1、S_ISUID 04000 文件的 (set user-id on execution)位

  2、S_ISGID 02000 文件的 (set group-id on execution)位

  3、S_ISVTX 01000 文件的sticky 位

  4、S_IRUSR (S_IREAD) 00400 文件[所有者](https://baike.baidu.com/item/所有者)具可读取权限

  5、S_IWUSR (S_IWRITE)00200 文件所有者具可写入权限

  6、S_IXUSR (S_IEXEC) 00100 文件所有者具可执行权限

  7、S_IRGRP 00040 用户组具可读取权限

  8、S_IWGRP 00020 用户组具可写入权限

  9、S_IXGRP 00010 用户组具可执行权限

  10、S_IROTH 00004 其他用户具可读取权限

  11、S_IWOTH 00002 其他用户具可写入权限

  12、S_IXOTH 00001 其他用户具可执行权限

  注：只有该文件的[所有者](https://baike.baidu.com/item/所有者)或有效用户识别码为0，才可以修改该文件权限。

  基于系统安全，如果欲将数据写入一执行文件，而该执行文件具有S_ISUID 或S_ISGID 权限，则这两个位会被清除。如果一目录具有S_ISUID 位权限，表示在此目录下只有该文件的所有者或root 可以删除该文件。

  返回值：权限改变成功返回0, 失败返回-1, 错误原因存于errno.

- **chown**

  ```c
  int chown(const char * path, uid_t owner, gid_t group)
  ```

  chown()会将参数path 指定文件的所有者变更为参数owner 代表的用户，而将该文件的组变更为参数group 组。如果参数owner 或group 为-1，对应的所有者或组不会有所改变。root 与文件所有者皆可改变文件组，但所有者必须是参数group 组的成员。当root 用chown()改变文件所有者或组时，该文件若具有S_ISUID或S_ISGID 权限，则会清除此权限位，此外如果具有S_ISGID 权限但不具S_IXGRP 位，则该文件会被强制锁定,文件模式会保留。
  
  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <sys/wait.h>
  #include <sys/types.h>
  #include <fcntl.h>
  #include <errno.h>
  
  #define MAX_CMDLINE_LENGTH  1024    /* max cmdline length in a line*/
  #define MAX_BUF_SIZE        4096    /* max buffer size */
  #define MAX_CMD_ARG_NUM     32      /* max number of single command args */
  #define WRITE_END 1     // pipe write end
  #define READ_END 0      // pipe read end
  #define MAX_CMD 32
  
  /* 
   * 需要大家完成的代码已经用注释`TODO:`标记
   * 可以编辑器搜索找到
   * 使用支持TODO高亮编辑器（如vscode装TODO highlight插件）的同学可以轻松找到要添加内容的地方。
   */
  
  /*  
      int split_string(char* string, char *sep, char** string_clips);
  
      基于分隔符sep对于string做分割，并去掉头尾的空格
  
      arguments:      char* string, 输入, 待分割的字符串 
                      char* sep, 输入, 分割符
                      char** string_clips, 输出, 分割好的字符串数组
  
      return:   分割的段数 
  */
  
  int split_string(char* string, char *sep, char** string_clips) {
      
      //char string_dup[MAX_BUF_SIZE];
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
  
  /*
      执行内置命令
      arguments:
          argc: 输入，命令的参数个数
          argv: 输入，依次代表每个参数，注意第一个参数就是要执行的命令，
          若执行"ls a b c"命令，则argc=4, argv={"ls", "a", "b", "c"}
          fd: 输出，命令输入和输出的文件描述符 (Deprecated)
      return:
          int, 若执行成功返回0，否则返回值非零
  */
  int exec_builtin(int argc, char**argv, int *fd) {
      if(argc == 0) {
          return 0;
      }
      /* TO DO: 添加和实现内置指令 */
      int i = 0;
      while(i < argc){
          if (strcmp(argv[i], "cd") == 0) {
              if(chdir(argv[i + 1]) == -1){
                  printf("Invalid Directory\n");
              }
              return -1;
          } else if (strcmp(argv[0], "exit") == 0){
              exit(0);
          }
          i++;
      }
      /*
      if (strcmp(argv[0], "cd") == 0) {
          if(chdir(argv[1]) == -1){
              printf("Invalid Directory\n");
              exit(-1);
          }
          return 0;
      } else if (strcmp(argv[0], "exit") == 0){
         exit(0);
      } else {
          // 不是内置指令时
          return -1;
      }*/
      // 不是内置指令时
      return -1;
  }
  
  /*
      从argv中删除重定向符和随后的参数，并打开对应的文件，将文件描述符放在fd数组中。
      运行后，fd[0]读端的文件描述符，fd[1]是写端的文件描述符
      arguments:
          argc: 输入，命令的参数个数
          argv: 输入，依次代表每个参数，注意第一个参数就是要执行的命令，
          若执行"ls a b c"命令，则argc=4, argv={"ls", "a", "b", "c"}
          fd: 输出，命令输入和输出使用的文件描述符
      return:
          int, 返回处理过重定向后命令的参数个数
  */
  
  int process_redirect(int argc, char** argv, int *fd) {
      /* 默认输入输出到命令行，即输入STDIN_FILENO，输出STDOUT_FILENO */
      fd[READ_END] = STDIN_FILENO;
      fd[WRITE_END] = STDOUT_FILENO;
      int i = 0, j = 0;
      while(i < argc) {
          int tfd;
          if(strcmp(argv[i], ">") == 0) {
              //TO DO: 打开输出文件从头写入
              tfd = open(argv[i+1],O_WRONLY | O_CREAT , S_IRWXU);
              if(tfd < 0) {
                  printf("open '%s' error: %s\n", argv[i+1], strerror(errno));
              } else {
                  //TO DO: 输出重定向
                  fd[WRITE_END] = tfd;
              }
              i += 2;
          } else if(strcmp(argv[i], ">>") == 0) {
              //TO DO: 打开输出文件追加写入
              tfd = open(argv[i+1],O_WRONLY | O_APPEND | O_CREAT , S_IRWXU);
              if(tfd < 0) {
                  printf("open '%s' error: %s\n", argv[i+1], strerror(errno));
              } else {
                  //TO DO:输出重定向
                  fd[WRITE_END] = tfd;
              }
              i += 2;
          } else if(strcmp(argv[i], "<") == 0) {
              //TO DO: 读输入文件
              tfd = open(argv[i + 1], O_RDONLY , S_IRWXU);
              if(tfd < 0) {
                  printf("open '%s' error: %s\n", argv[i+1], strerror(errno));
              } else {
                  //TO DO:输出重定向
                  fd[READ_END] = tfd;
              }
              i += 2;
          } else {
              argv[j++] = argv[i++];
          }
      }
      argv[j] = NULL;
      return j;   // 新的argc
  }
  
  
  
  /*
      在本进程中执行，且执行完毕后结束进程。
      arguments:
          argc: 命令的参数个数
          argv: 依次代表每个参数，注意第一个参数就是要执行的命令，
          若执行"ls a b c"命令，则argc=4, argv={"ls", "a", "b", "c"}
      return:
          int, 若执行成功则不会返回（进程直接结束），否则返回非零
  */
  int execute(int argc, char** argv) {
      int fd[2];
      // 默认输入输出到命令行，即输入STDIN_FILENO，输出STDOUT_FILENO 
      fd[READ_END] = STDIN_FILENO;
      fd[WRITE_END] = STDOUT_FILENO;
      // 处理重定向符，如果不做本部分内容，请注释掉process_redirect的调用
      argc = process_redirect(argc, argv, fd);
      if(exec_builtin(argc, argv, fd) == 0) {
          exit(0);
      }
      // 将标准输入输出STDIN_FILENO和STDOUT_FILENO修改为fd对应的文件
      dup2(fd[READ_END], STDIN_FILENO);
      dup2(fd[WRITE_END], STDOUT_FILENO);
      /* TO DO:运行命令与结束 */
      argv[argc] = NULL;
      execlp(argv[0],argv[0],argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],\
      argv[10],argv[11],argv[12],argv[13],argv[14],argv[16],argv[17],argv[18],argv[19],argv[20],argv[21],argv[22],\
      argv[23],argv[24],argv[25],argv[26],argv[27],argv[28],argv[29],argv[30],argv[31],NULL);
      return 0;
  }
  
  int main() {
      /* 输入的命令行 */
      char cmdline[MAX_CMDLINE_LENGTH] = {0};
      char cmd_clip[MAX_CMDLINE_LENGTH] = {0};
      char CWD[MAX_BUF_SIZE] = {0};           //current work directory
      char *commands[128];
      char *cmdline_p;                        //pointer to cmd clip (sep)
      int cmd_count_s;                        //分隔符(sep)分隔的指令数
      int cmd_count_p;                        //管道符(pipe)分隔的指令数
      while (1) {
          /* TO DO: 增加打印当前目录，格式类似"shell:/home/oslab ->"，你需要改下面的printf */
          getcwd(CWD,MAX_BUF_SIZE);
          if(strlen(CWD) == 0){
              printf("Work Dirctory is too long to print\n");
              return -2;
          }
          printf("shell:%s-> ",CWD);
          fflush(stdout);
          memset(cmdline,0,MAX_CMDLINE_LENGTH);
          fgets(cmdline, 256, stdin);
          strtok(cmdline, "\n");
  
          /* TO DO: 基于";"的多命令执行，请自行选择位置添加 */
          char *cmds[MAX_CMD] = {0};
          cmd_count_s = split_string(cmdline,";",cmds);     //被;分隔的命令条数
          cmdline_p = cmds[0];
          for(int k = 0; k < cmd_count_s; cmdline_p = cmds[++k])
          {
              /* 由管道操作符'|'分割的命令行各个部分，每个部分是一条命令 */
              /* 拆解命令行 */
              cmd_count_p = split_string(cmdline_p, "|", commands);
              
              if(cmd_count_p == 0) {
                  continue;
              } else if(cmd_count_p == 1) {     // 没有管道的单一命令
                  char *argv[MAX_CMD_ARG_NUM];
                  int argc;
                  int fd[2];
                  /* TO DO:处理参数，分出命令名和参数*/
                  argc = split_string(cmdline_p," ",argv);
  
                  /* 在没有管道时，内建命令直接在主进程中完成，外部命令通过创建子进程完成 */
                  if(exec_builtin(argc, argv, fd) == 0) {
                      continue;
                  }
                  /* TO DO:创建子进程，运行命令，等待命令运行结束*/
                  pid_t pid = fork();
                  if(pid == 0){
                      int fd[2];
                      fd[READ_END] = STDIN_FILENO;
                      fd[WRITE_END] = STDOUT_FILENO;
                      argc = process_redirect(argc, argv, fd);
                      if(exec_builtin(argc, argv, fd) == 0) {
                          exit(0);
                      }
                      dup2(fd[READ_END], STDIN_FILENO);
                      dup2(fd[WRITE_END], STDOUT_FILENO);
                      execlp(argv[0],argv[0],argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],\
                      argv[10],argv[11],argv[12],argv[13],argv[14],argv[16],argv[17],argv[18],argv[19],argv[20],argv[21],argv[22],\
                      argv[23],argv[24],argv[25],argv[26],argv[27],argv[28],argv[29],argv[30],argv[31],NULL);
                  }else wait(NULL);
  
              } else if(cmd_count_p == 2) {     // 两个命令间的管道
                  int pipefd[2];
                  int ret = pipe(pipefd);
                  if(ret < 0) {
                      printf("pipe error!\n");
                      continue;
                  }
                  // 子进程1
                  int pid = fork();
                  if(pid == 0) {  
                      /*TO DO:子进程1 将标准输出重定向到管道，注意这里数组的下标被挖空了要补全*/
                      close(pipefd[READ_END]);
                      dup2(pipefd[WRITE_END], STDOUT_FILENO);
                      close(pipefd[WRITE_END]);
                      /* 
                          在使用管道时，为了可以并发运行，所以内建命令也在子进程中运行
                          因此我们用了一个封装好的execute函数
                      */
                      char *argv[MAX_CMD_ARG_NUM];
  
                      int argc = split_string(commands[0], " ", argv);
                      execute(argc, argv);
                      exit(255);
                      
                  }
                  // 因为在shell的设计中，管道是并发执行的，所以我们不在每个子进程结束后才运行下一个
                  // 而是直接创建下一个子进程
                  // 子进程2
                  pid = fork();
                  if(pid == 0) {  
                      /* TO DO:子进程2 将标准输入重定向到管道，注意这里数组的下标被挖空了要补全 */
                      close(pipefd[WRITE_END]);
                      dup2(pipefd[READ_END], STDIN_FILENO);
                      close(pipefd[READ_END]);
                      char *argv[MAX_CMD_ARG_NUM];
                      /* TO DO:处理参数，分出命令名和参数，并使用execute运行
                      * 在使用管道时，为了可以并发运行，所以内建命令也在子进程中运行
                      * 因此我们用了一个封装好的execute函数
                      *
                      * 
                      */
                      int argc = split_string(commands[1], " ", argv);
                      execute(argc, argv);
                      exit(255);
                  }
                  close(pipefd[WRITE_END]);
                  close(pipefd[READ_END]);
                  
                  
                  while (wait(NULL) > 0);
              } else {    // 选做：三个以上的命令
                  int read_fd;    // 上一个管道的读端口（出口）
                  for(int i=0; i<cmd_count_p; i++) {
                      int pipefd[2];
                      /* TO DO:创建管道，n条命令只需要n-1个管道，所以有一次循环中是不用创建管道的*/
                      int ret ;
                      if(i < cmd_count_p - 1){
                          ret = pipe(pipefd);
                          if(ret < 0) {
                              printf("pipe error!\n");
                              continue;
                          }
                      } else ret = 1;
                      int pid = fork();
                      if(pid == 0) {
                          /* TODO:除了最后一条命令外，都将标准输出重定向到当前管道入口*/
                          if(ret != 1){
                              dup2(pipefd[WRITE_END], STDOUT_FILENO);
                          }
                          close(pipefd[WRITE_END]);
                          /* TODO:除了第一条命令外，都将标准输入重定向到上一个管道出口 */
                          if(i){
                              dup2(read_fd,STDIN_FILENO);
                          }
                          /* TODO:处理参数，分出命令名和参数，并使用execute运行
                          * 在使用管道时，为了可以并发运行，所以内建命令也在子进程中运行
                          * 因此我们用了一个封装好的execute函数
                          * 
                          * 
                          */
                          char *argv[MAX_CMD_ARG_NUM] = {0};
                          int argc = split_string(commands[i], " ", argv);
                          execute(argc, argv);
                          exit(255);
                      }
                      if(i)close(read_fd);
                      if(i < cmd_count_p - 1){
                          read_fd = pipefd[READ_END];
                      }
                      close(pipefd[WRITE_END]);
                      /* 父进程除了第一条命令，都需要关闭当前命令用完的上一个管道读端口 
                      * 父进程除了最后一条命令，都需要保存当前命令的管道读端口 
                      * 记得关闭父进程没用的管道写端口
                      * 
                      */
                      // 因为在shell的设计中，管道是并发执行的，所以我们不在每个子进程结束后才运行下一个
                      // 而是直接创建下一个子进程
                  }
                  // TODO:等待所有子进程结束
                  while (wait(NULL) > 0);
              }
          }//多命令执行
  
      }
  }
  
  ```
  
  
