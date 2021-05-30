#pragma once
#include "Obj.h"
class CDead :
	public CObj
{
public:
	explicit CDead();
	virtual ~CDead();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	FRAME m_tFrame;
	float m_fTime;
	float m_fPosY;
	int m_iColor;
};

