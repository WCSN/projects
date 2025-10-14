#!/bin/bash

ARGV0=$0 
ARGV1=$1
ARGV2=$2

PROGRAMM="./aspd.py"

function help()
{
  infotab
}

function infotab()
{
  echo -e "------------------------------------------------------"    
  echo -e " Send:"
  echo -e " [ 1]-all" 
  echo -e " [ 2]-avos          [ 3]-climat    [22]-фап/faw\n"
  echo -e " [ 4]-svc           [ 5]-svc_all\n"
  echo -e " [ 6]-vos           [ 7]-vos_att   [ 8]-vos_email"
  echo -e " [ 9]-vos_att_email [10]-vos_all\n"                                       
  echo -e " [11]-vrs           [12]-vrs_big   [13]-vrs_email"
  echo -e " [14]-vrs_real\n"
  echo -e " [15]-letter        [21]-interactive letter"
  echo -e "------------------------------------------------------"
  echo -e " Recv:"
  echo -e " [16]-recv"
  echo -e "------------------------------------------------------"
  echo -e " List:"
  echo -e " [17]-rlist         [18]-llist"
  echo -e "------------------------------------------------------"
  echo -e " Service:"
  echo -e " [19]-abook         [20]-status"
  echo -e "------------------------------------------------------"   
  echo -e " Other:"
  echo -e " [99]-clean"
  echo -e "------------------------------------------------------"
}

function clean_dir()
{
  echo "== Clean =="
  echo "$1 $2 Clean Send and Outbox directory:"
#  sudo rm -R ./store02/aspd_debug/*_Archive
  sudo rm -R ./store02/aspd_debug/outbox/*
  sudo rm -R ./store02/aspd_debug/Send/*
}

function views()
{
  echo "==== MSG $1 ==========================================="
  cat  "$1"
  echo "==== MSG $1 ==========================================="
}

function genbfz()
{
  echo "Generate bfz"
  dd if=/dev/random of=./tmp/file_512K.dat bs=512 count=1024
  for (( i=0; i<4; i++ )); do
	cat ./tmp/file_512K.dat >> ./src/examples/file_2M.bfz
  done
}

function runsend()
{
    if [ -z $ARGV2 ]; then echo "=== TEST BEGIN ============================================================="; fi
    touch ./store02/aspd_debug/infobox/send
    if [ -z $ARGV2 ]; then $PROGRAMM send; 
    echo "=== TEST END ==============================================================="; fi
}

function menu()
{
  echo "=== Выбор теста ================================="
  infotab
  echo "================================================="
}

function test_vos()
{
    echo "VOS"   
    cp ./src/examples/vos.txt ./store02/aspd_debug/outbox/
    views ./src/examples/vos.txt
    runsend
}

function test_vrs()
{
    cp ./src/examples/vrs.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/
    ls -1 ./store02/aspd_debug/outbox/
    views ./src/examples/vrs.txt
    runsend
}

function test_vrs_big()
{
    mkdir -p ./store02/aspd_debug/outbox/TESTDIR01
    mkdir -p ./store02/aspd_debug/outbox/TESTDIR02
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/TESTDIR01/
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/TESTDIR02/
    cp ./src/examples/vrs.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/
    genbfz
    cp ./src/examples/file_2M.bfz ./store02/aspd_debug/outbox/
    ls -1 ./store02/aspd_debug/outbox/
    views ./src/examples/vrs.txt
    runsend
    rm ./tmp/file_512K.dat
    rm ./src/examples/file_2M.bfz
}

function test_vrs_real()
{
    cp ./src/examples/vrs.txt ./store02/aspd_debug/outbox/
    ls -1 ./store02/aspd_debug/outbox/
    views ./src/examples/vrs.txt
    runsend
}

function test_vrs_email()
{
    cp -v ./src/examples/vrs_email.txt ./store02/aspd_debug/outbox/vrs.txt
    cp -v ./src/examples/*.doc* ./store02/aspd_debug/outbox/
    ls -1 ./store02/aspd_debug/outbox/
    views ./store02/aspd_debug/outbox/vrs.txt
    runsend
}

function test_vos_att()
{
    cp ./src/examples/vos_att.txt ./store02/aspd_debug/outbox/  
    cp ./src/examples/vos_fatt02.bz2 ./store02/aspd_debug/outbox/  
    cp ./src/examples/vos_fatt01.bz2 "./src/examples/vos file attach 01.bz2"

    views ./src/examples/vos_att.txt
    runsend
}

function test_vos_email()
{
    cp ./src/examples/vos_email.txt ./store02/aspd_debug/outbox/
    views ./src/examples/vos_email.txt
    runsend
}

function test_vos_att_email()
{
    cp ./src/examples/vos_att_email.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/*.docx ./store02/aspd_debug/outbox/
    views ./src/examples/vos_att_email.txt
    runsend
}

function test_vos_all()
{
    cp ./src/examples/vos*.txt ./store02/aspd_debug/outbox/
    runsend
}

function test_avos()
{
    cp ./src/examples/avos.txt ./store02/aspd_debug/outbox/
    views ./src/examples/avos.txt
    runsend
}

function test_climat()
{
    cp ./src/examples/avosclimat.txt ./store02/aspd_debug/outbox/
    views ./src/examples/avosclimat.txt
    runsend
}

function test_svc()
{
    cp ./src/examples/svc.txt ./store02/aspd_debug/outbox/
    views ./src/examples/svc.txt
    runsend
}

function test_svc_all()
{
    cp ./src/examples/svc*.txt ./store02/aspd_debug/outbox/
    views ./src/examples/svc*.txt
    runsend
}

function test_letter()
{
    views ./src/examples/letter.txt
    aspd m admin_vostok@aari.ru -s "Test e-mail" -f ./src/examples/letter.txt -a ./testaspd.sh -t "1. Добавиться текст из файла: ./src/examples/letter.txt\n 2. добавится аттач: ./testaspd\3. Добавится текст из -t\n"
}

function test_recv()
{
    test_vos
    runsend
    if [ -z $ARGV2 ]; then $PROGRAMM recv; fi
}

function test_rlist()
{
    if [ -z $ARGV2 ]; then $PROGRAMM rlist; fi
}

function test_llist()
{
    if [ -z $ARGV2 ]; then $PROGRAMM llist; fi
}

function test_abook()
{
    if [ -z $ARGV2 ]; then $PROGRAMM abook; fi
}

function test_status()
{
    if [ -z $ARGV2 ]; then $PROGRAMM status; fi
}

function test_iletter()
{
    if [ -z $ARGV2 ]; then $PROGRAMM letter; fi
}

function test_awrletter()
{
    if [ -z $ARGV2 ]; then $PROGRAMM letter grp ФАП awr; fi
}

#==========================================================================
function test_all()
{
    mkdir -p ./store02/aspd_debug/outbox/TESTDIR01
    mkdir -p ./store02/aspd_debug/outbox/TESTDIR02
    genbfz
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/TESTDIR01/
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/TESTDIR02/
    cp ./src/examples/avos*.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/svc*.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/vos*.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/vrs*.txt ./store02/aspd_debug/outbox/
    cp ./src/examples/*.doc* ./store02/aspd_debug/outbox/
    cp ./src/examples/file_2M.bfz ./store02/aspd_debug/outbox/
    runsend
}
###########################################################################
declare -a TYPETEST

if [ -z $1 ]; then
    menu
    read -p "Run test: (№/h/a)==> " TYPETEST
    echo "Выполняется #: $TYPETEST"
else 
    TYPETEST="$1"
fi

for TTEST in $TYPETEST; do
    echo "Выполняется #: $TTEST"
    
    if ! [[ "$TYPETEST" == "14" || "$TYPETEST" == "vrs_real" ]]; then
        clean_dir $0 "$TYPETEST"
    fi

    case "$TTEST" in
    1|all)
        echo "ALL" 
        test_all
    ;;
    2|avos)
        echo "AVOS"  
        test_avos
    ;;
    3|climat)
        echo "CLIMAT"   
        test_climat
    ;;
    4|svc)
        echo "SVC"      
        test_svc    
    ;;
    5|svc_all) 
        echo "SVC_ALL"   
        test_svc_all
    ;;
    6|vos) 
        echo "VOS"   
        test_vos
    ;;
    7|vos_att) 
        echo "VOS_ATT"   
        test_vos_att
    ;;
    8|vos_email)
        echo "VOS_EMAIL"      
        test_vos_email
    ;;
    9|vos_att_email)
        echo "VOS_ATT_EMAIL"      
        test_vos_att_email
    ;;
    10|vos_all) 
        echo "VOS_ALL"     
        test_vos_all
    ;;
    11|vrs) 
        echo "VRS"   
        test_vrs
    ;;
    12|vrs_big) 
        echo "VRS_BIG"     
        test_vrs_big
    ;;
    13|vrs_email) 
        echo "VRS_EMAIL"     
        test_vrs_email
    ;;
    14|vrs_real) 
        echo "VRS_BIG"     
        test_vrs_real
    ;;
    15|letter)
        echo "E-MAIL SEND admin_vostok@aari.ru"     
        test_letter
    ;;
    16|recv) 
        echo "RECV"     
        test_recv
    ;;
    17|rlist) 
        echo "Remote List"     
        test_rlist
    ;;
    18|llist) 
        echo "Local List"     
        test_llist
    ;;
    19|abook) 
        echo "Adress Book"     
        test_abook
    ;;
    20|status) 
        echo "Status"     
        test_status
    ;;
    21|iletter) 
        echo "Status"     
        test_iletter
    ;;
    22|awrlt) 
        echo "Status"     
        test_awrletter
    ;;
    99|clean) 
        echo "CLEAN ALL TEST DIR"     
        clean_dir $0 "CLEAN"
    ;;
    h|H|help) 
        echo "HELP"     
        echo "=== help ================================="
        help;
        echo "=== help ================================="
    ;;
    a|A|e|E|q|Q)
        a=0
    ;;
    *) echo "Test \"$TYPETEST\" unknown type!"
    ;;
    esac
done

echo "ALL END"

