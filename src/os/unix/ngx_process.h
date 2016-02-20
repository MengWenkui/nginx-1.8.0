
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PROCESS_H_INCLUDED_
#define _NGX_PROCESS_H_INCLUDED_


#include <ngx_setaffinity.h>
#include <ngx_setproctitle.h>


typedef pid_t       ngx_pid_t;

#define NGX_INVALID_PID  -1

typedef void (*ngx_spawn_proc_pt) (ngx_cycle_t *cycle, void *data);

/* master���̶�worker���̵���Ϣά�� */
typedef struct {
    ngx_pid_t           pid;
    int                 status;
    ngx_socket_t        channel[2];  /* worker��master����֮��ͨ��socketpairfd  */
	
    ngx_spawn_proc_pt   proc;  /* */
    void               *data;
    char               *name;

    unsigned            respawn:1;
    unsigned            just_spawn:1;
    unsigned            detached:1;
    unsigned            exiting:1;
    unsigned            exited:1;
} ngx_process_t;


typedef struct {
    char         *path;
    char         *name;
    char *const  *argv;
    char *const  *envp;
} ngx_exec_ctx_t;


#define NGX_MAX_PROCESSES         1024

/* ���������͵�����  */
 
/* 
NGX_PROCESS_XXX ������ʾ�����ӽ��̵�����. 
 
NGX_PROCESS_NORESPAWN: 
    �ӽ����˳�ʱ,�����̲����ٴδ���, �ñ�����ڴ��� "cache loader process". 

NGX_PROCESS_JUST_SPAWN:  
    �� nginx -s reload ʱ, �������δ���ص� proxy_cache_path, 
    ����Ҫ�ٴδ��� "cache loader process"����,���� NGX_PROCESS_JUST_SPAWN������������Ǻ�,
    ��ֹ "master�����ϵ�worker����,�ϵ�cache manager����,�ϵ�cache loader����(�������)����NGX_CMD_QUIT��SIGQUIT" ʱ,
    ����Ϊ����������ϵ�cache loader����. 

NGX_PROCESS_RESPAWN:  
    �ӽ����쳣�˳�ʱ,master�����´�����, �統worker��cache manager�쳣�˳�ʱ,�����̻����´�����. 

NGX_PROCESS_JUST_RESPAWN:  
    �� nginx -s reload ʱ, master�����ϵ�worker����,�ϵ�cache manager����,
    �ϵ�cache loader����(�������)���� ngx_write_channel(NGX_CMD_QUIT)(���ʧ������SIGQUIT�ź�); 
    �ñ��������ǽ�����������Щ���´������ӽ���;�����ľ����ϵ��ӽ���. 

NGX_PROCESS_DETACHED: �ȴ����滻 

����ο�:http://blog.csdn.net/gsnumen/article/details/7979484?reload

 */  


#define NGX_PROCESS_NORESPAWN     -1
#define NGX_PROCESS_JUST_SPAWN    -2
#define NGX_PROCESS_RESPAWN       -3
#define NGX_PROCESS_JUST_RESPAWN  -4
#define NGX_PROCESS_DETACHED      -5


#define ngx_getpid   getpid

#ifndef ngx_log_pid
#define ngx_log_pid  ngx_pid
#endif


ngx_pid_t ngx_spawn_process(ngx_cycle_t *cycle,
    ngx_spawn_proc_pt proc, void *data, char *name, ngx_int_t respawn);
ngx_pid_t ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx);
ngx_int_t ngx_init_signals(ngx_log_t *log);
void ngx_debug_point(void);


#if (NGX_HAVE_SCHED_YIELD)
#define ngx_sched_yield()  sched_yield()
#else
#define ngx_sched_yield()  usleep(1)
#endif


extern int            ngx_argc;
extern char         **ngx_argv;
extern char         **ngx_os_argv;

extern ngx_pid_t      ngx_pid;
extern ngx_socket_t   ngx_channel;
extern ngx_int_t      ngx_process_slot;
extern ngx_int_t      ngx_last_process;
extern ngx_process_t  ngx_processes[NGX_MAX_PROCESSES];


#endif /* _NGX_PROCESS_H_INCLUDED_ */
