#pragma once
#include "CASBaseObject.h"

class CASTomlNode : public CASBaseGCObject {
public:
	CASTomlNode(CString* szToml);

	CASTomlNode* Get(CString* szKey);
	void GetArray(CString* szKey, CScriptArray* outArray);
	void GetString(CString* szKey, CString* outValue);
	bool GetBool(CString* szKey);
	int64 GetInt(CString* szKey);
	float GetFloat(CString* szKey);
private:
	toml::table m_TomlTable;
};