#pragma once
#include "CUnit.h"

BEGIN(Client)

class CUnit_Lancer_Head;
class CLancerNeedle;

class CUnit_Lancer
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Lancer);

	enum eNeedle { eNeedle_Max = 4 };

private:
	CUnit_Lancer();
	virtual ~CUnit_Lancer();

public:
	static	CUnit_Lancer* Create(const UNIT_MODEL_DATA& tUnitModelData);

public:
	CLancerNeedle* Get_Needle(_int iIndex) { return m_pNeedle[iIndex]; }
	void Reset_NeedleNums() { m_iNeedleNums = 0; }


public:
	virtual void On_Die() override;

public:
	void TurnOn_Trail(_bool bOn);

public:
	virtual void	SetUp_Colliders(_bool bPlayer);
	virtual void	SetUp_HitStates(UNIT_TYPE eUnitType);
	virtual void	SetUp_ReserveState(UNIT_TYPE eUnitType);
	virtual void	On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc);

public:
	void			Turn_TransformParticle(_bool bOnOff);

protected:
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos) override;

protected:
	void SetUp_Trail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_Trail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_LowerTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_LowerTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_HorseTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_HorseTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_EyeTrail_R(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_EyeTrail_L(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_LateTick();

private:
	CUnit_Lancer_Head* m_pMyHead = nullptr; // 새로 붙인 머리
	
private:
	list<CGameObject*> m_TransformParticles;
	CLancerNeedle* m_pNeedle[eNeedle_Max] = { nullptr }; // 새로 붙인 머리

	_float	m_fNeedleCreateTime = 1.f;
	_float	m_fTimeAcc = 0.f;
	_uint	m_iNeedleNums = 0;

	STATE_TYPE m_eBreezeBegin = STATE_END;
	STATE_TYPE m_eBreezeLoop = STATE_END;

private:
	_float4 m_vTrailShader;

	CTrailEffect* m_pTrail_R = nullptr;
	CTrailEffect* m_pTrail_R2 = nullptr;
	CTrailEffect* m_pTrail_L = nullptr;
	CTrailEffect* m_pTrail_L2 = nullptr;

	CTrailEffect* m_pLowerTrail_R = nullptr;
	CTrailEffect* m_pLowerTrail_R2 = nullptr;
	CTrailEffect* m_pLowerTrail_L = nullptr;
	CTrailEffect* m_pLowerTrail_L2 = nullptr;

	CTrailEffect* m_pHorseTrail_R = nullptr;
	CTrailEffect* m_pHorseTrail_R2 = nullptr;
	CTrailEffect* m_pHorseTrail_L = nullptr;
	CTrailEffect* m_pHorseTrail_L2 = nullptr;

	CTrailEffect* m_pEyeTrail_R = nullptr;
	CTrailEffect* m_pEyeTrail_R2 = nullptr;
	CTrailEffect* m_pEyeTrail_L = nullptr;
	CTrailEffect* m_pEyeTrail_L2 = nullptr;
};

END