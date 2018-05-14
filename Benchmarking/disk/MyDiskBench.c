//#define _GNU_SOURCE          
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define SIZE 10737418240   			// 10 GB Disk File size 11
#define ONEMIL 1073741824   			// 1 GB Disk File size
#define MAX_THREADS 128

long blocksize;
//long long bytes;
long threadCount;
struct timeval beforeTime,afterTime;
pthread_mutex_t mutex;


void *ReadRandomLatency(void *fd)
{
	int fdread,i,r_nbr;
	long times,pos;
	char * r_buffer;
	time_t t;

	fdread = (int)fd;
	times = ONEMIL/(1024 * threadCount);

	srand((unsigned)time(&t));
	r_buffer = calloc(1024,sizeof(char));

	pthread_mutex_lock(&mutex);
	for ( i = 0 ; i < times ; i++ ) {
		
		r_nbr = rand() % times;
		lseek(fdread,r_nbr,SEEK_SET);
		memset(r_buffer,0,1024);
		read(fdread,r_buffer,1024)	;		
//			printf("SEEK at %ld",pos);
		
	}
	free(r_buffer);
	pthread_mutex_unlock(&mutex);	
	pthread_exit(NULL);
}

void *WriteRandomLatency(void *fd)
{
	int fwrite,r_nbr;
	long i, times,pos;
	char *buffer;
	time_t t;

	fwrite = (int)fd;
	times = ONEMIL/(1024 * threadCount) ;
		
	srand((unsigned)time(&t));              //Initialize Random number generation depending on the time 

	pthread_mutex_lock(&mutex);
	buffer  = calloc(1024,sizeof(char));
	memset(buffer,'P',1024 * sizeof(char));
	
	for (i = 0 ; i < times ; i++ ) {
		
		r_nbr = rand() % times;
		lseek(fwrite,r_nbr,SEEK_SET);
		write(fwrite,buffer,1024);
				//	printf("%ld\n",pos);
	}	
	free(buffer);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void *ReadRandom(void *fd)
{ 
	int fdread,i,ret;	
	char * r_buffer;
	long long times,r_nbr;
	time_t t;

	fdread = (int)fd;
	times = SIZE/(blocksize*threadCount);
	
	srand((unsigned)time(&t));
	
	pthread_mutex_lock(&mutex);
	r_buffer = calloc(blocksize,sizeof(char));

	for (i = 0 ; i < times ; i ++) {
				
		r_nbr = rand() % SIZE;
		lseek(fdread,r_nbr,SEEK_SET);
		memset(r_buffer,0,blocksize);

		if ((ret = read(fdread,r_buffer,blocksize)) > 0 ){
	      	//	printf("%d\t",ret);
			fsync(fdread);	
		}		
	}
	free(r_buffer);	
	pthread_mutex_unlock(&mutex);	
	pthread_exit(NULL);
}

void *WriteRandom(void *fd){	

	int i,fwrite,r_nbr;
	long long times;
	char *buffer;
	time_t t;

	fwrite = (int)fd;
	times = SIZE/(blocksize*threadCount);
	
		
//	printf("times is %d",times);
	srand((unsigned)time(&t));              //Initialize Random number generation depending on the time 

	pthread_mutex_lock(&mutex);

	buffer  = calloc(blocksize,sizeof(char));
	memset(buffer,'A',blocksize * sizeof(char));
	
	for (i = 0 ; i < times ; i++ ) {
		
		r_nbr = rand() % SIZE;
		lseek(fwrite,r_nbr,SEEK_SET);
		if(write(fwrite,buffer,blocksize) > 0 ){						
			fsync(fwrite);					
		}
	}
	free(buffer);	
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void *ReadSequential(void *fd){

	int fdread,i,rc;
	char *r_buffer;
	long long times;

	fdread = (int)fd;
	times = SIZE/(blocksize*threadCount);
	
	pthread_mutex_lock(&mutex);

	r_buffer = malloc(blocksize);

	for (i = 0 ; i < times ; i ++) {
		
		memset(r_buffer,0,blocksize);
		read(fdread,r_buffer,blocksize);
		fsync(fdread);					
		//if (read(fdread,r_buffer,blocksize) > 0 ){
	}
	free(r_buffer);
	pthread_mutex_unlock(&mutex);	
	//close(fdread);
	pthread_exit(NULL);	

}	

void *WriteSequential(void *fd) {
	
	int i,fwrite;
	long long bytes,times;
	char  *buffer;

	fwrite = (int)fd;
	
	times = SIZE/(blocksize*threadCount);
	
	pthread_mutex_lock(&mutex);
	
	buffer = calloc(blocksize,sizeof(char));
	memset(buffer,'Z',blocksize * sizeof(char));
	
	for(i = 0 ; i < times ; i++ ) {
	
	     if(write(fwrite,buffer,blocksize) > 0){
		fsync(fwrite);
		}
	}

	free(buffer);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{	
	char *s1,accss_pttrn[2];
	FILE *file1;
	long blk_type;
	int fd,i,ret;
	char s[]="\0";
	double t1,throughput;
	long iops;
	s1 = *(++argv);
	
	
	file1 = fopen(s1,"r");

	if (file1 == NULL) {
 		perror("Error opening file");
       	return(-1);
    }

    while ( EOF != fscanf(file1,"%s\n%ld\n%d",&accss_pttrn,&blk_type,&threadCount)) {
    }

    pthread_t thread[threadCount];  

	/*Check the block size and convert it to unix block format */
    if ( blk_type == 1000 ){          //1 MB = 1024KB
    	blocksize = 1048576;
    }
    else if (blk_type == 10000 ) {   //10 MB = 10240KB
    	blocksize = 10485760;
    }
    else if (blk_type == 100000 ) {     //100MB = 102400KB
    	blocksize = 104857600;
    }
    else if (blk_type == 1) {     //1KB = 1024 bytes
    	blocksize = 1024;
	}

	/*Check for Throughput computation starts here */
    if ( blk_type == 1000 || blk_type == 10000 || blk_type == 100000 ) 
    {
  
    	if (strcmp(accss_pttrn,"WS") == 0) {

    		
    		fd = open("/tmp/File3.txt",O_WRONLY | O_CREAT | O_TRUNC | O_SYNC | O_APPEND, S_IRWXU | S_IRGRP);	
    		pthread_mutex_init(&mutex,NULL);
    		gettimeofday(&beforeTime,NULL);

    		for ( i = 0 ; i < threadCount ; i ++) 
    		{					
//    			printf("Creating Thread %d\n",i);		
    			ret = pthread_create(&thread[i],NULL,WriteSequential,(void *)fd);		
    			if(ret){
    				printf("Error while creating thread ...\n");
    			}						
    		}								

    		for ( i = 0 ; i < threadCount ; i ++)
    		{
    			pthread_join(thread[i],NULL);
    		}	
    		gettimeofday(&afterTime,NULL);	
    		pthread_mutex_destroy(&mutex);	
    	
		close(fd);
    		t1 = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
    					((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
    		throughput = SIZE/(t1 * 1048576);
    		//printf("time %lf\n",t1);
    		printf("%s\t%d\t%ld\t%0.2lf\n",accss_pttrn,threadCount,blk_type,throughput);    		

    	}

    	else if (strcmp(accss_pttrn,"RS") == 0) {
    		
    		fd = open("/tmp/File3.txt",O_RDONLY , S_IRWXU | S_IRGRP);
    		pthread_mutex_init(&mutex,NULL);	
    		gettimeofday(&beforeTime,NULL);
    		for ( i = 0 ; i < threadCount ; i ++) 
    		{					
    		//	printf("Creating Thread %d\n",i);		
    			ret = pthread_create(&thread[i],NULL,ReadSequential,(void *)fd);		
    			if(ret){
    				printf("Error while creating thread ...\n");
    			}						
    		}								

    		for ( i = 0 ; i < threadCount ; i ++)
    		{
    			pthread_join(thread[i],NULL);
    		}
    		pthread_mutex_destroy(&mutex);	
    		gettimeofday(&afterTime,NULL);    		
    		close(fd);
    		t1 = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
    				((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
    		throughput = SIZE/(t1 * 1048576);
    		//printf("time %lf\n",t1);
    		printf("%s\t%d\t%ld\t%0.2lf\n",accss_pttrn,threadCount,blk_type,throughput);    		
    	}
    	else if (strcmp(accss_pttrn,"WR") == 0) {

    		
    		fd = open("/tmp/File3.txt",O_WRONLY | O_CREAT | O_TRUNC | O_SYNC | O_APPEND, S_IRWXU | S_IRGRP);	
    		lseek(fd, SIZE, SEEK_SET);
		write(fd,s,sizeof(s));
    		pthread_mutex_init(&mutex,NULL);
    		gettimeofday(&beforeTime,NULL);

    		for ( i = 0 ; i < threadCount ; i ++) 
    		{					
    	//		printf("Creating Thread %d\n",i);		
    			ret = pthread_create(&thread[i],NULL,WriteRandom,(void *)fd);		
    			if(ret){
    				printf("Error while creating thread ...\n");
    			}						
    		}								

    		for ( i = 0 ; i < threadCount ; i ++)
    		{
    			pthread_join(thread[i],NULL);
    		}	
    		gettimeofday(&afterTime,NULL);	
    		pthread_mutex_destroy(&mutex);	
    		close(fd);
    		t1 = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
    			((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
    		throughput = SIZE/(t1 * 1048576);
    //		printf("time %lf\n",t1);
    		printf("%s\t%d\t%ld\t%0.2lf\n",accss_pttrn,threadCount,blk_type,throughput);    		
    	}

    	else if (strcmp(accss_pttrn,"RR") == 0) {
    		
    		fd = open("/tmp/File3.txt",O_RDONLY , S_IRWXU | S_IRGRP);		
    		pthread_mutex_init(&mutex,NULL);
    		gettimeofday(&beforeTime,NULL);
    		for ( i = 0 ; i < threadCount ; i ++) 
    		{					
    //			printf("Creating Thread %d\n",i);		
    			ret = pthread_create(&thread[i],NULL,ReadRandom,(void *)fd);		
    			if(ret){ 
    				printf("Error while creating thread ...\n");
    			}						
    		}								
    		for ( i = 0 ; i < threadCount ; i ++)
    		{
    			pthread_join(thread[i],NULL);
    		}
    		pthread_mutex_destroy(&mutex);		
    		gettimeofday(&afterTime,NULL);    		
    		close(fd);
    		t1 = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
    			((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
    		throughput = SIZE/(t1 * 1048576);
    //		printf("time %lf\n",t1);
    		printf("%s\t%d\t%ld\t%0.2lf\n",accss_pttrn,threadCount,blk_type,throughput);    				
	
    	}

	}  //Check for block size for throughput test ends here
	else if ( blk_type == 1) {   
		//Here comes the 1 Million iterations  with 1 byte block size

		if (strcmp(accss_pttrn,"WR") == 0) {
			
			fd = open("/tmp/File3.txt",O_WRONLY | O_CREAT | O_TRUNC | O_APPEND | O_SYNC, S_IRWXU | S_IRGRP);	
    			lseek(fd, SIZE, SEEK_SET);
			write(fd,s,sizeof(s));
			pthread_mutex_init(&mutex,NULL);
			gettimeofday(&beforeTime,NULL);

			for ( i = 0 ; i < threadCount ; i ++) 
			{					
				//printf("Creating Thread %d\n",i);		
				ret = pthread_create(&thread[i],NULL,WriteRandomLatency,(void *)fd);		
				if(ret){
					printf("Error while creating thread ...\n");
				}						
			}								

			for ( i = 0 ; i < threadCount ; i ++)
			{
				pthread_join(thread[i],NULL);
			}
			pthread_mutex_destroy(&mutex);	
			gettimeofday(&afterTime,NULL);					
			
			t1 = (double)((((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
				((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000))*1000);
			//printf("time %lf\n",t1);
			iops = (1000000/t1);
			printf("%s\t%d\t%ld\t%lf\t%ld\n",accss_pttrn,threadCount,blk_type,t1,iops);  
			close(fd);
			
		}
		else if (strcmp(accss_pttrn,"RR") == 0) {

			pthread_mutex_init(&mutex,NULL);
			fd = open("/tmp/File3.txt",O_RDONLY | O_SYNC, S_IRWXU | S_IRGRP);		
			gettimeofday(&beforeTime,NULL);

			for ( i = 0 ; i < threadCount ; i ++) 
			{					
				//printf("Creating Thread %d\n",i);		
				ret = pthread_create(&thread[i],NULL,ReadRandomLatency,(void *)fd);		
				if(ret){
					printf("Error while creating thread ...\n");
				}						
			}								

			for ( i = 0 ; i < threadCount ; i ++)
			{
				pthread_join(thread[i],NULL);
			}	
			gettimeofday(&afterTime,NULL);
			pthread_mutex_destroy(&mutex);	
			t1 = (double)((((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
				((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000))*1000);
			//printf("time %lf\n",t1);
			iops = (1000000/t1);
			printf("%s\t%d\t%ld\t%lf\t%ld\n",accss_pttrn,threadCount,blk_type,t1,iops);
			close(fd);
		}

	}

	return 0;	
 }
