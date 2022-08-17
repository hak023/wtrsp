#ifndef _MSYNC_ERR_H_
#define _MSYNC_ERR_H_

enum eMSYNCErrCode
{
msECNoErr                  = 0x0000,
msECInvalidParm            = 0x0001,
msECCreateShmFail          = 0x0002,
msECLimitSecNum            = 0x0003,
msECLimitSecSize           = 0x0004,
msECSPNotInited            = 0x0005,
msECConfigLoadFail         = 0x0006,
msECInitializeFail         = 0x0007,
msECFailWrite_NotActive    = 0x0008,
msECFailRead_NotStandby    = 0x0009,
msECMax
};

inline const char * fnMSyncApiStringError(int _nCode)
{
switch(_nCode)
{
case msECNoErr               : return "Msync_SUCCESS";
case msECInvalidParm         : return "Msync_InvalidParm";
case msECCreateShmFail       : return "Msync_CreateShmFail";
case msECLimitSecNum         : return "Msync_LimitSecNum";
case msECLimitSecSize        : return "Msync_LimitSecNum";
case msECSPNotInited         : return "Msync_SPNotInited";
case msECConfigLoadFail      : return "Msync_ConfigLoadFail";
case msECInitializeFail      : return "Msync_InitializeFail";
case msECFailWrite_NotActive : return "Msync_FailWrite_NotActive";
case msECFailRead_NotStandby : return "Msync_FailRead_NotStandby";
default:                       return "Unknown MsyncError";
};
return "Unknown MsyncError";
}



#endif
