#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

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
	CGameObject* m_pTestObj = nullptr;
};

END

