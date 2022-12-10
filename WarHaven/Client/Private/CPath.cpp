#include "stdafx.h"
#include "CPath.h"

#include "UsefulHeaders.h"

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
    string    strPath = "../bin/resources/data/gamesystem/pathes/";
    strPath += m_strName;
    strPath += ".bin";

    ifstream	readFile(strPath, ios::binary);

    if (!readFile.is_open())
    {
        Call_MsgBox(L"Load 실패 ??!?! : CPath");
        return;
    }

    readFile.read((char*)&m_iNumPositions, sizeof(_uint));

    for (_uint i = 0; i < m_iNumPositions; ++i)
    {
        _float4 vPosition;
        readFile.read((char*)&vPosition, sizeof(_float4));
        m_vecPositions.push_back(vPosition);

    }

    readFile.close();

    return S_OK;
}

void CPath::Release()
{
}

void CPath::Save_CurPath()
{
    m_iNumPositions = m_vecPositions.size();
    /* Save Path */
    string    strPath = "../bin/resources/data/gamesystem/pathes/";
    strPath += m_strName;
    strPath += ".bin";

    ofstream	writeFile(strPath, ios::binary);

    if (!writeFile.is_open())
    {
        Call_MsgBox(L"SSave 실패 ??!?!");
        return;
    }

    writeFile.write((char*)&m_iNumPositions, sizeof(_uint));
    for (_uint i = 0; i < m_iNumPositions; ++i)
    {
        writeFile.write((char*)&m_vecPositions[i], sizeof(_float4));
    }

    writeFile.close();
}
