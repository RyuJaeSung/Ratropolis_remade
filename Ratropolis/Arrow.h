#pragma once
#include "Obj.h"
class CArrow :
	public CObj
{
public:
	explicit CArrow();
	virtual ~CArrow();
public:
	enum DIR
	{
		LEFT, RIGHT, END
	};


public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
	
public:
	void Parabola();
	void Move();
	bool Attack();
	

public:
	const void Set_Target(CObj* vTarget) { m_pTarget = vTarget; }
	const void Set_Damage(int iDamage) { m_iDamage = iDamage; }

private:
	D3DXVECTOR3 m_vStartPos;
	CObj*	m_pTarget;
	bool	m_bDelete_Target;
	int		m_iDamage;
	float	m_fSpeedX;
	float	m_fSpeedY;
	DIR		m_eDir;

	float m_fFG; //중력가속도
	float m_fEndTime; //도착지점 도달시간
	float m_fMaxHeight; //최대높이
	float m_fHeight;	// 최대높이 - 시작높이
	float m_fEndHeight; //도착점 높이
	float m_fTime; //진행 시간
	float m_fMaxTime; //최대높이까지 가는 시간
};