#pragma once
#include "Client_Defines.h"
BEGIN(Engine)
class CGameObject;
END
BEGIN(Client)
class CMap_Loader
{
private:
	CMap_Loader();
	virtual ~CMap_Loader();
public:
	//	void	Ready_GameObject(CGameObject* pGameObject, _uint iGroupIndex);
	static void Load_Data(wstring strPath, function<void(CGameObject*, _uint)> Func_ReadyObject, _int Index = 0);
public:
	HRESULT Load_MapData();
	HRESULT Load_ColliderData();
private:
	HRESULT SetUp_TerrainData(ifstream& rhsReadFile, string& strPath);
	HRESULT SetUp_StructureData(ifstream& rhsReadFile, string& strPath);
	HRESULT SetUp_InstancingData(ifstream& rhsReadFile, string& strPath);
	HRESULT SetUp_NavData(ifstream& rhsReadFile, string& strPath);
	HRESULT SetUp_LightData(ifstream& rhsReadFile, string& strPath);
private:
	HRESULT LoadPath(ifstream& rhsReadFile, string& strPath);
	HRESULT SetUp_LoadOption(wstring strDataPath, function<void(CGameObject*, _uint)> Func_ReadyObject, _int Index = 0);
private:
	wstring m_strDataName;
	wstring m_strDataPath;
	function<void(CGameObject*, _uint)> Ready_Object;
};
END
