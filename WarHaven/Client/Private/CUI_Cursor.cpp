#include "CUI_Cursor.h"
#include "Texture.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Renderer.h"
#include "CUser.h"

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

	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_ALPHA);

	return S_OK;
}

HRESULT CUI_Cursor::Initialize()
{
	return S_OK;
}

HRESULT CUI_Cursor::Start()
{
	__super::Start();

	ShowCursor(false);

	CUser::Get_Instance()->Set_Cursor(this);

	return S_OK;
}

void CUI_Cursor::My_Tick()
{
	__super::My_Tick();

	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	_float fFixPosX = 11.f;
	_float fFixPosY = 13.f;

	_float4 vPos = CFunctor::To_Descartes(_float4(m_ptMouse.x + fFixPosX, m_ptMouse.y + fFixPosY, 0.f));

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

void CUI_Cursor::Set_Mouse(MouseType eType)
{
	GET_COMPONENT(CTexture)->Set_CurTextureIndex(eType);
}
