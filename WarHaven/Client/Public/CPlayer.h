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

class CUnit;
class CTrailEffect;
class CScript_FollowCam;
class CCamera_Follow;

class CPlayer final
	: public CGameObject
{

	DECLARE_PROTOTYPE(CPlayer);

public:
	enum CLASS_HREO
	{
		/*CLASS_HREO_FIONA,
		CLASS_HREO_QANDA,
		CLASS_HREO_HOEDT,
		CLASS_HREO_LANCER,*/
		CLASS_HERO_END
	};

	enum CLASS_DEFAULT
	{
		CLASS_DEFAULT_WARRIOR,
		CLASS_DEFAULT_ENGINEER,
		/*CLASS_DEFAULT_SPEAR,
		CLASS_DEFAULT_ARCHER,
		CLASS_DEFAULT_PALADIN,
		CLASS_DEFAULT_PRIEST,
		,*/
		CLASS_DEFAULT_END
	};


private:
	CPlayer();
	virtual ~CPlayer();

public:
	static CPlayer* Create(wstring wstrCamKey, CLASS_DEFAULT eClass);

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

public:
	CUnit* Get_CurrentUnit() { return m_pCurrentUnit; }
	void Set_MainPlayer();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() { return S_OK; }
	virtual HRESULT Initialize_Prototype(wstring wstrCamKey, CLASS_DEFAULT eClass);
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


private:
	CUnit* m_pCurrentUnit = nullptr;
	CCamera_Follow* m_pFollowCam = nullptr;
	//CUnit* m_pHeroClass[CLASS_HERO_END] = { nullptr };
	CUnit* m_pDefaultClass[CLASS_DEFAULT_END] = { nullptr };

	_uint	m_iReserveState[CLASS_DEFAULT_END] = { 0 };
	

	_bool m_bIsMainPlayer = false;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	_float4 m_vCurrentPos;

};
END
