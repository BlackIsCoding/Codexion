# ifndef CODEXION_H
# define CODEXION_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct coder coder_v;
typedef struct s_waiter
{
    coder_v *coder;
    long    deadline;
} t_waiter;


typedef struct s_heap
{
    t_waiter    **waiters;
    int         size;
    int         capacity;
} t_heap;

typedef struct dongle
{
    pthread_mutex_t dongle_mutex;
    int available;
    long realesed_time;
    t_heap *heap;
    pthread_cond_t cond;
} dongle_v;

typedef struct argument arg_v;
typedef struct coder
{
    int id;
    int compile_count;
    pthread_t thread;
    pthread_mutex_t coder_mutex;
    dongle_v *right_dongle;
    dongle_v *left_dongle;
    arg_v *arguments;
    long last_compile_time;
} coder_v;

typedef struct argument
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
    coder_v         *coders;
    dongle_v *dongles;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
    pthread_t monitor;
    int stop;
    size_t start;
    int even;
    long            arrival_counter;
    pthread_mutex_t fifo_mutex;
} arg_v;

long	    ft_atoi(const char *nptr);
int	        ft_strcmp(const char *s1, const char *s2);
void*       simulation(void *arg);
void        setup(arg_v *argument);
void        set_dongles(coder_v *coder);
void        release_dongles(coder_v *coder);
size_t      get_time_ms();
void        precise_sleep(long time_ms);
t_heap  *heap_init(int capacity);
t_waiter    *new_waiter(coder_v *coder, long deadline);
void    sift_up(t_heap *heap, int i);
void    sift_down(t_heap *heap, int i);
void    heap_insert(t_heap *heap, coder_v *coder, long deadline);
t_waiter    *heap_extract_min(t_heap *heap);
void    heap_free(t_heap *heap);
void    register_in_heaps(coder_v *coder);
void    wake_next(dongle_v *dongle);
# endif