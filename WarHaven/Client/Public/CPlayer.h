#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

#include "CPlayerInfo.h"

#define HERO_END 4

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

class CUnit;
class CTrailEffect;
class CScript_FollowCam;
class CCamera_Follow;

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

	

	enum CLASS_HREO
	{
		CLASS_HREO_FIONA = 6,
		/*CLASS_HREO_QANDA,
		CLASS_HREO_HOEDT,
		CLASS_HREO_LANCER,*/
		CLASS_HERO_END = 10
	};

	enum CLASS_DEFAULT
	{
		CLASS_DEFAULT_WARRIOR = 0,
		/*CLASS_DEFAULT_SPEAR,
		CLASS_DEFAULT_ARCHER,
		CLASS_DEFAULT_PALADIN,
		CLASS_DEFAULT_PRIEST,
		,*/
		CLASS_DEFAULT_ENGINEER = 5,
		CLASS_DEFAULT_END
	};

	enum eTEAM_TYPE
	{
		//이거랑 &이면 메인플레이어
		eMAINPLAYER = (1 << 0),

		//둘중 하나랑 &면 어느 진영인지
		ePLAYERTEAM = (1 << 1),
		eENEMYTEAM = (1 << 2),

		//&해서 스쿼드 멤버인지 스쿼드장인지 여부
		eSQUADMEMBER = (1 << 3),
		eSQUADLEADER = (1 << 4)
	};

public:

private:
	CPlayer();
	virtual ~CPlayer();

public:
	static CPlayer* Create(CPlayerInfo* pPlayerInfo);

	/* Initialize_Prototype */
public:
	void Create_DefaultClass(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData);
	void Create_HeroClass(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData);

	/* Start */
public:
	HRESULT	Set_FollowCam(wstring wstrCamKey);
	HRESULT	Change_DefaultUnit(CLASS_DEFAULT eClass);
	HRESULT	Change_HeroUnit(CLASS_HREO eClass);


	void	Reserve_State(_uint eState);

	void	SetUp_UnitColliders(_bool bPlayer);
	void	SetUp_UnitHitStates(_bool bPlayer);

	void	Set_Postion(_float4 vPos);

	void	Set_LookToTarget();

public:
	void	Respawn_Unit(_float4 vPos, CLASS_DEFAULT eClass);

public:
	_uint	Get_ChangeHeroIndex(_uint eClass) { return m_iChangeHeroAnimIndex[eClass]; }
	_uint	Get_DefaultReserveStateIndex(_uint eClass) { return m_iReserveStateDefault[eClass]; }

	CUnit* Get_CurrentUnit() { return m_pCurrentUnit; }
	CLASS_DEFAULT Get_CurrentDefaultClass() { return m_eCurrentDefaultClass; }
	void Set_MainPlayer();

	wstring Get_PlayerName() { return m_pMyPlayerInfo->m_tPlayerInfo.wstrName; }
	_uint	Get_Level() { return m_pMyPlayerInfo->m_tPlayerInfo.iLevel; }
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	void	Set_TargetPlayer(CPlayer* pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }
	CPlayer*	Get_TargetPlayer() { return m_pTargetPlayer; }

public:
	_bool& AbleHero() { return m_bAbleHero; }
	_bool& IsHero() { return m_bIsHero; }

public:
	void	Set_TeamType(int eTeamType);

private: /* 킬뎃과 플레이어 정보 */
	KDA_STAT	m_tKdaStat;
	CPlayerInfo* m_pMyPlayerInfo = nullptr;

private:
	//어느 진영인지, 스쿼드멤버인지 스쿼드장인지 여부
	int	m_eTeamTypeFlag = eTEAM_TYPE::eMAINPLAYER;

private:
	CPlayer* m_pTargetPlayer = nullptr;

private:
	CUnit* m_pCurrentUnit = nullptr;
	CCamera_Follow* m_pFollowCam = nullptr;
	CUnit* m_pHeroClass[HERO_END] = { nullptr };
	CUnit* m_pDefaultClass[CLASS_DEFAULT_END] = { nullptr };

	// 변신 Index
	_uint	m_iChangeHeroAnimIndex[CLASS_DEFAULT_END] = { 0 };

	/* 예약 Index */
	_uint	m_iReserveStateDefault[CLASS_DEFAULT_END] = { 0 };
	_uint	m_iReserveStateHero[HERO_END] = { 0 };


	CLASS_DEFAULT	m_eCurrentDefaultClass = CLASS_DEFAULT_END;

	_bool m_bIsMainPlayer = false;

private:	// 화신 게이지
	_bool		m_bAbleHero = false;
	_bool		m_bIsHero = false;
	_float		m_fMaxGauge = 100.f;
	_float		m_fGauge = 0.f;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	void Update_HP();
	void Update_HeroGauge();
	void On_AbleHero();
	void On_FinishHero();
	void On_FinishHero_KeyInput();


};
END
