#pragma once
#include "Obj.h"
class CUi :
	public CObj
{
public:
	explicit CUi();
	virtual ~CUi();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

