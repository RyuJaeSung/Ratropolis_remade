#pragma once
#include "Obj.h"
class CCrad : public CObj
{
public:
	explicit CCrad();
	virtual ~CCrad();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

