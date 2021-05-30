#pragma once
#include "Obj.h"
class CMyFont :
	public CObj
{
public:
	void Get_Cont(const wstring wstrCont){ m_wstrObjectKey = wstrCont;}
	void Get_Color(const UINT uColor) { m_uColor = uColor; }
public:
	explicit CMyFont();
	virtual ~CMyFont();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	UINT	m_uColor;
};

