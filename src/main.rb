require 'Fiddle'
include Fiddle
x = Fiddle.dlopen("user32")
Function.new(x['MessageBoxA'],[TYPE_VOIDP]*4, TYPE_VOIDP).call 0, "Hello", 0, 16