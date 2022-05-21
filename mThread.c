#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
// for pid and thread id
#include<sys/types.h>
#include<unistd.h>
#include<sys/syscall.h>

//for defalut no of string
int defaultStringCount = 10;
char *str[200];

//function for taking no of inputs
void noOfString();
// function for taking input strings
void inputString();
//function to print strings;
void printString();  

// functions for thread
void *thread1Print();
void *thread2Print();


int main(int argc, char *argv[]){
	// file for output
	FILE *fp = fopen("output.txt", "w");

	// file to stream , creating dup
	int fd = fileno(fp), copy = dup(STDOUT_FILENO);

	do{
		noOfString();
	} while (defaultStringCount<10);
		
	inputString();

	pthread_t treads[2];
	// redirecting stdout to file using dup2
	dup2(fd,STDOUT_FILENO);
	
	// threads
	pthread_create(&treads[0], NULL, &thread1Print, NULL);  
	pthread_create(&treads[1], NULL, &thread2Print, NULL);  
 	
	for (int i = 0; i < 2; i++){
		pthread_join(treads[i], NULL);
	}

	fflush(stdout);
		fclose(fp);
	// after this dup2 stdout will return to normal
	dup2(copy,1);
	return 0;
}

void inputString(){
	// int c;
	for(int i=0; i<defaultStringCount; i++){
		str[i] = malloc(sizeof(char) * 1024);
		printf("Enter a string no %d \n", i+1);
		
		// to not skip 1st input
		if( i == 0){
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF);
		}
		// getting input string
		fgets(str[i], sizeof(char) * 200, stdin);
		sscanf(str[i], "%[^\n]", str[i]); 
	}
	printf("------------------------------ \n");
};

void noOfString(){
	printf("Enter no of string u wanto to write ?(least 10 required !!!) \n");
	scanf("%d", &defaultStringCount);
	int temp = defaultStringCount;
	if(temp<10){
		printf("-------------------------- \n");
		printf("No is less than 10. Enter either 10 or greater than 10 !!!\n");
		printf("\n");
	}
};

void printString(){
	for(int i=0; i < defaultStringCount; i++){
		if(i % 2 ==0 ){
			printf("%s \n", str[i]);
		}
	}
}

void *thread1Print(){
	//process id
	pid_t t1_id;
	//Thead id
	t1_id = syscall(SYS_gettid);
	for(int i=0; i < defaultStringCount; i++){
		time_t t1_time;
		t1_time = time(NULL);
		if(i % 2 ==0 ){
			printf("string => %s ", str[i]);
			printf("::->>> Thread_id :-> %d ", t1_id);
			printf("::->>> Thread_time :-> %s ", asctime(localtime(&t1_time)));
			printf("\n");
			// assigning 0 to string 
			str[i]=0;
		}
		sleep(1);
	}
}

void *thread2Print(){
	//process id
	pid_t t2_id;
	//Thead id
	t2_id = syscall(SYS_gettid);
	for(int i=0; i < defaultStringCount; i++){
		time_t t2_time;
		t2_time = time(NULL);
		if(i % 2 !=0 ){
			printf("string => %s ", str[i]);
			printf("::->>> Thread_id :-> %d ", t2_id);
			printf("::->>> Thread_time :-> %s ",asctime(localtime(&t2_time)));
			printf("\n");
			// assigning 0 to string 
			str[i]=0;
		}
		sleep(1);
	}
}
