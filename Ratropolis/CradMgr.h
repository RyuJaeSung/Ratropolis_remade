#pragma once

class CGraphic_Device;
class CCradMgr
{
private:
	CCradMgr();
	~CCradMgr();

public:
	HRESULT Initialize();
	void Update();
	void Render();
	void Release();

public:
	void Add_Render();
	void Render_CoolTime();
	void Render_Price();

public:
	HRESULT	LoadCradData(const wstring& wstrFilePath);
	void Select_Crad(int num = 0);
	void Change_Crad(list<CRADINFO*>& DeleteList, list<CRADINFO*>& AddList);
	void Setting_Crad();
	void Release_Crad(int iNumber);
	void Delete_Crad(int iNumber);
	void Clear_Crad();
	bool Skill_Time();

public:
	void Set_MouseCheck() { m_bMouseCheck = true; }
	void Set_Ckeck(bool Ckeck) { m_bFarmMoney = Ckeck; }
public:
	const bool Get_Ckeck() { return m_bFarmMoney; }
	CRADINFO* Get_Crad(const wstring& wstrObjectKey);
	list<CRADINFO*>& Get_ListBag() { return m_listBag; }
	list<CRADINFO*>& Get_ListHands() { return m_listHands; }
	list<CRADINFO*>& Get_ListCemetry() { return m_listCemetry; }

public:
	static CCradMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CCradMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CCradMgr* m_pInstance;
	CGraphic_Device* m_pDevice;
	map<wstring, CRADINFO*> m_mapCradInfo;
	INFO		m_tInfo;

	list<CRADINFO*> m_listBag;
	list<CRADINFO*> m_listHands;
	list<CRADINFO*> m_listCemetry;

	bool m_bMouseCheck;
	bool m_bFarmMoney;
	bool m_bNoMoney;	//ture : 가능 false : 불가능
	float m_fReSetTime;
	int m_iCoolTime;
	int m_iHandSize;
	int iReTurnPrice;
};

