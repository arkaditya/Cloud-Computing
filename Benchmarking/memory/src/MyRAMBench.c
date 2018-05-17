#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
//
#define MAX_THREADS 4

char *buffer;
long position;
char *bufferInit;
char *MemInit;
double timevalnew1,timevalnew2;
long long bufferlength = 1048576000;
long million_oper = 1048576;
//long million_oper;

unsigned int seed = 1234;
pthread_mutex_t mutex;

struct MemFileReader {
	char MemOperation[3];
	long block;
	int ThreadCount;
};

struct ThreadArguments {
	
	long blocksize;
	int ThreadCount;
	char *MemoryBlock;
};

void * RWSLatency(void *threadDetails)
{
	int i,j,threads,data_iter;
	long ntimes;
	char *Mem1;
	struct ThreadArguments *t1 = (struct ThreadArguments *)threadDetails;

	threads = (*t1).ThreadCount;
	Mem1 = (*t1).MemoryBlock;
	
	data_iter = 100/threads;
	ntimes = (million_oper/(1*threads));
	bufferInit = buffer;
	MemInit = Mem1;

	pthread_mutex_lock(&mutex);
//	printf("**** LOCK MUTEX******... %d\n",mutex);
	for ( j=0 ; j < data_iter ; j++){
		for ( i=0 ; i < ntimes ; i++){
			memcpy(buffer,Mem1,1);
			buffer = buffer + 1;	
			Mem1 = Mem1 + 1 ;
		}
		buffer = bufferInit;
		Mem1 = MemInit;				
	}
	pthread_mutex_unlock(&mutex);

//	printf("Successfully Copied from Mem1 to BUFFER\n");
	pthread_exit(buffer);
}

void * ReadWriteSequential(void *threadDetails)
{
	int i,j,bsize,threads,data_iter;
	long ntimes;
	char *Mem1;
	struct ThreadArguments *t1 = (struct ThreadArguments *)threadDetails;

	bsize = (*t1).blocksize;
	threads = (*t1).ThreadCount;
	Mem1 = (*t1).MemoryBlock;
	
	data_iter = 100/threads;
	ntimes = (bufferlength/(bsize*threads));
	
	bufferInit = buffer;
	MemInit = Mem1;
	pthread_mutex_lock(&mutex);

	for ( j=0 ; j < data_iter ; j++){
		for ( i=0 ; i < ntimes ; i++){
			memcpy(buffer,Mem1,bsize);
			buffer = buffer + bsize;	
			Mem1 = Mem1 + bsize ;
		}
		buffer = bufferInit;
		Mem1   = MemInit;				
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(buffer);
}

void  Sequential(void *threadDetails)
{
	
		int i,n,ret;
		char *bufferInitPos;
		long block_type;
		struct ThreadArguments *thread1 = (struct ThreadArguments *)threadDetails;		

		block_type = (*thread1).blocksize;
		n = (*thread1).ThreadCount;

		pthread_t thread[n];
		if ( block_type == 1024 || block_type == 1048576 || block_type == 10485760 ) {

		buffer = (char *)calloc(bufferlength,sizeof(char));
		bufferInitPos = buffer;  //To know the intital address of the buffer pointer, 
								 // so that we can free the memory later as buffer is incremented with each thread	
		pthread_mutex_init(&mutex,NULL);

		for ( i = 0 ; i < n ; i ++) 
		{	
			
			ret = pthread_create(&thread[i],NULL,ReadWriteSequential,(void *)thread1);
			if(ret){
				printf("Error while creating thread ...\n");
			}						
		}								

		for ( i = 0 ; i < n ; i ++)
		{
			pthread_join(thread[i],NULL);
		}

		pthread_mutex_destroy(&mutex);
		free(bufferInitPos);
	}
	else if ( block_type == 1 ) {       //Latency Test
			buffer = (char *)calloc(million_oper,sizeof(char));
			bufferInitPos = buffer;

			pthread_mutex_init(&mutex,NULL);

			for ( i = 0 ; i < n ; i ++) 
			{				
				//printf("Creating Thread RWS Latency%d\n",i);
				ret = pthread_create(&thread[i],NULL,RWSLatency,(void *)thread1);
				if(ret){
					printf("Error while creating thread ...\n");
				}						
			}								
			
			for ( i = 0 ; i < n ; i ++)
			{
				pthread_join(thread[i],NULL);
			}	

			pthread_mutex_destroy(&mutex);
			free(bufferInitPos);			
		}
}

void * RWRLatency( void *threadDetails)
{
	int i,j,threads,data_iter;
	long ntimes;
	char *MemInit;
	long bufferRange,offset=0;

	struct ThreadArguments *t1 = (struct ThreadArguments *)threadDetails;

	threads = (*t1).ThreadCount;
	MemInit = (*t1).MemoryBlock;
	data_iter = 100/threads;

	bufferRange = million_oper - 1 ;
	
	ntimes = (million_oper/threads);
	
	srand(time(0));
	for ( j=0 ; j < data_iter ; j++){
		for ( i =0 ; i < ntimes ; i++ ) {

		offset = (rand() % bufferRange);								
		memcpy(buffer+offset,MemInit+offset,1);

		}
	}
	
	pthread_exit(buffer);
}

void * ReadWriteRandom(void *threadDetails)
{
	int i,j,bsize,threads,data_iter;
	long ntimes;
	char *Mem1;
	long bufferRange,offset=0;

	struct ThreadArguments *t1 = (struct ThreadArguments *)threadDetails;

	bsize = (*t1).blocksize;
	threads = (*t1).ThreadCount;
	Mem1 = (*t1).MemoryBlock;

	bufferRange = (bufferlength/bsize) - 1 ;
	
	ntimes = (bufferlength/(bsize*threads));
	data_iter = 100/threads;
	
	bufferInit = buffer;
	MemInit = Mem1;
	pthread_mutex_lock(&mutex);

	srand(time(0));
	for ( j=0 ; j < data_iter ; j++){
		for ( i =0 ; i < ntimes ; i++ ) {
			offset = (rand() % bufferRange);							
			memcpy(buffer+offset,Mem1+offset,bsize);
			}
		buffer = bufferInit;
		Mem1   = MemInit;		
		}
	pthread_mutex_unlock(&mutex);
	pthread_exit(buffer);
}

void  RandomAccess(void * threadDetails)
{
	
	int i,n,ret;
	char *bufferInitPos;
	long block_type;
	struct ThreadArguments *thread1 = (struct ThreadArguments *)threadDetails;
	n = (*thread1).ThreadCount;
	pthread_t thread[n];

	block_type = (*thread1).blocksize;

	if ( block_type == 1024 || block_type == 1048576 || block_type == 10485760 ) {
		
		buffer = (char *)calloc(bufferlength,sizeof(char));	
		bufferInitPos = buffer;  //To know the intital address of the buffer pointer, 
								 // so that we can free the memory later as buffer is incremented with each thread
		pthread_mutex_init(&mutex,NULL);

		for ( i = 0 ; i < n ; i ++) 
		{	
			//printf("Creating Thread %d\n",i);
			ret = pthread_create(&thread[i],NULL,ReadWriteRandom,(void *)thread1);		
			if(ret){
				printf("Error while creating thread ...");
			}						
		}								

		for ( i = 0 ; i < n ; i ++)
		{
			pthread_join(thread[i],NULL);
		}

		pthread_mutex_destroy(&mutex);
		free(bufferInitPos);
	}
	else if ( block_type == 1 ) {       //Latency Test
	
		buffer = (char *)calloc(million_oper,sizeof(char));
		bufferInitPos = buffer;			
		pthread_mutex_init(&mutex,NULL);

		for ( i = 0 ; i < n ; i ++) 
		{	
			//printf("Creating Thread for RWR Latency%d\n",i);
			ret = pthread_create(&thread[i],NULL,RWRLatency,(void *)thread1);		
			if(ret){
				printf("Error while creating thread ...");
			}						
		}								

		for ( i = 0 ; i < n ; i ++)
		{
			pthread_join(thread[i],NULL);
		}

		pthread_mutex_destroy(&mutex);
		//gettimeofday(&afterTime,NULL);
		free(bufferInitPos);
	}					

}

void  ParallelThreadCreation(char *ch,long b,int n)
{

	long blkSize;
	double latency;
	double throughput;
	struct timeval beforeTime,afterTime;
	char * MemInit;
	struct ThreadArguments *thread1;	
	//struct ThreadArguments thread1;	
	
	thread1 = (struct ThreadArguments *)malloc(sizeof(struct ThreadArguments));

//	bufferlength =  1048576000;
//	million_oper = 1048576;
	
	if (b == 1) {
		blkSize = 1;
	}
	else if (b == 1000) {
		blkSize =1024;
	}
	else if (b == 1000000) {
		blkSize = 1048576;
	}
	else if (b == 10000000) {
		blkSize = 10485760;
	}
	

	if (b == 1) {
		MemInit = malloc(million_oper * sizeof(char));
		memset(MemInit,'B',million_oper * sizeof(char));
	}
	else {
		MemInit = malloc(bufferlength * sizeof(char));
		memset(MemInit,'Z',bufferlength * sizeof(char));
	}
	//printf("Size of MemInit %d\n",sizeof(MemInit));

	(*thread1).blocksize = blkSize;
	(*thread1).ThreadCount = n;
	(*thread1).MemoryBlock = MemInit;
	
	gettimeofday(&beforeTime,NULL);
		if (strcmp(ch,"RWS")==0) {

			Sequential(thread1);		
		}
		else if (strcmp(ch,"RWR")==0) {
			
			RandomAccess(thread1);				
		}
	
	gettimeofday(&afterTime,NULL);		

	free(MemInit);
	free(thread1);	 

	
	if (b == 1000 || b == 1000000 || b == 10000000 ){    
		timevalnew1 = ((double)((afterTime.tv_sec - beforeTime.tv_sec) + ((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000)));
		throughput = (bufferlength * 100)/((double)(timevalnew1 * 1073741824));    
		printf("%s\t\t%ld\t\t%d\t\t%lf\n",ch,b,n,throughput);
	}
	else {
		timevalnew2 = (((long)(afterTime.tv_sec - beforeTime.tv_sec) *1000000) + (afterTime.tv_usec - beforeTime.tv_usec));
		latency = ((timevalnew2)/((double)104857600));    //Seconds to Microseconds conversion 
		printf("%s\t\t%ld\t\t%d\t\t\t%lf Ms\n",ch,b,n,latency);	
	}
	
}

int main(int argc, char *argv[]) {

	char *s1;
	FILE *file1;	

	struct MemFileReader f1;
	s1 = *(++argv);          			//To get the multi chacracter filename
	file1 = fopen(s1,"r");

	if (file1 == NULL) {
		perror("Error opening file");
		return(-1);
	}

	while ( EOF != fscanf(file1,"%s\n%ld\n%d",&f1.MemOperation,&f1.block,&f1.ThreadCount)) {
	}

	ParallelThreadCreation(f1.MemOperation,f1.block,f1.ThreadCount);
	return 0;
}	
