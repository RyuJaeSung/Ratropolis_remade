#pragma once
#include "Obj.h"
class CErrMsg :
	public CObj
{
public:
	explicit CErrMsg();
	virtual ~CErrMsg();

public:
	void Set_Text(const wstring wstrTex) { m_wstrTex = wstrTex; }

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	wstring m_wstrTex;
	float m_fDeleteTime;
	int	m_iRColor;
};

