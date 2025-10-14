@echo off

set FL="\\SFRTVS-001\ASPD$\infobox\send"
set LOG="\\SFRTVS-001\ASPD$\infobox\log\aspd.log"

FOR /F "usebackq" %%a IN (`now -q`) DO ( 
set tm=%%a 
)

FOR /F "usebackq" %%a IN (`now -d`) DO ( 
set dt=%%a 
)

echo %dt% > %TEMP%\tmp.tmp

FOR /F "usebackq" %%a IN (`cut -f1-2 -d"." %TEMP%\tmp.tmp`) DO ( 
set dt=%%a 
)

echo %USERNAME% > %FL%

echo [%COMPUTERNAME%:%USERNAME%] Запущена отправка. Ожидайте ~1 мин %dt%%tm%  >> %LOG%

