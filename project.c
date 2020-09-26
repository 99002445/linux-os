#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>


sem_t modify;
pthread_mutex_t mutex;
int count = 1;
int numaccesser = 0;

void *modifier(void *modifierno)
{   
    sem_wait(&modify);
    count = count*2;
    printf("modifier %d modifies cnt to %d\n",(*((int *)modifierno)),count);
    sem_post(&modify);

}
void *accesser(void *accesserno)
{   
    
    pthread_mutex_lock(&mutex);
    numaccesser++;
    if(numaccesser == 1) {
        sem_wait(&modify); 
    }
    pthread_mutex_unlock(&mutex);
    
    printf("accesser %d: accessed cnt as %d\n",*((int *)accesserno),count);

    
    pthread_mutex_lock(&mutex);
    numaccesser--;
    if(numaccesser == 0) {
	sem_post(&modify); 
    }
    pthread_mutex_unlock(&mutex);
}
int main()
{   

    pthread_t r[10],w[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&modify,0,1);

    int a[10] = {1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < 10; ++i) {
        pthread_create(&r[i], NULL, (void *)accesser, (void *)&a[i]);
    }
    for(int i = 0; i < 5; ++i) {
        pthread_create(&w[i], NULL, (void *)modifier, (void *)&a[i]);
    }

    for(int i = 0; i < 10; ++i) {
        pthread_join(r[i], NULL);
    }
    for(int i = 0; i < 5; ++i) {
        pthread_join(w[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&modify);

    return 0;
    
}
