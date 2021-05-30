#pragma once
#include "Obj.h"
class CDefenseWall :
	public CObj
{
public:
	explicit CDefenseWall();
	virtual ~CDefenseWall();

public:
	void Render_Wall();

public:
	void Wall_Hp();
	const void Add_Militory(PLAYER::SOIDIER eID, CObj* pObj) { m_listSoilder[eID].emplace_back(pObj); }

public:
	int Get_WallState() { return m_iReinforce; }
	int Get_TileNum() { return m_iTileNum; }
	bool Get_SoiderSize();
	list<CObj*>& Get_SoilderList(PLAYER::SOIDIER eID) { return m_listSoilder[eID]; }
	
public:
	const void Set_LevelWall(int iLv) { m_iReinforce += iLv; }

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	list<CObj*> m_listSoilder[PLAYER::SOLDER_END];
	FRAME m_tFrame;
	//0. 나무(벽아직 세우지 않음) , 1. 나무벽  2.돌벽
	int m_iReinforce;
	int m_iHp;
	int m_iTileNum;
	int m_iMaxHp;
};

