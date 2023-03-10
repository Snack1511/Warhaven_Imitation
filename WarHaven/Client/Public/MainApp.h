#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
	class CGameInstance;
END

BEGIN(Client)
//class CGameInstance;
class CMainApp
{
	DECLARE_SINGLETON(CMainApp)

private:
	CMainApp();
	~CMainApp();

public:
	HRESULT Initialize();	
	HRESULT	Progress();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	//PxPvd* m_pPVD;

private:
	HRESULT Render();
	void	Release();
	HRESULT	Default_Initialize();
	HRESULT	SetUp_Engine();
	HRESULT	SetUp_Levels();
	HRESULT	SetUp_Statics();
	HRESULT	SetUp_ShaderFiles();
	HRESULT SetUp_Font();

private:
	HRESULT Load_SoundFile();
private:
	void	Col_Check();


};

END