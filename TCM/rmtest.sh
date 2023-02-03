#/bin/sh

DECODE_IMG_SAVE_DAY=7
DECODE_BIN_SAVE_DAY=7
ENCODE_BIN_SAVE_DAY=7
STAT_SAVE_DAY=7

echo "============== decode.img =============="
find /home/ibc/ibc/src/vIBC/trunk/src/NAS_INTERNAL -type f -name "??????????????????????_decode\.img" -ctime +$DECODE_IMG_SAVE_DAY -exec ls {} \;

echo "============== decode.tmp =============="
find /home/ibc/ibc/src/vIBC/trunk/src/NAS_INTERNAL -type f -name "??????????????????????_decode\.tmp" -ctime +$DECODE_BIN_SAVE_DAY -exec ls {} \;

echo "============== encode.tmp =============="
find /home/ibc/ibc/src/vIBC/trunk/src/NAS_INTERNAL -type f -name "??????????????????????_encode\.tmp" -ctime +$ENCODE_BIN_SAVE_DAY -exec ls {} \;

echo "============== TRSG_MSG.log =============="
find /home/ibc/ibc/src/vIBC/trunk/src/stat/TRSG -type f -name "TRSG_MSG\.???\.????????????\.log" -ctime +$STAT_SAVE_DAY -exec ls {} \;



