#pragma once
#include "Obj.h"
class CReward :
	public CObj
{
public:
	explicit CReward();
	virtual ~CReward();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	void Render_Box();
	void Render_Reward();
	void Render_Menu();
	void Render_Window();

public:
	bool ClickBox();
	bool KeyCkcek();

private:
	bool ClickBar[3];
	bool m_bClickBox;
	RECT m_rBar[3];
	RECT m_rBox;
	RECT m_rQuit;
	int m_iCkcekPos; //1.2.3 1,2,3번째 카드위에 마우스 있을때, 4번은 취소에 마우스가 있을때 그외 없음 
};

