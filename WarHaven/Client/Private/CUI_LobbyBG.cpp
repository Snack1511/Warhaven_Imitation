#include "CUI_LobbyBG.h"
#include "CUI_Renderer.h"
CUI_LobbyBG::CUI_LobbyBG()
{
}


CUI_LobbyBG::~CUI_LobbyBG()
{
}

HRESULT CUI_LobbyBG::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG.dds"));    

    GET_COMPONENT(CUI_Renderer)->Set_RenderGroup(RENDER_PRIORITY);
    GET_COMPONENT(CUI_Renderer)->Set_Ortho(true);
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
