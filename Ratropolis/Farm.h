#pragma once
#include "Obj.h"
class CFarm :
	public CObj
{
public:
	explicit CFarm();
	virtual ~CFarm();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	RECT	m_rAddPoP;
	bool	m_AddPop;
	float	m_fTime;
	float	m_fMaxTime;
	float	m_fCoolTime;
};

