@echo off
set summ=0 
for %%� in (Dataset\*.jpg) do (Binarisation.exe %%� Result\%%� )
echo %summ%
pause

