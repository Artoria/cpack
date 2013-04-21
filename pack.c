#include <stdio.h>
#include <string.h>

#define xstr(a)          #a
#define str(a)          xstr(a)
#define DATA(data)  extern char *data; __asm__("_" #data ": .int _"#data"+4 \n\t");
#define TYPED(id, type, a)     __asm__(".int " str(id) "\n\t ." #type " " #a "\n\t")
#define SIZEDTYPE(id, type, a) __asm__(".int " str(id) "\n\t .int 2f-1f\n\t1: ."#type " " #a "\n\t 2:")
#define BLOB(id, a) 	    SIZEDTYPE(id, incbin,a)	
#define STRING(id, a)       SIZEDTYPE(id, asciz, a)
#define STR(id, a)          SIZEDTYPE(id, ascii, a)
#define END(data)  	extern int data##_len; 	__asm__("_" #data"_len : .int _"#data"_len - _"#data "- 4");
#define DATALEN(data)  data##_len
#define SWITCH(t)             __asm__(".section "#t"\n\t");
#define SWITCHDO(t, ...)      __asm__(".section "#t"\n\t"); __VA_ARGS__; __asm__(".section .text""\n\t");

struct Entry{ int type; size_t size; char content[0]; };

#define ENTRY(ent)        ((struct Entry*)(ent))
#define GETENTRY(ent, type)  extern char *type; extern int DATALEN(type); struct Entry *ent = ENTRY(type);
#define NEXTENTRY(ent)    ENTRY((char *)ent + ENTRY(ent)->size + 8);
#define ENDENTRY(ent, type)  ((int)(ent) >= (int)&DATALEN(type))

int verbose = 0;

void PathStripPath(char *s){
  strcpy(s, (strrchr(s, '/') ?: s-1)+1);
}

void mkfiles(){
  GETENTRY(ind,     file_index);
  GETENTRY(content, file_content);
  char cwd[1024];
  getcwd(cwd, 1024);
  while(!ENDENTRY(ind, file_index) && !ENDENTRY(content, file_content)){
   if(ind->type == 1){ //Directory
    chdir(cwd);
    mkdir(ind->content);
    chdir(ind->content);
   }else{
    char s[ind->size];
    memcpy(s, ind->content, ind->size);
    PathStripPath(s);
    FILE *fp = fopen(s, "wb");
    if(verbose){fprintf(stderr, "Extracting %s\n", s);}
    fwrite(content->content, 1, content->size, fp);
    fclose(fp);
   }
    ind = NEXTENTRY(ind);
    content = NEXTENTRY(content);
  }
}

void runstring(){
  GETENTRY(cmd, strings);
  while(!ENDENTRY(cmd, strings)){
    if(cmd->type == 9) system(cmd->content); 
    cmd = NEXTENTRY(cmd);
  }
}

int main(void){
  mkfiles();
  runstring();
  return 0;
}



#define PACKAGE() "pack.h"

DATA(strings)
  STRING(9, "echo Extraction Finished.");
  STRING(9, "echo Will run main.rb");
  STRING(9, "type main.rb");
  STRING(9, "echo.");
  STRING(9, "ruby --disable-gems -I. main.rb");
END(strings)



#define DIR(a)         STRING(1, #a);
#define FILE(a)        STRING(0, #a);
  DATA(file_index)
   #include PACKAGE()   
  END(file_index)
#undef FILE


#define FILE(a)        BLOB(0, #a);
  DATA(file_content)
   #include PACKAGE()   
  END(file_content)
#undef FILE

#undef DIR
