
#ifndef ESIPUTIL_BUFFER_CONTROL_H
#define ESIPUTIL_BUFFER_CONTROL_H

namespace eSipUtil
{

class BufferControl
{
	public :
		BufferControl();
		BufferControl(const char * _pBuffer, unsigned int _unBufferLen);
		~BufferControl();

		void setBuffer(const char * _pBuffer, unsigned int _unBufferLen);
		void setStartPos(const char * _pPos);
		const char * getStartPos() const{ return m_pStartPos; }
		void setEndPos(const char * _pPos);
		const char * getEndPos() const{ return m_pEndPos; }
		inline bool eof() const{ return m_pStartPos >= m_pEndPos; }
		inline unsigned int getSize() const
		{
			if( eof() )
				return 0;
			else
				return m_pEndPos - m_pStartPos;
		}

		bool move(unsigned int _nMoveByte, bool _bReverse=false);
		bool isChar(char _c, bool _bReverse=false);
		bool isChars(const char * _pChars, bool _bReverse=false);
		bool isWhitespace(bool _bReverse=false);
		bool isStr(const char * _pszStr);
		const char * findChar(char _c, bool _bReverse=false);
		const char * findChars(const char * _pChars, bool _bReverse=false);
		const char * findWhitespace(bool _bReverse=false);
		const char * findStr(const char * _pszStr, bool _bReverse=false);
		void skipChar(char _c, bool _bReverse=false);
		void skipChars(const char * _pChars, bool _bReverse=false);
		void skipWhitespace(bool _bReverse=false);

	private :
		const char * m_pStartPos;
		const char * m_pEndPos;

};

}

#endif
