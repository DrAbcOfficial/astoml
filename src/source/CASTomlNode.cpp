#include "toml++/toml.hpp"

#include "CASTomlNode.h"

CASTomlNode::CASTomlNode(CString* szToml){
	m_TomlTable = toml::parse(szToml->c_str());
}
