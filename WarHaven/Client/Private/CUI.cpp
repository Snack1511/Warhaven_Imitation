#include "CUI.h"
#include "CMesh_Rect.h"

BEGIN(Client)

CUI::CUI(const CUI& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CUI::Initialize_Prototype()
{
	m_pMesh = Add_Component(CMesh_Rect);

	return S_OK;
}

HRESULT CUI::Start()
{
	return S_OK;
}

void CUI::Tick()
{
}

void CUI::Late_Tick()
{
}

END