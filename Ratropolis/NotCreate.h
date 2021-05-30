#pragma once
#include "Obj.h"
class CNotCreate :
	public CObj
{
public:
	explicit CNotCreate();
	virtual ~CNotCreate();

public:
	void Build();
	bool Ckeck_Build();
	void Change_Pos();
public:
	void Get_Name(const wstring& wstrName) { m_BuildingName = wstrName; }
	void Get_Build(bool Create) { m_bBuild = Create; }
	void Get_Render(bool bRender) { m_bRender = bRender; }
	void Get_BuildSize(int num) { m_iBuildSize = num; }

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	wstring m_BuildingName;
	float m_fAdd_PosY;
	bool m_bBuild;
	bool m_bRender;
	int	m_iBuildSize;
	int m_iTileNumber;
	int m_iMin;
	int m_iMax;
};

