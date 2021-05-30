#pragma once
class CObj;
class CGameMenu;
class CCradList;
class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Update();
	void Late_Update();
	void Render();
	void Release();
	
public:
	void Add_Object(OBJ::ID eID, CObj* pObj) { m_ObjList[eID].emplace_back(pObj); }
	void Clear_Object(OBJ::ID eID);
	void Crad_Option(CRAD::TYPE dwType, const int CradNumber);
	void Button_Ckeck();
	void Click_Mouse();
	bool NoMoney(int Money);
	void NoCreate();

public:
	void Option_Economy(CRAD::ECONOMY eCrad);
	void Option_Military(CRAD::MILITARY eCrad);
	void Option_Building(CRAD::BUILDING eCrad);
	void Option_Technology(CRAD::MYTECHNOLOGY eCrad);

public:
	void Delete_CoolTimeCount() { --m_iCoolTimeCount; }

public:
	void Set_ClickCrad(bool click) { m_bClickCrad = click; }

public:
	const list<CObj*> Get_Object(OBJ::ID eID) { return m_ObjList[eID]; }
	const wstring Get_Texture(CRAD::TYPE dwType, const int CradNumber);
	const bool Get_ClickCrad() { return m_bClickCrad; }
	const bool Get_NotCreate() { return m_bNotCreate; }
public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CObjMgr* m_pInstance;
	list<CObj*>	m_ObjList[OBJ::OBJ_END];
	CCradList*	m_pCradList;
	CGameMenu*	m_pMenu;
	D3DXVECTOR3 m_vSavePos;
	bool m_bClickCrad;
	bool m_bNotCreate;
	int m_iCount;
	int m_iCoolTimeCount;
};

