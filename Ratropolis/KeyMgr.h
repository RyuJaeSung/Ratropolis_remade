#pragma once
class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	void Update();
	bool KeyUp(const DWORD& dwKey);
	bool KeyDown(const DWORD& dwKey);
	bool KeyPressing(const DWORD& dwKey);

public:
	DWORD& Get_KeyUp() { return m_dwUp; }
	DWORD& Get_KeyDown() { return m_dwDown; }

public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CKeyMgr* m_pInstance;
	DWORD	m_dwKey;
	DWORD	m_dwUp;
	DWORD	m_dwDown;
};

