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
class CScript_FollowCam;
class CCamera_Follow;
class CPlayer;
class CAnimWeapon;
class CCannon;

class CUnit abstract : public CGameObject
{

public:
	struct UNIT_STATUS
	{
		CLASS_TYPE	eClass = WARRIOR;

		_float		fCamPower = 3.f;
		_float		fCamTime = 0.5f;


		WEAPON_TYPE eWeapon;
		_float fHP = 300.f;
		_float fMaxHP = 300.f;
		_float fGuardBreakSpeed = 10.f;
		_float fGuardDashSpeed = 10.f;
		_float fRunSpeed = 4.f;
		_float fRunBeginSpeed = 4.f;
		_float fWalkSpeed = 1.8f;
		/* 공격할 때 살짝 앞으로 가는 속도 */
		_float fShortDashSpeed = 8.f;
		_float fDashAttackSpeed = 16.f;
		_float fWallJumpSpeed = 15.f;
		_float fBackStepSpeed = 30.f;
		_float fSprintAttackSpeed = 11.F;
		_float fSprintJumpSpeed = 10.F;
		_float fSprintSpeed = 7.f;
		_float fJumpPower = 4.5f;
		_float fStoreSpeed = 0.f; // 저장할 스피트 값 입력


		_float	fAttackDamage = 50.f;
	};
	struct UNIT_MODEL_DATA
	{
		wstring		strModelPaths[MODEL_PART_END];
		string		strRefBoneName[MODEL_PART_END];
		_float4x4	matTransform[MODEL_PART_END];
	};

	struct SKILL_TRIGGER
	{
		_bool	bSkillQTrigger = false;
		_bool	bSkillETrigger = false;
		_bool	bSkillRTrigger = false;
	};

	struct STATE_HIT_TYPE
	{
		/* Hit, Dead 상태 */
		STATE_TYPE		eHitState = STATE_END;
		STATE_TYPE		eDeathState = STATE_END;

		/* 가드 성공시 상태 */
		STATE_TYPE		eGuardState = STATE_END;

		/* 그로기 상태 */
		STATE_TYPE		eGroggyState = STATE_END;

		/* Bounce */
		STATE_TYPE		eBounce = STATE_END;

		/* 가드 깨진 상태*/
		STATE_TYPE		eGuardBreakState = STATE_END;

		/* 찌르기맞고 죽기 전 상태 */
		STATE_TYPE		eStingHitState = STATE_END;

		/* 황소 베기 등 특정 FlyAttack 공격을 맞을 시 */
		STATE_TYPE		eFlyState = STATE_END;
	};

public:
	enum UNITCOLLIDER { TEAM, BODY, HEAD, WEAPON_L, WEAPON_R, GUARD, GUARDBREAK_L, GUARDBREAK_R, GROGGY, FLYATTACK, UNITCOLLIDER_END };
	enum WEAPONCOLLIDER { WEAPONCOLLIDER_END = 3 };
	enum COOL_TYPE { SKILL1, SKILL2, SKILL3, COOL_END };

protected:
	CUnit();
	virtual ~CUnit();

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);


public: /* Physics */
	_bool		Is_Air();
	void		Set_DirAsLook();
	void		Set_LookToTarget();


public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);
	void	Set_Passes(VTXANIM_PASS_TYPE ePassType);
	void	Set_FollowCam(CCamera_Follow* pFollowCam) { m_pFollowCam = pFollowCam; }
	CCamera_Follow* Get_FollowCam() { return m_pFollowCam; }

	void	Synchronize_CamPos();

	/* 들고있는 FollowCam에 Shake 넣기. 1인자 : 강도, 2인자 : 시간 (시간이 짧을 수록 강도를 조금만 줘도 세게 흔들림)*/
	void	Shake_Camera(_float fPower, _float fTime);
	/* 카메라 위치 조정 */
	void	Lerp_Camera(const _uint& iCameraLerpType);


public:
	virtual void	On_Respawn();
	virtual void	On_Reborn();
	virtual	void	On_Die();

	void Set_MainPlayer() { m_bIsMainPlayer = true; }
	_bool		Is_MainPlayer() { return m_bIsMainPlayer; }

public:
	_float			Calculate_Damage(_bool bHeadShot, _bool bGuard);
	virtual _bool	On_PlusHp(_float fHp, CUnit* pOtherUnit, _bool bHeadShot = false, _uint iDmgType = 2);
	void	On_FallDamage(_float fFallPower);
	void	On_Sliding(_float4 vHitNormal);

public:
	_bool		Can_Use(COOL_TYPE eType) { if (eType < COOL_END && m_fCoolAcc[eType] <= 0.f) return true; return false; }
	void		On_Use(COOL_TYPE eType) { if (eType < COOL_END) m_fCoolAcc[eType] = m_fCoolTime[eType]; }

	_bool		Can_Attack() { if (m_fAttackDelay <= 0.f) return true; return false; }
	void		On_Attack(CState* pState);


	//_uint		Get_HeroChangeIndex() { return m_iChangeHeroAnimIndex; };


	/* PhysX 충돌 검출 */
	_bool		Is_Weapon_R_Collision();
	/* 캐릭터에 부딪힌거 체크(지금 안씀) */
	_bool		Is_Weapon_R_CCT_Collision();


public:
	class CPath* Get_CurPath();

	UNIT_STATUS& Get_Status() { return m_tUnitStatus; }
	CPhysics* Get_PhysicsCom() { return m_pPhysics; }

	CUnit* Get_TargetUnit();

	STATE_TYPE	Get_CurState() { return m_eCurState; }
	CState* Get_CurStateP() { return m_pCurState; }

	const STATE_TYPE& Get_DefaultState() { return m_eDefaultState; }
	const STATE_TYPE& Get_SprintEndState() { return m_eSprintEndState; }

	SKILL_TRIGGER& Get_SkillTrigger() { 
		return m_tSkillTrigger; 
	}

	const STATE_HIT_TYPE& Get_HitType() { return m_tHitType; }
	void	Set_BounceState(STATE_TYPE eType) { m_tHitType.eBounce = eType; }

	void	Set_OwnerPlayer(CPlayer* pPlayer) { m_pOwnerPlayer = pPlayer; }
	void	On_ChangeToHero(_uint iIndex);

	CPlayer* Get_OwnerPlayer() { return m_pOwnerPlayer; }


	_float4	Get_FollowCamLook();
	_float4	Get_FollowCamLook_Turn();
	_float4	Get_FollowCamRight();

public:
	void TurnOn_TrailEffect(_bool bOn);

public:
	void	Enter_State(STATE_TYPE eType, void* pData = nullptr);
	void	Reserve_State(STATE_TYPE eType);

public:
	void	Teleport_Unit(_float4 vPosition);
	_float4		Get_HitPos();
	/* hit 지점의 법선벡터를 Look으로 한 행렬을 반환 */
	_float4x4	Get_HitMatrix();

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
	CBoneCollider* m_pWeaponCollider_L = nullptr;

	//우리가 알던 그 충돌체
	CCollider_Sphere* m_pUnitCollider[UNITCOLLIDER_END] = {};
	//CCollider_Sphere* m_pUnitWeaponCollider[WEAPONCOLLIDER_END] = {};

public:
	void	Enable_HitBoxColliders(_bool bEnable);
	void	Enable_UnitCollider(UNITCOLLIDER ePartType, _bool bEnable);
	void	Enable_GuardCollider(_bool bEnable);
	void	Enable_GroggyCollider(_bool bEnable);
	void	Enable_GuardBreakCollider(UNITCOLLIDER ePartType, _bool bEnable);
	void	Enable_FlyAttackCollider(_bool bEnable);


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

	virtual void	SetUp_Colliders(_bool bBlueTeam);
	void	SetUp_UnitCollider(UNITCOLLIDER ePartType, UNIT_COLLIDERDESC* arrColliderDesc,
		_uint iNumCollider = 1, _float4x4 matTransformation = DEFAULT_TRANS_MATRIX, _bool bEnable = true, CHierarchyNode* pRefBone = nullptr);

public:
	enum class UNIT_TYPE { ePlayer, eSandbag, eAI_TG, eAI_Default, eAI_idiot, eAI_Panden, eAI_Gela, eAI_Hwara, eUNIT_TYPE_END };
	virtual void SetUp_HitStates(UNIT_TYPE eUnitType);

public:
	CPlayer* Get_RevivalPlayer() { return m_pAdjRevivalPlayer; }
	CCannon* Get_AdjCannon() { return m_pAdjCannon; }

public:
	virtual void SetUp_ReserveState(UNIT_TYPE eUnitType) {};

protected:
	CPlayer* m_pOwnerPlayer = nullptr;
	CPlayer* m_pAdjRevivalPlayer = nullptr;

	CCannon* m_pAdjCannon = nullptr;

protected:
	// 얘가 max
	_float	m_fCoolTime[COOL_END] = {};
	// 얘가 cur
	_float	m_fCoolAcc[COOL_END] = {};

	_float	m_fAttackDelay = 0.f;

protected:
	UNIT_MODEL_DATA	m_tModelData;

protected:
	SKILL_TRIGGER m_tSkillTrigger;

protected:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CPhysics* m_pPhysics = nullptr;

	UNIT_STATUS		m_tUnitStatus;
	
	STATE_TYPE		m_eCurState = STATE_END;
	STATE_TYPE		m_eReserveState = STATE_END;

	STATE_TYPE		m_eDefaultState = STATE_END;
	STATE_TYPE		m_eSprintEndState = STATE_END;

	CState* m_pCurState = nullptr;

protected:
	_bool m_bControlable = true;
	_bool	m_bRespawn = false;

protected:
	CTrailEffect* m_pTrailEffect = nullptr;
	CTrailEffect* m_pTrailEffect2 = nullptr;

	CTrailEffect* m_pDistortionTrail = nullptr;

protected:
	CCamera_Follow* m_pFollowCam = nullptr;

protected:
	STATE_HIT_TYPE	m_tHitType;

	_uint			m_iDefaultType = 0;
	_float			m_fHitDelayAcc = 0.f;
	_float			m_fHitDelayTime = 0.15f;


protected:
	void SetUp_TrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
			_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	void SetUp_DistortionTrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag,
	_float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount, string strBoneName);
	virtual	HRESULT	SetUp_Model(const UNIT_MODEL_DATA& tData);
	virtual	HRESULT	SetUp_Navigation(CCell* pStartCell);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

protected:
	virtual void	Effect_Parring(_float4 vHitPos);
	virtual void	Effect_Hit(CUnit* pOtherUnit, _float4 vHitPos);
	virtual void	Effect_Fall(_float fFallPower);
public:
	void Create_Light(_float4 vPos, _float fRange, _float fRandomRange, _float fFadeInTime, _float fDuration,
		_float fFadeOutTime, _float4 Diffuse, LIGHTDESC::EASING_TYPE eInEasingType = LIGHTDESC::EASING_TYPE::EAS_Linear,
		LIGHTDESC::EASING_TYPE eOutEasingType = LIGHTDESC::EASING_TYPE::EAS_Linear);
	void Create_Light(CGameObject* pOwner, _float4 vOffset, _float fRange, _float fRandomRange,
		_float fFadeInTime, _float fDuration, _float fFadeOutTime, _float4 Diffuse, _bool bLoop);
	virtual void	Effect_HeroToDefaultUnit(CUnit* pOwner);

protected:
	void	Set_ShaderNoSpec(const _tchar* pModelPath);

private:
	void	On_InitSetting();
	//데스캠도 끝나고 진짜 죽음

protected:
	_bool		m_bIsMainPlayer = false;
	_bool		m_bDie = false;
	_float		m_fDeadTimeAcc = 0.f;
	_float		m_fDeadTime = 0.05f;

protected:
	list<CGameObject*>	m_DeathStones;
	void	Add_DeathStones(const list<CGameObject*>& StoneParticleList);

public:
	void	Start_Reborn();

	//AI가 Behavior를 변경할 때 호출
	virtual void On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc);
	virtual void On_FinishGame(_bool bWin);

protected:
	CAnimWeapon* m_pAnimWeapon = nullptr;

public:
	void	Set_AnimWeaponIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed);
	void	Set_AnimWeaponFrame(_uint iChangeFrame);



	/* 상태 체크 함수 */
private:
	//내가 공격 당했을 때 들어오는 함수
	void		On_Hit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo);
	//내가 가드했을 떄 들어오는 함수
	void		On_GuardHit(CUnit* pOtherUnit, _uint iOtherColType, _float4 vHitPos, void* pHitInfo);

	/* 딱 체력 0된 시점에 호출되는 함수 */
	void		On_DieBegin(CUnit* pOtherUnit, _float4 vHitPos);
	void		On_Bounce(void* pHitInfo);



};
END
