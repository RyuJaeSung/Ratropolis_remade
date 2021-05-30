#pragma once
#include "Texture.h"
class CMultiTex :
	public CTexture
{
public:
	explicit CMultiTex();
	virtual ~CMultiTex();

public:
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey, const int & rCount) override;
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const int & rCount) override;
	virtual void Release() override;

private:
	map<wstring, vector<TEXINFO*>> m_mapMultiTex;
};

