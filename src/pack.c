#include <stdio.h>
#include <windows.h>
#include <string.h>

#include "data.h"
int verbose = 0;
char cwd[1024];

char* p_get_filename(char *dest, char *src){
  return strcpy(dest, (strrchr(src, '/') ?: src-1)+1);
}



#include "handlers.h"

void mkfiles(){
  getcwd(cwd, 1024);
  int size = 0;
  int usize = 0;
  {
    GETENTRY(ind,     file_index);
    GETENTRY(content, file_content);
    while(!ENDENTRY(ind, file_index) && !ENDENTRY(content, file_content)){
      handlers[ind->type](ind, content);
      size += content->size;
      ind = NEXTENTRY(ind);
      content = NEXTENTRY(content);
    }
  }
  {
    GETENTRY(ind,     file_index);
    GETENTRY(content, file_content);
    while(!ENDENTRY(ind, file_index) && !ENDENTRY(content, file_content)){
      handlers[ind->type](ind, content);
      usize += content->size;
      printf("%.2lf%% (%d / %d) %s\r", usize * 10000.0 / size / 100.0, usize, size, ind->content);
      ind = NEXTENTRY(ind);
      content = NEXTENTRY(content);
    }
  }
  puts("");
}


void run(char *name){
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  GetStartupInfo(&si);
  CreateProcess(0, name, 0, 0, 0, 0, 0, 0, &si, &pi);
  WaitForSingleObject(pi.hProcess, INFINITE);
}

void runstring(){
  GETENTRY(cmd, strings);
  while(!ENDENTRY(cmd, strings)){
    if(cmd->type == 9) run(cmd->content); 
    cmd = NEXTENTRY(cmd);
  }
}

int main(void){
  mkfiles();
  runstring();
  return 0;
}



#define PACKAGE() "pack.h"
#include "header.h"

#define DIRSPECIAL(a)  SIZEDTYPE(2, int, #a);

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
#undef DIRSPECIAL
