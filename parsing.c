#include <stdio.h>
#include <unistd.h>
#include "codexion.h"



void print_string(char *str)
{
    while (*str)
    {
        write(1, str, 1);
        str = str + 1;
    }
}

static int	check_letters(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ')
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!(s[i] >= '0' && s[i] <= '9'))
		return (0);
	while (s[i])
	{
		if ((s[i] == '+' || s[i] == '-') && (s[i + 1] >= '0'
				&& s[i + 1] <= '9') && s[i - 1] == ' ')
		{
			i++;
			continue ;
		}
		if (!(s[i] >= '0' && s[i] <= '9') && s[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int check_args(int ac , char **av, arg_v *argument)
{
    size_t i;
    long number;
    int fifo, edf;
    size_t arc;

    arc = (size_t)ac;
    fifo = -1;
    edf = -1;

    if (arc != 9)
    {
        printf("PLEASE ENTER ALL THE MANDATORY ARGUMENTS !\n");
        return 1;
    }
    i = 1;
    while (i < arc)
    {
        if (i != arc - 1)
        {
            number = ft_atoi(av[i]);
            if (number > 2147483647 || number < 0)
            {
                print_string("SECOND CHECK: INVALID ARGUMENT DETECTED !\n");
                return 1;
            }

        }
        else
        {
            fifo = ft_strcmp(av[i], "fifo");
            edf = ft_strcmp(av[i], "edf");
            if (fifo == 0)
                argument->scheduler = "fifo";
            else if (edf == 0)
                argument->scheduler = "edf";
            else
            {
                print_string("INVALID SCHEDULER VALUE !\n");
                return 1;
            }
        }
        i++;
    }
    return 0;
}

int main(int ac, char **av)
{
    arg_v arguments;
    size_t i;
    int char_detected = 0;

    if (ac != 9)
        exit(1);

    i = 1;
    while(i < ac - 1)
    {
        if (!check_letters(av[i]))
        {
            char_detected = 1;
            break;
        }
        i++;
    }
    if (char_detected)
    {
        print_string("FIRST CHECK: INVALID ARGUMENT DETECTED !\n");
        exit(1);
    }
    if (check_args(ac, av, &arguments) == 1)
        exit(1);
    else
    {
        arguments.number_of_coders = ft_atoi(av[1]);

        if (arguments.number_of_coders < 2)
        {
            print_string("THERE MUST BE AT LEAST 2 CODERS\n");
            exit(1);
        }
        arguments.time_to_burnout = ft_atoi(av[2]);
        arguments.time_to_compile = ft_atoi(av[3]);
        arguments.time_to_debug = ft_atoi(av[4]);
        arguments.time_to_refactor = ft_atoi(av[5]);
        arguments.number_of_compiles_required = ft_atoi(av[6]);
        if (arguments.number_of_compiles_required == 0)
        {
            print_string("NUMBER OF COMPILATIONS MUST BE >0\n");
            exit(1);
        }
        arguments.dongle_cooldown = ft_atoi(av[7]);
        arguments.stop = 0;
        setup(&arguments);
    }
}