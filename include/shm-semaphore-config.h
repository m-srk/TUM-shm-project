#ifndef __INCLUDED_SHM_CONFIG__
#define __INCLUDED_SHM_CONFIG__

#include <stdlib.h>
#include <fcntl.h>      // O_ constants
#include <sys/mman.h>   // shared memory API
#include <unistd.h>     // POSIX API
#include <semaphore.h>  // semaphore API

#include "hashtable.h"

// Buffer data structures
#ifndef IPC_FAILURE
#define IPC_FAILURE 1
#endif

#ifndef SEMAPHORE_FAILURE
#define SEMAPHORE_FAILURE -1
#endif

#define STRINGS_EQUAL 0

#define MAX_BUFFERS 10000

// HT query types
#define NUM_QUERY_TYPES 3

#define INSERT_QUERY 0
#define READ_QUERY 1
#define DELETE_QUERY 2

// thread defines
#ifndef SERVER_THREAD_COUNT_CONF 
#define SERVER_THREAD_COUNT 6
#else
#define SERVER_THREAD_COUNT SERVER_THREAD_COUNT_CONF
#endif
 
#define SERVER_THREAD_SLEEP_MS 10

#define CLIENT_THREAD_COUNT 5
#define CLIENT_THREAD_SLEEP_MS 10

// sem names
#define LOGFILE "/tmp/tumproj.log"
#define SERVER_THREAD_MUTEX "/sem-server-mutex"

#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_PRODUCER_COUNT "/sem-producer-count"
#define SEM_CONSUMER_COUNT "/sem-consumer-count"
#define SEM_HT_QUERY_PROD "/sem-ht-producer"
#define SEM_HT_QUERY_CONS "/sem-ht-consumer"
#define SHARED_MEM_NAME "/posix-shm-tumproj"


typedef struct thread_task_cmd
{
    bool is_max_buff_count_hit;   

} thread_task_cmd_t;

typedef struct hashtable_query {
    int ht_query;
    int key;
    int value;
    int response;
} hashtable_query_t;

typedef struct query_buffer {
    hashtable_query_t hts[MAX_BUFFERS];
    int ht_prod_count;
    int ht_cons_count;
} query_buffer;

struct shared_memory {
    hashtable_query_t hts[MAX_BUFFERS];
    int producer_index;
    int consumer_index;
};

typedef struct server_shm_data {
    struct shared_memory* shared_mem_ptr;
    int fd_shm;
} server_shm_data_t;

typedef struct server_sem_data {
    sem_t* mutex_sem;
    sem_t* producer_count_sem;
    sem_t* consumer_count_sem;
} server_sem_data_t;

typedef struct hashtable_worker_in {
    server_sem_data_t* sem_data;
    server_shm_data_t* shm_data;
} ht_worker_input_t;


//-------------------------------------------
// Server API
bool open_and_map_shm();
bool init_sems();
void print_err(const char* args);
void release_shm_segment();

void register_threadid_ptr(pthread_t* tptr);
void register_HT_instance(HashTable *);

// thread runner func
void* consumer_task_runner(void* args);
void* htquery_task_runner(void* args);

//-------------------------------------------
// Client API
void set_rand_seed();
bool init_sems_client();
bool open_and_map_shm_client();
void unmap_shm_mem();

// thread runner func
void* run_client_task_rand(void*);

//-------------------------------------------


#endif //__INCLUDED_SHM_CONFIG__
