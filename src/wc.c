#include <stdio.h>
#include <string.h>
int isWord(char c);//判断是否是单词的一个字母
void show_file_info(char file_name[]);
int total_chars,total_lines,total_words;
int main(int argc, char *argv[]){
    total_chars = 0;
    total_lines = 0;
    total_words = 0;
    char file_name[256];
    if(argc > 1){
        for (int i = 1; i < argc; ++i) {
            strcpy(file_name,argv[i]);
            show_file_info(file_name);
        }
        if(argc>2){
            printf("%d %d %d 总用量\n",total_lines,total_words,total_chars);
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
    total_chars+=chars;
    total_lines+=lines;
    total_words+=words;
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