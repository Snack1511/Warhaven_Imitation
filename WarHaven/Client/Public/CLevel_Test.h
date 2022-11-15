#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUI_HUD;

class CLevel_Test final
	: public CLevel
{
private:
	CLevel_Test();
	virtual ~CLevel_Test();

public:
	static CLevel_Test* Create();

public:
	// CLevel��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;


private:
	HRESULT SetUp_Prototypes_JJ();
	HRESULT SetUp_Prototypes_TH();
	HRESULT SetUp_Prototypes_MJ();
	HRESULT SetUp_Prototypes_HR();
	HRESULT SetUp_Prototypes_YJ();

	void	Col_Check();


private:
	HRESULT SetUp_Warrior_TH();
	HRESULT SetUp_SpearMan_TH();

private:
	CGameObject* m_pTestObj = nullptr;

	CUI_HUD* m_pHUD = nullptr;
};

END

