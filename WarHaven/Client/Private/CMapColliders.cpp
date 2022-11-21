#include "stdafx.h"
#include "CMapColliders.h"

#include "UsefulHeaders.h"

CMapColliders::CMapColliders()
{
}

CMapColliders::~CMapColliders()
{
    Release();
}

CMapColliders* CMapColliders::Create(string wstrFileKey)
{
    CMapColliders* pInstance = new CMapColliders();

    if (FAILED(pInstance->SetUp_MapColliders(wstrFileKey)))
    {
        Call_MsgBox(L"Failed to SetUp_MapColliders : CMapColliders");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CMapColliders");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    return pInstance;
}

HRESULT CMapColliders::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMapColliders::Initialize()
{
    return S_OK;
}

HRESULT CMapColliders::Start()
{
    return S_OK;
}

void CMapColliders::Release()
{
    for (auto& elem : m_StaticBoxes)
        Safe_release(elem);

	m_StaticBoxes.clear();
}

void CMapColliders::My_Tick()
{
}

void CMapColliders::My_LateTick()
{
}

HRESULT CMapColliders::SetUp_MapColliders(string wstrFileKey)
{
	string savePath;
	savePath = "../bin/Data/MapData/PhysXBoxData/";
	savePath += wstrFileKey;
	savePath += ".bin";


	ifstream	readFile(savePath, ios::binary);


	if (!readFile.is_open())
	{
		return E_FAIL;
	}

	_uint iNumBoxes = 0;

	readFile.read((char*)&iNumBoxes, sizeof(_uint));

	for (_uint i = 0; i < iNumBoxes; ++i)
	{
		PxTransform tTransform;
		_float4 vScale;

		readFile.read((char*)&tTransform, sizeof(PxTransform));
		readFile.read((char*)&vScale, sizeof(_float4));

		m_StaticBoxes.push_back(Create_StaticBox(tTransform, vScale));
	}

    return S_OK;
}

PxRigidStatic* CMapColliders::Create_StaticBox(PxTransform tTransform, _float4 vScale)
{
	PxRigidStatic* pRigidStatic = nullptr;

	pRigidStatic = GAMEINSTANCE->Create_StaticActor(
		tTransform,
		PxBoxGeometry(vScale.x * 0.5f, vScale.y * 0.5f, vScale.z * 0.5f),
		CPhysX_Manager::SCENE_CURRENT);

	return pRigidStatic;
}
