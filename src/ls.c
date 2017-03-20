#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <time.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>

/*
 * 测试命令
 * ls /home/zyfncg/LinuxCmdLab/linuxlab
 */

//定义展示的文件信息
typedef struct stat_info{
	unsigned int i_node;
	char mode[11];
	int link_num;
	char owner[10];
	char group[10];
	long size;
	char mod_time[16];
	char name[256];
	char path[256];
}file_stat;
file_stat files[256];

char *uid_to_name(uid_t uid);
char *gid_to_name(uid_t gid);
void get_modeStr(mode_t mode, char *modestr); //获得权限字符串
void sort_files(file_stat stats[], int size);
int get_stats(char dir[], file_stat stats[]); //获取目录下的文件信息,返回目录下文件的数量
void display_file_info(file_stat *file); //根据flag输出单个文件信息
void display_dirs(file_stat files[], int size);


void execute(char dir[]); //输出目录
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
	char dirName[256];
    int isDefault;
	while(1){
        printf("%d\n",scanf("%s",cmd));
		if(strcmp(cmd,ls)==0){
			int isEnd = 0;
            dirName[0] = '.';
            dirName[1] = '\0';
			while(isEnd == 0){
				if(scanf("%s",param) > 0){
                    printf("%s\n",param);
                    if(param[0]=='-'){
                        if(set_flag(param)==0){
                            printf("参数错误！\n");
                            break;
                        }
                    }else{
                        stpcpy(dirName,param);
                        isEnd = 1;
                    }
                }else{
                    isEnd = 1;
                }
			}
            execute(dirName);
		}else if(strcmp(cmd,end)==0){
			break;
		}else{
			printf("输入命令有误，请重新输入\n");
		}
	}
	
	return 0;
}

int get_stats(char dir[], file_stat stats[]){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	char path[256];
	char modestr[11];
	char dirname[256];
	int count = 0;
	strcpy(dirname, dir);
	if((dp = opendir(dirname)) == NULL){
		printf("No such file or directory\n");
	}else{
		while((entry = readdir(dp)) != NULL){
			strcpy(path,dirname);
			int dir_len = strlen(dirname);
			if(path[dir_len-1]!='/'){
				path[dir_len]='/';
				path[dir_len+1]='\0';
			}
			strcat(path,entry->d_name);
			if(lstat(path, &statbuf)){
				perror("错误：");
				return 0;
			}
			get_modeStr(statbuf.st_mode, modestr);
			stats[count].i_node = statbuf.st_ino;
			strcpy(stats[count].mode,modestr);
            stats[count].link_num = statbuf.st_nlink;
			strcpy(stats[count].owner,uid_to_name(statbuf.st_uid));
			strcpy(stats[count].group,gid_to_name(statbuf.st_gid));
			strcpy(stats[count].mod_time,ctime(&(statbuf.st_mtime))+4);
            stats[count].size = statbuf.st_size;
			strcpy(stats[count].name,entry->d_name);
			strcpy(stats[count].path,path);

            count++;
		}
		closedir(dp);
        sort_files(stats,count);
	}
    return count;
}
void execute(char dir[]){
    file_stat stats[256];
    int items_num = get_stats(dir,stats);
    display_dirs(stats,items_num);
}
void sort_files(file_stat stats[], int size){
    file_stat temp;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size-i-1; ++j) {
            if(strcmp(stats[j].name,stats[j+1].name) > 0){
                temp = stats[j+1];
                stats[j+1] = stats[j];
                stats[j] = temp;
            }
        }
    }
}
void display_dirs(file_stat files[], int size){
    for (int i = 0; i < size; ++i) {
        display_file_info(&files[i]);
    }
}
void display_file_info(file_stat *file){
    printf("%-8d ",file->i_node);
    printf("%s ",file->mode);
    printf("%2d ",file->link_num);
    printf("%8s ",file->owner);
    printf("%8s ",file->group);
    printf("%8ld  ",file->size);
    printf("%.12s  ",file->mod_time);
    printf("%s\n",file->name);
}
int set_flag(char param[]){
    flag_a = 0;
    flag_d = 0;
    flag_i = 0;
    flag_l = 0;
    flag_R = 0;
    if(strlen(param)==2){
        switch (param[1]){
            case 'a': flag_a = 1;break;
            case 'd': flag_d = 1;break;
            case 'i': flag_i = 1;break;
            case 'l': flag_l = 1;break;
            case 'R': flag_R = 1;break;
            default:
                return 0;
        }
        return 1;
    }else{
        return 0;
    }
}
void get_modeStr(mode_t mode, char *modestr){
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