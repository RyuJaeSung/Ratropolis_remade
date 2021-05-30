#pragma once
#include "Obj.h"
class CCoolTime :
	public CObj
{
public:
	explicit CCoolTime();
	virtual ~CCoolTime();

public:
	enum TYPE {GOLD, TYPE_END};

public:
	void Option_Type();

public:
	void Render_CoolTimeBar();
	void Render_Type();
	void Render_TopBar(const TEXINFO* pTexInfo);
	void Render_TopBack(const TEXINFO* pTexInfo);
	void Render_BottomBar(const TEXINFO* pTexInfo);
	void Render_BottomBack(const TEXINFO* pTexInfo);


public:
	// 0 : ∞ÒµÂ√ﬂ∞° 
	void Set_Frame(const D3DXVECTOR3& vPos, float iConut, int iType) { m_tInfo.vPos = vPos;	m_tFrame.fMaxFrame = iConut; m_eType = (TYPE)iType; }
	void Set_Gold(int iGold) { m_iGold = iGold; }
	void Set_Pop(int iPop) { m_iPop = iPop; }

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
	
private:
	FRAME m_tFrame;
	TYPE m_eType;
	int	m_iTime;
	int m_iGold;
	int m_iPop;
	float m_fTurnAngle;
	bool m_fTurn;
};

