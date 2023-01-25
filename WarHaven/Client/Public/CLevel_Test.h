#pragma once
#include "CLevel_Stage.h"

BEGIN(Client)

class CUI_HUD;
class CUnit;
class CPlayer;

class CLevel_Test final
	: public CLevel_Stage
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
	virtual HRESULT OnExit() override;
};

END

