#pragma once
#include "Obj.h"
class CBarn :
	public CObj
{
public:
	explicit CBarn();
	virtual ~CBarn();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	int m_iCrad_Number;
};

