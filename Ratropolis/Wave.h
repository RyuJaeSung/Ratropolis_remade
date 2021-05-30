#pragma once
#include "Obj.h"
class CWave :
	public CObj
{
public:
	explicit CWave();
	virtual ~CWave();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

