@echo off

cp .\.settings\aari_pro00 .\aari.pro

cp .\.settings\brga_pro01 .\brga\brga.pro
cat .\.settings\brga_prowin >> .\brga\brga.pro
cat .\.settings\brga_pro02 >> .\brga\brga.pro

qmake -o Makefile aari.pro

cat .\.settings\addmake >> .\Makefile
cp .\.settings\.gitignore .\
