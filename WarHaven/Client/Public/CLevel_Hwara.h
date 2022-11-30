#pragma once
#include "CLevel_Stage.h"

BEGIN(Client)

class CLevel_Hwara final : public CLevel_Stage
{
private:
	CLevel_Hwara();
	virtual ~CLevel_Hwara();

public:
	static CLevel_Hwara* Create();

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