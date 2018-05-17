#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>

//FILL ME IN
#define BUFFER_SIZE 1048576000
#define BUFFER2 1048576
#define MAX_THREADS 8

char *writebuffer;     // Pointer to buffer of Client
char *readbuffer;      //Poiner to buffer of Server 

long bytesTransfrd;
pthread_mutex_t mutexclt;
char *hostname1;
int threadCount;
long blocksize;
long long iterations;

/*  Time variables */
struct timeval beforeTime,afterTime;
double timeInterval,rtt_time,throughput;

/****************** Client Module starts here **************************************/

void * PacketRoundTrip(void *addr){

	int writefd,byte_size,rc;
 	char ack;
 	long long ntimes;
 	struct addrinfo *res;
 //	size_t sock_option;
 	//double rtt_time;

 	res = (struct addrinfo *)addr;

	ntimes = BUFFER2/threadCount;

	writefd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (writefd < 0) {
		printf("Could not create socket!\n");
		exit(-1);
	}

 	rc = connect(writefd, res->ai_addr, res->ai_addrlen);
	if (rc < 0) {
			printf("Could not connect to server!\n");
			exit(-1);
		}	

	while(ntimes > 0 ) {      
			if ((byte_size = send(writefd,writebuffer,blocksize,0)) > 0) {									
					if (byte_size > 0) {
						recv(writefd,&ack,blocksize, 0);								
				}			
			}
			else {
					break;				
			}			
		ntimes = ntimes - 1;
	}
	
	close(writefd);
	return NULL;
}

void * ClientBufferWriter(void *addr) {
 	
 	int writefd,send_msg,rc,hundred_gb,j;
 	long ntimes;
 	char *bufferPos;
 	struct addrinfo *res;
 	res = (struct addrinfo *)addr;

	hundred_gb = 100;	
	ntimes = BUFFER_SIZE/(blocksize*threadCount);
	
	writefd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bufferPos = writebuffer;
	
	if (writefd < 0) {
		printf("Could not create socket!\n");
		exit(-1);
	}

 	rc = connect(writefd, res->ai_addr, res->ai_addrlen);
	if (rc < 0) {
			printf("Could not connect to server!\n");
			exit(-1);
		}
	
	while (ntimes > 0) {
                        if ((send_msg = send(writefd,writebuffer,blocksize, 0)) > 0) {
                                pthread_mutex_lock(&mutexclt);
                                writebuffer = writebuffer + send_msg;
                                pthread_mutex_unlock(&mutexclt);
                        }
                        else {
                                break;
                        }
                ntimes = ntimes -1 ;
        }       	
//	printf("Client bufferpointer is at %d\n",writebuffer);
 	
 	pthread_exit(NULL);
 	close(writefd);
 	return NULL;
}

void ClientProcess(long block,int threadCount) {

	int rc,i;
	char *InitPos;
	struct addrinfo hints, *res;
//	struct sockaddr srv;
	pthread_t thread[threadCount];
	
	writebuffer = (char *)calloc(BUFFER_SIZE,sizeof(char));
	memset(writebuffer,'S',BUFFER_SIZE * sizeof(char));
	InitPos = writebuffer;    //Initialize to start of Client buffer

	memset(&hints,0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	rc = getaddrinfo(hostname1,"11447", &hints, &res);
	if (rc != 0) {
		printf("Could not get address information!\n");
		exit(-1);
	}


	if ( block == 1024 || block == 32000 ) {
		
			pthread_mutex_init(&mutexclt,NULL);

			for (i = 0 ; i < threadCount ; i++){

			//	printf("Creating thread %d\t\n",i);
				pthread_create(&thread[i],NULL,ClientBufferWriter,(void *)res);				
			}
	
			for (i = 0 ; i < threadCount ; i++){
				//printf("Inside Client Join\t\n");
				pthread_join(thread[i],NULL);
			}
			pthread_mutex_destroy(&mutexclt);	
			
	}
	else if ( block == 1) {

		
			gettimeofday(&beforeTime,NULL);			
		
			for (i = 0 ; i < threadCount ; i++){
			//	printf("Creating thread %d\t\n",i);
				pthread_create(&thread[i],NULL,PacketRoundTrip,(void *)res);				
			}

			for (i = 0 ; i < threadCount ; i++){
				//printf("Inside Client Join\t\n");
				pthread_join(thread[i],NULL);
			}	

			gettimeofday(&afterTime,NULL);			
		//pthread_exit(NULL);
		rtt_time = (double)((((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
										((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000))/1000);
		printf("%d\t%d\t%lf\n",blocksize,threadCount,rtt_time);
	}

	freeaddrinfo(res);
	free(InitPos);
	//pthread_exit(NULL);	
}
/////////////// ** CLient Code ends here  */////////////////////

void * ServerBUfferReader(void * sock_desc) {
 	
 	int readfd;
 	ssize_t rcv_size;
 	long ntimes,tempbuff;
 	char *init,*chunk;            //temp char pointers for each thread receving data from client
 	
 	readfd = (int)sock_desc;	
 	ntimes = BUFFER_SIZE/(blocksize*threadCount);
 	tempbuff = BUFFER_SIZE/threadCount;
 	
 	chunk = malloc(tempbuff*sizeof(char));
 	memset(chunk,0,tempbuff);
 	
 	init = chunk;
 	
 	while (ntimes > 0){			
		if ((rcv_size = recv(readfd,chunk,blocksize, 0)) > 0){
				//memset(chunk,'0',tempbuff);
				chunk = chunk + blocksize;		
		}
		else {
			break;
		}
		ntimes =  ntimes - 1 ;
	}
					
 	memcpy(readbuffer,init,tempbuff);		
	readbuffer = readbuffer + tempbuff;   //increment buffer pointer at Server	 
 	free(init);
 	pthread_exit(NULL);
}

void *ServerAck(void * sock_desc)
{
	int readfd,byte_size;
 	char *chunk,ack = 'A';
 	readfd = (int)sock_desc;	
 	
 	chunk = malloc(sizeof(char));

 	while (1){		    //Read untill recv is true and exit after that
		if ((byte_size = recv(readfd,chunk,blocksize, 0)) > 0){
				send(readfd,&ack,blocksize, 0);    	//Send ack once the recv is a success for the furst byte				
			}
		else 	{
			break;
		}
	}

   free(chunk);
   return NULL;
}

void Server(long block,int threadCount) {
	
	size_t sockfd,newfd;
	int rc,i,sock_option;
	char *InitPos;
	
	struct addrinfo hints,*res;
	socklen_t addrlen;
	struct sockaddr_storage clt;

	pthread_t thread[threadCount];

	
	readbuffer = calloc(BUFFER_SIZE,sizeof(char));
	InitPos = readbuffer;       //Initialize at start of buffer  for bytes caculation

//	printf("Initialized buffer at %d\n",InitPos);	

	memset(&hints,0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	rc = getaddrinfo("0.0.0.0","11447",&hints, &res);
	if (rc != 0) {
		printf("Could not get address information!\n");
		exit(-1);
	}

//	printf("Creating SOCKETs ..\n");
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) {
		printf("Could not create socket!\n");
		exit(-1);
	 }

	sock_option = 1;
	rc = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&sock_option,sizeof(int));
	if ( rc < 0){
		printf("Sockopt error");
		exit(-1); 
	}

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		printf("Could not bind socked!");
		close(sockfd);
		exit(-1);
	}

	if (listen(sockfd, 8) == -1) {
		printf("Could not listen to socket!");
		close(sockfd);
		exit(-1);
	}

	if ( block == 1024 || block == 32000 ) {
		
		gettimeofday(&beforeTime,NULL);			

		for (i = 0 ; i < threadCount ; i++){

			newfd = accept(sockfd, (struct sockaddr *)&clt, &addrlen);
			if (newfd < 0) {
				printf("Could not accept client!\n");
				close(sockfd);
				exit(-1);
			}
		//	printf("Creating thread %d\n",i);
			pthread_create(&thread[i],NULL,ServerBUfferReader,(void *)newfd);				
		}

		for (i = 0 ; i < threadCount ; i++){

			pthread_join(thread[i],NULL);
		}


		bytesTransfrd = readbuffer - InitPos;
		gettimeofday(&afterTime,NULL);			

		timeInterval = (double)(((double)afterTime.tv_sec - (double)beforeTime.tv_sec) + \
			((double)(afterTime.tv_usec - beforeTime.tv_usec)/1000000));
		throughput = (bytesTransfrd*80)/(timeInterval * 1024 *1024);

		printf("TCP\t%ld\t%d\t%lf MB/sec\n",block,threadCount,throughput);
	}	
	else if (block == 1) {
		
			//pthread_mutex_init(&mutexsrv,NULL);
		for (i = 0 ; i < threadCount ; i++){

			newfd = accept(sockfd, (struct sockaddr *)&clt, &addrlen);
			if (newfd < 0) {
				printf("Could not accept client!\n");
				close(sockfd);
				exit(-1);
			}
				pthread_create(&thread[i],NULL,ServerAck,(void *)newfd);				
		}

		for (i = 0 ; i < threadCount ; i++){			
				pthread_join(thread[i],NULL);
		}
			
	}
	
	freeaddrinfo(res);	
	close(sockfd);
	free(InitPos);
	//pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	char *s1,*s2,protocol[3];
	long bsize;
	FILE *file1;
	struct hostent *srv;
	

	if (argc < 3){
		printf("Invalid no of arguments...Exiting Main\n");
		return -1;
	}

	s1 = argv[1];
	
	s2 = argv[2];
	
	file1 = fopen(s1,"r");

	if (file1 == NULL) {
 		perror("Error opening file");
       	return(-1);
    }


    while ( EOF != fscanf(file1,"%s\n%ld\n%d",&protocol,&bsize,&threadCount)) {
	}

	/*Converting block sizes to 1024 bytes format as using block sizes in multiples of 10 might return decimal/fraction */
	if (bsize == 1000) {
		blocksize = 1024;
		}
	else if (bsize == 32000) {
		blocksize = 32000;
	}
	else blocksize = bsize;


	if (strcmp(s2,"-S") == 0){
			
			Server(blocksize,threadCount);						
		}
	
	else if (strcmp(s2,"-C") == 0 && argv[3] != NULL){
			
			srv = gethostbyname(argv[3]);          //Get hostname from  argument
			hostname1 = srv->h_name ;
			sleep(10);
			ClientProcess(blocksize,threadCount);		
	}

	pthread_exit(NULL);
	return 0;
}	
