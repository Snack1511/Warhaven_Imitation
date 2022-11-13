#include "CUI_LobbyBG.h"

CUI_LobbyBG::CUI_LobbyBG()
{
}

CUI_LobbyBG::CUI_LobbyBG(const CUI_LobbyBG& Prototype)
    : CUI(Prototype)
{
}

CUI_LobbyBG::~CUI_LobbyBG()
{
}

HRESULT CUI_LobbyBG::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG.dds"));    

    Set_Pos(0.f, 0.f);
    Set_Scale(1280.f, 720.f);

    Set_Sort(1.f);

    return S_OK;
}

HRESULT CUI_LobbyBG::Initialize()
{
    return S_OK;
}

HRESULT CUI_LobbyBG::Start()
{
    __super::Start();

    return S_OK;
}
