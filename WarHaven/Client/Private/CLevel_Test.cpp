#include "stdafx.h"
#include "CLevel_Test.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"


CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
}

CLevel_Test* CLevel_Test::Create()
{
#ifdef _DEBUG
    if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
        return nullptr;
#endif

    CLevel_Test* pLevel = new CLevel_Test();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Test::Initialize()
{



    return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes()
{
    CSkyBox* pSkyBox = CSkyBox::Create();

    if (FAILED(pSkyBox->Initialize()))
        return E_FAIL;

    Ready_GameObject(pSkyBox, GROUP_DEFAULT);

    m_fLoadingFinish = 0.25f;




    m_fLoadingFinish = 1.f;


    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
    LightDesc.fRange = 1000.f;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Test::Enter()
{
    __super::Enter();

    CGameInstance::Get_Instance()->Change_Camera(L"Free");
    

    return S_OK;
}

void CLevel_Test::Tick()
{

#ifdef _DEBUG
    CImGui_Manager::Get_Instance()->Tick();
#endif

    CUser::Get_Instance()->KeyInput_FPSSetter();
}

void CLevel_Test::Late_Tick()
{
}

HRESULT CLevel_Test::Render()
{

    if (FAILED(CImGui_Manager::Get_Instance()->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Test::Exit()
{
    __super::Exit();

    return S_OK;
}


