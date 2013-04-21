#ifndef ONCE
  #define ONCE
  DATA(strings)
    STRING(9, "echo Extraction Finished.");
    STRING(9, "echo Will run main.rb");
    STRING(9, "type main.rb");
    STRING(9, "echo.");
    STRING(9, "ruby --disable-gems -I. main.rb");
  END(strings)
#endif


DIR(main)                                                    
  FILE(C:/ruby200/bin/ruby.exe)                             
  FILE(C:/ruby200/bin/msvcrt-ruby200.dll)                   
  FILE(main.rb)                                             
  FILE(ubygems.rb)                                         
