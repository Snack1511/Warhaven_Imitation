#include "..\Public\CLight_Manager.h"
#include "CLight.h"
#include "GameInstance.h"
#include "CMesh_Rect.h"
#include "CShader.h"

#include "GameObject.h"
#include "Transform.h"
#include "Easing_Utillity.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

CLight_Manager::~CLight_Manager()
{
	Release();
}

const LIGHTDESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

CLight* CLight_Manager::Get_Light(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	
	if (iter == m_Lights.end())
		return nullptr;

	return (*iter);
}

CLight* CLight_Manager::Get_FirstLight()
{
	if (m_Lights.empty())
		return nullptr;
	return m_Lights.front();
}

CLight* CLight_Manager::Get_LastLight()
{
	if (m_Lights.empty())
		return nullptr;
	return m_Lights.back();
}

HRESULT CLight_Manager::Add_Light(const LIGHTDESC& LightDesc)
{
	m_ReservedLights.push_back(LightDesc);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader* pShader, CMesh_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

void CLight_Manager::Clear_Lights()
{
	Release();
}

void CLight_Manager::Pop_Light()
{
	SAFE_DELETE(m_Lights.back());
	m_Lights.pop_back();
}

void CLight_Manager::Remove_Light(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	CLight* pLight = (*iter);
	SAFE_DELETE(pLight);
	m_Lights.erase(iter);
}

HRESULT CLight_Manager::Load_Lights(wstring wstrPath)
{
	wstring wstrFilePath = L"../bin/Lights/";
	wstrFilePath += wstrPath;
	wstrFilePath += L".bin";

	ifstream	readFile(wstrFilePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to wrtie File : CWindow_Map");
		return E_FAIL;
	}


	_uint iNumLights = 0;
	readFile.read((char*)&iNumLights, sizeof(_uint));

	for (_uint i = 0; i < iNumLights; ++i)
	{
		LIGHTDESC	tLightDesc;
		readFile.read((char*)&tLightDesc, sizeof(LIGHTDESC));
		tLightDesc.eType = LIGHTDESC::TYPE_POINT;
		if (FAILED(Add_Light(tLightDesc)))
			return E_FAIL;
	}

	readFile.close();

	return S_OK;
}

void CLight_Manager::Update_Lights()
{
	/*if (KEY(Y, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.x += 0.05f;
	if (KEY(H, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.x -= 0.05f;
	if (KEY(U, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.y += 0.05f;
	if (KEY(J, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.y -= 0.05f;
	if (KEY(I, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.z += 0.05f;
	if (KEY(K, TAP))
		m_Lights.back()->m_LightDesc.vDiffuse.z -= 0.05f;
	if (KEY(O, TAP))
	{
		wstring wstrTemp = L"R : ";
		wstrTemp += to_wstring(m_Lights.back()->m_LightDesc.vDiffuse.x);
		wstrTemp += L" G : ";
		wstrTemp += to_wstring(m_Lights.back()->m_LightDesc.vDiffuse.y);
		wstrTemp += L" B : ";
		wstrTemp += to_wstring(m_Lights.back()->m_LightDesc.vDiffuse.z);
		MessageBox(0, wstrTemp.c_str(), TEXT("DIFFUSE"), MB_OK);

	}*/




	for (auto iter = m_Lights.begin(); iter != m_Lights.end();)
	{
		CLight* pLight = *iter;

		//Random Range
		if (pLight->m_LightDesc.fRandomRange > 0.f)
		{
			_float fRange = frandom(-pLight->m_LightDesc.fRandomRange, pLight->m_LightDesc.fRandomRange);
			pLight->m_LightDesc.fRange = pLight->m_LightDesc.fOriginRange + fRange;
		}


		if (pLight->m_LightDesc.pOwner)
		{
			pLight->m_LightDesc.vPosition = pLight->m_LightDesc.pOwner->Get_Transform()->Get_World(WORLD_POS) + pLight->m_LightDesc.vOffset;


			pLight->m_LightDesc.fLightAcc += fDT(0);

			switch (pLight->m_LightDesc.eFadeType)
			{
			case LIGHTDESC::FADEIN:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightFadeInTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;
					pLight->m_LightDesc.eFadeType = LIGHTDESC::FADEOUTREADY;
				}
				else
				{
					_float4 vFadeDiff = XMLoadFloat4(&pLight->m_LightDesc.vTargetDiffuse);
					_float4 vFadeAmbi = XMLoadFloat4(&pLight->m_LightDesc.vTargetAmbient);
					_float4 vFadeSpec = XMLoadFloat4(&pLight->m_LightDesc.vTargetSpecular);

					Select_InEasingType(pLight, vFadeDiff, vFadeAmbi, vFadeSpec,
						pLight->m_LightDesc.fLightAcc, pLight->m_LightDesc.fLightFadeInTime);
				}
				break;

			case LIGHTDESC::FADEOUTREADY:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;
					pLight->m_LightDesc.eFadeType = LIGHTDESC::FADEOUT;

					if ((0.f < pLight->m_LightDesc.vTargetDiffuse.x) && (0.f < pLight->m_LightDesc.vTargetDiffuse.y) &&
						(0.f < pLight->m_LightDesc.vTargetDiffuse.z))
					{
						pLight->m_LightDesc.vDiffuse = pLight->m_LightDesc.vTargetDiffuse;
						pLight->m_LightDesc.vAmbient = pLight->m_LightDesc.vTargetAmbient;
						pLight->m_LightDesc.vSpecular = pLight->m_LightDesc.vTargetSpecular;
					}
				}
				break;

			case LIGHTDESC::FADEOUT:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightFadeOutTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;

					if (!pLight->m_LightDesc.bLoop)
					{
						SAFE_DELETE(pLight);
						iter = m_Lights.erase(iter);
						continue;
					}
					else
					{
						pLight->m_LightDesc.eFadeType = LIGHTDESC::FADEIN;
					}
				}
				else
				{
					_float4 vFadeDiff = XMLoadFloat4(&pLight->m_LightDesc.vTargetDiffuse);
					_float4 vFadeAmbi = XMLoadFloat4(&pLight->m_LightDesc.vTargetAmbient);
					_float4 vFadeSpec = XMLoadFloat4(&pLight->m_LightDesc.vTargetSpecular);

					Select_OutEasingType(pLight, vFadeDiff, vFadeAmbi, vFadeSpec,
						pLight->m_LightDesc.fLightAcc, pLight->m_LightDesc.fLightFadeOutTime);

				}
				break;
			}

			if (!pLight->m_LightDesc.pOwner->Is_Valid())
			{
					SAFE_DELETE(pLight);
					iter = m_Lights.erase(iter);
					continue;
			}
			else
			{
				++iter;
				continue;
			}
		}

		else
		{
			pLight->m_LightDesc.fLightAcc += fDT(0);

			switch (pLight->m_LightDesc.eFadeType)
			{
			case LIGHTDESC::FADEIN:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightFadeInTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;
					pLight->m_LightDesc.eFadeType = LIGHTDESC::FADEOUTREADY;
				}
				else
				{
					_float4 vFadeDiff = XMLoadFloat4(&pLight->m_LightDesc.vTargetDiffuse);
					_float4 vFadeAmbi = XMLoadFloat4(&pLight->m_LightDesc.vTargetAmbient);
					_float4 vFadeSpec = XMLoadFloat4(&pLight->m_LightDesc.vTargetSpecular);

					Select_InEasingType(pLight, vFadeDiff, vFadeAmbi, vFadeSpec,
						pLight->m_LightDesc.fLightAcc, pLight->m_LightDesc.fLightFadeInTime);
				}
				break;

			case LIGHTDESC::FADEOUTREADY:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;
					pLight->m_LightDesc.eFadeType = LIGHTDESC::FADEOUT;

					if ((0.f < pLight->m_LightDesc.vTargetDiffuse.x) && (0.f < pLight->m_LightDesc.vTargetDiffuse.y) &&
						(0.f < pLight->m_LightDesc.vTargetDiffuse.z))
					{
						pLight->m_LightDesc.vDiffuse = pLight->m_LightDesc.vTargetDiffuse;
						pLight->m_LightDesc.vAmbient = pLight->m_LightDesc.vTargetAmbient;
						pLight->m_LightDesc.vSpecular = pLight->m_LightDesc.vTargetSpecular;
					}
				}
				break;

			case LIGHTDESC::FADEOUT:
				if (pLight->m_LightDesc.fLightAcc >= pLight->m_LightDesc.fLightFadeOutTime)
				{
					pLight->m_LightDesc.fLightAcc = 0.f;
					SAFE_DELETE(pLight);
					iter = m_Lights.erase(iter);
					continue;
				}
				else
				{
					_float4 vFadeDiff = XMLoadFloat4(&pLight->m_LightDesc.vTargetDiffuse);
					_float4 vFadeAmbi = XMLoadFloat4(&pLight->m_LightDesc.vTargetAmbient);
					_float4 vFadeSpec = XMLoadFloat4(&pLight->m_LightDesc.vTargetSpecular);

					Select_OutEasingType(pLight, vFadeDiff, vFadeAmbi, vFadeSpec,
						pLight->m_LightDesc.fLightAcc, pLight->m_LightDesc.fLightFadeOutTime);

				}
				break;
			}
			
		}


		++iter;


	}

	for (auto& elem : m_ReservedLights)
	{
		CLight* pLight = CLight::Create(elem);

		m_Lights.push_back(pLight);
	}

	m_ReservedLights.clear();
}

void CLight_Manager::Select_InEasingType(CLight* pLight, _float4 vDiffuse, _float4 vAmbient, _float4 vSpecular, _float fTimeAcc, _float fTime)
{
	switch (pLight->m_LightDesc.eInEasingType)
	{
	case Engine::tagLightDesc::EAS_Linear:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::Linear(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::Linear(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::Linear(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_SinIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::SinIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::SinIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::SinIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_sinfOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::sinfOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::sinfOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::sinfOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_sinfInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::sinfInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::sinfInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::sinfInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseInOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseInOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseInOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_BounceEaseIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::BounceEaseIn(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::BounceEaseIn(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::BounceEaseIn(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_BounceEaseOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::BounceEaseOut(ZERO_VECTOR, vDiffuse, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::BounceEaseOut(ZERO_VECTOR, vAmbient, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::BounceEaseOut(ZERO_VECTOR, vSpecular, fTimeAcc, fTime);
		break;
	default:
		break;
	}
	
	
}

void CLight_Manager::Select_OutEasingType(CLight* pLight, _float4 vDiffuse, _float4 vAmbient, _float4 vSpecular, _float fTimeAcc, _float fTime)
{
	switch (pLight->m_LightDesc.eOutEasingType)
	{
	case Engine::tagLightDesc::EAS_Linear:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::Linear(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::Linear(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::Linear(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuadInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuadInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuadInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuadInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CubicInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CubicInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CubicInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CubicInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuarticInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuarticInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuarticInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuarticInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_QuinticInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::QuinticInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::QuinticInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::QuinticInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_SinIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::SinIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::SinIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::SinIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_sinfOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::sinfOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::sinfOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::sinfOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_sinfInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::sinfInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::sinfInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::sinfInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ExpoInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ExpoInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ExpoInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ExpoInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_CircularInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::CircularInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::CircularInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::CircularInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_ElasticEaseInOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::ElasticEaseInOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::ElasticEaseInOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::ElasticEaseInOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_BounceEaseIn:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::BounceEaseIn(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::BounceEaseIn(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::BounceEaseIn(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	case Engine::tagLightDesc::EAS_BounceEaseOut:
		pLight->m_LightDesc.vDiffuse = CEasing_Utillity::BounceEaseOut(vDiffuse, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vAmbient = CEasing_Utillity::BounceEaseOut(vAmbient, ZERO_VECTOR, fTimeAcc, fTime);
		pLight->m_LightDesc.vSpecular = CEasing_Utillity::BounceEaseOut(vSpecular, ZERO_VECTOR, fTimeAcc, fTime);
		break;
	}
}


void CLight_Manager::Release()
{
	for (auto& pLight : m_Lights)
		SAFE_DELETE(pLight);

	m_Lights.clear();
}
