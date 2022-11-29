#include "..\Public\GameInstance.h"

#include "Transform.h"
#include "Camera.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
	Release();
}

ComPtr<ID3D11Device>	CGameInstance::Get_Device()
{
	return m_pGraphicDevice->Get_Device();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_DeviceContext()
{
	return m_pGraphicDevice->Get_DeviceContext();
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHICDESC& GraphicDesc, const SOUNDDESC& SoundDesc, const TIMEDESC& TimeDesc)
{
	Initialize();

	/* 그래픽디바이스. */
	if (FAILED(m_pGraphicDevice->Ready_Graphic_Device(GraphicDesc)))
		return E_FAIL;  

	/* 인풋 디바이스. */
	if (FAILED(m_pKeyManager->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pInputDevice->Initialize(GraphicDesc.hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* FMOD */
	if (FAILED(m_pSoundDevice->Initialize(SoundDesc)))
		return E_FAIL;

	/* Initalize PhysX_Manager*/
	if (FAILED(m_pPhysXManager->Initialize()))
		return E_FAIL;

	/* Time Manager */
	if (FAILED(m_pTimeManager->Initialize(GraphicDesc.hWnd, TimeDesc)))
		return E_FAIL;

	/* Camera Manager*/
	if (FAILED(m_pCameraManager->Initialize(GraphicDesc)))
		return E_FAIL;

	/* Collision Manager */
	if (FAILED(m_pCollisionManager->Initialize()))
		return E_FAIL;

	/* Picking Manager */
	if (FAILED(m_pPickingManager->Initialize(GraphicDesc)))
		return E_FAIL;

	/* Prototype Manager */
	if (FAILED(m_pPrototypeManager->Initialize()))
		return E_FAIL;

	/* Frustum Manager */
	if (FAILED(m_pFrustumManager->Initialize()))
		return E_FAIL;

	/* Render Manager */
	if (FAILED(m_pRenderManager->Initialize()))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine( )
{
	m_pTimeManager->Tick();

	if (!m_pTimeManager->Can_Update())
		return S_OK;

	/* Event */
	m_pEventManager->Tick();

	/* Key */
	m_pKeyManager->Tick();
	m_pInputDevice->SetUp_DeviceStates();

	/* Object */
	m_pLevelManager->Tick();
	m_pObjectManager->Tick_GameObjects();
	m_pComponentManager->Tick();


	m_pRenderManager->Update();
	m_pPhysXManager->Tick();


	m_pLevelManager->Late_Tick();

	m_pComponentManager->Late_Tick();
	m_pObjectManager->LateTick_GameObjects();


	/* ViewProj */
	m_pCameraManager->Make_ViewProj();

	/* Frustum */
	m_pFrustumManager->Update();

	m_pComponentManager->Final_Tick();

	/* Other Events */
	m_pPickingManager->Execute_Picking();
	m_pCollisionManager->Tick();
	m_pLightManager->Update_Lights();

	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
#ifdef _DEBUG
	m_pRenderManager->Tick_Debug();
#endif // _DEBUG

	if (FAILED(m_pRenderManager->Render()))
		return E_FAIL;

	if (FAILED(m_pLevelManager->Render()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pCollisionManager->Render()))
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CGameInstance::Initialize()
{
	//_CrtSetBreakAlloc(583940);


#define SAFE_GET_INSTANCE(name, type) if (!(name = type::Get_Instance())) return E_FAIL;

	SAFE_GET_INSTANCE(m_pKeyManager, CKey_Manager);
	SAFE_GET_INSTANCE(m_pObjectManager, CObject_Manager);
	SAFE_GET_INSTANCE(m_pLevelManager, CLevel_Manager);
	SAFE_GET_INSTANCE(m_pEventManager, CEvent_Manager);
	SAFE_GET_INSTANCE(m_pCollisionManager, CCollision_Manager);
	SAFE_GET_INSTANCE(m_pCameraManager, CCamera_Manager);
	SAFE_GET_INSTANCE(m_pRenderManager, CRender_Manager);
	SAFE_GET_INSTANCE(m_pTimeManager, CTime_Manager);
	SAFE_GET_INSTANCE(m_pComponentManager, CComponent_Manager);
	SAFE_GET_INSTANCE(m_pPickingManager, CPicking_Manager);
	SAFE_GET_INSTANCE(m_pGraphicDevice, CGraphic_Device);
	SAFE_GET_INSTANCE(m_pShaderManager, CShader_Manager);
	SAFE_GET_INSTANCE(m_pSoundDevice, CSound_Device);
	SAFE_GET_INSTANCE(m_pInputDevice, CInput_Device);
	SAFE_GET_INSTANCE(m_pPrototypeManager, CPrototype_Manager);
	SAFE_GET_INSTANCE(m_pFontManager, CFont_Manager);
	SAFE_GET_INSTANCE(m_pResourceManager, CResource_Manager);
	SAFE_GET_INSTANCE(m_pFrustumManager, CFrustum_Manager);
	SAFE_GET_INSTANCE(m_pLightManager, CLight_Manager);
	SAFE_GET_INSTANCE(m_pTargetManager, CTarget_Manager);
	SAFE_GET_INSTANCE(m_pPhysXManager, CPhysX_Manager);

	return S_OK;
}

void CGameInstance::Release()
{
	m_pKeyManager->Destroy_Instance();
	m_pObjectManager->Destroy_Instance();
	m_pLevelManager->Destroy_Instance();
	m_pEventManager->Destroy_Instance();
	m_pCollisionManager->Destroy_Instance();
	m_pCameraManager->Destroy_Instance();
	m_pRenderManager->Destroy_Instance();
	m_pTimeManager->Destroy_Instance();
	m_pComponentManager->Destroy_Instance();
	m_pPickingManager->Destroy_Instance();
	m_pPrototypeManager->Destroy_Instance();
	m_pShaderManager->Destroy_Instance();
	m_pInputDevice->Destroy_Instance();
	m_pSoundDevice->Destroy_Instance();
	m_pFontManager->Destroy_Instance();
	m_pResourceManager->Destroy_Instance();
	m_pFrustumManager->Destroy_Instance();
	m_pTargetManager->Destroy_Instance();
	m_pLightManager->Destroy_Instance();
	m_pGraphicDevice->Destroy_Instance();
	m_pPhysXManager->Destroy_Instance();


}

void CGameInstance::Clear_Resources()
{
	m_pResourceManager->Clear_Resources();
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphicDevice->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphicDevice->Present();
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	return m_pInputDevice->Get_DIMouseMoveState(eMouseMove);
}

HRESULT CGameInstance::Load_SoundFile(wstring wstrFolderPath)
{
	return m_pSoundDevice->Load_SoundFile(wstrFolderPath);
}

void CGameInstance::Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	m_pSoundDevice->Play_Sound(strSoundKey, iGroupIndex, fVolumeRatio);
}


void CGameInstance::Play_BGM(const _tchar* strSoundKey)
{
	m_pSoundDevice->Play_BGM(strSoundKey);
}

void CGameInstance::Stop_Sound(CHANNEL_GROUP eType)
{
	m_pSoundDevice->Stop_Sound(eType);

}

void CGameInstance::Stop_All()
{
	m_pSoundDevice->Stop_All();
}

void CGameInstance::Set_Volume(_float fVolume)
{
	m_pSoundDevice->Set_Volume(fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP  eID, _float fVolume)
{
	m_pSoundDevice->Set_ChannelVolume(eID, fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume)
{
	m_pSoundDevice->Set_ChannelVolume(eID, iChannelIndex, fVolume);
}

_double CGameInstance::Get_DT(_uint iIndex)
{
	return m_pTimeManager->Get_DT();
}

_double CGameInstance::Get_FPSLimitTime()
{
	return m_pTimeManager->Get_FPSLimitTime();
}

void CGameInstance::Set_FPSLimitTIme(_double dTime)
{
	m_pTimeManager->Set_FPSLimitTIme(dTime);
}

_bool CGameInstance::Can_Update()
{
	return m_pTimeManager->Can_Update();
}


_float CGameInstance::Get_TimeSpeed(_uint iIndex)
{
	return m_pTimeManager->Get_TimeSpeed(iIndex);
}

void CGameInstance::Set_TimeSpeed(_uint iIndex, _float fSpeed)
{
	m_pTimeManager->Set_TimeSpeed(iIndex, fSpeed);
}

HRESULT CGameInstance::Exit_CurLevel()
{
	return m_pLevelManager->Exit_CurLevel();
}

KEY_STATE CGameInstance::Get_KeyState(KEY _key)
{
	return m_pKeyManager->Get_KeyState(_key);
}

vector<CKey_Manager::tKeyInfo>& CGameInstance::Get_KeyList()
{
	return m_pKeyManager->Get_KeyList();
}

HRESULT CGameInstance::Check_Group(const _uint& _eLeft, const _uint& _eRight)
{
	return m_pCollisionManager->Check_Group(_eLeft, _eRight);
}

ComPtr<ID3D11ShaderResourceView> CGameInstance::Get_Texture(wstring wstrFilePath)
{
	return m_pResourceManager->Get_Texture(wstrFilePath);
}

void CGameInstance::Create_ConvexMesh(_float3* pVertices, _uint iNumVertices, void* pIndices, _uint iNumPrimitive, PxConvexMesh** ppOut)
{
	m_pPhysXManager->Create_ConvexMesh(pVertices, iNumPrimitive, pIndices, iNumPrimitive, ppOut);
}

HRESULT CGameInstance::Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity)
{
	return m_pPhysXManager->Create_Scene(eScene, Gravity);
}

HRESULT CGameInstance::Delete_Scene(CPhysX_Manager::Scene eScene)
{
	return m_pPhysXManager->Delete_Scene(eScene);
}

HRESULT CGameInstance::Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity)
{
	return m_pPhysXManager->Change_Scene(eNextScene, Gravity);
}

void CGameInstance::Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut)
{
	m_pPhysXManager->Create_CylinderMesh(fRadiusBelow, fRadiusUpper, fHight, ppOut);
}

void CGameInstance::Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut)
{
	//m_pPhysXManager->Create_ConvexMesh(pVertices, iNumVertice, ppOut);
}

void CGameInstance::Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut)
{
	m_pPhysXManager->Create_Material(fStaticFriction, fDynamicFriction, fRestitution, ppOut);
}

void CGameInstance::Create_Shape(const PxGeometry& Geometry, PxMaterial* pMaterial, PxShape** ppOut)
{
	return m_pPhysXManager->Create_Shape(Geometry, pMaterial, ppOut);
}

PxRigidDynamic* CGameInstance::Create_DynamicActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, const PxReal& Density, const PxVec3& velocity, PxMaterial* pMaterial)
{
	return m_pPhysXManager->Create_DynamicActor(t, geometry, eScene, Density);
}

PxRigidStatic* CGameInstance::Create_StaticActor(const PxTransform& t, const PxGeometry& geometry, CPhysX_Manager::Scene eScene, PxMaterial* pMaterial)
{
	return m_pPhysXManager->Create_StaticActor(t, geometry, eScene, pMaterial);
}

void CGameInstance::Begin_PhysScene()
{
	return m_pPhysXManager->Begin_PhysScene();
}

void CGameInstance::End_PhysScene()
{
	return m_pPhysXManager->End_PhysScene();
}

void CGameInstance::Bake_StaticShadow(vector<CGameObject*>& MapList, _float fDistance)
{
	m_pRenderManager->Bake_StaticShadow(MapList, fDistance);
}

void CGameInstance::Start_RadialBlur(_float fTargetPower)
{
	m_pRenderManager->Start_RadialBlur(fTargetPower);
}

void CGameInstance::Stop_RadialBlur()
{
	m_pRenderManager->Stop_RadialBlur();
}

void CGameInstance::Start_ChromaticAberration(_float fTargetPower)
{
	m_pRenderManager->Start_ChromaticAberration(fTargetPower);
}

void CGameInstance::Stop_ChromaticAberration()
{
	m_pRenderManager->Stop_ChromaticAberration();

}

void CGameInstance::Start_GrayScale(_float fTargetPower)
{
	m_pRenderManager->Start_GrayScale(fTargetPower);
}

void CGameInstance::Stop_GrayScale()
{
	m_pRenderManager->Stop_GrayScale();
}

list<CGameObject*>& CGameInstance::Get_ObjGroup(const _uint& iGroupIdx)
{
	return m_pObjectManager->Get_ObjGroup(iGroupIdx);
}

void CGameInstance::Delete_Objects(const _uint& iGroupIdx)
{
	return m_pObjectManager->Delete_Objects(iGroupIdx);
}

void CGameInstance::Clear_All_Components()
{
	m_pComponentManager->Clear_All();
	m_pCollisionManager->Clear_All();
}

void CGameInstance::Compute_WorldRay()
{
	m_pPickingManager->Compute_WorldRay();
}

void CGameInstance::Regist_Mesh(CMesh* pMesh, _float fDistanceToPlayer)
{
	m_pPickingManager->Regist_Mesh(pMesh, fDistanceToPlayer);

}

_bool CGameInstance::Is_Picked(list<CGameObject*>& GameObjectList, _float4* pOut, _float4* pOutNormal)
{
	return m_pPickingManager->Is_Picked(GameObjectList, pOut, pOutNormal);
}

_bool CGameInstance::Is_Picked(CGameObject* pGameObject, _float4* pOut, _float4* pOutNormal)
{
	return m_pPickingManager->Is_Picked(pGameObject, pOut, pOutNormal);
}

_bool CGameInstance::Is_Picked(CMesh* pRenderer, _float4* pOut, _float4* pOutNormal)
{
	return m_pPickingManager->Is_Picked(pRenderer, pOut, pOutNormal);
}

_bool CGameInstance::Is_Picked_Mesh(CMesh* pRenderer, _uint3* pOutPickedIndex, _float4* pOut,_float4* pOutNormal)
{
	return m_pPickingManager->Is_Picked_Mesh(pRenderer, pOutPickedIndex, pOut, pOutNormal);
}

_bool CGameInstance::Is_Picked_Cubes(vector<CGameObject*>& GameObjectList, _float4* pOut, _uint* pOutIndex, _float4* pOutNormal)
{
	return m_pPickingManager->Is_Picked_Cubes(GameObjectList, pOut, pOutIndex, pOutNormal);
}

void CGameInstance::Add_Camera(wstring strKey, CCamera * pCamera)
{
	m_pCameraManager->Add_Camera(strKey, pCamera);
}

void CGameInstance::Add_Camera_Level(wstring strKey, CCamera* pCamera)
{
	m_pCameraManager->Add_Camera_Level(strKey, pCamera);
}

CCamera * CGameInstance::Change_Camera(wstring strKey)
{
	return m_pCameraManager->Change_Camera(strKey);
}

CCamera* CGameInstance::Reset_Camera(wstring strKey)
{
	return m_pCameraManager->Reset_Camera(strKey);
}

CCamera* CGameInstance::Get_CurCam()
{
	return m_pCameraManager->Get_CurCam();
}

_float4 CGameInstance::Get_CurCamLook()
{
	return m_pCameraManager->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
}

_float4 CGameInstance::Get_ViewPos()
{
	return m_pCameraManager->Get_ViewPos();
}

_float4x4 CGameInstance::Get_CurProjMatrix()
{
	return m_pCameraManager->Get_Instance()->Get_Proj();
}

_float4x4 CGameInstance::Get_CurViewMatrix()
{
	return m_pCameraManager->Get_Instance()->Get_View();
}

CCamera* CGameInstance::Find_Camera(wstring strKey)
{
	return m_pCameraManager->Find_Camera(strKey);
}

void CGameInstance::Delete_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Delete_GameObject(pGameObject);
}

void CGameInstance::Create_GameObject(CGameObject* pGameObject, const _uint& iGroupIdx)
{
	m_pEventManager->Create_GameObject(pGameObject, iGroupIdx);

}

void CGameInstance::Delete_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	m_pEventManager->Delete_Component(pComponent, pGameObject);

}

void CGameInstance::Create_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	m_pEventManager->Create_Component(pComponent, pGameObject);
}

void CGameInstance::Enable_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Enable_GameObject(pGameObject);
}

void CGameInstance::Disable_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Disable_GameObject(pGameObject);
}

void CGameInstance::Disable_Component(CComponent* pComponent)
{
	m_pEventManager->Disable_Component(pComponent);

}

void CGameInstance::Enable_Component(CComponent* pComponent)
{
	m_pEventManager->Enable_Component(pComponent);

}

void CGameInstance::Create_StaticObject(CGameObject * pGameObject, _hashcode hcClassName)
{
	m_pEventManager->Create_StaticObject(pGameObject, hcClassName);
}

void CGameInstance::Change_Level(CLevel* pLevel)
{
	m_pEventManager->Change_Level(pLevel);
}

void CGameInstance::Clear_All_Event()
{
	m_pEventManager->Clear_All_Event();
}

void CGameInstance::Clear_Enable_Events()
{
	m_pEventManager->Clear_Enable_Events();
}

CGameObject* CGameInstance::Clone_GameObject(_hashcode hcClassName)
{
	return m_pPrototypeManager->Clone_GameObject(hcClassName);
}

CComponent* CGameInstance::Clone_Component(_hashcode hcClassName)
{
	return m_pPrototypeManager->Clone_Component(hcClassName);
}

HRESULT CGameInstance::Add_GameObject_Prototype(CGameObject* pGameObject, _hashcode _hcCode)
{

	return m_pPrototypeManager->Add_GameObject_Prototype(pGameObject, _hcCode);
}

void CGameInstance::Delete_GameObject_Prototypes()
{
	m_pPrototypeManager->Delete_GameObject_Prototypes();
}

void CGameInstance::Delete_Component_Prototypes()
{
	m_pPrototypeManager->Delete_Component_Prototypes();
}

HRESULT CGameInstance::Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	return m_pFontManager->Add_Font(pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, const _float4& vColor, _float fScale)
{
	return m_pFontManager->Render_Font(pFontTag, pString, vPosition, vColor, fScale);
}

HRESULT CGameInstance::Load_EffectFile(const _tchar* pFilePath)
{
	return m_pShaderManager->Load_EffectFile(pFilePath);
}

HRESULT CGameInstance::Set_RawValue(const _uint& iIndex, const char* pConstantName, void* pData, _uint iDataSize)
{
	return m_pShaderManager->Set_RawValue(iIndex, pConstantName, pData, iDataSize);
}

HRESULT CGameInstance::Set_RawValue_All(const char* pConstantName, void* pData, _uint iDataSize)
{
	return m_pShaderManager->Set_RawValue_All(pConstantName, pData, iDataSize);
}

MODEL_DATA* CGameInstance::Get_ModelData(wstring wstrFilePath, MODEL_TYPE eType)
{
	return m_pResourceManager->Get_ModelData(wstrFilePath, eType);
}

CResource* CGameInstance::Get_Resource(wstring wstrResourceKey)
{
	return m_pResourceManager->Get_Resource(wstrResourceKey);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_vector vWorldPoint, _float fRange)
{
	return m_pFrustumManager->isIn_Frustum_InWorldSpace(vWorldPoint, fRange);
}

_bool CGameInstance::isIn_Frustum_InLocalSpace(_vector vLocalPoint, _float fRange)
{
	return m_pFrustumManager->isIn_Frustum_InLocalSpace(vLocalPoint, fRange);
}

void CGameInstance::Transform_ToLocalSpace(_matrix WorldMatrixInv)
{
	return m_pFrustumManager->Transform_ToLocalSpace(WorldMatrixInv);
}

HRESULT CGameInstance::Add_Light(const LIGHTDESC& LightDesc)
{
	return m_pLightManager->Add_Light(LightDesc);
}

void CGameInstance::Pop_Light()
{
	return m_pLightManager->Pop_Light();
}

HRESULT CGameInstance::Load_Lights(wstring wstrPath)
{
	return m_pLightManager->Load_Lights(wstrPath);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	return m_pLightManager->Get_LightDesc(iIndex);
}

void CGameInstance::Clear_Lights()
{
	m_pLightManager->Clear_Lights();
}

void CGameInstance::Remove_Light(_uint iIndex)
{
	m_pLightManager->Remove_Light(iIndex);
}

CLight* CGameInstance::Get_Light(_uint iIndex)
{
	return m_pLightManager->Get_Light(iIndex);
}

CLight* CGameInstance::Get_FirstLight()
{
	return m_pLightManager->Get_FirstLight();
}

CLight* CGameInstance::Get_LastLight()
{
	return m_pLightManager->Get_LastLight();
}

_int CGameInstance::Get_LightSize()
{
	return m_pLightManager->Get_LightSize();
}

ComPtr<ID3D11ShaderResourceView> CGameInstance::Get_RenderTarget_SRV(wstring pTargetTag)
{
	return m_pTargetManager->Get_SRV(pTargetTag);
}

CShader* CGameInstance::Get_DeferredShader()
{
	return m_pRenderManager->Get_DeferredShader();
}
