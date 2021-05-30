#pragma once
#include "Obj.h"
class CCreatePos :
	public CObj
{
public:
	explicit CCreatePos();
	virtual ~CCreatePos();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void Create_Militory(int Tile);
	bool NoCreate_Pop();
public:
	bool Get_Create() { return m_bRender; }

public:
	void Set_Create(bool Set) { m_bCreate = Set; }
	void Set_Name(PLAYER::SOIDIER eName) { m_eName = eName; }
	void Set_NameState(const wstring& wstrState) { m_wstrStage = wstrState; }
private:
	PLAYER::SOIDIER m_eName;
	wstring m_wstrStage;
	bool m_bRender;
	bool m_bCreate;
	int m_iWallNum;
	float m_fTime;
	float m_fMaxY;
	float m_fMinY;
	float m_fNowY;
	float m_fMove;
};

