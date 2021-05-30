#pragma once
#include "Scene.h"
class CMyMenu : 
	public CScene
{
public:
	explicit CMyMenu();
	virtual ~CMyMenu();

public:
	void Render_Font();
	void Key_Check();
public:
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;
	
private:
	FONT	m_tStart;
	FONT	m_tEnd;
};

