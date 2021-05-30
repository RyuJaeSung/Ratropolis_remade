#pragma once
#include "Scene.h"
class CCharacterSelect : 
	public CScene
{
public:
	explicit CCharacterSelect();
	virtual ~CCharacterSelect();

public:
	void Add_UI();

public:
	void Key_Check(POINT& pMouse);
	void BackGround_Render();
	void Font_Render();
	void Explanation_Render();
	void Check_Render();

public:
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	FONT	m_tStart;
	FONT	m_tBack;
};

