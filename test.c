#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat *info;
    {
    	
    };
    char dirname[] = {"test"};
    if((dp = opendir(dirname)) == NULL){
		printf("error\n");
    }else{
		while((entry = readdir(dp)) != NULL){
			lstat(entry->d_name, &statbuf);
			info = &statbuf;
			printf("%s\n", info->st_uid);
		}
    }

}