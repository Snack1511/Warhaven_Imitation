#pragma once
#include "CImGui_Window.h"
BEGIN(Client)
class CWindow_Map final
	: public CImGui_Window
{
public:
	enum TUPLEDATA { Tuple_CharPtr , Tuple_Bool, Tuple_Index};
private:
	CWindow_Map();
	virtual ~CWindow_Map();

public:
	static CWindow_Map* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
protected:

protected:
private:
	typedef vector<tuple<char*, bool>> DataComboArr;
	typedef tuple<char*, bool> SelectComboData;
private:
#pragma region Private 파일컨트롤
	void Func_FileControl();
	void Ready_FileArray();
	void Update_FileArray();
	void SetUp_FilePath(string& strFilePath, char* szData);
#pragma endregion

	void Ready_ObjectGroupID();
	void Ready_MeshGroupName();
	void Add_Object();
	void Delete_Object();

#pragma region Private 라이트컨트롤
	void Func_DataControl();
	void Func_LightControl();
	void Ready_LightGroup();
	void Ready_LightType();

	void Add_LightGroupList(char* pLightGroupName);
	void Delete_LightGroupList(char* pLightGroupName);

	void Add_Light(char* pLightName);
	void Delete_Light(char* pLightName);
#pragma endregion

#pragma region Private 기타 함수
	void Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
	_bool Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
	void Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
	void DebugData(const char* szTitleName, string& strData);
	void EmptyFunction() {}
#pragma endregion

private:
#pragma region Value 파일컨트롤
	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
	string m_strPath;
#pragma endregion

#pragma region Value 오브젝트 컨트롤
	vector<tuple<char*, bool>> m_arrObjectGroupId;
	vector<tuple<char*, bool>> m_arrMeshGroupName;
	vector<tuple<char*, bool>> m_arrObjectPrototypeName;
#pragma endregion

#pragma region Value 라이트컨트롤
	vector<tuple<char*, bool>> m_arrLightGroupCombo;
	vector<tuple<char*, bool>> m_arrLightTypeCombo;
#pragma endregion


};
END
