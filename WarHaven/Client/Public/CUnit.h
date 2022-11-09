#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CAnimator;
class CPhysics;
class CCell;
class CCollider;
class CCollider_Sphere;
class CShader;
END

BEGIN(Client)

class CState;

class CUnit abstract
	: public CGameObject
{
public:
	struct UNIT_STATUS
	{
		_float fHP = 100.f;
		_float fMaxHP = 100.f;
		_float fRunSpeed = 7.5f;
		_float fShortDashSpeed = 11.f;
		_float fWalkSpeed = 2.f;
		_float fDashSpeed = 16.f;
		_float fWallJumpSpeed = 15.f;
		_float fWireSpeed = 25.f;
		_float fJumpPower = 6.5f;
	};

	struct UNIT_MODEL_DATA
	{
		wstring	strModelPaths[MODEL_PART_END];
		string	strRefBoneName[MODEL_PART_END];
	};

public:
	enum COOL_TYPE {SKILL1, COOL_END};

protected:
	CUnit();
	virtual ~CUnit();

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType) {};

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);
	void	Set_Passes(VTXANIM_PASS_TYPE ePassType);

public:
	virtual void	On_PlusHp(_float fHp);


public:
	_bool		Can_Use(COOL_TYPE eType) { if (m_fCoolAcc[eType] <= 0.f) return true; return false; }
	void		On_Use(COOL_TYPE eType) { m_fCoolAcc[eType] = m_fCoolTime[eType]; }



public:
	UNIT_STATUS& Get_Status() { return m_tUnitStatus; }
	CPhysics* Get_PhysicsCom() { return m_pPhysics; }

	CUnit* Get_TargetUnit() { return m_pTargetUnit; }
	void	Set_TargetUnit(CUnit* pUnit) { m_pTargetUnit = pUnit; }

	STATE_TYPE	Get_CurState() { return m_eCurState; }
	CState*	Get_CurStateP() { return m_pCurState; }

public:
	void	Enter_State(STATE_TYPE eType, ANIM_TYPE eAnimType);


public:
	// CGameObject을(를) 통해 상속됨
	
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	_float	m_fCoolTime[COOL_END] = {};
	_float	m_fCoolAcc[COOL_END] = {};

protected:
	UNIT_MODEL_DATA	m_tModelData;


protected:
	CModel*			m_pModelCom = nullptr;
	CAnimator*		m_pAnimator = nullptr;
	CPhysics*		m_pPhysics = nullptr;

	UNIT_STATUS		m_tUnitStatus;
	STATE_TYPE		m_eCurState = STATE_END;


	CState* m_pCurState = nullptr;
	CUnit* m_pTargetUnit = nullptr;


protected:
	virtual	HRESULT	SetUp_Model(const UNIT_MODEL_DATA& tData);
	virtual	HRESULT	SetUp_Navigation(CCell* pStartCell);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	ANIM_TYPE		m_eAnimType = ANIM_END;
	
};
END
