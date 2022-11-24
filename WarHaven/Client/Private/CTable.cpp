#include "stdafx.h"
#include "CTable.h"

CTable::CTable()
{
}

CTable::~CTable()
{
	Release();
}

HRESULT CTable::Initialize()
{
	return S_OK;
}

void CTable::Release()
{
}
