#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <time.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
char *uid_to_name(uid_t uid);
char *gid_to_name(uid_t gid);
void get_modestr(mode_t mode, char modestr[]); //获得权限字符串
void display_dir(char dir[]); //输出目录

int set_flag(char param[]);
int flag_a;
int flag_d;
int flag_i;
int flag_l;
int flag_R;
char *ls = "ls";
char *end = "exit";

int main(){
    
	char cmd[3];
	char param[256];
	char dirname[256];
	while(1){
		scanf("%s",cmd);
		if(strcmp(cmd,ls)==0){
			int isEnd = 0;
			while(isEnd == 0){
				scanf("%s",param);
				if(param[0]=='-'){
					if(set_flag(param)==0){
						printf("参数错误！\n");
						break;
					}
				}else{
					stpcpy(dirname,param);
					display_dir(dirname);
				}
				isEnd = 1;
			}
		}else if(strcmp(cmd,end)==0){
			break;
		}else{
			printf("输入命令有误，请重新输入\n");
		}
	}
	
	return 0;
}
void display_dir(char dir[]){
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat *info;
    char path[256];
    char modestr[11];

    char dirname[256];
	strcpy(dirname, dir);
    if((dp = opendir(dirname)) == NULL){
		printf("No such file or directory\n");
    }else{
		while((entry = readdir(dp)) != NULL){
            //printf("%s\n", entry->d_name);
			strcpy(path,dirname);
			int dir_len = strlen(dirname);
			if(path[dir_len-1]!='/'){
				path[dir_len]='/';
				path[dir_len+1]='\0';
			}
			strcat(path,entry->d_name);
			if(lstat(path, &statbuf)){
				perror("错误：");
			}
			info = &statbuf;
            get_modestr(info->st_mode,modestr);

            printf("%8d ",(int)info->st_ino);
            
            printf("%s ", modestr);
			printf("%2d ", (int)info->st_nlink);
            printf("%9s ", uid_to_name(info->st_uid));
            printf("%9s ", gid_to_name(info->st_gid));
			printf("%.12s",ctime(&info->st_mtime)+4);
			printf("%s\n", entry->d_name);
		}
        closedir(dp);
    }
}
int set_flag(char param[]){
	return 1;
}
void get_modestr(mode_t mode, char modestr[]){
    strcpy(modestr, "----------");
    if(S_ISDIR(mode)) modestr[0] = 'd'; 
    if(S_ISCHR(mode)) modestr[0] = 'c'; 
    if(S_ISBLK(mode)) modestr[0] = 'b'; 
    if(S_ISLNK(mode)) modestr[0] = 'l';

    if(mode & S_IRUSR) modestr[1] = 'r';
    if(mode & S_IWUSR) modestr[2] = 'w';
    if(mode & S_IXUSR) modestr[3] = 'x';

    if(mode & S_IRGRP) modestr[4] = 'r';
    if(mode & S_IWGRP) modestr[5] = 'w';
    if(mode & S_IXGRP) modestr[6] = 'x';

    if(mode & S_IROTH) modestr[7] = 'r';
    if(mode & S_IWOTH) modestr[8] = 'w';
    if(mode & S_IXOTH) modestr[9] = 'x';
}

char *uid_to_name(uid_t uid){
    static char numstr[10];
    struct passwd *pw_ptr;

    if((pw_ptr = getpwuid(uid)) == NULL){
        sprintf(numstr, "%d", uid);
        return numstr;
    }else
        return pw_ptr->pw_name;
}
char *gid_to_name(uid_t gid){
    static char numstr[10];
    struct group *grp_ptr;

    if((grp_ptr = getgrgid(gid)) == NULL){
        sprintf(numstr, "%d", gid);
        return numstr;
    }else
        return grp_ptr->gr_name;
}