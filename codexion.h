# ifndef CODEXION_H
# define CODEXION_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>


typedef struct dongle
{
    pthread_mutex_t dongle_mutex;
    int available;
    long realesed_time;
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
    pthread_cond_t cond;
    pthread_mutex_t print_mutex;
    int stop;
} arg_v;

long	    ft_atoi(const char *nptr);
int	        ft_strcmp(const char *s1, const char *s2);
void*       simulation(void *arg);
void        setup(arg_v *argument);
void        set_dongles(coder_v *coder);
void        release_dongles(coder_v *coder);
void		print_dici_inti(const char *s, int *i, va_list args, int *ret);
void		print_str(const char *s, int *i, va_list args, int *ret);
void		print_hex(const char *s, int *i, va_list args, int *ret);
void		hexa_lower(int *ret, va_list args);
void		hexa_upper(int *ret, va_list args);
void		print_percent_unsigned(const char *s, int *i, int *ret);
size_t		ft_strlen(const char *s);
int			ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
int			ft_putnbr_fd(int n, int fd);
void		putnbrr_uns(unsigned int a, int *ret);
int			ft_printf(const char *s, ...);
void		print_char(const char *s, int *i, va_list args, int *ret);
void		print_address(va_list args, int *ret);

# endif