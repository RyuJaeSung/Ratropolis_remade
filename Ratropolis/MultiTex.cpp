#include "stdafx.h"
#include "MultiTex.h"
#include "Graphic_Device.h"

CMultiTex::CMultiTex()
{
}


CMultiTex::~CMultiTex()
{
	Release();
}

const TEXINFO * CMultiTex::Get_TexInfo(const wstring & wstrStateKey, const int & rCount)
{
	auto& iter_Find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() == iter_Find)
		return nullptr;

	return m_mapMultiTex[wstrStateKey][rCount];
}

HRESULT CMultiTex::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const int & rCount)
{
	auto& iter = m_mapMultiTex.find(wstrStateKey);
	if (iter != m_mapMultiTex.end())
		return E_FAIL;

	WCHAR szFilePath[MAX_PATH] = L"";

	for (int i = 0; i < rCount; ++i)
	{
		wsprintf(szFilePath, wstrFilePath.c_str(), i);
		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImageInfo)))
		{
			wstring wstrErrMsg = wstring(szFilePath) + L"Load Failed";
			ERR_MSG(wstrErrMsg.c_str());
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(
			CGraphic_Device::Get_Instance()->Get_Device(),
			szFilePath,
			pTexInfo->tImageInfo.Width,
			pTexInfo->tImageInfo.Height,
			pTexInfo->tImageInfo.MipLevels,
			0,
			pTexInfo->tImageInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			nullptr,
			nullptr,
			&pTexInfo->pTexture)))
		{
			wstring wstrErrMsg = wstring(szFilePath) + L"Texture Create Failed";
			ERR_MSG(wstrErrMsg.c_str());
			return E_FAIL;
		}
		m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo);
	}
	return S_OK;
}

void CMultiTex::Release()
{
	for (auto& rPair : m_mapMultiTex)
	{
		for (auto& pTexInfo : rPair.second)
		{
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
		}
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
}
