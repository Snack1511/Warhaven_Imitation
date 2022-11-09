#include "stdafx.h"
#include "CUI_Wrapper.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Renderer.h"
#include "CFader.h"
#include "CUI_Object.h"

#include "Texture.h"
#include "CShader.h"

#include "Functor.h"
#include "CUtility_File.h"

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

HRESULT CUI_Wrapper::Initialize_Prototype()
{
    Load_UI(m_wstrName);

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
    //GET_COMPONENT_FROM(m_m_pUI, CTexture);
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

void CUI_Wrapper::Load_UI(wstring m_wstrName)
{
    string savePath = "../Bin/Data/UIData/";
    savePath += CFunctor::To_String(m_wstrName);
    savePath += ".bin";

    ifstream	readFile(savePath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"UI Save Failed");
        return;
    }

    m_pUI = CUI_Object::Create();

    string strName = CUtility_File::Read_Text(&readFile);
    m_pUI->Set_Name(CFunctor::To_Wstring(strName));

    _float4 vPos;
    readFile.read((char*)&vPos, sizeof(_float4));
    m_pUI->Set_Pos(vPos.x, vPos.y);

    _float4 vScale;
    readFile.read((char*)&vScale, sizeof(_float4));
    m_pUI->Set_Scale(vScale.x, vScale.y);

    _bool bTarget = false;
    readFile.read((char*)&bTarget, sizeof(_bool));
    m_pUI->Set_MouseTarget(bTarget);

    _bool bMulti = false;
    readFile.read((char*)&bMulti, sizeof(_bool));
    m_pUI->Set_MultiTexture(bMulti);

    _float4 vColor;
    readFile.read((char*)&vColor, sizeof(_float4));
    m_pUI->Set_Color(vColor);

    _uint iMaxSize = 0;
    readFile.read((char*)&iMaxSize, sizeof(_uint));

    for (_uint i = 0; i < iMaxSize; ++i)
    {
        string strPath = CUtility_File::Read_Text(&readFile);
        m_pUI->Set_Texture(CFunctor::To_Wstring(strPath).c_str());
    }

    readFile.close();
}
