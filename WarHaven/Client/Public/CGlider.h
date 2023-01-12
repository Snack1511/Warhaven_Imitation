#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CAnimator;
class CHierarchyNode;
END


BEGIN(Client)

class CTrailEffect;
class CUnit;

class CGlider
	: public CGameObject
{
	DECLARE_PROTOTYPE(CGlider);

protected:
	CGlider();
	virtual ~CGlider();

public:
	static CGlider* Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName, 
		_float fRadianX = 270.f, _float fRadianY = 0.f, _float fRadianZ = 270.f);

public:
	enum eGliderState {eOpen, eLoop, eClose, eCNT};

public:
	void	Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);

	void	Set_GliderState(eGliderState eGilderState) { m_eCurState = eGilderState; }

public:
	void SetUp_GliderTrail();
	void TurnOn_Trail(_bool bOn);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;


private:
	CModel* m_pModelCom = nullptr;
	CUnit* m_pOwnerUnit = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CHierarchyNode* m_pOwnerBone = nullptr;
	eGliderState	m_eCurState = eOpen;

	_float		m_fScaleLoopTime = 0.f;
	_float4		m_vScale = ZERO_VECTOR;

private:
	HRESULT		SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, 
		_float fRadianX = 270.f, _float fRadianY = 0.f, _float fRadianZ = 270.f);
	void SetUp_RTrail(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount);
	void SetUp_LTrail(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
		_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount);

private:
	virtual void	Late_Tick() override;

private:
	CTrailEffect* m_pRWing = nullptr;
	CTrailEffect* m_pRWing2 = nullptr;
	CTrailEffect* m_pLWing = nullptr;
	CTrailEffect* m_pLWing2 = nullptr;
};

END