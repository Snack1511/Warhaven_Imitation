#include "stdafx.h"
#include "CPath.h"

#include "UsefulHeaders.h"
#include "CAIController.h"
CPath::CPath()
{
}

CPath::~CPath()
{
}

CPath* CPath::Create(string strKey)
{
    CPath* pInstance = new CPath;

    pInstance->m_strName = strKey;

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CPath");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CPath::Initialize()
{
    /* Load Path */
    string    strPath = "../bin/data/gamesystem/pathes/";
    strPath += m_strName;
    strPath += ".bin";

    ifstream	readFile(strPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load ���� ??!?! : CPath");
        return E_FAIL;
    }

    readFile.read((char*)&m_iNumPositions, sizeof(_uint));

    for (_uint i = 0; i < m_iNumPositions; ++i)
    {
        _float4 vPosition;
        readFile.read((char*)&vPosition, sizeof(_float4));
        m_vecPositions.push_back(vPosition);

    }

    readFile.close();

    m_iPrevIndex = m_iNumPositions - 1;

    return S_OK;
}

void CPath::Update_CurrentIndex(_float4 vCurrentPos)
{
    if (m_iCurIndex >= (m_iNumPositions))
    {
        m_iCurIndex = m_iNumPositions - 1;
        return;
    }

    _float4 vTargetPos = m_vecPositions[m_iCurIndex];
    
    _float fLength = (vCurrentPos - vTargetPos).Length();
    _float fCurSpeed = 0.1f;

    if (m_pOwnerController) 
    {
        fCurSpeed = GET_COMPONENT_FROM(m_pOwnerController->Get_OwnerPlayer()->Get_CurrentUnit(), CPhysics)->Get_Physics().fSpeed * fDT(0);
    }
    
    m_iPrevIndex = m_iCurIndex;

    if (fLength < fCurSpeed + fDT(0))
        m_iCurIndex++;
}

void CPath::Release()
{
}

_float4 CPath::Get_CurDir(_float4 vCurrentPos)
{
    if (m_iCurIndex == m_iNumPositions)
        return ZERO_VECTOR;

    _float4 vDir = m_vecPositions[m_iCurIndex] - vCurrentPos;
    vDir.y = 0.f;

    return vDir.Normalize();
}

_float4 CPath::Get_LatestPosition()
{
    return m_vecPositions[m_iCurIndex];
}

_float4 CPath::Get_FrontPosition()
{
    if (m_vecPositions.empty())
    {
        _uint size = _uint(m_vecPositions.size());
        assert(size);
        return ZERO_VECTOR;
    }
    return m_vecPositions.front();
}

void CPath::Init_Indices()
{
    m_iPrevIndex = m_iNumPositions - 1;
    m_iCurIndex = 0;
}

string CPath::Get_PathName()
{
    
    return m_strName;
}

void CPath::Save_CurPath()
{
    m_iNumPositions = m_vecPositions.size();
    /* Save Path */
    string    strPath = "../bin/data/gamesystem/pathes/";
    strPath += m_strName;
    strPath += ".bin";

    ofstream	writeFile(strPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave ���� ??!?!");
        return;
    }

    writeFile.write((char*)&m_iNumPositions, sizeof(_uint));
    for (_uint i = 0; i < m_iNumPositions; ++i)
    {
        writeFile.write((char*)&m_vecPositions[i], sizeof(_float4));
    }

    writeFile.close();
}
