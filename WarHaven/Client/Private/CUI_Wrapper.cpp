#include "stdafx.h"
#include "CUI_Wrapper.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Renderer.h"
#include "CFader.h"
#include "CUI_Object.h"

#include "Texture.h"
#include "CShader.h"

CUI_Wrapper::CUI_Wrapper()
{
}

CUI_Wrapper::CUI_Wrapper(const CUI_Wrapper& Prototype)
    : CGameObject(Prototype)
{
}

CUI_Wrapper::~CUI_Wrapper()
{
}

CUI_Wrapper* CUI_Wrapper::Create(wstring pUIName)
{
    CUI_Wrapper* pWrapper = new CUI_Wrapper;

    pWrapper->m_wstrUIName = pUIName;

    if (FAILED(pWrapper->Initialize_Prototype()))
    {
        Call_MsgBox(TEXT("Create Failed Wrappper"));
        SAFE_DELETE(pWrapper);
    }

    return pWrapper;
}

void CUI_Wrapper::Set_ShaderResources(CShader* pShaderCom, const char* pConstantName)
{

}

HRESULT CUI_Wrapper::Initialize_Prototype()
{
    //받아놓은 이름으로 UI 불러오는 코두

    m_pUI;

    return S_OK;
}

HRESULT CUI_Wrapper::Initialize()
{
    if (!m_pUI)
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Wrapper::Start()
{
    if (!m_pUI)
        return E_FAIL;

    CREATE_GAMEOBJECT(m_pUI, GROUP_UI);

    __super::Start();

    //m_pUI 여기서막 패스도 바꾸고

    //쉐이더리소스 바인딩도해주고


    return S_OK;
}

void CUI_Wrapper::My_Tick()
{
    __super::My_Tick();

    //즐겨
    //GET_COMPONENT_FROM(m_pUI, CTexture);
}

void CUI_Wrapper::My_LateTick()
{
    __super::My_LateTick();
    //놀아
}

void CUI_Wrapper::OnEnable()
{
    __super::OnEnable();

    ENABLE_GAMEOBJECT(m_pUI);
}

void CUI_Wrapper::OnDisable()
{
    __super::OnDisable();

    DISABLE_GAMEOBJECT(m_pUI);
}
