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
    CreateJobRequest += '<SourceContent File="dolphin.png" ID="1" NASCode="28" Path="animal">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width=""/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</SourceContent>'
    CreateJobRequest += '</SourceContentMap>'
    CreateJobRequest += '<TargetContentMap>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_240.png" ID="1" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="240" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="240"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_120.png" ID="2" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="120" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="120"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_90.png" ID="3" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="90" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="90"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_50.png" ID="4" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="50" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="50"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_25.png" ID="5" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="25" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="25"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '<TargetContent BinaryData="" File="dolphin_600.png" ID="6" NASCode="28" Path="animal" Size="40KB">'
    CreateJobRequest += '<Container ID="PNG">'
    CreateJobRequest += '<VideoCodec BitRate="64000" Height="600" ID="PNG" MaxFrameRate="" VariableFrameRate="" Width="600"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '</TargetContentMap>'
    CreateJobRequest += '<TranscodingList>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="1"/>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="2"/>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="3"/>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="4"/>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="5"/>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="6"/>'
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
    
    file_handler = logging.FileHandler('../../../log/TRSE_CLIENT/trse_client_{:%m%d}.log'.format(datetime.datetime.now()))
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    
    logger.info('create socket')
    c = socket(AF_INET, SOCK_STREAM)
    
    logger.info('connecting.. {host}:{port}'.format(host=HOST, port=PORT))
    
    c.connect((HOST, PORT))
    logger.info('connected')
    print(c.getblocking())
    print(c.gettimeout())
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
    
    
