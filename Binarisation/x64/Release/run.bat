@echo off
set summ=0 
for %%Â in (Dataset\*.jpg) do (Binarisation.exe %%Â Result\%%Â )
echo %summ%
pause

