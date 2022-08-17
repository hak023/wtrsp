
#ifndef	_DEBUG_H_
#define	_DEBUG_H_

#define	UNUSED(x) (x=x)
#define	ErrMessage	DebugMessage("[%s:%d]", __FILE__, __LINE__), DebugMessage

void StartLog(const char *pszLogName);
void DebugMessage(const char *pFormat, ...);

#endif
