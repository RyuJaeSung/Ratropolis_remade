#pragma once
#include "Obj.h"
class CCastle :
	public CObj
{
public:
	explicit CCastle();
	virtual ~CCastle();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

