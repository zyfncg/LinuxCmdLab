#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
char *uid_to_name(uid_t uid);
char *gid_to_name(uid_t gid);
void get_modestr(mode_t mode, char modestr[]);

int main(){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct stat *info;
    char name[256];
    char modestr[11];

    char dirname[] = {"linuxlab"};
    if((dp = opendir(dirname)) == NULL){
		printf("error\n");
    }else{
		while((entry = readdir(dp)) != NULL){
            printf("%s\n", entry->d_name);
			lstat(entry->d_name, &statbuf);
			info = &statbuf;
            strcpy(name,entry->d_name);
            get_modestr(info->st_mode,modestr);

            printf("%d ",(int)info->st_ino);
            printf("%d ", (int)info->st_nlink);
            printf("%s ", modestr);
            printf("%9s ", uid_to_name(info->st_uid));
            printf("%9s ", gid_to_name(info->st_gid));
			printf("%s\n", name);
		}
        closedir(dp);
    }

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