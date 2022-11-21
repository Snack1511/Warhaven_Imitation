#include "CDestructible_Model.h"
#include "MeshContainer.h"

#include "GameInstance.h"



CDestructible_Model::~CDestructible_Model()
{
}

CDestructible_Model::~CDestructible_Model()
{
}

CModel* CDestructible_Model::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix)
{
	CDestructible_Model* pInstance = new CDestructible_Model(iGroupIdx);

	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_Model(eType, wstrModelFilePath, TransformMatrix, 0)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CDestructible_Model");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	/*LOD 로드 방지*/
	pInstance->m_bInstancing = true;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDestructible_Model");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_ConvexMeshes()))
	{
		Call_MsgBox(L"Failed to SetUp_ConvexMeshes : CDestructible_Model");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDestructible_Model::SetUp_ConvexMeshes()
{
	for (auto& elem : m_MeshContainers)
	{
		_uint iNumVertices = elem.second->Get_NumVertices();
		PxVec3* pVec3 = new PxVec3[iNumVertices];

		PxConvexMesh

		GAMEINSTANCE->Create_ConvexMesh(
			&pVec3, iNumVertices, 


		)


	}


    return E_NOTIMPL;
}
