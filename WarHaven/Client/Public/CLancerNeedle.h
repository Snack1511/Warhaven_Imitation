#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
class CPhysics;



END

BEGIN(Client)

class CPlayer;
class CUnit;

class CLancerNeedle
	: public CGameObject
{
	DECLARE_PROTOTYPE(CLancerNeedle);

public:
	enum LANCERNEEDLE 
	{	LANCERNEEDLE_START,			LANCERNEEDLE_LOOP, 
		LANCERNEEDLE_ATTACKBEGIN,	LANCERNEEDLE_ATTACK, 
		LANCERNEEDLE_STOP,			LANCERNEEDLE_END };

private:
	CLancerNeedle();
	virtual ~CLancerNeedle();


public:
	static	CLancerNeedle* Create(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit, _uint iNeedleIndex);


public:
	virtual void	Needle_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Needle_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Needle_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

public:
	void On_ChangePhase(LANCERNEEDLE eNeedleState);
	void Enable_Needle(_bool bEnable);

public:
	HRESULT	 SetUp_Model(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit);



public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	CHierarchyNode* m_pOwnerBone = nullptr;
	CUnit*			m_pOwnerUnit = nullptr;
	_uint			m_iNeedleIndex = 0;

	CUnit*			m_pStinedUnit = nullptr; // Âñ¸° À¯´Ö

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	
private:
	void Chase_OwnerBoneMatrix();
	void Dragging_Unit();


private:
	LANCERNEEDLE	m_eNeedleState = LANCERNEEDLE_END;

	_float4	m_vScale = ZERO_VECTOR;
	_float4 m_vStartPos = _float4(-163.f, 6.f, -1.2f);
	_float4 m_vTargetPos = ZERO_VECTOR;
	_float4 m_vPrePos = ZERO_VECTOR;

	_float4	m_vLandLook = ZERO_VECTOR;
	_float	m_fSinAngle = 0.f;

	_float	m_fCurAcc = 0.f;
	_float	m_fTotalTime = 0.5f;


	_float m_fAngleStartValue = 0.f;
	_float m_fAngleTargetValue = 260.f;

	_float4 m_vLerpPos = ZERO_VECTOR;

	_float4x4 m_OwnerBoneOffsetMatrix;

private:
	CGameObject* m_pNiddleMesh = nullptr;
	_bool m_bStartNeedle = true;





};

END