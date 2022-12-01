#include "stdafx.h"
#include "CMesh_Particle.h"

#include "UsefulHeaders.h"

#include "CMesh_Cube.h"
#include "CUtility_PhysX.h"

#include "CInstanceMesh.h"

#include "Easing_Utillity.h"




CMesh_Particle::CMesh_Particle()
{
}

CMesh_Particle::CMesh_Particle(const CMesh_Particle& _origin)
	: CEffect(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_bCloned(true)
	, m_pConvexMesh(_origin.m_pConvexMesh)
	, m_fLifeTime(_origin.m_fLifeTime)
{
	m_pInstanceMatrices = new _float4x4[m_iNumInstance];
	ZeroMemory(m_pInstanceMatrices, sizeof(_float4x4) * m_iNumInstance);


	/* 피직스 굽기 */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(tTransform));
	tTransform.q.w = 1.f;

	/*for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxRigidDynamic* pActor = nullptr;

		pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.f);

		m_vecRigidDynamics.push_back(pActor);
		pActor->setActorFlag(PxActorFlag::Enum::eDISABLE_SIMULATION, true);
		pActor->putToSleep();
	}*/

}

CMesh_Particle::~CMesh_Particle()
{
	if (!m_bCloned)
		Safe_release(m_pConvexMesh);


	SAFE_DELETE_ARRAY(m_pInstanceMatrices);
}

CMesh_Particle* CMesh_Particle::Create(wstring wstrModelFilePath, _uint iNumInstance, wstring strName, _float fDensity, _float fLifeTime
	, wstring wstrTextureFilePath, wstring wstrNormalTexturePath, wstring	wstrConvexMeshPath)
{
	CMesh_Particle* pInstance = new CMesh_Particle();

	pInstance->m_iNumInstance = iNumInstance;
	pInstance->m_fLifeTime = fLifeTime;
	pInstance->m_hcMyCode = Convert_ToHash(strName);
	pInstance->m_fDensity = fDensity;
	pInstance->m_wstrColorMapPath = wstrTextureFilePath;
	pInstance->m_wstrMaskMapPath = wstrNormalTexturePath;
	pInstance->m_wstrPath = wstrConvexMeshPath;

	if (FAILED(pInstance->SetUp_MeshParticle(wstrModelFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshParticle : CMesh_Particle");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CMesh_Particle");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}



void CMesh_Particle::Start_Particle(_float4 vPos, _float4 vDir, _float fPower, _float4x4 matWorld)
{
	m_pTransform->Get_Transform().matMyWorld = matWorld;
	m_pTransform->Make_WorldMatrix();

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxTransform tTransform;
		ZeroMemory(&tTransform, sizeof(PxTransform));

		_float4 vAngles = _float4(frandom(-10.f, 10.f), frandom(-10.f, 10.f), frandom(-10.f, 10.f));

		if (m_iNumInstance == 1)
			vAngles = _float4(0.f, 0.f, 0.f);

		vAngles = XMQuaternionRotationRollPitchYawFromVector(vAngles.XMLoad());

	


		memcpy(&tTransform.q, &vAngles, sizeof(_float4));

		//위치 랜덤으로 뽑고 그 방향으로 힘 주기
		_float4 vRandDir =
		{
			frandom(-1.f, 1.f),
			frandom(-0.5f, 1.f),
			frandom(-1.f, 1.f),
			0.f
		};

		vRandDir.Normalize();

		if (m_iNumInstance > 1)
		{
			vPos += vRandDir * 0.2f;
		}

		tTransform.p.x = vPos.x;
		tTransform.p.y = vPos.y;
		tTransform.p.z = vPos.z;

		vRandDir *= fPower * frandom(0.9f, 1.1f);


		PxRigidDynamic* pActor = nullptr;

		pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, m_fDensity);

		if (!pActor)
			continue;

		pActor->addForce(CUtility_PhysX::To_PxVec3(vRandDir));
		m_vecRigidDynamics.push_back(pActor);



		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);

		//피직스월드매트릭스에 역행렬을 곱해야대

		m_pInstanceMatrices[i] = matPhysX;// * matWorldInv;
	}

	m_fReverseAcc = 99.f;
	m_bReverse = false;
	m_vecMatrices.clear();
	ENABLE_GAMEOBJECT(this);
}

void CMesh_Particle::Set_DeathParticle()
{
	m_bDeathParticle = true;
	m_vecMatrices.clear();
	m_vecMatrices.resize(m_iNumInstance);
	Update_NodeSave();

}

void CMesh_Particle::Start_Reverse(CUnit* pUnit)
{
	if (m_bReverse)
		return;

	m_pRebornUnit = pUnit;

	m_fReverseAcc = 0.f;

	if (!m_bReverse && !m_vecMatrices.empty())
	{
		m_bReverse = true;

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			Safe_release(m_vecRigidDynamics[i]);
		}

		m_vecRigidDynamics.clear();
	}
}

HRESULT CMesh_Particle::Initialize_Prototype()
{
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODELINSTANCE,
		VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements);

	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXINSTANCE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));


	Add_Component<CRenderer>(pRenderer);

	



	return S_OK;
}

HRESULT CMesh_Particle::Initialize()
{
	return S_OK;
}

HRESULT CMesh_Particle::Start()
{
	CGameObject::Start();


	return S_OK;
}

void CMesh_Particle::OnEnable()
{
	
}

void CMesh_Particle::OnDisable()
{
	__super::OnDisable();
	m_fTimeAcc = 0.f;

	if (!m_vecRigidDynamics.empty())
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			Safe_release(m_vecRigidDynamics[i]);
		}

		m_vecRigidDynamics.clear();

	}

}

HRESULT CMesh_Particle::SetUp_MeshParticle(wstring wstrModelFilePath)
{
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	CModel* pModelCom = nullptr;

	pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath.c_str(), m_iNumInstance, m_matTrans);
	Add_Component(pModelCom);


	if (!m_wstrColorMapPath.empty())
	{
		pModelCom->Change_Texture(0, 1, m_wstrColorMapPath);
	}

	if (!m_wstrMaskMapPath.empty())
	{
		pModelCom->Change_Texture(0, aiTextureType_NORMALS, m_wstrMaskMapPath);
	}


	//ConvexMesh 하나 구워놓고 일단
	CModel* pConvexModel = pModelCom;
	if (!m_wstrPath.empty())
	{
		pConvexModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, m_wstrPath.c_str(), m_iNumInstance, m_matTrans);
		Add_Component(pConvexModel);
	}


	CMeshContainer* pMesh = (pConvexModel->Get_MeshContainers().front().second);

	FACEINDICES32* pIndices = pMesh->CMesh::Get_Indices();
	_uint iNumPrimitive = pMesh->Get_NumPrimitive();

	_uint iNumVertices = pMesh->Get_NumVertices();
	_float3* pVerticesPos = pMesh->Get_VerticesPos();

	GAMEINSTANCE->Create_ConvexMesh(
		pVerticesPos,
		iNumVertices,
		pIndices,
		iNumPrimitive,
		&m_pConvexMesh);

	if (!m_pConvexMesh)
		return E_FAIL;


	m_pInstanceMatrices = new _float4x4[m_iNumInstance];
	ZeroMemory(m_pInstanceMatrices, sizeof(_float4x4) * m_iNumInstance);


	return S_OK;
}

void CMesh_Particle::My_Tick()
{

	/* 시간으로 하는데 거리 비례해서 ? */
	if (m_bReverse)
		Update_Reverse();
	else
	{
		Update_NodeSave();


		m_fTimeAcc += fDT(0);

		if (m_fTimeAcc >= m_fLifeTime)
			DISABLE_GAMEOBJECT(this);
	}


	
}

void CMesh_Particle::My_LateTick()
{
	if (!m_pInstanceMatrices)
		return;

	if (m_bReverse)
		return;

	if (m_vecRigidDynamics.empty())
		return;

	_float4x4 matWorldInv = m_pTransform->Get_WorldMatrix();
	matWorldInv.Inverse();

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		PxTransform tTransform = m_vecRigidDynamics[i]->getGlobalPose();

		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);

		//피직스월드매트릭스에 역행렬을 곱해야대

		m_pInstanceMatrices[i] = matPhysX;// * matWorldInv;
	}

	vector<pair<_uint, class CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();

	for (auto& elem : vecMC)
	{
		static_cast<CInstanceMesh*>(elem.second)->ReMap_Instances(m_pInstanceMatrices);
	}
		

}

void CMesh_Particle::Update_Reverse()
{
	if (!m_bDeathParticle)
		return;

	m_fReverseAcc += fDT(0.f);

	if (m_fReverseAcc >= m_fRebornTime)
	{
		//다음으로
		m_fReverseAcc = 0.f;
		//남은 갯수가 적을 수록 시간이 늘어나
		if (m_vecMatrices[0].size() <= 4)
		{
			m_fRebornTime *= 3.5f;
		}

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_vecMatrices[i].pop_back();
			m_pInstanceMatrices[i] = m_vecMatrices[i].back();

		}

		

		//매트릭스갱신

		if (m_vecMatrices[0].size() <= 1)
		{
			//끝
			m_fRebornTime = m_fRebornTimeOrigin;
			m_vecMatrices.clear();
			m_bReverse = false;
			DISABLE_GAMEOBJECT(this);

			if (m_pRebornUnit)
			{
				m_pRebornUnit->On_Reborn();

			}

			return;
		}
	}
	else
	{
		_uint iSize = m_vecMatrices[0].size();
		_float fRatio = m_fReverseAcc / m_fRebornTime;

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			//시간 안지났으면 보간

			//맨 뒤에서 그 전꺼로 보간

			_float4x4 matPrev = m_vecMatrices[i][iSize - 1];
			_float4x4 matTarget = m_vecMatrices[i][iSize - 2];

			_vector vRight, vUp, vLook, vPos;

			if (iSize == 2)
			{
				vRight = CEasing_Utillity::QuadOut(matPrev.XMLoad().r[0], matTarget.XMLoad().r[0], m_fReverseAcc, m_fRebornTime).XMLoad();
				vUp = CEasing_Utillity::QuadOut(matPrev.XMLoad().r[1], matTarget.XMLoad().r[1], m_fReverseAcc, m_fRebornTime).XMLoad();
				vLook = CEasing_Utillity::QuadOut(matPrev.XMLoad().r[2], matTarget.XMLoad().r[2], m_fReverseAcc, m_fRebornTime).XMLoad();
				vPos = CEasing_Utillity::QuadOut(matPrev.XMLoad().r[3], matTarget.XMLoad().r[3], m_fReverseAcc, m_fRebornTime).XMLoad();
			}
			else
			{
				vRight = XMVectorLerp(matPrev.XMLoad().r[0], matTarget.XMLoad().r[0], fRatio);
				vUp = XMVectorLerp(matPrev.XMLoad().r[1], matTarget.XMLoad().r[1], fRatio);
				vLook = XMVectorLerp(matPrev.XMLoad().r[2], matTarget.XMLoad().r[2], fRatio);
				vPos = XMVectorLerp(matPrev.XMLoad().r[3], matTarget.XMLoad().r[3], fRatio);
			}


			XMVectorSetW(vPos, 1.f);

			XMMATRIX	CurMat;
			CurMat.r[0] = XMVector3Normalize(vRight);
			CurMat.r[1] = XMVector3Normalize(vUp);
			CurMat.r[2] = XMVector3Normalize(vLook);
			CurMat.r[3] = vPos;

			m_pInstanceMatrices[i] = CurMat;
		}
	}

	vector<pair<_uint, class CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();

	for (auto& elem : vecMC)
	{
		static_cast<CInstanceMesh*>(elem.second)->ReMap_Instances(m_pInstanceMatrices);
	}
}

void CMesh_Particle::Update_NodeSave()
{
	if (!m_bDeathParticle)
		return;

	/* 이동  매트릭스 저장 */
	m_fReverseAcc += fDT(0.f);

	
	if (m_fReverseAcc >= m_fReverseTime)
	{

		if (m_vecMatrices[0].size() < 40)
		{

			m_fReverseAcc = 0.f;
			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				m_vecMatrices[i].push_back(m_pInstanceMatrices[i]);
			}


		}

	}

}
