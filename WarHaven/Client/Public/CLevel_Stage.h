#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUnit;
class CPlayer;

class CLevel_Stage abstract
	: public CLevel
{
protected:
	CLevel_Stage();
	virtual ~CLevel_Stage();

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT OnExit() override;
	virtual HRESULT Exit() override;

private:
	_bool	m_bStaticShadowBake = false;
	_float	m_fDealyAcc = 0.f;
	_float	m_fDelayTime = 0.1f;
};

END

