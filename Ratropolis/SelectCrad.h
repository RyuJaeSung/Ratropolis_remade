#pragma once
#include "Obj.h"
class CSelectCrad :
	public CObj
{
public:
	explicit CSelectCrad();
	virtual ~CSelectCrad();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	bool KeyCkeck();
	void RandomCrad();
	void Render_Crad();
	void Render_Window();

private:
	CRADINFO* m_tCradInfo[3];
	wstring m_wstrKey[3];
	RECT m_rCrad[3];
	RECT m_rQuit;
	int m_iCkcekPos; //1.2.3 1,2,3번째 카드위에 마우스 있을때, 4번은 취소에 마우스가 있을때 그외 없음
};

