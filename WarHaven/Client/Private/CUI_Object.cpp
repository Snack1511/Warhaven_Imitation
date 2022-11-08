#include "CUI_Object.h"

CUI_Object::CUI_Object()
{
}

CUI_Object::CUI_Object(const CUI_Object& Prototype)
	: CUI(Prototype)
{
}

CUI_Object::~CUI_Object()
{
}

HRESULT CUI_Object::Initialize_Prototype()
{
	SetTexture(TEXT("../Bin/Resources/Textures/UI/Default.png"));

	return S_OK;
}

HRESULT CUI_Object::Initialize()
{
	return S_OK;
}

HRESULT CUI_Object::Start()
{
	__super::Start();

	return S_OK;
}
