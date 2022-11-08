#pragma once
#define HASHING(Type, Value) hash<Type>()(Value)
#include "CImGui_Window.h"
BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_Map final
	: public CImGui_Window
{
public:
	enum TUPLEDATA { Tuple_CharPtr , Tuple_Bool, Tuple_Index};

	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

	struct MTO_DATA
	{
		wstring strMeshName;
		wstring strGroupName;
		wstring strMeshPath;
		_float4x4 ObjectStateMatrix;

	public:
		void Initialize()
		{
			strMeshName = wstring();
			strGroupName = wstring();
			strMeshPath = wstring();
			ObjectStateMatrix.Identity();
		}
	};

	const char* ArrObjectGroup[GROUP_END] =
	{
		"Static",
		"Default",
		"Enemy",
		"Decoration",
		"Prop",
		"UI",
		"Player",
		"Loading",
		"Effect",
	};
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
	void Add_MeshGroup(char* pMeshGroupName);
	void Delete_MeshGroup(char* pMeshGroupName);
	void Clear_MeshGroup(char* pMeshGroupName);
	void Add_Object(string MeshGroup, string Meshpath, string MeshName);
	void Delete_Object(string MeshName, vector<CGameObject*>& ObjList, list<MTO_DATA>& DataList);

	void SetUp_CurObjectlist();
	void Func_DataControl();
#pragma region Private 라이트컨트롤
	void Func_LightControl();
	void Ready_LightGroup();
	void Ready_LightType();

	void Add_LightGroupList(char* pLightGroupName);
	void Delete_LightGroupList(char* pLightGroupName);

	void Add_Light(char* pLightName);
	void Delete_Light(char* pLightName);
#pragma endregion

#pragma region Private 기타 함수
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
	_bool		Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
	void		Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
	void		DebugData(const char* szTitleName, string& strData);
	void		Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void		Show_TreeData(TREE_DATA& tTree);
	string		CutOut_Ext(string& Origin);
	void		EmptyFunction() {}
#pragma endregion

private:
#pragma region Value 파일컨트롤
	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
	string m_strPath;

	_int m_SelectObjectGroupIDIndex = 0;
	_int m_SelectMeshGroupIndex = 0;
	string m_strCurSelectObjectName = "Test....";
	_int m_iCurSelectObjectIndex = 0;
#pragma endregion

#pragma region Value 오브젝트 컨트롤
	typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
	typedef map<size_t, list<MTO_DATA>> DATAGROUPING;
	vector<tuple<char*, bool>> m_arrObjectGroupId;
	vector<tuple<char*, bool>> m_arrMeshGroupName;
	map<size_t, vector<CGameObject*>> m_ObjectGroupMap;
	map<size_t, list<MTO_DATA>> m_ObjectDataGroupMap;
	map<size_t, _int> m_ObjNameCallStack;
	CGameObject* m_pCurSelectGameObject = nullptr;
#pragma endregion

#pragma region Value 라이트컨트롤
	vector<tuple<char*, bool>> m_arrLightGroupCombo;
	vector<tuple<char*, bool>> m_arrLightTypeCombo;
#pragma endregion

	TREE_DATA			m_MeshRootNode;
	string				m_CurSelectedMeshFilePath;
	string				m_CurSelectedMeshName;
	vector<string>		m_vecSelectedMeshFilePath;
	vector<string>		m_vecSelectedMeshName;

	list<CGameObject*> m_CurObjectList;

};
END
