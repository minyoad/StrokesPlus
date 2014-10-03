

#include "stdafx.h"
#include <winreg.h>
#include "Registry.h"

#define CLASS_NAME_LENGTH 255

/* IMPORTANT NOTES ABOUT CREGISTRY:
	
	CRegistry never keeps a key open past the end of a function call.
	This is incase the application crashes before the next call to close
	the registry 
	
	INCLUDE FILES
	"winreg.h" and "afxdisp.h" must be included in "stdafx.h"

	KEY NAMES:
	Key names must not begin with a \ and only absolute strings are accepted
	
*/



CRegistry::CRegistry()
{
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	m_nLastError = ERROR_SUCCESS;
}

CRegistry::~CRegistry()
{
	ClearKey();
}


BOOL CRegistry::ClearKey()
{
	/* Call CloseKey to write the current key to the registry and close the 
	key. An application should not keep keys open any longer than necessary. 
	Calling CloseKey when there is no current key has no effect.*/

	m_strCurrentPath.empty();
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	return TRUE;
}



BOOL CRegistry::SetRootKey(HKEY hRootKey)
{
	// sets the root key
	// make sure to set it to a valid key
	if (hRootKey != HKEY_CLASSES_ROOT &&
			hRootKey != HKEY_CURRENT_USER &&
			hRootKey != HKEY_LOCAL_MACHINE &&
			hRootKey != HKEY_USERS) return FALSE;

	m_hRootKey = hRootKey;
	return TRUE;
}


BOOL CRegistry::CreateKey(std::string strKey)
{
	/* Use CreateKey to add a new key to the registry. 
		Key is the name of the key to create. Key must be 
		an absolute name. An absolute key 
		begins with a backslash (\) and is a subkey of 
		the root key. */

	//ASSERT(strKey[0] != '\\');
	HKEY hKey;

	DWORD dwDisposition = 0;

	if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(strKey.c_str()), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
			&dwDisposition)	!= ERROR_SUCCESS) return FALSE;
	
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	m_strCurrentPath = strKey;
	return TRUE;
}


BOOL CRegistry::DeleteKey(std::string strKey)
{
	/* Call DeleteKey to remove a specified key and its associated data, 
	if any, from the registry. Returns FALSE is there are subkeys
	Subkeys must be explicitly deleted by separate calls to DeleteKey.
	DeleteKey returns True if key deletion is successful. On error, 
	DeleteKey returns False. */
	
	// need to open the key first with RegOpenKeyEx
	//ASSERT(FALSE); // not yet implemented
	//ASSERT(strKey[0] != '\\');

	if (!KeyExists(strKey)) return TRUE;
	if (::RegDeleteKey(m_hRootKey, strKey.c_str()) != ERROR_SUCCESS) return FALSE;
	return TRUE;
}



BOOL CRegistry::DeleteValue(std::string strName)
{
	/* Call DeleteValue to remove a specific data value 
		associated with the current key. Name is string 
		containing the name of the value to delete. Keys can contain 
		multiple data values, and every value associated with a key 
		has a unique name. */

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	HKEY hKey;
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegDeleteValue(hKey, LPCTSTR(strName.c_str()));
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}


int CRegistry::GetDataSize(std::string strValueName)
{
	/* Call GetDataSize to determine the size, in bytes, of 
	a data value associated with the current key. ValueName 
	is a string containing the name of the data value to query.
	On success, GetDataSize returns the size of the data value. 
	On failure, GetDataSize returns -1. */

	HKEY hKey;
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) return -1;

	DWORD dwSize = 1;
	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strValueName.c_str()),
		NULL, NULL, NULL, &dwSize);
	::RegCloseKey(hKey);

	if (lResult != ERROR_SUCCESS) return -1;
	return (int)dwSize;
}

DWORD CRegistry::GetDataType(std::string strValueName)
{
	HKEY hKey;
	//ASSERT(m_strCurrentPath.GetLength() > 0);

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_QUERY_VALUE, &hKey);

	if (m_nLastError != ERROR_SUCCESS) return 0;

	DWORD dwType = 1;
	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strValueName.c_str()),
		NULL, &dwType, NULL, NULL);
	::RegCloseKey(hKey);		

	if (m_nLastError == ERROR_SUCCESS) return dwType;

	return 0;
}



int CRegistry::GetSubKeyCount()
{
	/* Call this function to determine the number of subkeys.
		the function returns -1 on error */
	HKEY hKey;
	//ASSERT(m_strCurrentPath.GetLength() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwSubKeyCount;
}


int CRegistry::GetValueCount()
{
	/* Call this function to determine the number of subkeys.
		the function returns -1 on error */
	HKEY hKey;
	//ASSERT(m_strCurrentPath.GetLength() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwValueCount;
}


BOOL CRegistry::KeyExists(std::string strKey, HKEY hRootKey)
{
	/* Call KeyExists to determine if a key of a specified name exists.
		 Key is the name of the key for which to search. */

	//ASSERT(strKey[0] != '\\');
	HKEY hKey;

	if (hRootKey == NULL) hRootKey = m_hRootKey;
	
	LONG lResult = ::RegOpenKeyEx(hRootKey, LPCTSTR(strKey.c_str()), 0,
		KEY_ALL_ACCESS, &hKey);
	::RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

BOOL CRegistry::SetKey(std::string strKey, BOOL bCanCreate)
{
	/* Call SetKey to make a specified key the current key. Key is the 
		name of the key to open. If Key is null, the CurrentKey property 
		is set to the key specified by the RootKey property.

		CanCreate specifies whether to create the specified key if it does 
		not exist. If CanCreate is True, the key is created if necessary.

		Key is opened or created with the security access value KEY_ALL_ACCESS. 
		OpenKey only creates non-volatile keys, A non-volatile key is stored in 
		the registry and is preserved when the system is restarted. 

		OpenKey returns True if the key is successfully opened or created */

	//ASSERT(strKey[0] != '\\');
	HKEY hKey;


	// close the current key if it is open
	if (strKey.length() == 0)
	{
		m_strCurrentPath.empty();
		return TRUE;
	}

	DWORD dwDisposition;
	if (bCanCreate) // open the key with RegCreateKeyEx
	{
		if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(strKey.c_str()), 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
				&dwDisposition) != ERROR_SUCCESS) return FALSE;
		m_strCurrentPath = strKey;
		if (!m_bLazyWrite) ::RegFlushKey(hKey);
		::RegCloseKey(hKey);	
		return TRUE;
	}

	// otherwise, open the key without creating
	// open key requires no initial slash
	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(strKey.c_str()), 0,
		KEY_ALL_ACCESS, &hKey);
	if (m_nLastError != ERROR_SUCCESS) return FALSE;
	m_strCurrentPath = strKey;
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return TRUE;
}


BOOL CRegistry::ValueExists(std::string strName)
{
	/* Call ValueExists to determine if a particular key exists in 
		the registry. Calling Value Exists is especially useful before 
		calling other TRegistry methods that operate only on existing keys.

		Name is the name of the data value for which to check.
	ValueExists returns True if a match if found, False otherwise. */

	HKEY hKey;
	LONG lResult;
	//ASSERT(m_strCurrentPath.GetLength() > 0);

	
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		NULL, NULL, NULL);
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}


void CRegistry::RenameValue(std::string strOldName, std::string strNewName)
{
	/* Call RenameValue to change the name of a data value associated 
		with the current key. OldName is a string containing the current 
		name of the data value. NewName is a string containing the replacement 
		name for the data value.
		
		If OldName is the name of an existing data value for the current key, 
		and NewName is not the name of an existing data value for the current 
		key, RenameValue changes the data value name as specified. Otherwise 
		the current name remains unchanged.
	*/
	//ASSERT(FALSE); // functionality not yet implemented
}


/*
COleDateTime CRegistry::ReadDateTime(std::string strName, COleDateTime dtDefault)
{
	/* Call ReadDate to read a date value from a specified data value 
	associated with the current key. Name is the name of the data value to read.
	If successful, ReadDate returns a Delphi TDateTime value. The integral part 
	of a TDateTime value is the number of days that have passed since 12/30/1899. 
	The fractional part of a TDateTime value is the time of day.
	On error, an exception is raised, and the value returned by this function 
	should be discarded. */
/*
	DWORD dwType = REG_BINARY;
	COleDateTime dt;
	DWORD dwSize = sizeof(dt);
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return dtDefault;
	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&dt, &dwSize) != ERROR_SUCCESS) dt = dtDefault;
	::RegCloseKey(hKey);	
	return dt;
}*/


double CRegistry::ReadFloat(std::string strName, double fDefault)
{
	/* Call ReadFloat to read a float value from a specified 
		data value associated with the current key. Name is the name 
		of the data value to read.
		
		If successful, ReadFloat returns a double value. 
		On error, an exception is raised, and the value returned by 
		this function should be discarded. */

	DWORD dwType = REG_BINARY;
	double d;
	DWORD dwSize = sizeof(d);
	HKEY hKey;

	//MessageBoxA(NULL, "In ReadFloat", "",NULL);
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return fDefault;
	//MessageBoxA(NULL, "After RegOpenKeyEx", "",NULL);
	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&d, &dwSize) != ERROR_SUCCESS) d = fDefault;
	::RegCloseKey(hKey);	
	//MessageBoxA(NULL, "End", "",NULL);
	return d;
}

std::string CRegistry::ReadString(std::string strName, std::string strDefault)
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	BOOL bSuccess = TRUE;
	_TCHAR sz[255];
	HKEY hKey;
	
								 
	//ASSERT(m_strCurrentPath.GetLength() > 0);

	// make sure it is the proper type
	dwType = GetDataType(strName);
	
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ/* && dwType != REG_MULTI_SZ*/)
	{
		return strDefault;
	}

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey);
	if (m_nLastError != ERROR_SUCCESS) return strDefault;

	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)sz, &dwSize);
	if (m_nLastError != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	
	if (!bSuccess) return strDefault;
	if(dwType == REG_EXPAND_SZ){
		TCHAR cOutputPath[32000]; 
		ExpandEnvironmentStrings((LPCTSTR)sz,cOutputPath,sizeof(cOutputPath) / sizeof(*cOutputPath));
		return std::string((LPCTSTR)cOutputPath);
	} else {
		return std::string((LPCTSTR)sz);
	}
}

DWORD CRegistry::ReadDword(std::string strName, DWORD dwDefault)
{
	DWORD dwType = REG_DWORD;
	DWORD dw;
	DWORD dwSize = sizeof(dw);
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	//MessageBoxA(NULL, "In ReadDword", "",NULL);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return dwDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&dw, &dwSize) != ERROR_SUCCESS) dw = dwDefault;
	::RegCloseKey(hKey);	
	return dw;
}



int CRegistry::ReadInt(std::string strName, int nDefault)
{
	DWORD dwType = REG_BINARY;
	int n;
	DWORD dwSize = sizeof(n);
	HKEY hKey;

	//MessageBoxA(NULL, "In ReadInt", "",NULL);
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return nDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&n, &dwSize) != ERROR_SUCCESS) n = nDefault;
	::RegCloseKey(hKey);	
	return n;
}

BOOL CRegistry::ReadBool(std::string strName, BOOL bDefault)
{
	DWORD dwType = REG_BINARY;
	BOOL b;
	DWORD dwSize = sizeof(b);
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return bDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&b, &dwSize) != ERROR_SUCCESS) b = bDefault;
	::RegCloseKey(hKey);	
	return b;
}

/*
COLORREF CRegistry::ReadColor(std::string strName, COLORREF rgbDefault)
{
	DWORD dwType = REG_BINARY;
	COLORREF rgb;
	DWORD dwSize = sizeof(rgb);
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return rgbDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&rgb, &dwSize) != ERROR_SUCCESS) rgb = rgbDefault;
	::RegCloseKey(hKey);	
	return rgb;
}

BOOL CRegistry::ReadFont(std::string strName, CFont* pFont)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(LOGFONT);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	LOGFONT lf;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)&lf, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	if (bSuccess)
	{
		pFont->Detach();
		pFont->CreateFontIndirect(&lf);
	}
	return bSuccess;
}


BOOL CRegistry::ReadPoint(std::string strName, CPoint* pPoint)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CPoint);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)pPoint, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}

BOOL CRegistry::ReadSize(std::string strName, CSize* pSize)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CSize);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)pSize, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}

BOOL CRegistry::ReadRect(std::string strName, CRect* pRect)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CRect);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	//ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName.c_str()), NULL,
		&dwType, (LPBYTE)pRect, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}
*/



BOOL CRegistry::WriteBool(std::string strName, BOOL bValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&bValue, sizeof(bValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}
/*
BOOL CRegistry::WriteDateTime(std::string strName, COleDateTime dtValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&dtValue, sizeof(dtValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}*/


BOOL CRegistry::WriteString(std::string strName, std::string strValue, int iType)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	_TCHAR sz[255];
	
	if (strValue.length() > 254) return FALSE;

#ifdef _UNICODE
	wstrcpy(sz, LPCTSTR(strValue));
#else
	strcpy(sz, LPCTSTR(strValue.c_str()));
#endif

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
#ifdef _UNICODE
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_SZ, (LPBYTE)sz, wstrlen(sz) + 1)
		 != ERROR_SUCCESS) bSuccess = FALSE;
#else
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		(iType==2 ? REG_EXPAND_SZ : REG_SZ), (LPBYTE)sz, strlen(sz) + 1)
		 != ERROR_SUCCESS) bSuccess = FALSE;
#endif
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteFloat(std::string strName, double fValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&fValue, sizeof(fValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteInt(std::string strName, int nValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&nValue, sizeof(nValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteDword(std::string strName, DWORD dwValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}
/*
BOOL CRegistry::WriteColor(std::string strName, COLORREF rgbValue)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&rgbValue, sizeof(rgbValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteFont(std::string strName, CFont* pFont)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)&lf, sizeof(lf))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WritePoint(std::string strName, CPoint* pPoint)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)pPoint, sizeof(CPoint))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteSize(std::string strName, CSize* pSize)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)pSize, sizeof(CSize))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteRect(std::string strName, CRect* pRect)
{
	//ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath.c_str()), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName.c_str()), 0,
		REG_BINARY, (LPBYTE)pRect, sizeof(CRect))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}
*/
