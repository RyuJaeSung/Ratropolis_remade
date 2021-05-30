#pragma once
class CGraphic_Device;
class CCradList
{
public:
	CCradList();
	~CCradList();
public:
	enum TYPE { BAG, CEMETRY, END};
	void Create(TYPE eType) { m_eType = eType; m_Execution = true; }

public:
	bool Get_Execution() { return m_Execution; }
	
public:
	void Render_Window();
	void Render_Button();
	void Render_Crad();

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	
private:
	CGraphic_Device* m_pDevice;
	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3	m_vSize;
	D3DXVECTOR3	m_vCradPos;
	D3DXVECTOR3 m_vCradSize;
	D3DXVECTOR3 m_vButtonPos;
	D3DXVECTOR3	m_vButtonSize;
	list<CRADINFO*> ListCrad;

	RECT m_rButtonRect;
	bool m_Execution;
	bool m_ButtonCkeck;
	TYPE m_eType;
};

