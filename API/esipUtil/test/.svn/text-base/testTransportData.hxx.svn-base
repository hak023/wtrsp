
#ifndef ESIPUTIL_TEST_TRANSPORT_DATA_H
#define ESIPUTIL_TEST_TRANSPORT_DATA_H

typedef enum
{
	E_TEST_TRANSPORT_DATA_TYPE_simpleMsg = 0,
	E_TEST_TRANSPORT_DATA_TYPE_perform,
	
	E_TEST_TRANSPORT_DATA_TYPE_num,
}ETestTransportDataType;

struct TestTransportDataTypeInfo
{
	ETestTransportDataType m_eTestTransportDataType;
	const char * m_pszDesc;
};

const char * getTestTransportDataTypeStr(ETestTransportDataType _eType)
{
	static const char * pszUnknown = "Unknown_TestTransportDataType";
	
	static const TestTransportDataTypeInfo arrTestTransportDataTypeInfo[E_TEST_TRANSPORT_DATA_TYPE_num] =
	{
		{E_TEST_TRANSPORT_DATA_TYPE_simpleMsg,		"TEST_simpleMsg"},
		{E_TEST_TRANSPORT_DATA_TYPE_perform,			"TEST_perform"},
	};

	if( _eType >=0 && _eType < E_TEST_TRANSPORT_DATA_TYPE_num )
		return arrTestTransportDataTypeInfo[_eType].m_pszDesc;
	else
		return pszUnknown;
}

struct SendingData
{
	enum
	{
		E_CONST_MAX_DATA_LEN = 50000,
	};

	ETestTransportDataType m_eTestTransportDataType;
	unsigned int m_unDataLen;
	char m_Data[E_CONST_MAX_DATA_LEN];

	void reset()
	{
		m_eTestTransportDataType = E_TEST_TRANSPORT_DATA_TYPE_num;
		m_unDataLen = 0;
	}

	unsigned int getHeaderLength() const
	{
		static const unsigned int unHeaderLength = sizeof(ETestTransportDataType) + sizeof(unsigned int);
		return unHeaderLength;
	}

	unsigned int getLength() const
	{
		return m_unDataLen + getHeaderLength();
	}
};
typedef struct SendingData SendingData_t;

#endif

