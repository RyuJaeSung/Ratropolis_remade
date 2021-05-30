#include "stdafx.h"
#include "SingleTex.h"
#include "Graphic_Device.h"

CSingleTex::CSingleTex()
{
}


CSingleTex::~CSingleTex()
{
	Release();
}

const TEXINFO * CSingleTex::Get_TexInfo(const wstring & wstrStateKey /*= L""*/, const int & rCount /*= 1*/)
{
	return m_pTexInfo;
}

HRESULT CSingleTex::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const int & rCount /*= 1*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(m_pTexInfo));

	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImageInfo)))
	{
		wstring wstrErrMsg = wstrFilePath + L"Load Failed";
		ERR_MSG(wstrErrMsg.c_str());
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphic_Device::Get_Instance()->Get_Device(),
		wstrFilePath.c_str(),
		m_pTexInfo->tImageInfo.Width,
		m_pTexInfo->tImageInfo.Height,
		m_pTexInfo->tImageInfo.MipLevels,
		0,
		m_pTexInfo->tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_pTexInfo->pTexture)))
	{
		wstring wstrErrMsg = wstrFilePath + L"Texture Create Failed";
		ERR_MSG(wstrErrMsg.c_str());
		return E_FAIL;
	}
	return S_OK;
}

void CSingleTex::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
