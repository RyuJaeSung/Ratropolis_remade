#pragma once
#include "Texture.h"
class CSingleTex :
	public CTexture
{
public:
	explicit CSingleTex();
	virtual ~CSingleTex();

	// CTexture을(를) 통해 상속됨
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & rCount = 1) override;
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const int & rCount = 1) override;
	virtual void Release() override;

private:
	TEXINFO* m_pTexInfo;
};

