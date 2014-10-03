

#ifndef __REGISTRY_H__
#define __REGISTRY_H__

class CRegistry
{
public:
	CRegistry();
	~CRegistry();

int m_nLastError;

// CRegistry properties	
protected:
	HKEY m_hRootKey;
	BOOL m_bLazyWrite;
	std::string m_strCurrentPath;

public:
	inline BOOL PathIsValid() {
		return (m_strCurrentPath.length() > 0); }
	inline std::string GetCurrentPath() {
		return m_strCurrentPath; }
	inline HKEY GetRootKey() {
		return m_hRootKey; }


//CRegistry	methods
public:
	BOOL ClearKey();
	BOOL SetRootKey(HKEY hRootKey);
	BOOL CreateKey(std::string strKey);
	BOOL DeleteKey(std::string strKey);
	BOOL DeleteValue(std::string strName);
	int GetDataSize(std::string strValueName);
	DWORD GetDataType(std::string strValueName);
	int GetSubKeyCount();
	int GetValueCount();
	BOOL KeyExists(std::string strKey, HKEY hRootKey = NULL);
	BOOL SetKey(std::string strKey, BOOL bCanCreate);
	BOOL ValueExists(std::string strName);
	void RenameValue(std::string strOldName, std::string strNewName);

	// data reading functions
	//COleDateTime ReadDateTime(std::string strName, COleDateTime dtDefault);
	double ReadFloat(std::string strName, double fDefault);
	std::string ReadString(std::string strName, std::string strDefault);
	int ReadInt(std::string strName, int nDefault);
	BOOL ReadBool(std::string strName, BOOL bDefault);
	/*COLORREF ReadColor(std::string strName, COLORREF rgbDefault);
	BOOL ReadFont(std::string strName, CFont* pFont);
	BOOL ReadPoint(std::string strName, POINT* pPoint);
	BOOL ReadSize(std::string strName, CSize* pSize);
	BOOL ReadRect(std::string strName, CRect* pRect);
	*/
	DWORD ReadDword(std::string strName, DWORD dwDefault);

	// data writing functions
	BOOL WriteBool(std::string strName, BOOL bValue);
	//BOOL WriteDateTime(std::string strName, COleDateTime dtValue);
	BOOL WriteString(std::string strName, std::string strValue, int iType);
	BOOL WriteFloat(std::string strName, double fValue);
	BOOL WriteInt(std::string strName, int nValue);
	/*BOOL WriteColor(std::string strName, COLORREF rgbValue);
	BOOL WriteFont(std::string strName, CFont* pFont);
	BOOL WritePoint(std::string strName, CPoint* pPoint);
	BOOL WriteSize(std::string strName, CSize* pSize);
	BOOL WriteRect(std::string strName, CRect* pRect);
	*/
	BOOL WriteDword(std::string strName, DWORD dwValue);

};// end of CRegistry class definition


#endif