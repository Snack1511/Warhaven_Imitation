#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CLevel_Loading final 
	: public CLevel
{
private:
	CLevel_Loading();
	virtual ~CLevel_Loading() = default;

public:
	static CLevel_Loading* Create();

public:
	void	Set_NextLevel(LEVEL_TYPE_CLIENT	eLevelID) { m_eNextLevel = eLevelID; }

public:
	// CLevel��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render();
	virtual HRESULT Exit() override;

private:
	LEVEL_TYPE_CLIENT			m_eNextLevel = LEVEL_END;
	CShader* m_pLoadingBarShader = nullptr;
	

};

END