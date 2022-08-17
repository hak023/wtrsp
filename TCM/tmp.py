import zlib
import time
import threading
import sys
import logging
import datetime
import xml.etree.ElementTree as ET
from socket import *

#Server Info
HOST = "121.134.202.25"
PORT = 30001

#global variable
TransactionID = 0
SendAliveCheckTimer = 1
SHUTDOWN = False
lock = threading.Lock()
SessionID = ''


senddatatest='1234567890'
print(senddatatest[3:])
dataLen=len(senddatatest)

start=0
split=3
end=split
print('dataLen:%d' % dataLen)

while True :
    time.sleep(1)
    print(senddatatest[start:end])

    start = start + split
    end = end + split

    if dataLen <= end :
        print(senddatatest[start:])
        break
