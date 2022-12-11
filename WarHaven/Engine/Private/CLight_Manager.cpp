#include "..\Public\CLight_Manager.h"
#include "CLight.h"
#include "GameInstance.h"
#include "CMesh_Rect.h"
#include "CShader.h"

#include "GameObject.h"
#include "Transform.h"
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

			if (pLight->m_LightDesc.fLightAcc > pLight->m_LightDesc.fLightTime)
			{
				SAFE_DELETE(pLight);
				iter = m_Lights.erase(iter);
				continue;
			}
			else
			{
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

			
		}
		else
		{
			pLight->m_LightDesc.fLightAcc += fDT(0);

			if (pLight->m_LightDesc.fLightAcc > pLight->m_LightDesc.fLightTime)
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
			

	}

	for (auto& elem : m_ReservedLights)
	{
		CLight* pLight = CLight::Create(elem);

		m_Lights.push_back(pLight);
	}

	m_ReservedLights.clear();
}

void CLight_Manager::Release()
{
	for (auto& pLight : m_Lights)
		SAFE_DELETE(pLight);

	m_Lights.clear();
}
