#pragma once
class CScene;
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	bool Update();
	void Late_Update();
	void Render();
	void Release();
	
public:
	void Scene_Change(SCENE::ID eNextScene);
public:
	STAGEINFO& Get_Stage();
	CScene* Get_Scene() { return m_pScene; }
public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CSceneMgr*	m_pInstance;
	SCENE::ID	m_eNewScene;
	SCENE::ID	m_eNowScene;
	STAGEINFO m_eNULL;
	CScene*	m_pScene;
};

