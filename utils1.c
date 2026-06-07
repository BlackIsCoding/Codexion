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
    if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
        pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
		//precise_sleep(10);
		if (coder->left_dongle->realesed_time || coder->right_dongle->realesed_time)
			precise_sleep(coder->arguments->dongle_cooldown);
	}
    else
    {
        pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
        pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
		if (coder->left_dongle->realesed_time || coder->right_dongle->realesed_time)
			precise_sleep(coder->arguments->dongle_cooldown);
    }
}

void release_dongles(coder_v *coder)
{
    coder->right_dongle->realesed_time = get_time_ms();
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
    coder->left_dongle->realesed_time = get_time_ms();
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
}


