package.path = package.path .. ";./scr/lib/moon/?.lua"

if jit.arch == "x86" then
  package.cpath = package.cpath .. ";./scr/lib/x86/?.dll"
else
  package.cpath = package.cpath .. ";./scr/lib/x64/?.dll"
end
