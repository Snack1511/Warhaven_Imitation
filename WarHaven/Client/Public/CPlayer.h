#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

#include "CPlayerInfo.h"

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
class CGameObject;
END

BEGIN(Client)

class CUnit;
class CTrailEffect;
class CScript_FollowCam;
class CCamera_Follow;
class CUI_UnitHUD;
class CAIController;

class CPlayer final : public CGameObject
{
	DECLARE_PROTOTYPE(CPlayer);

public:
	struct KDA_STAT
	{
		//총 점수
		_uint	iTotalScore = 0;

		//총 죽인 적
		_uint	iTotalKillCount = 0;
		_uint	iDeathCount = 0;
		//죽지않고 죽인 적
		_uint	iCurKillCount = 0;
		//헤드샷으로 죽인 적
		_uint	iHeadShotKillCount = 0;
		//죽인 영웅
		_uint	iHeroKillCount = 0;
		//연속 처치 카운트
		_uint	iKillStreak = 0;
		//처치 기여
		_uint	iTotalAssist = 0;
	};



	enum OUTLINETYPE
	{
		eENEMY,
		eSQUADMEMBER,
		eEnd
	};

public:

private:
	CPlayer();
	virtual ~CPlayer();

public:
	static CPlayer* Create(CPlayerInfo* pPlayerInfo);

	/* Initialize_Prototype */
public:
	void Create_Class(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData);
	/*Collision*/
public:
	virtual void	Player_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Player_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Player_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);

	/* Start */
public:
	HRESULT	Set_FollowCam(wstring wstrCamKey);
	HRESULT	Change_UnitClass(CLASS_TYPE eDefaultClass);


	void	Reserve_State(_uint eState);

	void	Set_Unit_ReserveState(_uint eClassType, _uint eState);

	void	SetUp_UnitColliders(_bool bBlueTeam);
	void	SetUp_UnitHitStates();

	void	Set_Postion(_float4 vPos);

	void	Set_LookToTarget();

public:
	void	Respawn_Unit(_float4 vPos, CLASS_TYPE eClass);

public:
	_uint	Get_ChangeHeroIndex(_uint eClass) { return m_iChangeHeroAnimIndex[eClass]; }
	_uint	Get_DefaultReserveStateIndex(_uint eClass) { return m_iReserveStateDefault[eClass]; }

	CUnit* Get_CurrentUnit() { return m_pCurrentUnit; }
	CLASS_TYPE Get_PrevClass() { return m_ePrevClass; }
	CLASS_TYPE Get_CurClass() { return m_eCurrentClass; }



	void Set_MainPlayer();
	void Set_LeaderPlayer() { m_bIsLeaderPlayer = true; }

	wstring Get_PlayerName() { return m_pMyPlayerInfo->m_tPlayerInfo.wstrName; }
	_uint	Get_Level() { return m_pMyPlayerInfo->m_tPlayerInfo.iLevel; }
	CPlayerInfo* Get_PlayerInfo() { return m_pMyPlayerInfo; }

	void	Enable_OnStart() { m_bEnableOnStart = true; }

	void	Set_UnitType(_uint eUnitType) { m_iUnitType = eUnitType; }

	HRESULT SetUp_Collider();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	/* 죽어서 돌 되자마자 들어오는 함수*/
	void	On_Die();
	/* 죽고나서 5초 딜레이까지 지나고 들어오는 함수 */
	void	On_RealDie();
	void	On_Reborn();

public:
	void	Set_TargetPlayer(CPlayer* pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }
	CPlayer* Get_TargetPlayer() { return m_pTargetPlayer; }

public:
	_bool& AbleHero() { return m_bAbleHero; }
	_bool& IsHero() { return m_bIsHero; }
	_bool	IsMainPlayer() { return m_bIsMainPlayer; }
	_bool	IsLeaderPlayer() { return m_bIsLeaderPlayer; }

	_float& Get_Gauge() { return m_fGauge; }



public:
	void	Set_Squad(CSquad* pSquad) { m_pMySquad = pSquad; }
	void	Set_Team(CTeamConnector* pTeamConnector) { m_pMyTeam = pTeamConnector; }
	void	Set_TeamType(eTEAM_TYPE eTeamType);
	void	Set_OutlineType(OUTLINETYPE eOutlineType);

	CTeamConnector* Get_Team() { return m_pMyTeam; }
	CSquad* Get_Squad() { return m_pMySquad; }
	OUTLINETYPE Get_OutlineType() {		return m_eOutlineType;	}
public:
	void On_ChangeBehavior(BEHAVIOR_DESC* pBehavior);
private:
	_bool	m_bEnableOnStart = false;

private: /* 킬뎃과 플레이어 정보 */
	KDA_STAT	m_tKdaStat;
	CPlayerInfo* m_pMyPlayerInfo = nullptr;
	CSquad* m_pMySquad = nullptr;
	CTeamConnector* m_pMyTeam = nullptr;
	_bool m_bIsMainPlayer = false;
	_bool m_bIsLeaderPlayer = false;

private: /*AI 추가용*/
	CAIController* m_pAIController = nullptr;


private:
	//어떤 타입인지(적, 샌드백)
	_uint	m_iUnitType = 0;


private:
	//어느 진영인지
	eTEAM_TYPE	m_eTeamType = eTEAM_TYPE::eBLUE;

	OUTLINETYPE m_eOutlineType = OUTLINETYPE::eEnd;

private:
	CPlayer* m_pTargetPlayer = nullptr;

private:
	/* 돌 되고나서 5초 딜레이 후 진짜 죽는 상태로 감 */
	_bool	m_bDieDelay = false;
	_float		m_fDieDelayAcc = 0.f;
	_float		m_fDieCoolTime = 5.f;

	/* 죽어있는 상태 */
	_bool	m_bDie = false;

private:
	CUnit* m_pCurrentUnit = nullptr;
	CCamera_Follow* m_pFollowCam = nullptr;
	CUnit* m_pAllUnitClass[CLASS_END] = { nullptr };

	// 변신 Index
	_uint	m_iChangeHeroAnimIndex[CT_DEFAULT_END] = {};

	/* 예약 Index */
	_uint	m_iReserveStateDefault[CLASS_END] = {};


	CLASS_TYPE		m_ePrevClass = CLASS_END;
	CLASS_TYPE		m_eCurrentClass = CLASS_TYPE::CLASS_END;
private:
	CCollider_Sphere* m_pSightRangeCollider = nullptr;
private:
	CUI_UnitHUD* m_pUnitHUD = nullptr;

private:
	_float m_fEnable_UnitHUDis = 35.f;

private:	// 화신 게이지
	_bool		m_bAbleHero = false;
	_bool		m_bIsHero = false;
	_float		m_fMaxGauge = 100.f;
	_float		m_fGauge = 0.f;

private:
	list<CGameObject*>	m_DeadLights;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	void Create_UnitHUD();
	void Enable_UnitHUD();

	void Frustum_UnitHUD();
	void TransformProjection();

private:
	void Update_HP();
	void Update_HeroGauge();
	void On_AbleHero();
	public: void On_FinishHero();
private:
	void On_FinishHero_KeyInput();

private:
	void	Update_DieDelay();
};
END
