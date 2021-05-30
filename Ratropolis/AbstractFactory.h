#pragma once
class CErrMsg;
class CCrad;
class CMyFont;
class CObj;
template <class T>
class CAbstractFactroy
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float Gold)
	{
		CObj* pObj = new T;
		pObj->Set_Angle(Gold);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(D3DXVECTOR3& vecPos)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(vecPos);
		return pObj;
	}

	static CObj* Create(const wstring& wstrTex)
	{
		CObj* pObj = new T;
		dynamic_cast<T*>(pObj)->Set_Text(wstrTex);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecSize)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(vecPos);
		pObj->Set_Size(vecSize);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecSize, const wstring wstrCont, UINT uColor)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(vecPos);
		pObj->Set_Size(vecSize);
		dynamic_cast<CMyFont*>(pObj)->Get_Cont(wstrCont);
		dynamic_cast<CMyFont*>(pObj)->Get_Color(uColor);
		pObj->Initialize();
		return pObj;
	}


	static CObj* Create(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecSize, wstring wstrObjectKey, wstring wstrStateKey, DWORD dwCount)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(vecPos);
		pObj->Set_Size(vecSize);
		pObj->Set_Path(wstrObjectKey, wstrStateKey, dwCount);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(D3DXVECTOR3& vecPos, D3DXVECTOR3& vecSize, float fAngle, wstring wstrObjectKey, wstring wstrStateKey, DWORD dwCount)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(vecPos);
		pObj->Set_Size(vecSize);
		pObj->Set_Angle(fAngle);
		pObj->Set_Path(wstrObjectKey, wstrStateKey, dwCount);
		pObj->Initialize();
		return pObj;
	}
};