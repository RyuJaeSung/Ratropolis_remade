#pragma once
#include "Obj.h"
class CLongBow :
	public CObj
{
public:
	explicit CLongBow();
	virtual ~CLongBow();

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
	void RightMove();
	void LeftMove();
	void Attack();
	void Find_Target();
	void Create_Arrow();

private:
	CObj* m_pTarget;
	bool m_bTurn;
	float m_fWallPosX;
	int m_iTileConut;
	float m_iAttackPos;
	float m_fDetection;
	float m_fTime;
	FRAME m_tFrame;
	PLAYER::STATE m_eNowState;
	PLAYER::STATE m_eNewState;

};

