// #include "codexion.h"

// t_heap  *heap_init(int capacity)
// {
//     t_heap  *heap;

//     heap = malloc(sizeof(t_heap));
//     if (!heap)
//         return (NULL);
//     heap->waiters = malloc(sizeof(t_waiter *) * capacity);
//     if (!heap->waiters)
//     {
//         free(heap);
//         return (NULL);
//     }
//     heap->size = 0;
//     heap->capacity = capacity;
//     return (heap);
// }

// t_waiter    *new_waiter(coder_v *coder, long deadline)
// {
//     t_waiter    *waiter;

//     waiter = malloc(sizeof(t_waiter));
//     if (!waiter)
//         return (NULL);
//     waiter->coder = coder;
//     waiter->deadline = deadline;
//     return (waiter);
// }

// void    sift_up(t_heap *heap, int i)
// {
//     int         parent;
//     t_waiter    *tmp;

//     while (i > 0)
//     {
//         parent = (i - 1) / 2;
//         if (heap->waiters[parent]->deadline <= heap->waiters[i]->deadline)
//             break ;
//         tmp = heap->waiters[parent];
//         heap->waiters[parent] = heap->waiters[i];
//         heap->waiters[i] = tmp;
//         i = parent;
//     }
// }

// void    sift_down(t_heap *heap, int i)
// {
//     int         left;
//     int         right;
//     int         smallest;
//     t_waiter    *tmp;

//     while (1)
//     {
//         left = 2 * i + 1;
//         right = 2 * i + 2;
//         smallest = i;
//         if (left < heap->size &&
//             heap->waiters[left]->deadline < heap->waiters[smallest]->deadline)
//             smallest = left;
//         if (right < heap->size &&
//             heap->waiters[right]->deadline < heap->waiters[smallest]->deadline)
//             smallest = right;
//         if (smallest == i)
//             break ;
//         tmp = heap->waiters[i];
//         heap->waiters[i] = heap->waiters[smallest];
//         heap->waiters[smallest] = tmp;
//         i = smallest;
//     }
// }

// void    heap_insert(t_heap *heap, coder_v *coder, long deadline)
// {
//     t_waiter    *waiter;

//     if (heap->size >= heap->capacity)
//         return ;
//     waiter = new_waiter(coder, deadline);
//     if (!waiter)
//         return ;
//     heap->waiters[heap->size] = waiter;
//     sift_up(heap, heap->size);
//     heap->size++;
// }

// t_waiter    *heap_extract_min(t_heap *heap)
// {
//     t_waiter    *min;

//     if (heap->size == 0)
//         return (NULL);
//     min = heap->waiters[0];
//     heap->size--;
//     if (heap->size > 0)
//     {
//         heap->waiters[0] = heap->waiters[heap->size];
//         sift_down(heap, 0);
//     }
//     return (min);
// }

// void    heap_free(t_heap *heap)
// {
//     int i;

//     if (!heap)
//         return ;
//     i = 0;
//     while (i < heap->size)
//     {
//         free(heap->waiters[i]);
//         i++;
//     }
//     free(heap->waiters);
//     free(heap);
// }

// // void    register_in_heaps(coder_v *coder)
// // {
// //     long    deadline;

// //     deadline = coder->last_compile_time + coder->arguments->time_to_burnout;
// //     pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
// //     heap_insert(coder->left_dongle->heap, coder, deadline);
// //     pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
// //     pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
// //     heap_insert(coder->right_dongle->heap, coder, deadline);
// //     pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
// // }

// // void register_in_heaps(coder_v *coder)
// // {
// //     long priority;

// //     if (ft_strcmp(coder->arguments->scheduler, "edf") == 0)
// //     {
// //         priority = coder->last_compile_time
// //                  + coder->arguments->time_to_burnout;
// //     }
// //     else
// //     {
// //         pthread_mutex_lock(&coder->arguments->fifo_mutex);
// //         priority = coder->arguments->arrival_counter++;
// //         pthread_mutex_unlock(&coder->arguments->fifo_mutex);
// //     }

// //     pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
// //     heap_insert(coder->left_dongle->heap, coder, priority);
// //     pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);

// //     pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
// //     heap_insert(coder->right_dongle->heap, coder, priority);
// //     pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
// //}

// void register_in_heaps(coder_v *coder)
// {
//     long priority;

//     if (ft_strcmp(coder->arguments->scheduler, "edf") == 0)
//     {
//         priority = coder->last_compile_time + coder->arguments->time_to_burnout;
//     }
//     else
//     {
//         // FIFO: Priority is the timestamp when they start waiting.
//         // Earlier timestamp = smaller number = higher priority in min-heap.
//         priority = (long)get_time_ms();
//     }

//     pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
//     heap_insert(coder->left_dongle->heap, coder, priority);
//     pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);

//     pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
//     heap_insert(coder->right_dongle->heap, coder, priority);
//     pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
// }

// void    wake_next(dongle_v *dongle)
// {
//     // t_waiter    *next;

//     // next = heap_extract_min(dongle->heap);
//     // if (next)
//     // {
//     //     free(next);
//     pthread_cond_broadcast(&dongle->cond);
//     // }
// }

#include "codexion.h"

t_heap  *heap_init(int capacity)
{
    t_heap  *heap;

    heap = malloc(sizeof(t_heap));
    if (!heap)
        return (NULL);
    heap->waiters = malloc(sizeof(t_waiter *) * capacity);
    if (!heap->waiters)
    {
        free(heap);
        return (NULL);
    }
    heap->size = 0;
    heap->capacity = capacity;
    return (heap);
}

t_waiter    *new_waiter(coder_v *coder, long deadline)
{
    t_waiter    *waiter;

    waiter = malloc(sizeof(t_waiter));
    if (!waiter)
        return (NULL);
    waiter->coder = coder;
    waiter->deadline = deadline;
    return (waiter);
}

void    sift_up(t_heap *heap, int i)
{
    int         parent;
    t_waiter    *tmp;

    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (heap->waiters[parent]->deadline <= heap->waiters[i]->deadline)
            break ;
        tmp = heap->waiters[parent];
        heap->waiters[parent] = heap->waiters[i];
        heap->waiters[i] = tmp;
        i = parent;
    }
}

void    sift_down(t_heap *heap, int i)
{
    int         left;
    int         right;
    int         smallest;
    t_waiter    *tmp;

    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < heap->size &&
            heap->waiters[left]->deadline < heap->waiters[smallest]->deadline)
            smallest = left;
        if (right < heap->size &&
            heap->waiters[right]->deadline < heap->waiters[smallest]->deadline)
            smallest = right;
        if (smallest == i)
            break ;
        tmp = heap->waiters[i];
        heap->waiters[i] = heap->waiters[smallest];
        heap->waiters[smallest] = tmp;
        i = smallest;
    }
}

void    heap_insert(t_heap *heap, coder_v *coder, long deadline)
{
    t_waiter    *waiter;

    if (heap->size >= heap->capacity)
        return ;
    waiter = new_waiter(coder, deadline);
    if (!waiter)
        return ;
    heap->waiters[heap->size] = waiter;
    sift_up(heap, heap->size);
    heap->size++;
}

t_waiter    *heap_extract_min(t_heap *heap)
{
    t_waiter    *min;

    if (heap->size == 0)
        return (NULL);
    min = heap->waiters[0];
    heap->size--;
    if (heap->size > 0)
    {
        heap->waiters[0] = heap->waiters[heap->size];
        sift_down(heap, 0);
    }
    return (min);
}

void    heap_free(t_heap *heap)
{
    int i;

    if (!heap)
        return ;
    i = 0;
    while (i < heap->size)
    {
        free(heap->waiters[i]);
        i++;
    }
    free(heap->waiters);
    free(heap);
}

void register_in_heaps(coder_v *coder)
{
    long priority;

    if (ft_strcmp(coder->arguments->scheduler, "edf") == 0)
    {
        priority = coder->last_compile_time + coder->arguments->time_to_burnout;
    }
    else
    {
        pthread_mutex_lock(&coder->arguments->fifo_mutex);
        priority = coder->arguments->arrival_counter++;
        printf("REGISTERED coder %d with priority %ld\n", coder->id, priority);
        pthread_mutex_unlock(&coder->arguments->fifo_mutex);
    }

    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
    heap_insert(coder->left_dongle->heap, coder, priority);
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);

    pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
    heap_insert(coder->right_dongle->heap, coder, priority);
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

void    wake_next(dongle_v *dongle)
{
    pthread_cond_broadcast(&dongle->cond);
}

