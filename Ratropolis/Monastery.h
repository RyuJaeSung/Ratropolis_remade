#pragma once
#include "Obj.h"
class CMonastery :
	public CObj
{
public:
	explicit CMonastery();
	virtual ~CMonastery();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void Render_Building();
	void Render_Bell();
	void Render_DeleteCrad();
	void Render_Window();
	void Render_Button();
	void Render_Crad();

public:
	void Delete_Crad();
	void Time();
	void Ckeck();

private:
	RECT	m_rButtonRect;
	RECT	m_rBell;
	bool	m_ButtonCkeck;
	bool	m_bCreate_Bell;
	bool	m_bDelete_Crad;
	float	m_fTime;
	float	m_fMaxTime;
	float	m_fCoolTime;
};

