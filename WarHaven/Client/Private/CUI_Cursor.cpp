#include "CUI_Cursor.h"
#include "Texture.h"
#include "Functor.h"
#include "GameInstance.h"
#include "Renderer.h"

CUI_Cursor::CUI_Cursor()
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor& Prototype)
	: CUI(Prototype)
{
}

CUI_Cursor::~CUI_Cursor()
{
}

HRESULT CUI_Cursor::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/UI/Cursors/T_CursorNormal.dds"));

	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Cursors/T_CursorClick.dds"));
	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Cursors/T_CursorOver.dds"));
	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Cursors/T_CursorDisabled.dds"));

	Set_Scale(32.f);

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_ALPHA);

	return S_OK;
}

HRESULT CUI_Cursor::Initialize()
{
	return S_OK;
}

HRESULT CUI_Cursor::Start()
{
	__super::Start();

	ShowCursor(true);

	RECT tScreen;
	GetClientRect(g_hWnd, &tScreen);

	ClientToScreen(g_hWnd, (LPPOINT)&tScreen);
	ClientToScreen(g_hWnd, (LPPOINT)&tScreen.right);

	ClipCursor(&tScreen);

	return S_OK;
}

void CUI_Cursor::My_Tick()
{
	__super::My_Tick();

	CheckInRect();

	_float4 vPos = CFunctor::To_Window(_float4(m_ptMouse.x, m_ptMouse.y, 0.f));
	Set_Pos(vPos.x, -vPos.y);

	if (KEY(LBUTTON, HOLD))
	{
		GET_COMPONENT(CTexture)->Set_CurTextureIndex(1);
	}
	else
	{
		GET_COMPONENT(CTexture)->Set_CurTextureIndex(0);
	}
}