#pragma once
#include "CLevel_Stage.h"

BEGIN(Client)

class CUI_HUD;
class CUnit;
class CPlayer;

class CLevel_BootCamp final
	: public CLevel_Stage
{
private:
	CLevel_BootCamp();
	virtual ~CLevel_BootCamp();

public:
	static CLevel_BootCamp* Create();

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;
};

END

