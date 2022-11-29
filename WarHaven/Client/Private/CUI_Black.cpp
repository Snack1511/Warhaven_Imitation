#include "CUI_Black.h"

CUI_Black::CUI_Black()
{
}

CUI_Black::~CUI_Black()
{
}

HRESULT CUI_Black::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    SetTexture(TEXT("../Bin/Resources/Textures/Black.png"));

    Set_Pos(0.f, 0.f);
    Set_Scale(1280.f, 720.f);

    return S_OK;
}

HRESULT CUI_Black::Initialize()
{
    return S_OK;
}

HRESULT CUI_Black::Start()
{
    __super::Start();

    return S_OK;
}
