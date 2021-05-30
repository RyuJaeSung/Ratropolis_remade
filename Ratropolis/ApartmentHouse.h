#pragma once
#include "Obj.h"
class CApartmentHouse :
	public CObj
{
public:
	explicit CApartmentHouse();
	virtual ~CApartmentHouse();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

