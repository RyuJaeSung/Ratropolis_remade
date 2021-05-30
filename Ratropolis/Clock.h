#pragma once
#include "Obj.h"
class CClock :
	public CObj
{
public:
	explicit CClock();
	virtual ~CClock();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

