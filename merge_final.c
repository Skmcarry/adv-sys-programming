#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
 

char *reverse_string(char *str);
 
#define SIZE 104857600
 
int
main(int argc, char *argv[])
{
	int file1, file2;
	FILE *fout;
	char *buffer1, *buffer2,*token1,*token2,*file1_arr,*file2_arr;
	char *file1_temp,*file2_temp;
	long line1=0,line2=0, lineout = 0;
	struct timeval before, after;
	int duration;
	int ret = 1;
 
	if (argc != 4) {
		fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
		goto leave0;
	}
	if ((file1 = open(argv[1], O_RDONLY)) ==-1) {
		perror(argv[1]);
		goto leave0;
	}
	if ((file2 = open(argv[2], O_RDONLY)) ==-1) {
		perror(argv[2]);
		goto leave1;
	}
	if ((fout = fopen(argv[3], "wt")) == NULL) {
		perror(argv[3]);
		goto leave2;
	}
 
		
	buffer1 = (char*)malloc(sizeof(char)*SIZE);
	if ((buffer1 == NULL)){
		fprintf(stderr, "Memory allocation error of file1\n");
		goto leave3;
	}
	buffer2 = (char*)malloc(sizeof(char)*SIZE);
	if ((buffer2 == NULL)){
		fprintf(stderr, "Memory allocation error of file2\n");
		goto leave4;
	}
 
	
	if (read(file1, buffer1, SIZE) < 1){
		fprintf(stderr, "Error while reading file1\n");
		goto leave5;
	}
	if (read(file2, buffer2, SIZE) < 1){
		fprintf(stderr, "Error while reading file2\n");
		goto leave3;
	}
 

	gettimeofday(&before, NULL);
	
	token1 = strtok_r(buffer1, "\n", &file1_arr);
	token2 = strtok_r(buffer2, "\n", &file2_arr);
 
	for(file1_temp=buffer1,file2_temp=buffer2;;file1_temp=NULL,file2_temp=NULL){
		if (token1 != NULL) {
			fwrite(reverse_string(token1),strlen(token1),1,fout);
			fputc(0x0a,fout);
			token1=strtok_r(NULL,"\n",&file1_arr);
			line1++;
		}

		if (token2 != NULL) {
			fwrite(reverse_string(token2),strlen(token2),1,fout);
			fputc(0x0a,fout);
			token2=strtok_r(NULL,"\n",&file2_arr);
			line2++;
		}
		if(token1 != NULL && token2 == NULL)
			break;
	}
 	lineout=line1+line2;
	gettimeofday(&after, NULL);
 
	duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
	printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
	printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
	ret = 0;
leave5:
	free(buffer1);
leave4:
	free(buffer2);
leave3:
	fclose(fout);
leave2:
	close(file2);
leave1:
	close(file1);
leave0:
	return ret;
}

 
char *reverse_string(char *str){
	int i;
	char temp;
	int length = strlen(str);
 
	for (i = 0; i < length / 2; i++) {
		temp = str[i];
		str[i] = str[(length - 1) - i];
		str[(length - 1) - i] = temp;
	}
	return str;
}

