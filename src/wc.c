#include <stdio.h>
#include <memory.h>

int isWord(char c);//判断是否是单词的一个字母
void show_file_info(char file_name[]);
int main(){
    char file_name[256]="/home/zyfncg/CLionProjects/LinuxCMD/linuxlab/file2";
    char cmd[10];
    while (1){
        scanf("%s",cmd);
        if(strcmp(cmd,"wc")==0){
            scanf("%s",file_name);
            show_file_info(file_name);
        } else if(strcmp(cmd,"exit")==0){
            break;
        } else{
            printf("命令输入错误，请重新输入\n");
        }
    }

    return 0;
}
void show_file_info(char file_name[]){
    FILE *file;
    file = fopen(file_name,"r");
    int lines = 0;
    int words = 0;
    int chars = 0;

    char c = getc(file);
    if(c != EOF){
        chars++;
        char prec = c;
        while((c = getc(file)) != EOF){
            chars++;
            if(c=='\n'){
                lines++;
                if(isWord(prec)==1){
                    words++;
                }
            }else if(c==' '){
                if(isWord(prec)==1){
                    words++;
                }
            }
            prec = c;
        }
        if(isWord(prec)){
            words++;
        }
    }
    fclose(file);
    printf("%d %d %d %s\n",lines,words,chars,file_name);
}
int isWord(char c){
    if(c != ' ' && c != '\n'){
        return 1;
    } else{
        return 0;
    }
}