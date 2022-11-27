#pragma once
#include "CLevel_Stage.h"

BEGIN(Client)

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
};

END