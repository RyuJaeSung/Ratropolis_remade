#pragma once
class CGraphic_Device;
class CMouseMgr
{
private:
	CMouseMgr();
	~CMouseMgr();

public:
	void Initialize();
	void Render();
	void Release();

public:
	void Set_State(int iState) { m_iState = iState; }


public:
	static CMouseMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CMouseMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}


private:
	static CMouseMgr* m_pInstance;
	CGraphic_Device* m_pDevice;
	int m_iState;
};

