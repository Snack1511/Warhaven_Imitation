#include "..\Public\Model.h"

#include "MeshContainer.h"
#include "CInstanceMesh.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Texture.h"
#include "CShader.h"
#include "Model.h"

#include "GameInstance.h"

#include "CResource_Material.h"
#include "CResource_Mesh.h"
#include "CResource_Bone.h"
#include "CResource_Animation.h"

#include "Transform.h"
#include "GameObject.h"

CModel::CModel(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{

}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_bCloned(true)
	, m_bCulling(rhs.m_bCulling)
	, m_bInstancing(rhs.m_bInstancing)
	, m_TransformMatrix(rhs.m_TransformMatrix)
	, m_eMODEL_TYPE(rhs.m_eMODEL_TYPE)
	, m_bLOD(rhs.m_bLOD)
	, m_pInstancingMatrices(nullptr)
{
	for (auto& elem : rhs.m_MeshContainers)
	{
		CMeshContainer* pNewContainer = elem.second->Clone();
		m_MeshContainers.push_back(make_pair(elem.first, pNewContainer));
	}

	for (auto& elem : rhs.m_vecHierarchyNodes)
	{
		CHierarchyNode* pCloneNode = elem.second->Clone();
		pCloneNode->ReFind_Parent(this);
		m_vecHierarchyNodes.push_back(make_pair(elem.first, pCloneNode));
	}

	SetUp_MeshContainersPtr();
}

CModel::~CModel()
{
	Release();
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);

	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_Model(eType, wstrModelFilePath, TransformMatrix, 0)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CModel");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);

	pInstance->m_wstrModelFilePath = wstrModelFilePath;
	pInstance->m_bInstancing = true;

	if (FAILED(pInstance->SetUp_InstancingModel(eType, wstrModelFilePath, wstrInstanceFilePath, TransformMatrix, 0)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CModel");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);
	pInstance->m_wstrModelFilePath = wstrModelFilePath;
	pInstance->m_bInstancing = true;
	pInstance->m_iNumInstance = iNumInstance;

	if (FAILED(pInstance->SetUp_InstancingModel(eType, wstrModelFilePath, iNumInstance, TransformMatrix, 0)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CModel");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);
	pInstance->m_wstrModelFilePath = wstrModelFilePath;
	pInstance->m_bInstancing = true;
	pInstance->m_iNumInstance = iNumInstance;
	pInstance->m_pInstancingMatrices = new VTXINSTANCE[iNumInstance];
	pInstance->m_bCulling = true;

	for (_uint i = 0; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
	{
		pInstance->m_pIntancingMatricesLOD[i] = new _float4x4[iNumInstance];
	}

	ZeroMemory(pInstance->m_pInstancingMatrices, sizeof(VTXINSTANCE) * iNumInstance);
	memcpy(pInstance->m_pInstancingMatrices, pInstanceData, sizeof(VTXINSTANCE) * iNumInstance);

	if (FAILED(pInstance->SetUp_InstancingModel(eType, wstrModelFilePath, iNumInstance, pInstanceData, TransformMatrix, 0)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CModel");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


wstring CModel::Get_TextureFilePath(_uint iIndex)
{
	CTexture* pTexture = m_Materials.front().second.pTextures[iIndex];

	if (pTexture)
	{
		return pTexture->Get_vecTexture()[pTexture->Get_CurTextureIndex()].strFilePath;
	}

	return wstring();
}

wstring CModel::Get_TextureFilePathFromParts(_uint iMeshPartType)
{
	for (auto& elem : m_Materials)
	{
		if (elem.first == iMeshPartType)
			return elem.second.pTextures[1]->Get_vecTexture().front().strFilePath;
	}

	return wstring();
}

CTexture* CModel::Get_TextureFromParts(_uint iMeshPartType, _uint iMeshPartType2)
{
	for (auto& elem : m_Materials)
	{
		if (elem.first == iMeshPartType || elem.first == iMeshPartType2)
			return elem.second.pTextures[1];
	}

	return nullptr;
}

_float4x4 CModel::Get_BoneMatrix(const char* pBonename)
{
	_float4x4 BoneMatrix;
	CHierarchyNode* pNode = Find_HierarchyNode(pBonename);
	if (!pNode)
		return BoneMatrix.Identity();

	BoneMatrix = pNode->Get_CombinedMatrix();

	BoneMatrix *= m_TransformMatrix;
	BoneMatrix *= m_pOwner->Get_Transform()->Get_WorldMatrix();

	return BoneMatrix;
}

_float4 CModel::Get_Color(_uint iMeshPartType)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			return elem.second->m_vFontColor;
	}

	return ZERO_VECTOR;
}
void CModel::Set_NewModel(wstring wstrModelFilePath)
{
	m_wstrModelFilePath = wstrModelFilePath;
	Release();

	SetUp_Model(m_eMODEL_TYPE, m_wstrModelFilePath, m_TransformMatrix, 0);
}

void CModel::Set_NewTransformMatrix(_float4x4 TransformMatrix)
{
	m_TransformMatrix = TransformMatrix;
	Release();

	SetUp_Model(m_eMODEL_TYPE, m_wstrModelFilePath, m_TransformMatrix, 0);
}

void CModel::Change_Texture(_uint iModelPart, _uint iTextureIndex, wstring wstrFilePath)
{
	for (auto& elem : m_Materials)
	{
		if (elem.first == iModelPart)
		{
			if (nullptr == elem.second.pTextures[iTextureIndex])
			{
				elem.second.pTextures[iTextureIndex] = CTexture::Create(0, wstrFilePath.c_str(), 1);
				continue;
			}


			elem.second.pTextures[iTextureIndex]->Add_Texture(wstrFilePath.c_str());
			elem.second.pTextures[iTextureIndex]->Set_CurTextureIndex(elem.second.pTextures[iTextureIndex]->Get_TextureSize() - 1);

		}
	}
}

void CModel::Set_ShaderPass(_uint iModelPart, _uint iShaderPass)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iModelPart)
			elem.second->Set_CurPass(iShaderPass);
	}
}

void CModel::Set_ShaderPassToAll(_uint iShaderPass)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_CurPass(iShaderPass);
	}
}

void CModel::Set_Owner(CGameObject* pOwner)
{
	__super::Set_Owner(pOwner);

	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_Owner(pOwner);
	}
}

void CModel::Set_ShaderFlag(_float4 vFlag)
{

	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_ShaderFlag(vFlag);
	}
}

void CModel::Set_ShaderFlag(_uint iMeshPartType, _float4 vFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
		{
			elem.second->Set_ShaderFlag(vFlag);
			
		}

		if (m_bLOD)
		{
			//만약 나머지 값이 그거면
			for (_uint i = 0; i < 3; ++i)
			{
				_uint iNewPartType = elem.first % 10;

				if (iNewPartType == iMeshPartType)
					elem.second->Set_ShaderFlag(vFlag);

			}
		}

		
	
	}
}

void CModel::Set_ShaderColor(_uint iMeshPartType, _float4 vColor)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			elem.second->Set_Color(vColor);

		if (m_bLOD)
		{
			//만약 나머지 값이 그거면
			for (_uint i = 0; i < 3; ++i)
			{
				_uint iNewPartType = elem.first % 10;

				if (iNewPartType == iMeshPartType)
					elem.second->Set_Color(vColor);

			}
		}
	}
}

void CModel::Set_OutlineFlag(_float4 vOutlineFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_OutlineFlag(vOutlineFlag);
	}
}

void CModel::Set_OutlineFlag(_uint iMeshPartType, _float4 vOutlineFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			elem.second->Set_OutlineFlag(vOutlineFlag);

		if (m_bLOD)
		{
			//만약 나머지 값이 그거면
			for (_uint i = 0; i < 3; ++i)
			{
				_uint iNewPartType = elem.first % 10;

				if (iNewPartType == iMeshPartType)
					elem.second->Set_OutlineFlag(vOutlineFlag);

			}
		}
	}
}

void CModel::Set_RimLightFlag(_float4 vRimLightFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_RimLightFlag(vRimLightFlag);
	}
}


void CModel::Set_RimLightFlag(_uint iMeshPartType, _float4 vRimLightFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			elem.second->Set_RimLightFlag(vRimLightFlag);

		if (m_bLOD)
		{
			//만약 나머지 값이 그거면
			for (_uint i = 0; i < 3; ++i)
			{
				_uint iNewPartType = elem.first % 10;

				if (iNewPartType == iMeshPartType)
					elem.second->Set_RimLightFlag(vRimLightFlag);

			}
		}
	}
}


void CModel::Switch_MeshContainerBone(_uint iMeshPartType, string strBoneName)
{
	CMeshContainer* pMeshCon = nullptr;

	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
		{
			pMeshCon = elem.second;
			break;
		}

	}

	if (pMeshCon)
	{
		if (pMeshCon->m_Bones.size() == 1)
		{
			pMeshCon->m_Bones.front() = Find_HierarchyNode(strBoneName.c_str());
		}

	}
}

HRESULT CModel::Add_Model(wstring wstrModelFilePath, _uint iMeshPartType)
{
	m_wstrModelFilePath = wstrModelFilePath;

	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, m_eMODEL_TYPE);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();
	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (FAILED(Create_ModelData(pResource, pModelData->m_vecResType[i], iMeshPartType)))
			return E_FAIL;
	}

	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();

	/* 매칭시켜주기 */
	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Add_Model(wstring wstrModelFilePath, _uint iMeshPartType, string strBoneName)
{
	m_wstrModelFilePath = wstrModelFilePath;

	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, m_eMODEL_TYPE);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();
	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (FAILED(Create_ModelData(pResource, pModelData->m_vecResType[i], iMeshPartType)))
			return E_FAIL;

		if (pModelData->m_vecResType[i] == RES_MESH)
		{
			m_MeshContainers.back().second->m_strRefBoneName = strBoneName;
			//m_MeshContainers.back().second->m_TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
		}
	}



	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();

	/* 매칭시켜주기 */
	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;





	return S_OK;
}

HRESULT CModel::Enable_ModelParts(_uint iMeshPartType, _bool bEnable)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
		{
			if (bEnable)
				ENABLE_COMPONENT(elem.second);
			else
				DISABLE_COMPONENT(elem.second);
		}
	}

	return S_OK;
}

void CModel::Set_TransformMatrix(_uint iMeshPartType, _float4x4 matTransform)
{
	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		if (!m_MeshContainers[i].first)
			continue;

		if ((m_MeshContainers[i].first % 10) == iMeshPartType)
		{
			if (!m_MeshContainers[i].second)
				continue;

			m_MeshContainers[i].second->m_PrevTransformMatrix = matTransform;
		}
	}
		
}

HRESULT CModel::Delete_ModelParts(_uint iMeshContainerType)
{
	for (auto iter = m_MeshContainers.begin(); iter != m_MeshContainers.end();)
	{
		if (iter->first == iMeshContainerType)
		{
			SAFE_DELETE((iter->second));
			iter = m_MeshContainers.erase(iter);
		}
		else
			++iter;

	}

	for (auto iter = m_Materials.begin(); iter != m_Materials.end();)
	{
		if (iter->first == iMeshContainerType)
		{
			for (auto& pTexture : iter->second.pTextures)
			{
				SAFE_DELETE(pTexture);
			}

			iter = m_Materials.erase(iter);
		}
		else
			++iter;

	}


	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();
	return S_OK;
}

HRESULT CModel::Initialize_Prototype()
{
	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();


	/* NonAnim이면 LOD도 읽기 */
	if (m_eMODEL_TYPE == TYPE_NONANIM || m_bInstancing)
	{
		SetUp_InstancingModel_LOD();
	}
	else
		SetUp_Model_LOD();


	return S_OK;
}

HRESULT CModel::Initialize()
{


	return S_OK;
}

void CModel::Tick()
{

}

void CModel::Late_Tick()
{
	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode.second->Update_CombinedTransformationMatrix();

}

void CModel::Final_Tick()
{
	if (!m_bCulling)
		return;

	if (m_eMODEL_TYPE == TYPE_ANIM)
	{
		//1.일단 절두체
		_float4 vWorldPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
		vWorldPos.y += 0.6f;
		_float fRange = 2.f;

		//절두체에 없으면 
		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vWorldPos.XMLoad(), fRange))
		{
			//다끄기
			for (_uint i = 0; i < m_iNumMeshContainers; ++i)
			{
				m_MeshContainers[i].second->Set_Enable(false);
			}
		}
		else
		{
			//d있으면 LOD 체크
			if (m_bLOD)
			{
				_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
				_float4 vScale = m_pOwner->Get_Transform()->Get_Scale();
				_float fScaleLength = vScale.x;
				_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();

				if (vScale.y > fScaleLength)
					fScaleLength = vScale.y;

				if (vScale.z > fScaleLength)
					fScaleLength = vScale.z;

				vWorldPos;

				_float fCurDistance = (vCamPos - vWorldPos).Length();

				//거리가 멀면 LOD true
				m_eLOD_Level = eLOD_LEVEL::eLOD1;

				for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
				{
					_float fLODDistance = m_fAnimLODDistance;
					//i만큼 곱해서 단계를 나눔
					fLODDistance *= (_float)i;

					//fLODDistance += m_fAnimLODDistance;

					//현재 거리가 단계보다 크면
					if (fCurDistance > fLODDistance)
						m_eLOD_Level = (eLOD_LEVEL)i;
				}

				//cout << (_uint)m_eLOD_Level << endl;

				//LOD메쉬만 켜주기
				for (_uint i = 0; i < m_iNumMeshContainers; ++i)
				{
					if (m_MeshContainers[i].first == 0)
					{
						m_MeshContainers[i].second->Set_Enable(false);
						continue;
					}

					if (m_MeshContainers[i].first == 4 || m_MeshContainers[i].first == 5)
					{
						m_MeshContainers[i].second->Set_Enable(true);
						continue;
					}


					//10보다 크고 20보다 작아야함
					if (m_MeshContainers[i].first >= (_uint)m_eLOD_Level * 10 && m_MeshContainers[i].first < ((_uint)m_eLOD_Level + 1) * 10)
						m_MeshContainers[i].second->Set_Enable(true);
					else
						m_MeshContainers[i].second->Set_Enable(false);

				}


			}
			else
			{
				//LOD가 아니면 0제외 다키기
				for (_uint i = 0; i < m_iNumMeshContainers; ++i)
				{
					if (m_MeshContainers[i].first == 0 || m_MeshContainers[i].first >= 10)
						{
							m_MeshContainers[i].second->Set_Enable(false);
							continue;
						}

					m_MeshContainers[i].second->Set_Enable(true);
				}
			}

		}
		return;

	}


	/* INSTANCING */
	if (m_bInstancing)
	{
		if (m_bLOD)
		{
			_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();

			/* 갯수들 일단 다 초기화 */
			ZeroMemory(m_iLODNumInstance, sizeof(_uint) * (_uint)eLOD_LEVEL::eLOD_END);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				//1. 절두체로 걸러

				_float4x4 matInstance;
				memcpy(&matInstance, &m_pInstancingMatrices[i], sizeof(_float4x4));

				if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pInstancingCenterPos[i].XMLoad(), m_pInstancingMaxRange[i]))
				{
					continue;
				}

				//2. 살아남은 애들 LOD 체크
				_float fCurDistance = (vCamPos - m_pInstancingCenterPos[i]).Length();

				/* 너무많은 잔디같은애면 거리 체크 까지*/
				if (m_bHardLOD)
				{
					if (fCurDistance > 40.f)
						continue;
				}


				eLOD_LEVEL eLODLevel = eLOD_LEVEL::eLOD1;

				/*if (m_iNumInstance > 10)
				{
					eLODLevel = eLOD_LEVEL::eLOD3;
				}
				else*/
				{
					for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
					{
						_float fLODDistance = (m_bHardLOD) ? m_fLODDistance * 0.5f : m_fLODDistance;
						//i만큼 곱해서 단계를 나눔
						fLODDistance *= (_float)i;
						fLODDistance += m_pInstancingMaxRange[i];

						//현재 거리가 단계보다 크면
						if (fCurDistance > fLODDistance)
							eLODLevel = (eLOD_LEVEL)i;
					}
				}

				


				/* LOD별 인스턴스 리맵 정보 갱신 */
				_uint iCurLODNumInstance = m_iLODNumInstance[(_uint)eLODLevel];

				m_pIntancingMatricesLOD[(_uint)eLODLevel][iCurLODNumInstance] = matInstance;

				m_iLODNumInstance[(_uint)eLODLevel]++;


			}

			//3. 이제 remap 해야함
			for (auto& elem : m_MeshContainers)
			{
				_uint iCurNumInstance = m_iLODNumInstance[elem.first];
				_float4x4* matInstance = m_pIntancingMatricesLOD[elem.first];

				if (iCurNumInstance == 0)
				{
					elem.second->Set_Enable(false);
					continue;
				}
				else
					elem.second->Set_Enable(true);

				CInstanceMesh* pMesh = nullptr;
#ifdef _DEBUG
				pMesh = dynamic_cast<CInstanceMesh*>(elem.second);

				if (!pMesh)
				{
					elem.second->Set_Enable(false);
					continue;

				}
#else
				pMesh = static_cast<CInstanceMesh*>(elem.second);

#endif
				static_cast<CInstanceMesh*>(elem.second)->ReMap_Instances(iCurNumInstance, matInstance);
				}
			}

		return;

		}

	/* NONANIM MODEL */

	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
	_float4 vScale = m_pOwner->Get_Transform()->Get_Scale();
	_float fScaleLength = vScale.x;
	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();

	if (vScale.y > fScaleLength)
		fScaleLength = vScale.y;

	if (vScale.z > fScaleLength)
		fScaleLength = vScale.z;

	_uint iNumMeshContainers = m_iNumMeshContainers / 2;

	if (m_bLOD)
	{
		_float4 vWorldPos = m_vLODCenterPos.MultiplyCoord(matWorld);

		_float fCurDistance = (vCamPos - vWorldPos).Length();

		//거리가 멀면 LOD true
		m_eLOD_Level = eLOD_LEVEL::eLOD1;

		for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
		{
			_float fLODDistance = m_fLODDistance;
			//i만큼 곱해서 단계를 나눔
			fLODDistance *= (_float)i;

			fLODDistance += m_fLODMaxRange;

			//현재 거리가 단계보다 크면
			if (fCurDistance > fLODDistance)
				m_eLOD_Level = (eLOD_LEVEL)i;
		}
	}


	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		if (m_bLOD) // LOD 있는 객체면 일단 거르고 시작
		{
			//현재 단계 아닌 애들은 일단 컷
			if (m_MeshContainers[i].first != (_uint)m_eLOD_Level)
			{
				m_MeshContainers[i].second->Set_Enable(false);
				continue;
			}
		}

		_float4 vWorldPos = m_MeshContainers[i].second->Get_CenterPos().MultiplyCoord(matWorld);
		_float fRange = m_MeshContainers[i].second->Get_MaxRange();
		fRange *= fScaleLength;


		//통과면은 절두체까지해서 처리
		if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vWorldPos.XMLoad(), fRange))
		{
			m_MeshContainers[i].second->Set_Enable(true);
		}
		else
		{
			m_MeshContainers[i].second->Set_Enable(false);
		}
	}
}

HRESULT CModel::Render(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex, const char* pConstantBoneName)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	if (!m_MeshContainers[iMeshContainerIndex].second->Is_Valid())
		return S_OK;

	m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResourceFlag(pShader, "g_vFlag");
	m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResource(pShader, "g_vColor");

	if (m_eMODEL_TYPE == TYPE_ANIM)
		m_MeshContainers[iMeshContainerIndex].second->SetUp_BoneMatrices(pShader, "g_Bones");

	if (FAILED(pShader->Begin(m_MeshContainers[iMeshContainerIndex].second->Get_CurPass())))
		return E_FAIL;

	return m_MeshContainers[iMeshContainerIndex].second->Render();
}

HRESULT CModel::Bake_Shadow(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	//m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResourceFlag(pShader, "g_vFlag");

	if (m_eMODEL_TYPE == TYPE_ANIM)
		m_MeshContainers[iMeshContainerIndex].second->SetUp_BoneMatrices(pShader, "g_Bones");

	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	return m_MeshContainers[iMeshContainerIndex].second->Render();
}

void CModel::Release()
{
	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		SAFE_DELETE(pHierarchyNode.second);
	m_vecHierarchyNodes.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		SAFE_DELETE(pMeshContainer.second);

	m_MeshContainers.clear();

	/*Instancing Culling Data */
	SAFE_DELETE_ARRAY(m_pInstancingMatrices);
	SAFE_DELETE_ARRAY(m_pInstancingCenterPos);
	SAFE_DELETE_ARRAY(m_pInstancingMaxRange);

	for (_uint i = 0; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
	{
		SAFE_DELETE_ARRAY(m_pIntancingMatricesLOD[i]);
	}

	if (true == m_bCloned)
		return;

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			SAFE_DELETE(Material.second.pTextures[i]);
		}
	}

	m_Materials.clear();
}

void CModel::Start()
{
	__super::Start();

	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_Owner(m_pOwner);
		elem.second->Start();
	}

	for (auto& elem : m_vecHierarchyNodes)
	{
		elem.second->m_pOwner = m_pOwner;
	}

#ifdef MEMORY_SAVE
	if (m_bLOD)
	{
		if (m_eMODEL_TYPE == TYPE_ANIM)
		{
			for (auto iter = m_MeshContainers.begin(); iter != m_MeshContainers.end();)
			{


				if (iter->first == 1 ||
					iter->first == 2 ||
					iter->first == 3)
				{
					SAFE_DELETE(iter->second);
					iter = m_MeshContainers.erase(iter);
				}
				else
					++iter;
			}
		}
		else
		{
			for (auto iter = m_MeshContainers.begin(); iter != m_MeshContainers.end();)
			{
				if (iter->first == 0)
				{
					SAFE_DELETE(iter->second);
					iter = m_MeshContainers.erase(iter);
				}
				else
					++iter;
			}
		}
		
	}

	m_iNumMaterials = m_Materials.size();
	m_iNumMeshContainers = m_MeshContainers.size();

#endif // _DEBUG
}

CHierarchyNode* CModel::Find_HierarchyNode(const char* pBoneName)
{
	auto	iter = find_if(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [&](pair<_uint, CHierarchyNode*> pNode)
		{
			return !strcmp(pBoneName, pNode.second->Get_Name());
		});

	if (iter == m_vecHierarchyNodes.end())
		return nullptr;

	return iter->second;
}

_uint CModel::Find_MaterialIndex(const char* pMatName)
{
	_uint iIdx = 0;

	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].second.strName == pMatName)
		{
			return i;
		}
	}

	return 0;
}

void CModel::Sort_HierarchyNode()
{
	sort(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [](pair<_uint, CHierarchyNode*> pSour, pair < _uint, CHierarchyNode*> pDest)
		{
			return pSour.second->Get_Depth() < pDest.second->Get_Depth();
		});
}

HRESULT CModel::Bind_SRV(CShader* pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex].second->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].second.pTextures[eType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].second.pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);
}

HRESULT CModel::SetUp_Model(MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	if (m_eMODEL_TYPE == TYPE_NONANIM)
	{
		if (iMeshPartType == 0)
			m_wstrModelFilePath = wstrModelFilePath;
	}
	else
	{
		m_wstrModelFilePath = wstrModelFilePath;
	}

	m_eMODEL_TYPE = eType;
	if (m_eMODEL_TYPE == TYPE_NONANIM)
		m_bCulling = true;

	if (m_eMODEL_TYPE == TYPE_ANIM)
		m_bCulling = true;

	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;
		Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_eMODEL_TYPE = eType;
	m_bCulling = false;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (pModel_Data->m_vecResType[i] == RES_MESH)
		{
			Create_InstancingMesh(static_cast<CResource_Mesh*>(pResource), wstrInstanceFilePath, TransformMatrix, iMeshPartType);
		}
		else
		{
			Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
		}

	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}


HRESULT CModel::SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_bInstancing = true;
	m_eMODEL_TYPE = eType;
	m_bCulling = false;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (pModel_Data->m_vecResType[i] == RES_MESH)
		{
			if (FAILED(Create_InstancingMesh(static_cast<CResource_Mesh*>(pResource), iNumInstance, TransformMatrix, iMeshPartType)))
				return E_FAIL;
		}
		else
		{
			Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
		}

	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_eMODEL_TYPE = eType;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (pModel_Data->m_vecResType[i] == RES_MESH)
		{
			Create_InstancingMesh(static_cast<CResource_Mesh*>(pResource), iNumInstance, pInstanceData, TransformMatrix, iMeshPartType);
		}
		else
		{
			Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
		}

	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;
	return S_OK;
}

HRESULT CModel::SetUp_MeshContainersPtr()
{
	for (auto& elem : m_MeshContainers)
	{
		if (FAILED(elem.second->SetUp_Ptr(this)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::SetUp_Model_LOD()
{
	for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
	{
		if (FAILED(Load_LOD((eLOD_LEVEL)i)))
		{
			m_bLOD = false;
			return S_OK;
		}
	}


	m_iNumMeshContainers = m_MeshContainers.size();
	m_iNumMaterials = m_Materials.size();
	m_bLOD = true;
	
	m_fLODMaxRange = 0.f;
	for (auto& elem : m_MeshContainers)
	{
		m_vLODCenterPos += elem.second->Get_CenterPos();
		_float fRange = elem.second->Get_MaxRange();

		m_fLODMaxRange += fRange;
	}

	m_vLODCenterPos /= m_iNumMeshContainers;

	return S_OK;
}

HRESULT CModel::SetUp_AnimModel_LOD()
{
	//모든 메쉬컨테이너 별로 LOD를 따야함
	//한 fbx 주소로 모델타입을 해야함

	//_uint iCurIndex = 0;


	wstring	wstrPrevPath;

	vector<pair<_uint, class CMeshContainer*>>	vecMCTemp = m_MeshContainers;

	for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
	{
		for (auto elem : vecMCTemp)
		{
			if (elem.first == 0 || elem.first == 4 || elem.first == 5)
				continue;

			m_wstrModelFilePath = elem.second->m_wstrFilePath;
			//이전꺼랑 같으면 넘기기
			if (m_wstrModelFilePath == wstrPrevPath)
			{
				continue;
			}

			wstrPrevPath = m_wstrModelFilePath;
			if (FAILED(Load_Anim_LOD((eLOD_LEVEL)i, elem.first)))
			{
				m_bLOD = false;
				return S_OK;
			}
		}

	}


	m_iNumMeshContainers = m_MeshContainers.size();
	m_iNumMaterials = m_Materials.size();
	m_bLOD = true;

	return S_OK;
}

HRESULT CModel::SetUp_InstancingModel_LOD()
{
	if (!m_pInstancingMatrices)
	{
		m_bLOD = false;
		return S_OK;

	}

	for (_uint i = 1; i < (_uint)eLOD_LEVEL::eLOD_END; ++i)
	{
		if (FAILED(Load_InstancingLOD((eLOD_LEVEL)i)))
		{
			m_bLOD = false;
			return S_OK;
		}
	}


	m_iNumMeshContainers = m_MeshContainers.size();
	m_iNumMaterials = m_Materials.size();

	m_bLOD = true;
	if (m_iNumInstance > 200)
		m_bHardLOD = true;
	Bake_LODFrustumInfo();

	return S_OK;
}

HRESULT CModel::Load_LOD(eLOD_LEVEL eLevel)
{
	_int iLevelNum = (_int)eLevel;


	/* LOD 찾기 */
	m_wstrModelFilePath;

	wstring wstrNewPath;
	//1. 확장자까지 잘라보기
	wstrNewPath = CutPath_R(m_wstrModelFilePath, L".");

	//4. _LOD3 더하기
	wstrNewPath += L"_Lod";
	wstrNewPath += to_wstring(iLevelNum);
	wstrNewPath += L".FBX";


	if (FAILED(SetUp_Model(m_eMODEL_TYPE, wstrNewPath, m_TransformMatrix, iLevelNum)))
	{
		//실패시 LOD는 false인걸루
		m_bLOD = false;
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CModel::Load_Anim_LOD(eLOD_LEVEL eLevel, _uint iMeshPartType)
{
	_int iLevelNum = (_int)eLevel;


	/* LOD 찾기 */
	m_wstrModelFilePath;

	wstring wstrNewPath;
	//1. 확장자까지 잘라보기
	wstrNewPath = CutPath_R(m_wstrModelFilePath, L".");

	//4. _LOD3 더하기
	wstrNewPath += L"_Lod";
	wstrNewPath += to_wstring(iLevelNum);
	wstrNewPath += L".FBX";

	iLevelNum = iMeshPartType + iLevelNum * 10;

	if (FAILED(SetUp_Model(m_eMODEL_TYPE, wstrNewPath, m_TransformMatrix, iLevelNum)))
	{
		//실패시 LOD는 false인걸루
		m_bLOD = false;
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CModel::Load_InstancingLOD(eLOD_LEVEL eLevel)
{
	_int iLevelNum = (_int)eLevel;

	/* LOD 찾기 */
	m_wstrModelFilePath;

	wstring wstrNewPath;
	//1. 확장자까지 잘라보기
	wstrNewPath = CutPath_R(m_wstrModelFilePath, L".");

	//4. _LOD3 더하기
	wstrNewPath += L"_Lod";
	wstrNewPath += to_wstring(iLevelNum);
	wstrNewPath += L".FBX";

	//5. 일단 함 찾아보기
	if (FAILED(SetUp_InstancingModel(m_eMODEL_TYPE, wstrNewPath, m_iNumInstance, m_pInstancingMatrices, m_TransformMatrix, iLevelNum)))
	{
		return E_FAIL;
	}


	return S_OK;
}


HRESULT CModel::Create_ModelData(CResource* pResource, _uint iResType, _uint iMeshPartType)
{
	switch (iResType)
	{
	case RES_ANIMATION:
	{
		return E_FAIL;
	}
	break;

	case RES_MATERIAL:
	{
		CResource_Material* pResource_Material = dynamic_cast<CResource_Material*>(pResource);
		if (!pResource_Material)
			return E_FAIL;

		if (FAILED(Create_Material(pResource_Material, iMeshPartType)))
			return E_FAIL;
	}
	break;

	case RES_BONE:
	{
		CResource_Bone* pResource_Bone = dynamic_cast<CResource_Bone*>(pResource);
		if (!pResource_Bone)
			return E_FAIL;

		if (FAILED(Create_HierarchyNode(pResource_Bone, nullptr, 0, iMeshPartType)))
			return E_FAIL;
	}
	break;

	case RES_MESH:
	{
		CResource_Mesh* pResource_Mesh = dynamic_cast<CResource_Mesh*>(pResource);
		if (!pResource_Mesh)
			return E_FAIL;

		if (FAILED(Create_MeshContainer(pResource_Mesh, m_TransformMatrix, iMeshPartType)))
			return E_FAIL;
	}
	break;


	default:
		break;
	}

	return S_OK;
}

HRESULT CModel::Create_Material(CResource_Material* pResource, _uint iMeshPartType)
{
	MATERIAL_DESC& tMatDesc = pResource->Get_MatDesc();
	MODEL_MATERIAL			Material;

	Material.strName = pResource->Get_Name();
	Material.vColor = tMatDesc.vMixColor;

	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
	{
		if (tMatDesc.TexturePaths[j].empty())
		{
			Material.pTextures[j] = nullptr;
			continue;
		}

		Material.pTextures[j] = CTexture::Create(0, wstring(tMatDesc.TexturePaths[j].begin(), tMatDesc.TexturePaths[j].end()).c_str(), 1);

		if (nullptr == Material.pTextures[j])
			return E_FAIL;
	}

	m_Materials.push_back(make_pair(iMeshPartType, Material));

	return S_OK;
}

HRESULT CModel::Create_MeshContainer(CResource_Mesh* pResource, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CMeshContainer* pMeshContainer = CMeshContainer::Create(pResource, TransformMatrix, m_wstrModelFilePath);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_InstancingMesh(CResource_Mesh* pResource, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CInstanceMesh* pMeshContainer = CInstanceMesh::Create(wstrInstanceFilePath, pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_InstancingMesh(CResource_Mesh* pResource, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CInstanceMesh* pMeshContainer = CInstanceMesh::Create(iNumInstance, pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_InstancingMesh(CResource_Mesh* pResource, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CInstanceMesh* pMeshContainer = CInstanceMesh::Create(iNumInstance, pInstanceData, pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);
	pMeshContainer->Set_SaveMemory();

	return S_OK;
}

HRESULT CModel::Create_HierarchyNode(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, _uint iMeshPartType)
{
	//1. 일단 모두 만듬
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pResource, pParent, iDepth, ANIM_DIVIDE::eDEFAULT);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	//2. 만들어진거 모두 가져옴
	vector<CHierarchyNode*>	vecNodes;
	pHierarchyNode->Get_AllNodes(vecNodes);

	sort(vecNodes.begin(), vecNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

	if (m_vecHierarchyNodes.empty())
	{
		for (_uint i = 0; i < vecNodes.size(); ++i)
		{
			m_vecHierarchyNodes.push_back(make_pair(iMeshPartType, vecNodes[i]));
		}
		return S_OK;

	}


	for (_uint i = 0; i < vecNodes.size(); ++i)
	{
		_bool bExist = false;

		for (_uint j = 0; j < m_vecHierarchyNodes.size(); ++j)
		{
			// 이미 있는지 검색
			if (!strcmp(m_vecHierarchyNodes[j].second->Get_Name(), vecNodes[i]->Get_Name()))
			{
				//이미 있으면 그녀석이랑 나를 대체해야함.

				//박힌돌
				//m_vecHierarchyNodes[j].second;

				////굴러온돌
				//vecNodes[i];

				////1. 이녀석의 부모한테서 새로 연결
				////2. 원래의 자식들을 새로 부모로
				//CHierarchyNode* pNewNode = vecNodes[i]->Clone();

				//for (auto& pChildNode : m_vecHierarchyNodes[j].second->m_pChildrenNodes)
				//{
				//	pChildNode->m_pParent = pNewNode;
				//}

				//pNewNode->m_pChildrenNodes.clear();
				//pNewNode->m_pChildrenNodes = m_vecHierarchyNodes[j].second->m_pChildrenNodes;\
				//
				//pNewNode->m_pParent = m_vecHierarchyNodes[j].second->m_pParent;

				//if (pNewNode->m_pParent)
				//	pNewNode->m_iDepth = pNewNode->m_pParent->m_iDepth + 1;

				//SAFE_DELETE(m_vecHierarchyNodes[j].second);
				//m_vecHierarchyNodes[j].second = pNewNode;

				bExist = true;
				break;
			}
		}

		if (bExist)
			continue;

		// 처음 보는 녀석이다.

		if (vecNodes[i]->m_pParent)
		{
			CHierarchyNode* pRealParentNode = Find_HierarchyNode(vecNodes[i]->m_pParent->Get_Name());
			CHierarchyNode* pNewNode = vecNodes[i]->Clone();
			pNewNode->m_pChildrenNodes.clear();
			pNewNode->m_pParent = pRealParentNode;
			pNewNode->m_iDepth = pRealParentNode->m_iDepth + 1;
			pRealParentNode->Add_NewChild(vecNodes[i]);

			m_vecHierarchyNodes.push_back(make_pair(iMeshPartType, pNewNode));
		}
		else
		{
			return E_FAIL;
		}

	}

	for (_uint i = 0; i < vecNodes.size(); ++i)
		SAFE_DELETE(vecNodes[i]);


	Sort_HierarchyNode();

	return S_OK;
}

void CModel::Bake_LODFrustumInfo()
{
	m_pInstancingCenterPos = new _float4[m_iNumInstance];
	ZeroMemory(m_pInstancingCenterPos, sizeof(_float4) * m_iNumInstance);
	m_pInstancingMaxRange = new _float[m_iNumInstance];
	ZeroMemory(m_pInstancingMaxRange, sizeof(_float) * m_iNumInstance);

	_float4 vRealCenterPos;

	{
		_float4 vMin = _float4(99909.f, 99099.f, 99099.f), vMax = _float4(-90999.f, -99909.f, -99999.f);

		for (auto& elem : m_MeshContainers)
		{
			_uint iNumVertices = elem.second->Get_NumVertices();
			_float3* pVerticesPos = elem.second->Get_VerticesPos();

			for (_uint j = 0; j < iNumVertices; ++j)
			{
				_float4 vCurVerticesPos;
				memcpy(&vCurVerticesPos, &pVerticesPos[j], sizeof(_float3));
				vCurVerticesPos.w = 1.f;

				if (vCurVerticesPos.x < vMin.x)
					vMin.x = vCurVerticesPos.x;
				else if (vCurVerticesPos.x > vMax.x)
					vMax.x = vCurVerticesPos.x;

				if (vCurVerticesPos.y < vMin.y)
					vMin.y = vCurVerticesPos.y;
				else if (vCurVerticesPos.y > vMax.y)
					vMax.y = vCurVerticesPos.y;

				if (vCurVerticesPos.z < vMin.z)
					vMin.z = vCurVerticesPos.z;
				else if (vCurVerticesPos.z > vMax.z)
					vMax.z = vCurVerticesPos.z;
			}


		}

		vRealCenterPos = (vMin + vMax) * 0.5f;
	}



	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float4x4 matInstance;
		memcpy(&matInstance, &m_pInstancingMatrices[i], sizeof(_float4x4));


		m_pInstancingCenterPos[i] = vRealCenterPos.MultiplyCoord(matInstance);


		//2. MaxRange 찾기
		//m_pInstancingCenterPos[i] = (vMin + vMax) * 0.5f;

		_float fMaxRange = 0.f;

		for (auto& elem : m_MeshContainers)
		{
			_uint iNumVertices = elem.second->Get_NumVertices();
			_float3* pVerticesPos = elem.second->Get_VerticesPos();

			for (_uint j = 0; j < iNumVertices; ++j)
			{
				_float4 vCurVerticesPos;
				memcpy(&vCurVerticesPos, &pVerticesPos[j], sizeof(_float3));
				vCurVerticesPos.w = 1.f;

				vCurVerticesPos = vCurVerticesPos.MultiplyCoord(matInstance);

				_float fDistance = (vCurVerticesPos - m_pInstancingCenterPos[i]).Length();

				if (fDistance > fMaxRange)
					fMaxRange = fDistance;
			}
		}

		m_pInstancingMaxRange[i] = max(fMaxRange, 1.f);
	}



}
