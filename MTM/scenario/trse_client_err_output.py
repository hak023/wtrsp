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
    CreateJobRequest += '<SourceContent File="6430001449.mp3" ID="1" NASCode="67" Path="source/0100003428/">'
    #CreateJobRequest += '<SourceContent BinaryData="==" File="" ID="0" NASCode="" Path="">'
    CreateJobRequest += '<Container ID="MP3">'
    CreateJobRequest += '<VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width=""/>'
    CreateJobRequest += '<AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate=""/>'
    #CreateJobRequest += '<Metadata Album="???" Artist="Various Artists" Comment="" Genre="??" Copyright="" Title="" Track="2" Year=""/>'
    #CreateJobRequest += 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" ImageType="JPEG" PlayDevice="" PlayTime="0" Site="" Title="I Still Need You" Track="7" Year="2022" />' 
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</SourceContent>'
    CreateJobRequest += '</SourceContentMap>'
    CreateJobRequest += '<TargetContentMap>'
    CreateJobRequest += '<TargetContent BinaryData="" File="20220728_err_testfile.mp3" ID="1" NASCode="10" Path="aaa/bbb">'
    CreateJobRequest += '<Container ID="MP3C">'
    CreateJobRequest += '<VideoCodec BitRate="0" Height="0" ID="NONE" MaxFrameRate="0" VariableFrameRate="0" Width="0"/>'
    CreateJobRequest += '<AudioCodec BitPerSample="16" BitRate="192000" Channel="2" ID="MP3C" SampleRate="44100"/>'
    CreateJobRequest += '<Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year=""/>'
    CreateJobRequest += '</Container>'
    CreateJobRequest += '</TargetContent>'
    #CreateJobRequest += '<TargetContent BinaryData="" File="testfile_2.wma" ID="2" NASCode="10" Path="test/lhj">'
    #CreateJobRequest += '<Container ID="WMA">'
    #CreateJobRequest += '<VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width=""/>'
    #CreateJobRequest += '<AudioCodec BitPerSample="16" BitRate="160000" Channel="2" ID="WMA2" SampleRate="44100"/>'
    #CreateJobRequest += '<Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year=""/>'
    #CreateJobRequest += '</Container>'
    #CreateJobRequest += '</TargetContent>'
    CreateJobRequest += '</TargetContentMap>'
    CreateJobRequest += '<TranscodingList>'
    CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="1"/>'
    #CreateJobRequest += '<Transcoding SourceContentID="0" TargetContentID="2"/>'
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
    
    file_handler = logging.FileHandler('../../log/TRSE_CLIENT/trse_client_{:%m%d}.log'.format(datetime.datetime.now()))
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
    
    
