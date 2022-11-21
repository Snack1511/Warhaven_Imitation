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
	
	
	//wstring wstrName = L"TestParticle";
	//if(FAILED(Add_Effect(Convert_ToHash(wstrName.c_str()), CMesh_Particle::Create(
	////L"../bin/resources/meshes/map/environments/building/SM_Architecture_Building_ShipBase01a.fbx", 20, wstrName
	//L"../bin/resources/meshes/map/environments/module/bridge/SM_Module_Bridge_Bridge01a_Lod1.fbx", 20, wstrName
	//))))
	//	return E_FAIL;


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

CGameObject* CEffects_Factory::Create_MeshParticle(wstring wstrKey, _float4 vPos, _float4 vDir, _float fPower)
{
	CGameObject* pGameObject = nullptr;

	_hashcode _hcCode = Convert_ToHash(wstrKey);

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);

	}


		return nullptr;
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

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HItSmokeParticle"), "HItSmokeParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SoilParticle_R_Foot"), "SoilParticle_R_Foot")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SoilParticle_L_Foot"), "SoilParticle_L_Foot")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"GroundHitParticle"), "GroundHitParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SmashSoilParticle"), "SmashSoilParticle")))
		return E_FAIL;

	/*Hit effect*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSpark"), "HitSpark")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_D"), "HitSlash_D"))) //down
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_Right"), "HitSlash_Right"))) //right
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_RD"), "HitSlash_RD"))) //right down
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_Left"), "HitSlash_Left"))) //Left
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HitSlash_LD"), "HitSlash_LD"))) //Left dpwn
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BloodEffect"), "BloodEffect"))) //bone
		return E_FAIL;

	
	return S_OK;
}
