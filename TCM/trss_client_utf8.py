import zlib
import time
import threading
import sys
import logging
import datetime
import xml.etree.ElementTree as ET
from socket import *

#Server Info
LOC_HOST = "121.134.202.25"
LOC_PORT = 33002

#Local Info
RMT_HOST = "121.134.202.25"
RMT_PORT = 30003

#global variable
TransactionID = 0
SendAliveCheckTimer = 60
SHUTDOWN = False
lock = threading.Lock()
SessionID = ''
JobID = ''

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
    EstablishSessionRequest += '<HEADER From="vwtrss_sim_33002" SessionID="" To="vwtrsg" TransactionID="%d">'  % (GetTid())
    EstablishSessionRequest += '<Error Code="" Description="" Source=""/>'
    EstablishSessionRequest += '</HEADER>'
    EstablishSessionRequest += '<BODY>'
    EstablishSessionRequest += '<EstablishSessionRequest>'
    EstablishSessionRequest += '<Endpoint ID="" IP="" PWD="">'
    EstablishSessionRequest += '</Endpoint>'
    EstablishSessionRequest += '</EstablishSessionRequest>'
    EstablishSessionRequest += '</BODY>'
    EstablishSessionRequest += '</WTRS.MSG>'
    
    TcpSendMsg(EstablishSessionRequest)
    lock.release()

def SendJobStatusChangedNotify_Created(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_Created = '<?xml version="1.0"?>'
    JobStatusChangedNotify_Created += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_Created += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">'  % (SessionID, _tid)
    JobStatusChangedNotify_Created += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_Created += '</HEADER>'
    JobStatusChangedNotify_Created += '<BODY>'
    JobStatusChangedNotify_Created += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_Created += '<JobState ID="%s" Kind="Realtime" Status="Created" ServiceName="%s" StartTime="%s" EndTime="" ResultCode="0000" />' % (JobID, _serviceName, current_time)
    JobStatusChangedNotify_Created += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_Created += '</BODY>'
    JobStatusChangedNotify_Created += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_Created)
    lock.release()

def SendJobStatusChangedNotify_Waiting(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_Waiting = '<?xml version="1.0"?>'
    JobStatusChangedNotify_Waiting += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_Waiting += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">'  % (SessionID, _tid)
    JobStatusChangedNotify_Waiting += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_Waiting += '</HEADER>'
    JobStatusChangedNotify_Waiting += '<BODY>'
    JobStatusChangedNotify_Waiting += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_Waiting += '<JobState ID="%s" Kind="Realtime" Status="Waiting" ServiceName="%s" StartTime="%s" EndTime="" ResultCode="0000" />' % (JobID, _serviceName, current_time)
    JobStatusChangedNotify_Waiting += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_Waiting += '</BODY>'
    JobStatusChangedNotify_Waiting += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_Waiting)
    lock.release()

def SendJobStatusChangedNotify_JobStarted(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_JobStarted = '<?xml version="1.0"?>'
    JobStatusChangedNotify_JobStarted += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_JobStarted += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">'  % (SessionID, _tid)
    JobStatusChangedNotify_JobStarted += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_JobStarted += '</HEADER>'
    JobStatusChangedNotify_JobStarted += '<BODY>'
    JobStatusChangedNotify_JobStarted += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_JobStarted += '<JobState ID="%s" Kind="Realtime" Status="JobStarted" ServiceName="%s" StartTime="%s" EndTime="" ResultCode="0000">' % (JobID, _serviceName, current_time)
    JobStatusChangedNotify_JobStarted += '<Transcodes Cancel="0" Current="0" Fail="0" Success="0" Total="1" />'
    JobStatusChangedNotify_JobStarted += '</JobState>'
    JobStatusChangedNotify_JobStarted += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_JobStarted += '</BODY>'
    JobStatusChangedNotify_JobStarted += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_JobStarted)
    lock.release()

def SendJobStatusChangedNotify_TranscodingStarted(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_TranscodingStarted = '<?xml version="1.0"?>'
    JobStatusChangedNotify_TranscodingStarted += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_TranscodingStarted += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">' % (SessionID, _tid)
    JobStatusChangedNotify_TranscodingStarted += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_TranscodingStarted += '</HEADER>'
    JobStatusChangedNotify_TranscodingStarted += '<BODY>'
    JobStatusChangedNotify_TranscodingStarted += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_TranscodingStarted += '<JobState ID="%s" Kind="Realtime" Status="TranscodingStarted" ServiceName="%s" StartTime="%s" EndTime="" ResultCode="0000">' % (JobID, _serviceName, current_time)
    JobStatusChangedNotify_TranscodingStarted += '<Transcodes StartTime="%s" EndTime="%s" TransSessionID="%s_1" Cancel="0" Current="1" Fail="0" Success="0" Total="1" ResultCode="0000" ResultDescription="0">' % (current_time, current_time, SessionID)
    JobStatusChangedNotify_TranscodingStarted += '<Transcode>'
    JobStatusChangedNotify_TranscodingStarted += '<Source NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112388780_35584528_src.K3G" BinaryData="\\\\172.17.107.5\\nas_wtrsp\\temp\\2021052522091524324200_decode.tmp">'
    JobStatusChangedNotify_TranscodingStarted += '<Container ID="K3G" />'
    JobStatusChangedNotify_TranscodingStarted += '</Source>'
    JobStatusChangedNotify_TranscodingStarted += '<Target NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112693652_35583984_tar.K3G" BinaryData="\\\\172.17.107.5\\nas_wtrsp\\temp\\2021052522091119124480_encode.tmp">'
    JobStatusChangedNotify_TranscodingStarted += '<Container ID="K3G">'
    JobStatusChangedNotify_TranscodingStarted += '<VideoCodec Width="176" Height="144" MaxFrameRate="15" BitRate="256000" VariableFrameRate="0" ID="MPEG4" />'
    JobStatusChangedNotify_TranscodingStarted += '<AudioCodec Channel="1" SampleRate="8000" BitRate="12200" BitPerSample="16" ID="AMR" />'
    JobStatusChangedNotify_TranscodingStarted += '</Container>'
    JobStatusChangedNotify_TranscodingStarted += '</Target>'
    JobStatusChangedNotify_TranscodingStarted += '</Transcode>'
    JobStatusChangedNotify_TranscodingStarted += '</Transcodes>'
    JobStatusChangedNotify_TranscodingStarted += '</JobState>'
    JobStatusChangedNotify_TranscodingStarted += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_TranscodingStarted += '</BODY>'
    JobStatusChangedNotify_TranscodingStarted += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_TranscodingStarted)
    lock.release()

def SendJobStatusChangedNotify_TranscodingStopped(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_TranscodingStopped =  '<?xml version="1.0"?>'
    JobStatusChangedNotify_TranscodingStopped += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_TranscodingStopped += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">' % (SessionID, _tid)
    JobStatusChangedNotify_TranscodingStopped += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_TranscodingStopped += '</HEADER>'
    JobStatusChangedNotify_TranscodingStopped += '<BODY>'
    JobStatusChangedNotify_TranscodingStopped += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_TranscodingStopped += '<JobState ID="%s" Kind="Realtime" Status="TranscodingStopped" ServiceName="%s" StartTime="%s" EndTime="" ResultCode="0000">' % (JobID, _serviceName, current_time)
    JobStatusChangedNotify_TranscodingStopped += '<Transcodes StartTime="%s" EndTime="%s" TransSessionID="%s_1" Cancel="0" Current="1" Fail="0" Success="1" Total="1" ResultCode="0000" ResultDescription="12">' % (current_time, current_time, SessionID)
    JobStatusChangedNotify_TranscodingStopped += '<Transcode>'
    JobStatusChangedNotify_TranscodingStopped += '<Source NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112388780_35584528_src.K3G" BinaryData="\\\\172.17.107.5\\nas_wtrsp\\temp\\2021052522091524324200_decode.tmp">'
    JobStatusChangedNotify_TranscodingStopped += '<Container ID="K3G" />'
    JobStatusChangedNotify_TranscodingStopped += '</Source>'
    #JobStatusChangedNotify_TranscodingStopped += '<Target NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112693652_35583984_tar.K3G" BinaryData="">'
    JobStatusChangedNotify_TranscodingStopped += '<Target NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112693652_35583984_tar.K3G" BinaryData="2022042814150800484344_encode.tmp">'
    #JobStatusChangedNotify_TranscodingStopped += '<Target NASCode="" File="" WTRSSPath="C:\\TRSS\\temp\\202105252209112693652_35583984_tar.K3G" BinaryData="2022042814150800484344_encode.tmp1">'
    JobStatusChangedNotify_TranscodingStopped += '<Container ID="K3G">'
    JobStatusChangedNotify_TranscodingStopped += '<VideoCodec Width="176" Height="144" MaxFrameRate="15" BitRate="256000" VariableFrameRate="0" ID="MPEG4" />'
    JobStatusChangedNotify_TranscodingStopped += '<AudioCodec Channel="1" SampleRate="8000" BitRate="12200" BitPerSample="16" ID="AMR" />'
    JobStatusChangedNotify_TranscodingStopped += '</Container>'
    JobStatusChangedNotify_TranscodingStopped += '</Target>'
    JobStatusChangedNotify_TranscodingStopped += '</Transcode>'
    JobStatusChangedNotify_TranscodingStopped += '</Transcodes>'
    JobStatusChangedNotify_TranscodingStopped += '</JobState>'
    JobStatusChangedNotify_TranscodingStopped += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_TranscodingStopped += '</BODY>'
    JobStatusChangedNotify_TranscodingStopped += '</WTRS.MSG>'
    #print (JobStatusChangedNotify_TranscodingStopped)
    TcpSendMsg(JobStatusChangedNotify_TranscodingStopped)
    lock.release()

def SendJobStatusChangedNotify_JobStopped(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_JobStopped =  '<?xml version="1.0"?>'
    JobStatusChangedNotify_JobStopped += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_JobStopped += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">'  % (SessionID, _tid)
    JobStatusChangedNotify_JobStopped += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_JobStopped += '</HEADER>'
    JobStatusChangedNotify_JobStopped += '<BODY>'
    JobStatusChangedNotify_JobStopped += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_JobStopped += '<JobState ID="%s" Kind="Realtime" Status="JobStopped" ServiceName="%s" StartTime="%s" EndTime="%s" ResultCode="0000">' % (JobID, _serviceName, current_time, current_time)
    JobStatusChangedNotify_JobStopped += '<Transcodes Cancel="0" Current="0" Fail="0" Success="6" Total="6" />'
    JobStatusChangedNotify_JobStopped += '</JobState>'
    JobStatusChangedNotify_JobStopped += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_JobStopped += '</BODY>'
    JobStatusChangedNotify_JobStopped += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_JobStopped)
    lock.release()

def SendJobStatusChangedNotify_Destroyed(_tid, _serviceName):
    global lock
    global SessionID
    global JobID
    lock.acquire()
    now = datetime.datetime.now()
    current_time =  now.strftime('%Y-%m-%d %H:%M:%S')
    JobStatusChangedNotify_Destroyed =  '<?xml version="1.0"?>'
    JobStatusChangedNotify_Destroyed += '<WTRS.MSG Version="1.0">'
    JobStatusChangedNotify_Destroyed += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">'  % (SessionID, _tid)
    JobStatusChangedNotify_Destroyed += '<Error Code="0000" Description="" Source="" />'
    JobStatusChangedNotify_Destroyed += '</HEADER>'
    JobStatusChangedNotify_Destroyed += '<BODY>'
    JobStatusChangedNotify_Destroyed += '<JobStatusChangedNotify>'
    JobStatusChangedNotify_Destroyed += '<JobState ID="%s" Kind="Realtime" Status="Destroyed" ServiceName="%s" StartTime="%s" EndTime="%s" ResultCode="0000">' % (JobID, _serviceName, current_time, current_time)
    JobStatusChangedNotify_Destroyed += '<Transcodes Cancel="0" Current="0" Fail="0" Success="1" Total="1" />'
    JobStatusChangedNotify_Destroyed += '</JobState>'
    JobStatusChangedNotify_Destroyed += '</JobStatusChangedNotify>'
    JobStatusChangedNotify_Destroyed += '</BODY>'
    JobStatusChangedNotify_Destroyed += '</WTRS.MSG>'

    TcpSendMsg(JobStatusChangedNotify_Destroyed)
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
        LinkTestRequest += '<HEADER From="vwtrss_sim_33002" SessionID="%s" To="vwtrsg" TransactionID="%d">' % (SessionID, GetTid())
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
    global JobID
    while not SHUTDOWN :
        header = c.recv(30)
        bodySizeStr = header[10:]
        
        body = c.recv(int(bodySizeStr))
        recvLen = len(body)

        while recvLen < int(bodySizeStr) :
            contineRecv = (int(bodySizeStr) - recvLen)
            body += c.recv(abs(contineRecv))
            recvLen = int(len(body))
            print('byte[body size : %d]' % (len(body)))
        else :
            print('byte[body size : %d] ok' % (len(body)))
            
        decompress_msg = zlib.decompress(body)
        decompress_msg = decompress_msg[0:-1]
        logger.info('[RECV]' + decompress_msg.decode('utf-8'))

        root = ET.fromstring(decompress_msg)

        body = root.find('BODY')
        if body.find('EstablishSessionResponse') is not None :
            header = root.find('HEADER')
            SessionID = header.attrib.get('SessionID')
            
        elif body.find('CreateJobRequest') is not None :
             print('CreateJobRequest recv')
             header = root.find('HEADER')
             tid = int(header.attrib.get('TransactionID'))
             job = body.find('CreateJobRequest').find('Job')
             JobID = job.attrib.get('ID')    
             serviceName = job.attrib.get('ServiceName')
             
             SendJobStatusChangedNotify_Created(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_Waiting(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_JobStarted(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_TranscodingStarted(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_TranscodingStopped(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_JobStopped(tid, serviceName)
             time.sleep(3)
             SendJobStatusChangedNotify_Destroyed(tid, serviceName)
             
             
        elif body.find('CloseSessionResponse') is not None :
            SHUTDOWN = True


if __name__ == "__main__":   
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)

    formatter = logging.Formatter('%(asctime)s [%(levelname)s] - %(message)s')

    file_handler = logging.FileHandler('../log/TRSS_CLIENT/trss_client_{:%m%d}.log'.format(datetime.datetime.now()))
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

    LOC_PORT = int(input('intput port: '))
    
    logger.info('create socket')
    c = socket(AF_INET, SOCK_STREAM)
    c.bind((LOC_HOST, LOC_PORT))
    c.connect((RMT_HOST, RMT_PORT))
    logger.info('connected')

    SendEstablishSessionRequest()
    t1 = threading.Thread(target=SendAliveCheck, args=())
    t1.daemon = True
    t1.start()
    
    t2 = threading.Thread(target=RecvTcpData, args=())
    t2.daemon = True
    t2.start()

    while not SHUTDOWN :
        time.sleep(1)
            
    c.close()
    
    
