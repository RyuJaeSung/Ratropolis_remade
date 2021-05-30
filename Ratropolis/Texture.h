#pragma once
class CTexture
{
public:
	explicit CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"", const int& rCount = 1) = 0;

public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const int& rCount = 1) = 0;
	virtual void Release() = 0;
};

