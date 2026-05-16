#include "codexion.h"

void* simulation(void *arg)
{
    coder_v *coder = (coder_v *)arg;
    size_t id;

    id = (*coder).id;

    set_dongles(coder);
    pthread_mutex_lock(&(*coder).arguments->print_mutex);
    printf("%zu has taken a dongle\n", id);
    printf("%zu has taken a dongle\n", id);
    printf("%zu is compiling\n", id);
    printf("%zu is debugging\n", id);
    printf("%zu is refactoring\n", id);
    pthread_mutex_unlock(&(*coder).arguments->print_mutex);
    release_dongles(coder);
    return NULL; 
}

void setup(arg_v *argument)
{
    size_t i;
    
    pthread_cond_init(&(*argument).cond, NULL);
    pthread_mutex_init(&(*argument).print_mutex, NULL);
    (*argument).dongles = malloc(sizeof(dongle_v) * (*argument).number_of_coders);
    i = 0;
    while (i < (*argument).number_of_coders)
    {
        (*argument).dongles[i].available = 1;
        (*argument).dongles[i].realesed_time = 0;
        pthread_mutex_init(&(*argument).dongles[i].dongle_mutex, NULL);
        i++;
    }

    (*argument).coders = malloc(sizeof(coder_v) * (*argument).number_of_coders);
    i = 0;
    while(i < (*argument).number_of_coders)
    {
        (*argument).coders[i].id = i + 1;
        (*argument).coders[i].compile_count = 0;
        (*argument).coders[i].last_compile_time = 0;
        (*argument).coders[i].arguments = argument;
        (*argument).coders[i].right_dongle = &(*argument).dongles[i];
        if (i == 0)
            (*argument).coders[i].left_dongle = &(*argument).dongles[(*argument).number_of_coders - 1];
        else
            (*argument).coders[i].left_dongle = &(*argument).dongles[i - 1];
        pthread_mutex_init(&(*argument).coders[i].coder_mutex, NULL);
        pthread_create(&(*argument).coders[i].thread, NULL, simulation, &(*argument).coders[i]);
        i++;
    }

    i = 0;
    while(i < (*argument).number_of_coders)
    {
        pthread_join((*argument).coders[i].thread, NULL);
        i++;
    }
}