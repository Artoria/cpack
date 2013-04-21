#define IF_NOT_EXIST(a)  "if not exist" #a
DATA(strings)
  STRING(9, "cmd /c " IF_NOT_EXIST(c:\ruby3.0\bin\ruby.exe) IF_NOT_EXIST(main\ruby2.0.exe) "echo Downloading Ruby 2.0 & main\\aria2c.exe -o main\\ruby2.0.exe --check-certificate=false http://rubyforge.org/frs/download.php/76804/rubyinstaller-2.0.0-p0.exe -s 1000");
  STRING(9, "cmd /c " IF_NOT_EXIST(c:\ruby3.0\bin\ruby.exe) " main\\ruby2.0.exe /SILENT /DIR=\"C:\\ruby3.0\"");
  STRING(9, "c:\\ruby3.0\\bin\\ruby main\\main.rb");
END(strings)
