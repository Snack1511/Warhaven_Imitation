#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CPlayer;
class CUnit;

class CUI_Revive : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Revive);
	DECLARE_GAMEOBJECT(CUI_Revive);

private:
	CUI_Revive();
	virtual ~CUI_Revive();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_RevivalGauge(CShader* pShader, const char* pConstName);

public:
	void Set_ReviveUnitTransform(CTransform* pReviveUnitTransform)
	{
		m_pReviveUnitTransform = pReviveUnitTransform;
	}

	void Set_ReviveIcon(_uint iIconIndex);
	void Set_ClassIcon(CPlayer* pDeadPlayer);
	void Set_GaugeRatio(_float fCurTime, _float fMaxTime);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Update_Pos();

private:
	CUI_Object* m_pReviveIcon = nullptr;

	enum ClassIcon { Class_BG, Class_Icon, Class_Gauge, Class_End };
	CUI_Object* m_pClassIcon[Class_End];

	_uint m_iIndex = 0;

	CTransform* m_pReviveUnitTransform = nullptr;

	_float m_fGaugeRatio = 0.5f;

private:
	void Create_ReviveIcon();
	void Create_ClassIcon();
};

END