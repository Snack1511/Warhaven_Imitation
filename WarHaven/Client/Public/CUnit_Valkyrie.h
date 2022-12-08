#pragma once
#include "CUnit.h"

BEGIN(Client)
class CTrailEffect;

class CUnit_Valkyrie
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Valkyrie);

private:
	CUnit_Valkyrie();
	virtual ~CUnit_Valkyrie();

public:
	static	CUnit_Valkyrie* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);


public:
	virtual void On_Die() override;

public:
	void TurnOn_ValkyrieTrail(_bool bOn);

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

protected:
	void SetUp_ValkyrieTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_ValkyrieTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_ValkyrieLowerTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_ValkyrieLowerTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	_float4 m_vTrailShader;

	CTrailEffect* m_pValkyrieTrail_R = nullptr;
	CTrailEffect* m_pValkyrieTrail_R2 = nullptr;
	CTrailEffect* m_pValkyrieTrail_L = nullptr;
	CTrailEffect* m_pValkyrieTrail_L2 = nullptr;

	CTrailEffect* m_pValkyrieLowerTrail_R = nullptr;
	CTrailEffect* m_pValkyrieLowerTrail_R2 = nullptr;
	CTrailEffect* m_pValkyrieLowerTrail_L = nullptr;
	CTrailEffect* m_pValkyrieLowerTrail_L2 = nullptr;
};

END