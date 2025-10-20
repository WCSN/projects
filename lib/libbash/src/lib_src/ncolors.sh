############################################################################
##
## libbash part - color function
##
## wocson (c) 2025 BSD
##
############################################################################

BLK=`tput setaf 0`
RED=`tput setaf 1`
GRN=`tput setaf 2`
YEL=`tput setaf 3`
BLU=`tput setaf 4`
MAG=`tput setaf 5`
CYA=`tput setaf 6`
WHT=`tput setaf 7`

BLD=`tput bold`
RST=`tput sgr0`

BBLK=`tput setaf 0`
BRED=`tput setaf 1`
BGRN=`tput setaf 2`
BYEL=`tput setaf 3`
BBLU=`tput setaf 4`
BMAG=`tput setaf 5`
BCYA=`tput setaf 6`
BWHT=`tput setaf 7`


echo -e "=${BLK}BLACK${RST}"
echo -e "=${RED}RED${RST}"
echo -e "=${GRN}GREEN${RST}"
echo -e "=${YEL}YELLOW${RST}"
echo -e "=${BLU}BLUE${RST}"
echo -e "=${MAG}MAGENTA${RST}"
echo -e "=${CYA}CYAN${RST}"
echo -e "=${WHT}WHITE${RST}"

#===================================

echo -e "=${BLD}${BLK}BLACK${RST}"
echo -e "=${BLD}${RED}RED${RST}"
echo -e "=${BLD}${GRN}GREEN${RST}"
echo -e "=${BLD}${YEL}YELLOW${RST}"
echo -e "=${BLD}${BLU}BLUE${RST}"
echo -e "=${BLD}${MAG}MAGENTA${RST}"
echo -e "=${BLD}${CYA}CYAN${RST}"
echo -e "=${BLD}${WHT}WHITE${RST}"
