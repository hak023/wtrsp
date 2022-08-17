#ifndef CBASE64_H
#define CBASE64_H

#include <string>

class CBase64
{
public:
	CBase64();
	~CBase64();
	static int m_fnEncode(char *text, int numBytes, char **encodedText);
	static int m_fnDecode(char * text, unsigned char * dst, int numBytes);
};

#endif
