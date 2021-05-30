#pragma once
#include "Obj.h"
class CChangeWall :
	public CObj
{
public:
	explicit CChangeWall();
	virtual ~CChangeWall();

	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
	
public:
	void Change_Wall(int Tile);
	bool Before_Wall(int Tile);

public:
	void Set_Not(bool Set) { m_bNot = Set; }
	bool Get_No_Create() { return m_bCreate; }

private:
	int m_iWall;
	bool m_bNot;
	bool m_bCreate;
	float m_fTime;
	float m_fMaxY;
	float m_fMinY;
	float m_fNowY;
	float m_fMove;
	int m_iWallNum;
};

