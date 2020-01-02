#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

struct Data 
{
    int thread_id;
    int* number_of_cribs;
    pthread_mutex_t* mutex;
    pthread_cond_t* moo;
    pthread_cond_t* refilled;
};

static int max_number_of_cribs = 0;

void* start_cow_routine(void* thread_data)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    Data* data = (Data*) thread_data;
    
    while(true) 
    {
        int sleepTime = (rand() / (float) RAND_MAX) * 8;
        sleepTime += 2;

        sleep(sleepTime);
        pthread_mutex_lock(data->mutex);

        if(*data->number_of_cribs == 0)
        {
            printf("[%d] valov je prazdny \n", data->thread_id);
            pthread_cond_signal(data->moo);
            pthread_cond_wait(data->refilled, data->mutex);
        }
        
        printf("[%d] som hladna \n", data->thread_id);
        (*data->number_of_cribs)--;
        pthread_mutex_unlock(data->mutex);
    }
    
    return 0;
}

void* start_nurse_routine(void* thread_data) 
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    Data* data = (Data*) thread_data;
    
    while(true){
        pthread_mutex_lock(data->mutex);
        pthread_cond_wait(data->moo, data->mutex);
        printf("osetrovatel: cekujem\n");
        
        if(*data->number_of_cribs == 0)
        {
            printf("osetrovatel: valov prazdny, naplnam\n");
            printf("osetrovatel: zacal som \n");
            *data->number_of_cribs = max_number_of_cribs;
            pthread_cond_broadcast(data->refilled);
            pthread_mutex_unlock(data->mutex);
        }
    }
    
    return 0;
}


int main() 
{    
    srand(time(0));    
    printf("\e[1;1H\e[2J");
    
    while(1)
    {
        pthread_mutex_t mutex;
        pthread_cond_t moo;
        pthread_cond_t refilled;
        
        pthread_mutex_init(&mutex, 0);
        pthread_cond_init(&moo, 0);
        pthread_cond_init(&refilled, 0);
        
        printf("Hladne kobyly v0.0.1: \n");
        printf("1. Zacat simulaciu \n");
        printf("2. Skoncit \n");
        printf("3. Pomoc \n");
        
        int option;
        scanf("%i", &option);
        
        if(option == 1)
        {                   
            int number_of_threads = 0;
            
            printf("Zadajte pocet krav\n");
            scanf("%i", &number_of_threads);
            number_of_threads++;
            printf("Zadajte maximalny pocet valovov\n");
            scanf("%i", &max_number_of_cribs);
            
            int number_of_cribs = max_number_of_cribs;
            
            pthread_t thread[number_of_threads] = {};
            Data* dataBuffer = (Data*) malloc(sizeof(Data) * number_of_threads);
            
            for(int i = 0; i < number_of_threads; i++)
            {
                Data* data = dataBuffer + i;
                data->thread_id = i;
                data->mutex = &mutex;
                data->moo = &moo;
                data->refilled = &refilled;
                data->number_of_cribs = &number_of_cribs;
                
                if(i == 0) pthread_create(&thread[0], 0, &start_nurse_routine, data);
                else pthread_create(&thread[i], 0, &start_cow_routine, data);
            }
            
            printf("\e[1;1H\e[2J");
            char str[10] = {};
            do
            {
                scanf("%s", str);
            } while(strcmp("exit", str) != 0);
            
            for(int i = 0; i < number_of_threads; i++)
            {    
                pthread_cancel(thread[i]);
                pthread_join(thread[i], 0);
            }    
            
            free(dataBuffer);
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&moo);
            pthread_cond_destroy(&refilled);    
        }
        else if (option == 2)
        {          
            break;
        }
        else if (option == 3)
        {
            printf("HELP"); //TODO: 
        }
        else{
            printf("Neplatna operacia");
        }
    }
    
    return 0;
}