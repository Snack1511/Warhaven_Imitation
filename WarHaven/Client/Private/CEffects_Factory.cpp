#include "stdafx.h"
#include "CEffects_Factory.h"

#include "GameInstance.h"


#include "Transform.h"
#include "CCell.h"
#include "Model.h"
#include "Texture.h"

#include "Functor.h"

#include "CDefault_Effect.h"
#include "CSword_Effect.h"

#include "CCamera_Default.h"

#include "CMesh_Particle.h"

#include "CDefaultArrow.h"
#include "CPurpleArrow.h"
#include "CSnipeArrow.h"
#include "CQandaMeteor.h"
#include "CQandaCrow.h"

#include "CCannonBall.h"

IMPLEMENT_SINGLETON(CEffects_Factory);

CEffects_Factory::CEffects_Factory()
{
}

CEffects_Factory::~CEffects_Factory()
{
}

HRESULT CEffects_Factory::Initialize()
{
	//이펙트 만들어놓기
	if(FAILED(Add_Effect(HASHCODE(CSword_Effect), CSword_Effect::Create())))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(CDefaultArrow::Create(), HASHCODE(CDefaultArrow))))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(CSnipeArrow::Create(), HASHCODE(CSnipeArrow))))
		return E_FAIL;
	
	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(CPurpleArrow::Create(), HASHCODE(CPurpleArrow))))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(CQandaCrow::Create(), HASHCODE(CQandaCrow))))
		return E_FAIL;

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(CQandaMeteor::Create(), HASHCODE(CQandaMeteor))))
		return E_FAIL;

	if (FAILED(SetUp_StoneParticles()))
		return E_FAIL;

	if (FAILED(SetUp_MultiEffects()))
		return E_FAIL;

	return S_OK;
}

void CEffects_Factory::On_EnterLevel()
{
	for (auto& hcCode : m_EffectsResourcesCode)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = GAMEINSTANCE->Clone_GameObject(hcCode);
		if (!pGameObject)
			continue;
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(pGameObject);
	}

}

void CEffects_Factory::On_ExitLevel()
{
	for (auto& elem : m_Effects)
	{
		elem.second.clear();
	}
	m_Effects.clear();
}

list<CGameObject*> CEffects_Factory::Create_MultiEffects(wstring wstrKey, CGameObject* pFollowTarget, _float4 vPos)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_Effects(hcCode, pFollowTarget, vPos));
	}

	return EffectsList;
}

list<CGameObject*> CEffects_Factory::Create_MultiEffects(wstring wstrKey, _float4 vPos)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_Effects(hcCode, vPos));
	}

	return EffectsList;
}

list<CGameObject*> CEffects_Factory::Create_MultiEffects(wstring wstrKey, _float4x4 matWorld)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_Effects(hcCode, matWorld));
	}

	return EffectsList;
}

list<CGameObject*> CEffects_Factory::Create_Multi_MeshParticle(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_MeshParticle(hcCode, vPos, vDir, fPower, matWorld));
	}

	return EffectsList;
}

list<CGameObject*> CEffects_Factory::Create_Multi_MeshParticle_Death(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_MeshParticle(hcCode, vPos, vDir, fPower, matWorld));
		static_cast<CMesh_Particle*>(EffectsList.back())->Set_DeathParticle();
	}

	return EffectsList;
}

list<CGameObject*> CEffects_Factory::Create_MultiEffects(wstring wstrKey, _float4 vPos, _float4x4 matWorld)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_Effects(hcCode, vPos, matWorld));
	}

	return EffectsList;
}


CGameObject* CEffects_Factory::Create_MeshParticle(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	CGameObject* pGameObject = nullptr;

	_hashcode _hcCode = Convert_ToHash(wstrKey);

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CMesh_Particle*>(pGameObject)->Start_Particle(vPos, vDir, fPower, matWorld);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		static_cast<CMesh_Particle*>(pEffect)->Start_Particle(vPos, vDir, fPower, matWorld);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}


	return pGameObject;
}

CGameObject* CEffects_Factory::Create_MeshParticle_Death(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	CGameObject* pGameObject = nullptr;

	_hashcode _hcCode = Convert_ToHash(wstrKey);

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CMesh_Particle*>(pGameObject)->Start_Particle(vPos, vDir, fPower, matWorld);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		static_cast<CMesh_Particle*>(pEffect)->Start_Particle(vPos, vDir, fPower, matWorld);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	static_cast<CMesh_Particle*>(pGameObject)->Set_DeathParticle();


	return pGameObject;
}

CGameObject* CEffects_Factory::Create_MeshParticle(_hashcode _hcCode, _float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	CGameObject* pGameObject = nullptr;


	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CMesh_Particle*>(pGameObject)->Start_Particle(vPos, vDir, fPower, matWorld);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		static_cast<CMesh_Particle*>(pEffect)->Start_Particle(vPos, vDir, fPower, matWorld);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}


	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CEffect*>(pGameObject)->Self_Reset(pFollowTarget, vPos);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Self_Reset(pFollowTarget, vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(vPos);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4x4 worldMat)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(worldMat);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(worldMat);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4 vPos, _float4x4 worldMat)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(vPos, worldMat);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(vPos, worldMat);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects_Turn(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CEffect*>(pGameObject)->Self_Reset_Turn(pFollowTarget, vPos);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Self_Reset_Turn(pFollowTarget, vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_LandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"Grass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"SmallRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_BigLandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"BigGrass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"BigSmallRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_EightGatesLandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"EightGatesGrass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"EightGatesRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4 vPos, _float4 vTargetPosition)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(vPos, vTargetPosition);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(vPos, vTargetPosition);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;

}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(pFollowTarget);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(pFollowTarget);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;

}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pOwner, CHierarchyNode* pRightHandBone)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
		ENABLE_GAMEOBJECT(pEffect);
	}

	static_cast<CEffect*>(pGameObject)->Set_FollowTarget(pOwner);
	static_cast<CEffect*>(pGameObject)->Set_RefBone(pRightHandBone);

	return static_cast<CEffect*>(pGameObject);
}

HRESULT CEffects_Factory::Add_Effect(_hashcode _hcCode, CEffect* pEffect)
{
	if (!pEffect)
		return E_FAIL;

	/*if (FAILED(pEffect->Initialize()))
		return E_FAIL;*/

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(pEffect, _hcCode)))
		return E_FAIL;

	//m_Effects.emplace(_hcCode, list<CEffect*>());


	return S_OK;
}

_bool CEffects_Factory::Find_Effect(_hashcode _hcCode)
{
	auto iter = m_Effects.find(_hcCode);

	if (iter == m_Effects.end())
		return false;

	return true;
}

void CEffects_Factory::Collect_Effect(_hashcode _hcCode, CEffect* pEffect)
{
	m_Effects[_hcCode].push_back(pEffect);
}

list<string> CEffects_Factory::Read_AllEffectsFile(const char* pFolderPath)
{
	list<string>	EffectsTemp;

	for (filesystem::directory_iterator FileIter(pFolderPath);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		string strFileKey = strFileName.substr(0, iFind2);

		CEffect* pNewEffect = CEffect::Create_Effect_FromBinFile(strFileKey);

		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create_Effect");
			return EffectsTemp;

		}

		_hashcode hcCode = Convert_ToHash(CFunctor::To_Wstring(strFileKey));

		if (FAILED(Add_Effect(hcCode, pNewEffect)))
		{
			Call_MsgBox(L"Failed to Add_Effect");
			return EffectsTemp;
		}

		EffectsTemp.push_back(strFileKey);
		m_EffectsResourcesCode.push_back(hcCode);
	}







	return EffectsTemp;
}

HRESULT CEffects_Factory::Add_MultiEffects(_hashcode MultiEffectsName, _hashcode EffectsCode)
{
	/*GAMEINSTANCE->Find_GameObject_Prototype()

	auto iter = m_Effects.find(EffectsCode);

	if (iter == m_Effects.end())
	{
		Call_MsgBox(L"No Effects : Add_MultiEffects");
		return E_FAIL;
	}*/

	m_MultiEffects[MultiEffectsName].push_back(EffectsCode);

	return S_OK;

}

HRESULT CEffects_Factory::Combine_EffectsGroup(list<string>& listTemp, _hashcode MultiEffectsName, string strKey)
{
	//폴더 뒤져서 이름 다 가져오기

	//strKey가 있으면 넣어

	for (auto& elem : listTemp)
	{
		_int iFind = elem.find(strKey);
		if (iFind >= 0)
		{
			if (FAILED(Add_MultiEffects(MultiEffectsName, Convert_ToHash(CFunctor::To_Wstring(elem)))))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEffects_Factory::SetUp_MultiEffects()
{
	list<string>	listTemp = Read_AllEffectsFile("../bin/effects");

	if(FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SmallSparkParticle"), "SmallSparkParticle")))
		return E_FAIL;


	/* BigSpark */
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BigSparkParticle"), "BigSparkParticle")))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"BigSparkParticle"), Convert_ToHash(L"SparkMesh_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"BigSparkParticle"), Convert_ToHash(L"SparkMesh_1"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SkillLightParticle"), "SkillLightParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SparkMesh"), "SparkMesh")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Parring_Particle"), "Parring_Particle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSmokeParticle"), "HitSmokeParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SoilParticle_R_Foot"), "SoilParticle_R_Foot")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SoilParticle_L_Foot"), "SoilParticle_L_Foot")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"GroundHitParticle"), "GroundHitParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SmashSoilParticle"), "SmashSoilParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Fall_Particle"), "Fall_Particle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ShieldCharge"), "ShieldCharge")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ShieldWall"), "ShieldWall")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Wind_Attack"), "Wind_Attack")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Cure_Smoke"), "Cure_Smoke")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Cure_Particle"), "Cure_Particle")))
		return E_FAIL;


	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Cannon_Shoot"), "Cannon_Shoot")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Cannon_Smoke"), "Cannon_Smoke")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Cannon_Ground"), "Cannon_Ground")))
		return E_FAIL;

	/*Hit effect*/
		/*slash*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSpark"), "HitSpark")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Hit_Particle"), "Hit_Particle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_D"), "HitSlash_D"))) //down
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_Right"), "HitSlash_Right"))) //right
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_RD"), "HitSlash_RD"))) //right down
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_RU"), "HitSlash_RU"))) //right Up
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_Left"), "HitSlash_Left"))) //Left
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_LD"), "HitSlash_LD"))) //Left down
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_LU"), "HitSlash_LU"))) //Left up
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"StingBlood"), "StingBlood")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Oxen_Smoke"), "Oxen_Smoke")))
		return E_FAIL;
		/*smash*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Smash_Right"), "Smash_Right")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Smash_Left"), "Smash_Left")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Smash_D"), "Smash_D")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SmashSpark"), "SmashSpark")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Heading"), "Heading")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HammerFlare"), "HammerFlare")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"KillSmoke_Right"), "KillSmoke_Right")))
		return E_FAIL;

		/*cat*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"PoisonTest"), "PoisonTest")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Addiction"), "Addiction")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Poison_End"), "Poison_End")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Arrow_Hit"), "Arrow_Hit")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Arrow_Blood"), "Arrow_Blood")))
		return E_FAIL;

	/*Transform*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HenshinFlare"), "HenshinFlare")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HenshinParticle"), "HenshinParticle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"UnHenshin"), "UnHenshin")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Transform_Particle"), "Transform_Particle")))
		return E_FAIL;


		/*valkyrie*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Shield_Attack"), "Shield_Attack")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Shield_Flare"), "Shield_Flare")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FionaParring"), "FionaParring")))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FionaParring"), Convert_ToHash(L"FionaParring_Mesh_0"))))
		return E_FAIL;
		/*Quanda*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Charge_Test"), "Charge_Test")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Charge_Particle"), "Charge_Particle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Charge_End"), "Charge_End")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Crow_Feathers"), "Crow_Feathers")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Crow_Steam"), "Crow_Steam")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Crow_Boom"), "Crow_Boom")))
		return E_FAIL;
	/*Dodge*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Dodge"), "Dodge")))
		return E_FAIL;

	/*priest*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Catch_Particle"), "Catch_Particle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Catch_Mesh"), "Catch_Mesh")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Catching"), "Catching")))
		return E_FAIL;
	
	
	/*environment*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"TrainigRoomSmoke"), "TrainigRoomSmoke")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Leaf_Particle"), "Leaf_Particle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireTorch"), "FireTorch")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BonFire"), "BonFire")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Paden_Envir"), "Paden_Envir")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Paden_Smoke"), "Paden_Smoke")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Paden_Roof"), "Paden_Roof")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Paden_In"), "Paden_In")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ash"), "ash")))
		return E_FAIL;
		

	/* Dead Body */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Warrior"), Convert_ToHash(L"WarriorDead_Slice0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Warrior"), Convert_ToHash(L"WarriorDead_Slice1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Warrior"), Convert_ToHash(L"WarriorDead_Slice2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Warrior"), Convert_ToHash(L"WarriorDead_Slice3"))))
		return E_FAIL;
	
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Engineer"), Convert_ToHash(L"EngineerDead_Slice0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Engineer"), Convert_ToHash(L"EngineerDead_Slice1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Engineer"), Convert_ToHash(L"EngineerDead_Slice2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Engineer"), Convert_ToHash(L"EngineerDead_Slice3"))))
		return E_FAIL;


	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Paladin"), Convert_ToHash(L"PaladinDead_Slice0"))))
		return E_FAIL;																  
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Paladin"), Convert_ToHash(L"PaladinDead_Slice1"))))
		return E_FAIL;																  
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Paladin"), Convert_ToHash(L"PaladinDead_Slice2"))))
		return E_FAIL;																  
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Paladin"), Convert_ToHash(L"PaladinDead_Slice3"))))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Archer"), Convert_ToHash(L"ArcherDead_Slice0"))))
		return E_FAIL;																 
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Archer"), Convert_ToHash(L"ArcherDead_Slice1"))))
		return E_FAIL;																 
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Archer"), Convert_ToHash(L"ArcherDead_Slice2"))))
		return E_FAIL;																 
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeadBody_Archer"), Convert_ToHash(L"ArcherDead_Slice3"))))
		return E_FAIL;

	/* Stone */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeathStoneParticle"), Convert_ToHash(L"DeathStoneParticle_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeathStoneParticle"), Convert_ToHash(L"DeathStoneParticle_1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeathStoneParticle"), Convert_ToHash(L"DeathStoneParticle_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"DeathStoneParticle"), Convert_ToHash(L"DeathStoneParticle_3"))))
		return E_FAIL;

	/* Wood */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodenDestroyParticle"), Convert_ToHash(L"WoodenLog_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodenDestroyParticle"), Convert_ToHash(L"WoodenLog_1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodenDestroyParticle"), Convert_ToHash(L"WoodenLog_2"))))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodenHitParticle"), Convert_ToHash(L"WoodenParticle_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodenHitParticle"), Convert_ToHash(L"WoodenParticle_1"))))
		return E_FAIL;

	/*fish*/
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_3"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_4"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FishBarrelParticle"), Convert_ToHash(L"FishParticle_5"))))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"BarrelParticle"), Convert_ToHash(L"WoodenLog_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"BarrelParticle"), Convert_ToHash(L"FishParticle_5"))))
		return E_FAIL;

	/*Stone spark particle*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"StoneSpark"), "StoneSpark")))
		return E_FAIL;

	/*DeadLight*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"DeadLight"), "DeadLight")))
		return E_FAIL;

	/*Dominion Aura*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BlueAura_A"), "BlueAura_A")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BlueAura_R"), "BlueAura_R")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BlueAura_C"), "BlueAura_C")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RedAura_A"), "RedAura_A")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RedAura_R"), "RedAura_R")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RedAura_C"), "RedAura_C")))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffects_Factory::SetUp_StoneParticles()
{
	/* stone */

	/* 질량 */
	_float fStoneDensity = 10.f;

	/* 이 시간 후에 사라짐 */
	_float fStoneLifeTime = 15.f;

	_uint	iStoneNumInstance = 15;

	wstring wstrName = L"DeathStoneParticle_0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/stone/SM_Stone_11.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds"
	))))
		return E_FAIL;

	wstrName = L"DeathStoneParticle_1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/stone/SM_Stone_04.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_32.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_32.dds"
	))))
		return E_FAIL;

	wstrName = L"DeathStoneParticle_2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/stone/SM_Stone_17.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_32.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_32.dds"
	))))
		return E_FAIL;

	wstrName = L"DeathStoneParticle_3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/stone/SM_Stone_36.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_32.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_32.dds"
	))))
		return E_FAIL;


	/* ========================================================================== */

	iStoneNumInstance = 20;
	fStoneDensity = 100.f;
	wstrName = L"WoodenParticle_0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/wood/SM_Wood_05.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_WoodDebris_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_WoodDebrisN_01.dds"

	))))
		return E_FAIL;

	wstrName = L"WoodenParticle_1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/effects/fbx/wood/SM_Wood_06.fbx", iStoneNumInstance, wstrName, fStoneDensity, fStoneLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_WoodDebris_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_WoodDebrisN_01.dds"
	))))
		return E_FAIL;

	_uint iWoodLogNumInstance = 8;
	_float fWoodDensity = 10.f;
	_float fWoodLifeTime = 10.f;

	wstrName = L"WoodenLog_0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/map/environments/structure/wood/wood01/SM_Structure_Wood_Log01a.fbx", iWoodLogNumInstance, wstrName, fWoodDensity, fWoodLifeTime
	))))
		return E_FAIL;

	wstrName = L"WoodenLog_1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/map/environments/structure/wood/wood03/SM_Structure_Wood_Plank01a.fbx", iWoodLogNumInstance, wstrName, fWoodDensity, fWoodLifeTime
	))))
		return E_FAIL;

	wstrName = L"WoodenLog_2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/map/environments/structure/wood/wood07/SM_Structure_Wood_Log04c.fbx", iWoodLogNumInstance, wstrName, fWoodDensity, fWoodLifeTime
	))))
		return E_FAIL;

	/* ================ Wood ================ */


	/* ================ fishbarrel ================ */

	_uint iFishNumInstance = 5;
	_float fFishDensity = 500.f;
	_float fFishLifeTime = 10.f;

	wstrName = L"FishParticle_0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/Map/Environments/Prop/Etc/SM_Prop_Etc_Fish01a.fbx", iFishNumInstance, wstrName, fFishDensity, fFishLifeTime
	))))
		return E_FAIL;

	wstrName = L"FishParticle_1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/Map/Environments/Prop/Etc/SM_Prop_Etc_Fish02a.fbx", iFishNumInstance, wstrName, fFishDensity, fFishLifeTime
	))))
		return E_FAIL;

	wstrName = L"FishParticle_2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/Map/Environments/Prop/Etc/SM_Prop_Etc_Fish03a.fbx", iFishNumInstance, wstrName, fFishDensity, fFishLifeTime
	))))
		return E_FAIL;

	wstrName = L"FishParticle_3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/Map/Environments/Prop/Etc/SM_Prop_Etc_Fish04a.fbx", iFishNumInstance, wstrName, fFishDensity, fFishLifeTime
	))))
		return E_FAIL;

	wstrName = L"FishParticle_4";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/Map/Environments/Prop/Etc/SM_Prop_Etc_Fish05a.fbx", iFishNumInstance, wstrName, fFishDensity, fFishLifeTime
	))))
		return E_FAIL;

	wstrName = L"FishParticle_5";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/map/environments/structure/wood/Plank22/SM_Structure_Wood_Plank01a.fbx", iWoodLogNumInstance, wstrName, fWoodDensity, fWoodLifeTime
	))))
		return E_FAIL;

	/**/

	/*===================== Warrior =====================*/

	_float fDeadBodyDensity = 10.f;
	_float fDeadBodyLifeTime = 15.f;

	wstrName = L"WarriorDead_Slice0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice0.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice0.fbx"

	))))
		return E_FAIL;

	wstrName = L"WarriorDead_Slice1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice1.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds"

	))))
		return E_FAIL;

	wstrName = L"WarriorDead_Slice2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice2.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds"

	))))
		return E_FAIL;

	wstrName = L"WarriorDead_Slice3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice3.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds"

	))))
		return E_FAIL;

	wstrName = L"WarriorDead_Weapon";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/weapons/longsword/SM_WP_LongSword0001_A00.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime, wstring(), wstring(),
		wstring(), XMMatrixScaling(0.01f, 0.01f, 0.01f) //* XMMatrixRotationX(XMConvertToRadians(180.0f))// * XMMatrixRotationY(XMConvertToRadians(90.0f))
	))))
		return E_FAIL;


	/* ========================================================================== */

	/*===================== WarHammer =====================*/

	wstrName = L"EngineerDead_Slice0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice0.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice0.fbx"

	))))
		return E_FAIL;

	wstrName = L"EngineerDead_Slice1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice1.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice1.fbx"

	))))
		return E_FAIL;

	wstrName = L"EngineerDead_Slice2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice2.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice2.fbx"

	))))
		return E_FAIL;

	wstrName = L"EngineerDead_Slice3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice3.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice3.fbx"

	))))
		return E_FAIL;

	wstrName = L"EngineerDead_Weapon";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/weapons/hammer/SM_WP_WarHammer0001_A00.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime, wstring(), wstring(),
		wstring(), XMMatrixScaling(0.01f, 0.01f, 0.01f)
	))))
		return E_FAIL;


	/* ========================================================================== */


	/*===================== Paladin =====================*/

	wstrName = L"PaladinDead_Slice0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Paladin0004_Body_A00_Slice0.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice0.fbx"

	))))
		return E_FAIL;

	wstrName = L"PaladinDead_Slice1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Paladin0004_Body_A00_Slice1.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice1.fbx"

	))))
		return E_FAIL;

	wstrName = L"PaladinDead_Slice2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Paladin0004_Body_A00_Slice2.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice2.fbx"

	))))
		return E_FAIL;

	wstrName = L"PaladinDead_Slice3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Paladin0004_Body_A00_Slice3.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice3.fbx"

	))))
		return E_FAIL;

	wstrName = L"PaladinDead_Weapon";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/weapons/hammer/SM_WP_WarHammer0001_A00.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime, wstring(), wstring(),
		wstring(), XMMatrixScaling(0.01f, 0.01f, 0.01f)
	))))
		return E_FAIL;


	/* ========================================================================== */


	/*===================== Archer =====================*/

	wstrName = L"ArcherDead_Slice0";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Archer0010_Body_A00_Slice0.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Engineer0004_Body_A00_Slice0.fbx"

	))))
		return E_FAIL;

	wstrName = L"ArcherDead_Slice1";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Archer0010_Body_A00_Slice1.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice1.fbx"

	))))
		return E_FAIL;

	wstrName = L"ArcherDead_Slice2";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Archer0010_Body_A00_Slice2.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice2.fbx"

	))))
		return E_FAIL;

	wstrName = L"ArcherDead_Slice3";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/characters/deadbody/SM_Archer0010_Body_A00_Slice3.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime,
		L"../bin/resources/textures/effects/warhaven/texture/T_Stone_01.dds",
		L"../bin/resources/textures/effects/warhaven/texture/T_StoneN_01.dds",
		L"../bin/resources/meshes/characters/deadbody/SM_Warrior0004_Body_A00_Slice3.fbx"

	))))
		return E_FAIL;

	wstrName = L"ArcherDead_Weapon";
	if (FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
		L"../bin/resources/meshes/weapons/hammer/SM_WP_WarHammer0001_A00.fbx", 1, wstrName, fDeadBodyDensity, fDeadBodyLifeTime, wstring(), wstring(),
		wstring(), XMMatrixScaling(0.01f, 0.01f, 0.01f)
	))))
		return E_FAIL;


	/* ========================================================================== */



	return S_OK;
}
