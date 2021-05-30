#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
{
}


CObj::~CObj()
{
}

const CRAD::TYPE CObj::Get_Type()
{
	if (!lstrcmp(__T("Economy"), m_wstrStateKey.c_str()))
		return CRAD::ECONOMY_CRAD;
	else if (!lstrcmp(__T("Military"), m_wstrStateKey.c_str()))
		return CRAD::MILITARY_CRAD;
	else if (!lstrcmp(__T("Building"), m_wstrStateKey.c_str()))
		return CRAD::BUILDING_CRAD;
	else if (!lstrcmp(__T("Technology"), m_wstrStateKey.c_str()))
		return CRAD::TECHNOLOGY_CRAD;

	return CRAD::CRAD_END;
}
