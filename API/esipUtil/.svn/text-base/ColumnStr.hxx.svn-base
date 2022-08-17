
#ifndef ESIPUTIL_COLUMN_DATA_H
#define ESIPUTIL_COLUMN_DATA_H

#include <string>
#include <list>
#include <vector>

namespace eSipUtil
{

struct ColumnInfo
{
	std::string m_clsColumnTitleName;
	unsigned int m_unMaxColumnLen;

	ColumnInfo()
	{
		m_unMaxColumnLen = 0;
	}
};

typedef std::list< std::pair<std::string, std::string> > list_LineData;			//first:key(Column TitleName),   second:value
typedef std::list< list_LineData > list_LineDataAll;
typedef std::vector< ColumnInfo > vec_ColumnInfo;

struct ColumnStrConfig
{
	std::string m_clsName;
	vec_ColumnInfo m_vecColumnInfo;
	std::string m_clsColumnDelimiter;
	std::string m_clsColumnComment;

	ColumnStrConfig()
	{
	}
};

class ColumnStr
{
	public :
		ColumnStr();
		~ColumnStr();

		bool m_fnInit(const ColumnStrConfig & _rstColumnMgrConfig);
		bool m_fnCreateTitleComment(std::string & _rclsStr) const;
		bool m_fnCreateOneLine(const list_LineData & _rlistLineData, std::string & _rclsStr) const;
		bool m_fnCreateAllLine(const list_LineDataAll & _rlistLineDataAll, std::string & _rclsStr) const;
		bool m_fnParsingTotalLine(const char * _pszTotalStr, unsigned int _unMaxTotalStrLen, list_LineDataAll & _rlistLineDataAll) const;

	private :
		bool m_bInit;
		unsigned int m_unColumnNum;
		ColumnStrConfig m_stColumnStrConfig;
		
};

}

#endif

