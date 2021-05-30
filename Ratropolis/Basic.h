#pragma once
#include "Obj.h"
class CBasic :
	public CObj
{
public:
	explicit CBasic();
	virtual ~CBasic();

public:
	void Add_Attack();
	void State();
	void Move();
	void Attack();
public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	bool	m_bAttackTime;
	bool	m_bStartPos;
	bool	m_bState;
	float	m_fDetection;
	float m_fTime;
	FRAME m_tFrame;
	PLAYER::STATE m_eNowState;
	PLAYER::STATE m_eNewState;
};

