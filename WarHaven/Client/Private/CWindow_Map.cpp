#include "stdafx.h"
#include "CWindow_Map.h"

CWindow_Map::CWindow_Map()
{
}

CWindow_Map::~CWindow_Map()
{
}

CWindow_Map* CWindow_Map::Create()
{
	CWindow_Map* pInstance = new CWindow_Map;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Map");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Map::Initialize()
{
	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Map).name(), ImVec2(400.f, 50.f), ImGuiWindowFlags_AlwaysAutoResize);

	return S_OK;
}

void CWindow_Map::Tick()
{
}

HRESULT CWindow_Map::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::Text("MapTool");

	/*
	���� ���� �޺��ڽ�
	���ϸ� �Է� â
	������Ʈ �׷� �޺��ڽ�
	������Ʈ ���� ����Ʈ
	���õ� ������Ʈ �̸� �ؽ�Ʈ
	���̺� / �ε� ��ư
	*/

	

	__super::End();

	return S_OK;
}

void CWindow_Map::Ready_FileArray(const char* lpFolderPath, vector<tuple<char*, bool>>& szDataArr, bool bClear)
{
    string Path = lpFolderPath;
    Path += "*";
    if (szDataArr.empty())
    {
        WIN32_FIND_DATAA FileData;
        HANDLE hFind = FindFirstFileA(Path.c_str(), &FileData);
        if (INVALID_HANDLE_VALUE == hFind)
        {
            DWORD ERR = GetLastError();
            Call_MsgBox(TEXT("������;;"));
            assert(ERR);

        }
        while (FindNextFileA(hFind, &FileData))
        {
            //. .. �� �ý��������� ���� �ʿ䰡 ����
            if (!strcmp(FileData.cFileName, ".")
                || !strcmp(FileData.cFileName, ".."))
                continue;
            char* pFileName = new char[260];
            memcpy_s(pFileName, sizeof(char) * 260, FileData.cFileName, sizeof(char) * 260);
            szDataArr.push_back(make_tuple(pFileName, false));
        }
        FindClose(hFind);
    }
}

_bool CWindow_Map::Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction)
{
    _bool bReturn = false;
    if (szDataArr.empty())
        return bReturn;
    if (ImGui::BeginCombo(szLabel, get<0>(szDataArr[(*pCurIndex)])))
    {
        for (_uint i = 0; i < szDataArr.size(); ++i)
        {
            if (ImGui::Selectable(get<0>(szDataArr[i]), &get<1>(szDataArr[i])))
            {
                get<1>(szDataArr[(*pCurIndex)]) = false;
                (*pCurIndex) = i;
                SelectFunction();
                bReturn = true;
            }

        }
        ImGui::EndCombo();
    }
    return bReturn;
}
