#/bin/sh

multiple_cmd() { 
    ls -al $1; 
    ls $1 
}; 
export -f multiple_cmd; 

find /home/ibc/ibc/src/vIBC/trunk/src/stat/TRSG -type f -name "TRSG_MSG.001.20220711155*" -ctime +7 -exec bash -c 'multiple_cmd "$0"' {} \;
