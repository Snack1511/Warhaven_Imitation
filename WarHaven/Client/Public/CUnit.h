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
class CBoneCollider;
class CHierarchyNode;
class CPhysXCharacter;
END

BEGIN(Client)

class CState;
class CTrailEffect;
class CUnit abstract
	: public CGameObject
{
public:
	enum CLASS_TYPE
	{
		CLASS_WARRIOR,
		CLASS_SPEAR,
		CLASS_ARCHER,
		CLASS_PALADIN,
		CLASS_PRIEST,
		CLASS_ENGINEER,
		CLASS_FIONA,
		CLASS_QANDA,
		CLASS_HOEDT,
		CLASS_LANCER,
		CLASS_END
	};

public:
	struct UNIT_STATUS
	{
		WEAPON_TYPE eWeapon;
		_float fHP = 100.f;
		_float fMaxHP = 100.f;
		_float fRunSpeed = 4.f;
		_float fRunBeginSpeed = 4.f;
		_float fWalkSpeed = 1.2f;
		_float fDashSpeed = 16.f;
		_float fWallJumpSpeed = 15.f;
		_float fSprintAttackSpeed = 11.F;
		_float fSprintJumpSpeed = 10.F;
		_float fSprintSpeed = 8.f;
		_float fJumpPower = 2.5f;
	};
	struct UNIT_MODEL_DATA
	{
		wstring	strModelPaths[MODEL_PART_END];
		string	strRefBoneName[MODEL_PART_END];
	};

public:
	enum UNITCOLLIDER{BODY, HEAD, WEAPON_L, WEAPON_R, UNITCOLLIDER_END};
	enum WEAPONCOLLIDER{ WEAPONCOLLIDER_END = 3};
	enum COOL_TYPE {SKILL1, COOL_END};

protected:
	CUnit();
	virtual ~CUnit();

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType);

public: /* Physics */
	_bool		Is_Air();
	void		Set_DirAsLook();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);
	void	Set_Passes(VTXANIM_PASS_TYPE ePassType);

public:
	virtual void	On_PlusHp(_float fHp);


public:
	_bool		Can_Use(COOL_TYPE eType) { if (m_fCoolAcc[eType] <= 0.f) return true; return false; }
	void		On_Use(COOL_TYPE eType) { m_fCoolAcc[eType] = m_fCoolTime[eType]; }

	_bool		Is_Weapon_R_Collision();


public:
	UNIT_STATUS& Get_Status() { return m_tUnitStatus; }
	CPhysics* Get_PhysicsCom() { return m_pPhysics; }

	CUnit* Get_TargetUnit() { return m_pTargetUnit; }
	void	Set_TargetUnit(CUnit* pUnit) { m_pTargetUnit = pUnit; }

	STATE_TYPE	Get_CurState() { return m_eCurState; }
	CState*	Get_CurStateP() { return m_pCurState; }


public:
	void SetUp_TrailEffect(WEAPON_TYPE eWeapon);
	void TurnOn_TrailEffect(_bool bOn);


public:
	void	Enter_State(STATE_TYPE eType);
	void	Reserve_State(STATE_TYPE eType);

public:
	void	Teleport_Unit(_float4 vPosition);

	void	Set_Controlable() { m_bControlable = !m_bControlable; }

public:
	struct UNIT_DESC
	{
		_bool bRight = false;
	};
	//기준 : 같은 유닛이어도 각기 다른 설정이 필요한 건 다 여기서
	//ex) 1. 모델 파츠별 조명 여부, 2. 충돌체 설정, 3. 시작 상태
	HRESULT	MakeUp_Unit(const UNIT_DESC& tUnitDesc);


public:
	// CGameObject을(를) 통해 상속됨
	
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	//PhysX 용 충돌체
	CPhysXCharacter* m_pPhysXCharacter = nullptr;
	CBoneCollider* m_pWeaponCollider_R = nullptr;

	//우리가 알던 그 충돌체
	CCollider_Sphere* m_pUnitCollider[UNITCOLLIDER_END] = {};
	//CCollider_Sphere* m_pUnitWeaponCollider[WEAPONCOLLIDER_END] = {};

public:
	void	Enable_UnitCollider(UNITCOLLIDER ePartType, _bool bEnable);

	struct UNIT_COLLIDERDESC
	{
		_float fRadius = 0.f;
		_float4 vOffsetPos = ZERO_VECTOR;
		_uint	eColType;
		
	};
	
	struct UNIT_COLLIDREINFODESC
	{
		_float4x4 matModelTransformation = DEFAULT_TRANS_MATRIX;
		_float4 vStartPos = ZERO_VECTOR;
		_uint	iNumCollider = 1;
		_bool	bEnable = true;
	};


	void	SetUp_UnitCollider(UNITCOLLIDER ePartType, UNIT_COLLIDERDESC* arrColliderDesc, 
		_uint iNumCollider = 1, _float4x4 matTransformation = DEFAULT_TRANS_MATRIX, _bool bEnable = true, CHierarchyNode* pRefBone = nullptr);


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
	_bool m_bControlable = true;

protected:
	CTrailEffect* m_pTrailEffect = nullptr;

protected:
	virtual	HRESULT	SetUp_Model(const UNIT_MODEL_DATA& tData);
	virtual	HRESULT	SetUp_Navigation(CCell* pStartCell);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	
};
END
