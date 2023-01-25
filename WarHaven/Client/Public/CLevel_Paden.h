#pragma once
#include "CLevel_Stage.h"

BEGIN(Client)

class CTrigger;

class CLevel_Paden final : public CLevel_Stage
{
private:
	CLevel_Paden();
	virtual ~CLevel_Paden();

public:
	static CLevel_Paden* Create();

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
	void BirdSound();
	void WindSound();//Env_CannonWind_0

private:
	CTrigger* m_pPadenTrigger_A = nullptr;
	CTrigger* m_pPadenTrigger_R = nullptr;
	CTrigger* m_pPadenTrigger_C = nullptr;

private:
	_float m_fBirdTimeAcc = 0.f;
	_float m_fBird = 5.f;
	_float m_fWindTimeAcc = 0.f;
};

END