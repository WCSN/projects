@echo off

cp .\.conf\aari_pro00 .\aari.pro

cp .\.conf\brga_pro01 .\brga\brga.pro
cat .\.conf\brga_prowin >> .\brga\brga.pro
cat .\.conf\brga_pro02 >> .\brga\brga.pro

qmake -o Makefile aari.pro

cat .\.conf\addmake >> .\Makefile
cp .\.conf\.gitignore .\
