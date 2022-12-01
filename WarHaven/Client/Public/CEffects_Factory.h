#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CEffect;

class CEffects_Factory
{
	DECLARE_SINGLETON(CEffects_Factory)

private:
	CEffects_Factory();
	~CEffects_Factory();

public:
	HRESULT	Initialize();

public:
	void	On_EnterLevel();
	void	On_ExitLevel();

public:
	list<CGameObject*>			Create_MultiEffects(wstring	wstrKey, CGameObject* pFollowTarget, _float4 vPos);
	list<CGameObject*>			Create_MultiEffects(wstring	wstrKey, _float4 vPos);
	list<CGameObject*>			Create_MultiEffects(wstring	wstrKey, _float4x4 matWorld);
	list<CGameObject*>			Create_MultiEffects(wstring	wstrKey, _float4 vPos, _float4x4 matWorld);

	list<CGameObject*>			Create_Multi_MeshParticle(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld);
	list<CGameObject*>			Create_Multi_MeshParticle_Death(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld);


	CGameObject* Create_MeshParticle(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld);
	CGameObject* Create_MeshParticle_Death(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld);
	CGameObject* Create_MeshParticle(_hashcode _hcCode, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld);

	CGameObject* Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos);
	CGameObject* Create_Effects(_hashcode _hcCode, _float4 vPos);
	CGameObject* Create_Effects(_hashcode _hcCode, _float4x4 worldMat);
	CGameObject* Create_Effects(_hashcode _hcCode, _float4 vPos, _float4x4 matWorld);

	CGameObject* Create_Effects_Turn(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos);
	CGameObject* Create_LandingEffects(_float4 vPos, CCell* pCurCell);
	CGameObject* Create_BigLandingEffects(_float4 vPos, CCell* pCurCell);
	CGameObject* Create_EightGatesLandingEffects(_float4 vPos, CCell* pCurCell);
	CGameObject* Create_Effects(_hashcode _hcCode, _float4 vPos, _float4 vTargetPosition);
	CGameObject* Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget);
	CGameObject* Create_Effects(_hashcode _hcCode, CGameObject* pOwner, CHierarchyNode* pRightHandBone);


public:
	HRESULT	Add_Effect(_hashcode _hcCode, CEffect* pEffect);
	_bool Find_Effect(_hashcode _hcCode);

public:
	void	Collect_Effect(_hashcode _hcCode, CEffect* pEffect);

private:
	map<_hashcode, list< CEffect*>>	m_Effects;

	map<_hashcode, list<_hashcode>>	m_MultiEffects;

	list<_hashcode>		m_EffectsResourcesCode;

private:
	list<string>	Read_AllEffectsFile(const char* pFolderPath);

private:
	HRESULT	Add_MultiEffects(_hashcode MultiEffectsName, _hashcode EffectsCode);
	HRESULT	Combine_EffectsGroup(list<string>& listTemp, _hashcode MultiEffectsName, string strKey);

	HRESULT	SetUp_MultiEffects();

private:
	HRESULT	SetUp_StoneParticles();
};

END