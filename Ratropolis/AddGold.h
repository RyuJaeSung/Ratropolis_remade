#pragma once
#include "Obj.h"
class CAddGold :
	public CObj
{
public:
	explicit CAddGold();
	virtual ~CAddGold();

	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	TCHAR m_AddGold[16];
	bool CreateGold;
	float m_fTime;
	float m_fDeleteTime;
	float m_fDeleteY;
	int	m_iRColor;
};
