#-*- coding: euc-kr -*-
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

def GetTid():
    global TransactionID
    if TransactionID == 2147483647 :
        TransactionID = 0
    TransactionID = TransactionID + 1
    return TransactionID

def TcpSendMsg(SendMsg):
    compress_msg = zlib.compress(SendMsg.encode('utf-8'))
    header='WTRSTP/1.00000000000000%07d' % len(compress_msg)
    c.send(header.encode('utf-8') + compress_msg)
    logger.info('[SEND]' + SendMsg)

def SendEstablishSessionRequest():
    global lock
    lock.acquire()

    EstablishSessionRequest = '<?xml version="1.0"?>'
    EstablishSessionRequest += '<WTRS.MSG Version="1.0">'
    EstablishSessionRequest += '<HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="" To="urn:wtrsp:system:wtrsg" TransactionID="%d">'  % (GetTid())
    EstablishSessionRequest += '<Error Code="" Description="" Source=""/>'
    EstablishSessionRequest += '</HEADER>'
    EstablishSessionRequest += '<BODY>'
    EstablishSessionRequest += '<EstablishSessionRequest>'
    EstablishSessionRequest += '<Endpoint ID="urn:wtrsp:auth.endpoint:MMSC_ACPT" IP="192.168.230.181" PWD="MMSC_DEV_TEST">'
    EstablishSessionRequest += '</Endpoint>'
    EstablishSessionRequest += '</EstablishSessionRequest>'
    EstablishSessionRequest += '</BODY>'
    EstablishSessionRequest += '</WTRS.MSG>'
    TcpSendMsg(EstablishSessionRequest)
    lock.release()

def SendCloseSessionRequest():
    global lock
    global SessionID
    lock.acquire()
    
    CloseSessionRequest = '<?xml version="1.0"?>'
    CloseSessionRequest += '<WTRS.MSG Version="1.0">'
    CloseSessionRequest += '<HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="%s" To="urn:wtrsp:system:wtrsg" TransactionID="%d">' % (SessionID, GetTid())
    CloseSessionRequest += '<Error Code="" Description="" Source="" />'
    CloseSessionRequest += '</HEADER>'
    CloseSessionRequest += '<BODY>'
    CloseSessionRequest += '<CloseSessionRequest/>'
    CloseSessionRequest += '</BODY>'
    CloseSessionRequest += '</WTRS.MSG>' 
    
    TcpSendMsg(CloseSessionRequest)
    lock.release()

def SendCreateJobRequest():
    global lock
    global SessionID
    lock.acquire()
    
    CreateJobRequest = '<?xml version="1.0"?>'
    CreateJobRequest += '<WTRS.MSG Version="1.0">'
    CreateJobRequest += '<HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="%s" To="urn:wtrsp:system:wtrsg" TransactionID="%d">' % (SessionID,GetTid())
    CreateJobRequest += '<Error Code="" Description="" Source=""/>'
    CreateJobRequest += '</HEADER>'
    CreateJobRequest += '<BODY>'
    CreateJobRequest += '<CreateJobRequest>'
    CreateJobRequest += '<Job Kind="Realtime" ServiceName="Z1yp1whm">'
    CreateJobRequest += '<SourceContentMap>'
    CreateJobRequest += '<SourceContent File="Winner_Winner_Funky_Chicken_Dinner.mp3" ID="1" NASCode="67" Path="vwtrss/mp3">'
    CreateJobRequest += '<Container ID="MP3">'
    CreateJobRequest += '<VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width=""/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '<Metadata Album="New Single Album 이유씨케이알 앨범" Artist="이유씨케이알 아티스트" Copyright="이유씨케이알 카피라이트" Date="0721" Genre="POP" ImageData="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" ImageType="JPEG" PlayDevice="" PlayTime="0" Site="" Title="mungmung!" Track="7" Year="2022" />'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</SourceContent>'
    CreateJobRequest += '</SourceContentMap>'
    CreateJobRequest += '<TargetContentMap>'
    CreateJobRequest += '<TargetContent BinaryData="" File="MetaTestFile_1.mp3" ID="1" NASCode="10" Path="vwtrss/test">'
    CreateJobRequest += '<Container ID="MP3C">'
    CreateJobRequest += '<VideoCodec BitRate="0" Height="0" ID="NONE" MaxFrameRate="0" VariableFrameRate="0" Width="0"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="16" BitRate="32000" Channel="1" ID="MP3C" SampleRate="44100"/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="MP3TestFile_2.mp3" ID="2" NASCode="10" Path="vwtrss/test">'
    CreateJobRequest += '<Container ID="MP3">'
    CreateJobRequest += '<VideoCodec BitRate="0" Height="0" ID="NONE" MaxFrameRate="0" VariableFrameRate="0" Width="0"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="16" BitRate="320000" Channel="2" ID="MP3" SampleRate="44100"/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="MetaTestFile_3.wma" ID="3" NASCode="10" Path="vwtrss/test">'
    CreateJobRequest += '<Container ID="WMA">'
    CreateJobRequest += '<VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width=""/>'
    CreateJobRequest += '<AudioCodec BitPerSample="16" BitRate="32000" Channel="2" ID="WMA2" SampleRate="44100"/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '</TargetContentMap>'
    CreateJobRequest += '<TranscodingList>'
    CreateJobRequest += '<Transcoding SourceContentID="1" TargetContentID="1"/>'
    CreateJobRequest += '<Transcoding SourceContentID="1" TargetContentID="2"/>'
    CreateJobRequest += '<Transcoding SourceContentID="1" TargetContentID="3"/>'
    CreateJobRequest += '</TranscodingList>'
    CreateJobRequest += '</Job>'
    CreateJobRequest += '</CreateJobRequest>'
    CreateJobRequest += '</BODY>'
    CreateJobRequest += '</WTRS.MSG>'
    
    TcpSendMsg(CreateJobRequest)
    lock.release()

def SendAliveCheck():
    global SHUTDOWN
    global lock
    global SessionID
    global SendAliveCheckTimer
    time.sleep(2)
    while not SHUTDOWN :
        lock.acquire()
        
        LinkTestRequest = '<?xml version="1.0"?>'
        LinkTestRequest += '<WTRS.MSG Version="1.0">'
        LinkTestRequest += '<HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="%s" To="urn:wtrsp:system:wtrsg" TransactionID="%d">' % (SessionID, GetTid())
        LinkTestRequest += '<Error Code="" Description="" Source=""/>'
        LinkTestRequest += '</HEADER>'
        LinkTestRequest += '<BODY>'
        LinkTestRequest += '<LinkTestRequest/>'
        LinkTestRequest += '</BODY>'
        LinkTestRequest += '</WTRS.MSG>' 
        
        TcpSendMsg(LinkTestRequest)
        lock.release()
        time.sleep(SendAliveCheckTimer)

def RecvTcpData():
    global SHUTDOWN
    global SessionID
    while not SHUTDOWN :
        header = c.recv(30)
        bodySizeStr = header[10:]
        
        body = c.recv(int(bodySizeStr))
        recvLen = len(body)

        while recvLen < int(bodySizeStr) :
            contineRecv = (int(bodySizeStr) - recvLen)
            body += c.recv(abs(contineRecv))
            recvLen = int(len(body))
        
            print('byte[body size : %d/%d]' % (len(body), int(bodySizeStr)))
        else :
            print('byte[body size : %d/%d] ok' % (len(body), int(bodySizeStr)))
        
        try:
            decompress_msg = zlib.decompress(body)
        except Exception as e:
            continue
        decompress_msg = decompress_msg[0:-1]
        logger.info('[RECV]' + decompress_msg.decode('utf-8'))
        
        root = ET.fromstring(decompress_msg)

        body = root.find('BODY')
        if body.find('EstablishSessionResponse') is not None :
            header = root.find('HEADER')
            SessionID = header.attrib.get('SessionID')
        elif body.find('CloseSessionResponse') is not None :
            SHUTDOWN = True

if __name__ == "__main__":   
    
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    
    formatter = logging.Formatter('%(asctime)s [%(levelname)s] - %(message)s')
    
    file_handler=logging.FileHandler("../../../log/TRSE_CLIENT/trse_client_" + format(datetime.datetime.now().strftime("%m%d") +".log"))
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    
    logger.info('create socket')
    c = socket(AF_INET, SOCK_STREAM)
    
    logger.info('connecting.. {host}:{port}'.format(host=HOST, port=PORT))
    
    c.connect((HOST, PORT))
    logger.info('connected')
    SendEstablishSessionRequest()
    t1 = threading.Thread(target=SendAliveCheck, args=())
    t1.daemon = True
    t1.start()

    t2 = threading.Thread(target=RecvTcpData, args=())
    t2.daemon = True
    t2.start()

    while not SHUTDOWN :
        display_menu = '1. SendCloseSessionRequest\n'
        display_menu += '2. SendCreateJobRequest'
        print(display_menu)
        
        x = int(input('intput: '))
        print('')
        if x == 1 :
            SendCloseSessionRequest()
            time.sleep(1)
            continue
        elif x == 2 :
            SendCreateJobRequest()
        else :
            print('else')
            
    c.close()
    logger.info('end...')
    
    
