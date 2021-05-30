#pragma once
#include "Obj.h"
class CShield :
	public CObj
{
public:
	explicit CShield();
	virtual ~CShield();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void State();
	void Stand();
	void Move();
	void Attack();
	void Find_Target();
	void Defense();

private:
	CObj* pTarget;
	float m_fWallPosX;
	bool m_bDefense;
	int m_iSkill;
	int m_iTileConut;
	int m_iDefense;
	int m_iNowHp;
	float m_iAttackPos;
	float m_fDetection;
	float m_fTime;
	FRAME m_tFrame;
	PLAYER::STATE m_eNowState;
	PLAYER::STATE m_eNewState;
};

