#include "codexion.h"

static long	function(const char *nptr, int i, int sign, long *nbr)
{
	long	temp;

	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		temp = *nbr;
		*nbr = *nbr * 10 + (nptr[i] - '0');
		if (*nbr > 2147483647 && sign == 1)
			return (2147483648);
		if (*nbr > 2147483648 && sign == -1)
			return (2147483649);
		if (temp > *nbr)
		{
			if (sign == -1)
				return (0);
			return (-1);
		}
		i++;
	}
	return (*nbr * sign);
}

long	ft_atoi(const char *nptr)
{
	long	nbr;
	size_t	i;
	int		sign;

	nbr = 0;
	i = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	return (function(nptr, i, sign, &nbr));
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void set_dongles(coder_v *coder)
{
    dongle_v    *first;
    dongle_v    *second;
    t_waiter    *w;
    long        elapsed;


    if (coder->arguments->number_of_coders == 1)
        return;
    if (coder->id % 2 == 0)
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
    else
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }

    pthread_mutex_lock(&first->dongle_mutex);
    while (!first->available
        || first->heap->size == 0
        || first->heap->waiters[0]->coder != coder)
        pthread_cond_wait(&first->cond, &first->dongle_mutex);
    w = heap_extract_min(first->heap);
    free(w);
    first->available = 0;
    if (first->realesed_time != 0)
    {
        elapsed = (long)get_time_ms() - first->realesed_time;
        if (elapsed < coder->arguments->dongle_cooldown)
            precise_sleep(coder->arguments->dongle_cooldown - elapsed);
    }
    pthread_mutex_unlock(&first->dongle_mutex);

    pthread_mutex_lock(&second->dongle_mutex);
    while (!second->available
        || second->heap->size == 0
        || second->heap->waiters[0]->coder != coder)
        pthread_cond_wait(&second->cond, &second->dongle_mutex);
    w = heap_extract_min(second->heap);
    free(w);
    second->available = 0;
    if (second->realesed_time != 0)
    {
        elapsed = (long)get_time_ms() - second->realesed_time;
        if (elapsed < coder->arguments->dongle_cooldown)
            precise_sleep(coder->arguments->dongle_cooldown - elapsed);
    }
    pthread_mutex_unlock(&second->dongle_mutex);
}

void release_dongles(coder_v *coder)
{
    pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
    coder->right_dongle->available = 1;
    coder->right_dongle->realesed_time = get_time_ms();
    wake_next(coder->right_dongle);
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);


    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
    coder->left_dongle->available = 1;
    coder->left_dongle->realesed_time = get_time_ms();
    wake_next(coder->left_dongle);
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
}