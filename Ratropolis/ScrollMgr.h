#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	const float Get_ScrollX() { return m_fScrollX; }
	const POINT Get_MousePos() {return m_tMousePos;	}
public:
	void Set_ScrollX(const float& fScrollX) { m_fScrollX += fScrollX; }
public:
	void Map_Move();
	const void ReSet() { m_fScrollX = 0.f; }
public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CScrollMgr* m_pInstance;
	POINT m_tMousePos;
	float m_fScrollX;
	float m_fSpeed;
};

