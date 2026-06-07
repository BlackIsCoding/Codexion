#include "codexion.h"

size_t get_time_ms()
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void precise_sleep(long time_ms)
{
    long start;

    start = get_time_ms();
    while ((get_time_ms() - start) < time_ms)
        usleep(10);
}

void *monitor(void *arg)
{
    arg_v *monitor = (arg_v *)arg;
    size_t i = 0;
    int flag = 1;
    while (!monitor->stop)
    {
        i = 0;
        flag = 1;
        while(i < (*monitor).number_of_coders)
        {
            if ((*monitor).coders[i].compile_count < (*monitor).number_of_compiles_required)
                flag = 0;
            pthread_mutex_lock(&monitor->stop_mutex);
            if (monitor->stop)
            {
                pthread_mutex_unlock(&monitor->stop_mutex);
                break;
            }
            if (get_time_ms() - monitor->coders[i].last_compile_time >= monitor->time_to_burnout)
            {
                monitor->stop = 1;
                printf("%zu %d burned out\n", get_time_ms() - monitor->start, monitor->coders[i].id);
                pthread_mutex_unlock(&monitor->stop_mutex);
                break;
            }
            else
                pthread_mutex_unlock(&monitor->stop_mutex);
            i++;
        }
        if (flag)
        {
            pthread_mutex_lock(&monitor->stop_mutex);
            (*monitor).stop = 1;
            pthread_mutex_unlock(&monitor->stop_mutex);
            break;
        }
    }
    return NULL;
}

void *simulation(void *arg)
{
    coder_v *coder;
    size_t  id;
    size_t  time;
    size_t  start;
    coder = (coder_v *)arg;
    start = coder->arguments->start;
    id = coder->id;

    while (!(*coder).arguments->stop)
    {
        if ((*coder).compile_count < (*coder).arguments->number_of_compiles_required)
        {
            if (coder->arguments->stop)
                break;

            if (coder->id % 2 == 0)
                usleep(1000);

            set_dongles(coder);

            if (coder->arguments->stop)
            {
                release_dongles(coder);
                break;
            }


            pthread_mutex_lock(&coder->arguments->print_mutex);
            if (coder->arguments->stop)
            {
                release_dongles(coder);
                pthread_mutex_unlock(&coder->arguments->print_mutex);
                break;
            }
            time = get_time_ms();
            printf("%zu %zu has taken a dongle\n", time - start, id);
            pthread_mutex_unlock(&coder->arguments->print_mutex);



            time = get_time_ms();
            pthread_mutex_lock(&coder->arguments->print_mutex);
            printf("%zu %zu has taken a dongle\n", time - start, id);
            if (coder->arguments->stop)
            {
                pthread_mutex_unlock(&coder->arguments->print_mutex);
                release_dongles(coder);
                break;
            }
            else
                pthread_mutex_unlock(&coder->arguments->print_mutex);


            time = get_time_ms();
            pthread_mutex_lock(&coder->arguments->print_mutex);
            coder->last_compile_time = time;
            printf("%zu %zu is compiling\n", time - start, id);
            (*coder).compile_count += 1;
            coder->left_dongle->realesed_time = 0;
		    coder->right_dongle->realesed_time = 0;
            pthread_mutex_unlock(&coder->arguments->print_mutex);
            precise_sleep(coder->arguments->time_to_compile);
            release_dongles(coder);
            

            if (coder->arguments->stop)
                break;

            time = get_time_ms();
            pthread_mutex_lock(&coder->arguments->print_mutex);
            printf("%zu %zu is debugging\n", time - start, id);
            pthread_mutex_unlock(&coder->arguments->print_mutex);
            precise_sleep(coder->arguments->time_to_debug);


            if (coder->arguments->stop)
                break;

            time = get_time_ms();
            pthread_mutex_lock(&coder->arguments->print_mutex);
            printf("%zu %zu is refactoring\n", time - start, id);
            pthread_mutex_unlock(&coder->arguments->print_mutex);
            precise_sleep(coder->arguments->time_to_refactor);

            if (coder->arguments->stop)
                break;
        }
        else
            break;
    }
    return (NULL);
}

void setup(arg_v *argument)
{
    size_t i;

    pthread_mutex_init(&argument->print_mutex, NULL);
    pthread_mutex_init(&argument->stop_mutex, NULL);

    argument->dongles = malloc(sizeof(dongle_v) * argument->number_of_coders);

    i = 0;
    while (i < argument->number_of_coders)
    {
        argument->dongles[i].available = 1;
        argument->dongles[i].realesed_time = 0;

        pthread_mutex_init(
            &argument->dongles[i].dongle_mutex,
            NULL);

        i++;
    }
    
    argument->coders = malloc(sizeof(coder_v) * argument->number_of_coders);
    i = 0;
    while (i < argument->number_of_coders)
    {
        argument->coders[i].id = i + 1;
        argument->coders[i].compile_count = 0;
        argument->coders[i].last_compile_time = get_time_ms();
        argument->coders[i].arguments = argument;

        argument->coders[i].right_dongle =
            &argument->dongles[i];

        if (i == 0)
            argument->coders[i].left_dongle =
                &argument->dongles[
                    argument->number_of_coders - 1];
        else
            argument->coders[i].left_dongle =
                &argument->dongles[i - 1];

        pthread_mutex_init(
            &argument->coders[i].coder_mutex,
            NULL);

        pthread_create(
            &argument->coders[i].thread,
            NULL,
            simulation,
            &argument->coders[i]);

        i++;
    }

    pthread_create(&(*argument).monitor, NULL, monitor, argument);
    i = 0;
    while (i < argument->number_of_coders)
    {
        pthread_join(argument->coders[i].thread, NULL);
        i++;
    }
}