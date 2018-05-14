#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <immintrin.h>

#define ITERATIONS 1000000000000
#define MAX_THREADS 4

/* Global Declaration */
long long int load_var;      // For Calculating Iterations/Thread 
pthread_mutex_t mutex;
struct timeval beforeTime,afterTime;
double timeInterval;

struct CPUFileReader {		//Structure for reading each file of a specific format

	FILE *file;
	char precision[2];
	int ThreadCount;
};

void *CPUBenchmark_QP(void *args)
{

long long j;
char u;
for (j = 0 ; j < load_var ; j++ ) {
		u = 'R' + 'P' + '$';
}

pthread_exit(NULL);
return NULL;
}

void *CPUBenchmark_HP(void *args)
{

long long j;

short arr1 = 765;
short arr2 = 540;
short arr3,arr4,arr5;

	for (j = 0 ; j < load_var ; j++ ) 	
	{
	
		arr3 = arr1 + arr2;
		arr4 = arr3 + arr2;
		arr5 = arr2 + arr3;
		//sprintf(temp, "1",sizeof("1"));
	}

pthread_exit(NULL);
return NULL;
}

void *CPUBenchmark_SP(void *arg)
{

long long i;
	
//printf("%ld",load_var);

__m256 a = _mm256_set_ps(1201.1112,1216.01,3131.34,5108.1,8134.87,9198.29,2991.11,190);  //256 bit  vector each

__m256 b = _mm256_set_ps(1567.11,1219.01,1310.34,1344.1,784.87,5198.29,4919.11,190);

__m256 c;

    for ( i = 0; i < load_var ; i++)
    {
		 c = _mm256_add_ps(a,b); 
	}


pthread_exit(NULL);
return NULL;
}

void *CPUBenchmark_DP(void *arg)
{

long long i;

	__m256d a = _mm256_set_pd(1900011.11,11110.01344,131000.1234,108999.1999);  //256 bit  vector each
	__m256d b = _mm256_set_pd(1211111.19991,888121.11655,131567.3489,131000.34);
	__m256d c = _mm256_set_pd(981361.1900,451121.8915,5416789.1229,912210.0971);
	__m256d d,e,f,g,h,j;

	for ( i = 0; i < load_var ; i++)
    {
		 d = _mm256_add_pd(a,b);
		 e = _mm256_add_pd(b,c);
		 f = _mm256_sub_pd(b,c); 
		 g = _mm256_sub_pd(a,b); 
		 h = _mm256_add_pd(a,c);
		 j = _mm256_sub_pd(a,c); 
	}

pthread_exit(NULL);
return NULL;
}

void  ParallelThreadCreation(int n,char* ch)
{
		int i,ret;
		pthread_t thread[n];
		double GigaOps;
		GigaOps = 0;

			if (strcmp(ch,"DP")==0) {		 	
				
				gettimeofday(&beforeTime,NULL);			
				//printf("Inside DP\n");
				for ( i = 0 ; i < n ; i ++) 
				{	
					//printf("Creating Thread %d\n",i);
					ret = pthread_create(&thread[i],NULL,CPUBenchmark_DP,NULL);
					if(ret) {
					printf("Error while creatig pthread_create Thread %d",ret);  					
					}				
				}
				for ( i = 0 ; i < n ; i ++)
				{
					pthread_join(thread[i],NULL);
				}
				gettimeofday(&afterTime,NULL);			
				timeInterval = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
						((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
		 		//printf("time is %lf\n",timeInterval);
		 		GigaOps = (double)(ITERATIONS/(double)((double)(100000*timeInterval*1000000000)));	
		 		printf("%s\t%d\t%lf\n",ch,n,GigaOps);
				
			}
		 else if (strcmp(ch,"SP")==0) {

		 				 		
		 		gettimeofday(&beforeTime,NULL);			
				for ( i = 0 ; i < n ; i ++) 
				{	
					//printf("Creating Thread %d\n",i);
					ret = pthread_create(&thread[i],NULL,CPUBenchmark_SP,NULL);
					if(ret) {
					printf("Error while creatig pthread_create Thread %d",ret);  					
					}					
				}
				for ( i = 0 ; i < n ; i ++)
				{
					pthread_join(thread[i],NULL);
				}
				gettimeofday(&afterTime,NULL);			
				timeInterval = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
						((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
		 		//printf("time is %lf\n",timeInterval);
		 		GigaOps = (double)(ITERATIONS/((double)(100000*timeInterval*1000000000)));	
		 		printf("%s\t%d\t%lf\n",ch,n,GigaOps);
		 	}
		 else if (strcmp(ch,"HP")==0) {

		 		gettimeofday(&beforeTime,NULL);				
				for ( i = 0 ; i < n ; i ++) 
				{	
					//printf("Creating Thread %d\n",i);
					ret = pthread_create(&thread[i],NULL,CPUBenchmark_HP,NULL);
					if(ret) {
					printf("Error while creatig pthread_create Thread %d",ret); 					
					}			
				}
				for ( i = 0 ; i < n ; i ++)
				{
					pthread_join(thread[i],NULL);
				}
				gettimeofday(&afterTime,NULL);	
				timeInterval = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
						((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
		 		//printf("time is %lf\n",timeInterval);
		 		GigaOps = (double)(ITERATIONS/((double)(100000*timeInterval*1000000000)));	
		 		printf("%s\t%d\t%lf\n",ch,n,GigaOps);
		 	}
		 else if (strcmp(ch,"QP")==0) {
				
				gettimeofday(&beforeTime,NULL);			
				//load_var = load_var/4;
				for ( i = 0 ; i < n ; i ++) 
				{	
					//printf("Creating Thread %d\n",i);
					ret = pthread_create(&thread[i],NULL,CPUBenchmark_QP,NULL);
					if(ret) {
					printf("Error while creatig pthread_create Thread %d",ret);  
					}										
				}
				for ( i = 0 ; i < n ; i ++)
				{
					pthread_join(thread[i],NULL);
				}
				gettimeofday(&afterTime,NULL);	
				timeInterval = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
						((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
		 		//printf("time is %lf\n",timeInterval);
		 		GigaOps = (double)(ITERATIONS/((double)(100000*timeInterval*1000000000)));	
		 		printf("%s\t%d\t%lf\n",ch,n,GigaOps);
			}
		 	
		 //Giga OPerations per second calculation based on No of instructions in the for loop for each precision added with 
		  // usec/10^6 for micro sec to second conversion 
		 		
}

//FILL ME IN
int main(int argc, char *argv[]) {
 
char *s1;
struct CPUFileReader f1;

if (argv[1])
s1 = *(++argv);          			//To get the multi chacracter filename

f1.file = fopen(s1,"r");

if(f1.file == NULL) {
       perror("Error opening file");
       return(-1);
    }

while ( EOF != fscanf(f1.file,"%s\n%d",&f1.precision,&f1.ThreadCount)) {
 }

load_var = ITERATIONS / f1.ThreadCount;   
fclose(f1.file);

ParallelThreadCreation(f1.ThreadCount,f1.precision);


return 0;
}
