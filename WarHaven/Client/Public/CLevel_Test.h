#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUI_HUD;
class CUnit;

class CLevel_Test final
	: public CLevel
{
private:
	CLevel_Test();
	virtual ~CLevel_Test();

public:
	static CLevel_Test* Create();

public:
	// CLevel을(를) 통해 상속됨
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

	void	My_FreeCam();


private:
	HRESULT SetUp_Warrior_TH();
	HRESULT SetUp_SpearMan_TH();
	HRESULT SetUp_WarHammer_TH();
	HRESULT SetUp_Valkyrie_TH();



	HRESULT	SetUp_Warrior_Sandback();

	void	Change_Player();

private:
	CGameObject* m_pTestObj = nullptr;

	CUnit* m_pWarrior = nullptr;
	CUnit* m_pWarHammer = nullptr;

	CUI_HUD* m_pHUD = nullptr;

private:
	_bool	m_bStaticShadowBake = false;
	_float	m_fDealyAcc = 0.f;
	_float	m_fDelayTime = 0.1f;
};

END

