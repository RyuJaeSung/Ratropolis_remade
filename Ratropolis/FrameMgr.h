#pragma once
class CFrameMgr final
{
private:
	CFrameMgr();
	~CFrameMgr();
public:
	void Initialize(float fFPS);
	bool LockFrame();
	void RenderFrame();

public:
	static CFrameMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CFrameMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CFrameMgr* m_pInstance;
	LARGE_INTEGER	m_CpuTick;
	LARGE_INTEGER	m_BeginTime;
	LARGE_INTEGER	m_EndTime;

	float			m_fDeltaTime;
	float			m_fSecondPerFrame;
	float			m_fFPSTime;

	int				m_iFPS;
	TCHAR			m_szFPS[32];
};

