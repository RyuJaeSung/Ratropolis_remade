#pragma once

template<class T>
void Safe_Delete(T& rObj)
{
	if (rObj)
	{
		delete rObj;
		rObj = nullptr;
	}
}
template<typename T>
void Safe_Release(T& rComObject)
{
	if (rComObject)
	{
		rComObject->Release();
	}
}