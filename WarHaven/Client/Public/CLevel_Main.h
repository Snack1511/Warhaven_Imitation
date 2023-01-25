#pragma once
#include "Level.h"
class CLevel_Main : public CLevel
{
private:
	CLevel_Main();
	virtual ~CLevel_Main();

public:
	static CLevel_Main* Create();

public:
	// CLevel��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;

};

