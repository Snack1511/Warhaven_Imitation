#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

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
	enum class eCUSTOM_TYPE
	{
		eDEFAULT,
		eLEADER,
	};

private:
	CPlayer();
	virtual ~CPlayer();

public:
	static CPlayer* Create(wstring wstrCamKey, CLASS_DEFAULT eClass, eCUSTOM_TYPE eCustomType = eCUSTOM_TYPE::eDEFAULT);

	/* Initialize_Prototype */
public:
	void Create_DefaultClass();
	void Create_HeroClass();

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

	wstring Get_PlayerName() { return m_wstrName; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() { return S_OK; }
	virtual HRESULT Initialize_Prototype(wstring wstrCamKey, CLASS_DEFAULT eClass);
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

private:
	eCUSTOM_TYPE	m_eCustomType = eCUSTOM_TYPE::eDEFAULT;
	wstring	m_wstrHelmetMeshPath[CLASS_END];
	wstring	m_wstrBodyMeshPath[CLASS_END];
	wstring	m_wstrWeaponMeshPath[CLASS_END];

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

private:	// 이름
	wstring m_wstrName = TEXT("쥬신");

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



private:
	void Ready_Customizing(eCUSTOM_TYPE eType);
};
END
