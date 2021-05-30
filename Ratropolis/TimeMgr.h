#pragma once
class CTimeMgr
{
private:
	CTimeMgr();
	~CTimeMgr();

public:
	const float Get_DeltaTime() { return m_fDeltaTime; }

public:
	void Update();

public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTimeMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	static CTimeMgr* m_pInstance;

	LARGE_INTEGER m_CpuTick;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;

	float		m_fDeltaTime;
};

