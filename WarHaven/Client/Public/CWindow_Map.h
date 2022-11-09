//#pragma once
//#define HASHING(Type, Value) hash<Type>()(Value)
//#include "CImGui_Window.h"
//#include "Functor.h"
//BEGIN(Engine)
//class CGameObject;
//END
//
//BEGIN(Client)
//class CWindow_Map final
//	: public CImGui_Window
//{
//public:
//	enum TUPLEDATA { Tuple_CharPtr , Tuple_Bool, Tuple_Index};
//
//	typedef struct MAPDATA
//	{
//		wstring ObjectDataPath;
//		wstring NavDataPath;
//		wstring LightDataPath;
//		void Initialize()
//		{
//			ObjectDataPath = wstring();
//			NavDataPath = wstring();
//			LightDataPath = wstring();
//		}
//		void Make_Path(string BasePath, string DataName) 
//		{
//			Initialize();
//			ObjectDataPath = CFunctor::To_Wstring(BasePath);
//			ObjectDataPath += TEXT("ObjectData/");
//			ObjectDataPath += CFunctor::To_Wstring(DataName);
//			ObjectDataPath += TEXT(".GroupData");
//
//			NavDataPath = CFunctor::To_Wstring(BasePath);
//			NavDataPath += TEXT("NavData/");
//			NavDataPath += CFunctor::To_Wstring(DataName);
//			NavDataPath += TEXT(".NavData");
//
//			LightDataPath = CFunctor::To_Wstring(BasePath);
//			LightDataPath += TEXT("LightData/");
//			LightDataPath += CFunctor::To_Wstring(DataName);
//			LightDataPath += TEXT(".LightData");
//		}
//		void SaveData(ofstream& rhsWriteFile, string BasePath, string DataName)
//		{
//			Initialize();
//			ObjectDataPath = CFunctor::To_Wstring(BasePath);
//			ObjectDataPath += TEXT("ObjectData/");
//			ObjectDataPath += CFunctor::To_Wstring(DataName);
//			ObjectDataPath += TEXT(".GroupData");
//
//			NavDataPath = CFunctor::To_Wstring(BasePath);
//			NavDataPath += TEXT("NavData/");
//			NavDataPath += CFunctor::To_Wstring(DataName);
//			NavDataPath += TEXT(".NavData");
//
//			LightDataPath = CFunctor::To_Wstring(BasePath);
//			LightDataPath += TEXT("LightData/");
//			LightDataPath += CFunctor::To_Wstring(DataName);
//			LightDataPath += TEXT(".LightData");
//
//			string SaveFullPath = BasePath;
//			SaveFullPath += DataName;
//			SaveFullPath += ".MapData";
//			rhsWriteFile.open(SaveFullPath, ios::binary);
//			if (!rhsWriteFile.is_open())
//			{
//				Call_MsgBox(L"SSave 실패 ??!?!");
//				assert(0);
//			}
//			string strObjectDataPath = CFunctor::To_String(ObjectDataPath);
//			string strNavDataPath = CFunctor::To_String(NavDataPath);
//			string strLightDataPath = CFunctor::To_String(LightDataPath);
//
//			_int ObjDataPathLength = _int(strObjectDataPath.size()) + 1;
//			rhsWriteFile.write((char*)&ObjDataPathLength, sizeof(_int));
//			char* szObjDataPath = new char[ObjDataPathLength];
//			strcpy_s(szObjDataPath, sizeof(char) * ObjDataPathLength, strObjectDataPath.c_str());
//			rhsWriteFile.write(szObjDataPath, sizeof(char) * ObjDataPathLength);
//
//			_int NavPathLength = _int(strNavDataPath.size()) + 1;
//			rhsWriteFile.write((char*)&NavPathLength, sizeof(_int));
//			char* szNavDataPath = new char[NavPathLength];
//			strcpy_s(szNavDataPath, sizeof(char) * NavPathLength, strNavDataPath.c_str());
//			rhsWriteFile.write(szNavDataPath, sizeof(char) * NavPathLength);
//
//			_int LightPathLength = _int(strLightDataPath.size()) + 1;
//			rhsWriteFile.write((char*)&LightPathLength, sizeof(_int));
//			char* szLightDataPath = new char[LightPathLength];
//			strcpy_s(szLightDataPath, sizeof(char) * LightPathLength, strLightDataPath.c_str());
//			rhsWriteFile.write(szLightDataPath, sizeof(char) * LightPathLength);
//
//			Safe_Delete_Array(szObjDataPath);
//			Safe_Delete_Array(szNavDataPath);
//			Safe_Delete_Array(szLightDataPath);
//
//			rhsWriteFile.close();
//		}
//		void LoadData(ifstream& rhhReadFile, string FilePath)
//		{
//			Initialize();
//
//			string LoadFullPath = FilePath;
//			rhhReadFile.open(LoadFullPath, ios::binary);
//			if (!rhhReadFile.is_open())
//			{
//				Call_MsgBox(L"Load 실패 ??!?!");
//				assert(0);
//			}
//
//			_int ObjPathLength = 0;
//			rhhReadFile.read((char*)&ObjPathLength, sizeof(_int));
//			char* szObjDataPath = new char[ObjPathLength];
//			rhhReadFile.read(szObjDataPath, sizeof(char)* ObjPathLength);
//
//			_int NavPathLength = 0;
//			rhhReadFile.read((char*)&NavPathLength, sizeof(_int));
//			char* szNavDataPath = new char[NavPathLength];
//			rhhReadFile.read(szNavDataPath, sizeof(char) * NavPathLength);
//
//			_int LightPathLength = 0;
//			rhhReadFile.read((char*)&LightPathLength, sizeof(_int));
//			char* szLightDataPath = new char[LightPathLength];
//			rhhReadFile.read(szLightDataPath, sizeof(char) * LightPathLength);
//			rhhReadFile.close();
//
//			string strObjPath = szObjDataPath;
//			string strNavPath = szNavDataPath;
//			string strLightPath = szLightDataPath;
//
//			ObjectDataPath = CFunctor::To_Wstring(strObjPath);
//			NavDataPath = CFunctor::To_Wstring(strNavPath);
//			LightDataPath = CFunctor::To_Wstring(strLightPath);
//
//			Safe_Delete_Array(szObjDataPath);
//			Safe_Delete_Array(szNavDataPath);
//			Safe_Delete_Array(szLightDataPath);
//		}
//	};
//
//
//
//	struct TREE_DATA
//	{
//		string strFullPath;
//		string strFileName;
//		string strFolderPath;
//		vector<TREE_DATA>	vecChildren;
//	};
//
//	struct MTO_DATA
//	{
//		wstring strMeshName;
//		//wstring strGroupName;
//		wstring strMeshPath;
//		_float4x4 ObjectStateMatrix;
//		_byte byteLightFlag;
//
//	public:
//		void Initialize()
//		{
//			strMeshName = wstring();
//			//strGroupName = wstring();
//			strMeshPath = wstring();
//			ObjectStateMatrix.Identity();
//			byteLightFlag = 0;
//		}
//
//	};
//
//	const char* ArrObjectGroup[GROUP_END] =
//	{
//		"Static",
//		"Default",
//		"Enemy",
//		"Decoration",
//		"Prop",
//		"UI",
//		"Player",
//		"Loading",
//		"Effect",
//	};
//private:
//	CWindow_Map();
//	virtual ~CWindow_Map();
//
//public:
//	static CWindow_Map* Create();
//
//public:
//	// CImGui_Window을(를) 통해 상속됨
//	virtual HRESULT Initialize() override;
//	virtual void Tick() override;
//	virtual HRESULT Render() override;
//protected:
//
//protected:
//
//private:
//	typedef vector<tuple<char*, bool>> DataComboArr;
//	typedef tuple<char*, bool> SelectComboData;
//	typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
//	typedef map<size_t, vector<MTO_DATA>> DATAGROUPING;
//private:
//#pragma region Private 파일컨트롤
//	void Func_FileControl();
//	void Ready_FileArray();
//	void Ready_ObjectGroupID();
//
//	void Update_FileArray();
//	void SetUp_FilePath(string& strFilePath, char* szData);
//	void SetUp_CurPickingGroup();
//
//	void Add_MeshGroup(char* pMeshGroupName);
//	void Delete_MeshGroup(char* pMeshGroupName);
//	
//	void Add_Object(string MeshGroup, string Meshpath, string MeshName);
//	void Add_Object(string MeshGroup, MTO_DATA& tData);
//	void Delete_Object(string MeshName, vector<CGameObject*>& ObjList, vector<MTO_DATA>& DataList);
//	
//	void Clear_MeshGroup(char* pMeshGroupName);
//#pragma endregion
//
//
//	void Func_DataControl();
//
//#pragma region Private 라이트컨트롤
//	void Func_LightControl();
//
//	void Ready_LightGroup();
//	void Ready_LightType();
//
//	void Add_LightGroupList(char* pLightGroupName);
//	void Delete_LightGroupList(char* pLightGroupName);
//
//	void Add_Light(char* pLightName);
//	void Delete_Light(char* pLightName);
//#pragma endregion
//
//#pragma region Priavate 파일 입출력 함수
//	void Save_MapData(string BasePath, string SaveName);
//	void Load_MapData(string FilePath);
//
//	void Save_ObjectGroup(string BasePath, string SaveName);
//	void Load_ObjectGroup(string FilePath);
//
//	void Save_NavGroup(string BasePath, string SaveName);
//	void Load_NavGroup(string FilePath);
//
//	void Save_LightGroup(string BasePath, string SaveName);
//	void Load_LightGroup(string FilePath);
//#pragma endregion
//
//#pragma region Private 기타 함수
//	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
//	_bool		Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
//	void		Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
//	void		DebugData(const char* szTitleName, string& strData);
//	void		Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
//	void		Show_TreeData(TREE_DATA& tTree);
//	string		CutOut_Ext(string& Origin);
//	void		EmptyFunction() {}
//#pragma endregion
//
//private:
//#pragma region Value 파일컨트롤
//	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
//	string m_strPath;
//
//	_int m_SelectObjectGroupIDIndex = 0;
//	_int m_SelectMeshGroupIndex = 0;
//	string m_strCurSelectObjectName = "Test....";
//	_int m_iCurSelectObjectIndex = 0;
//	vector<tuple<char*, bool>> m_arrObjectGroupId;
//	vector<tuple<char*, bool>> m_arrMeshGroupName;
//	map<size_t, vector<CGameObject*>> m_ObjectGroupMap;
//	map<size_t, vector<MTO_DATA>> m_ObjectDataGroupMap;
//	map<size_t, _int> m_ObjNameCallStack;
//#pragma endregion
//
//#pragma region Value 오브젝트 컨트롤
//	CGameObject* m_pCurSelectGameObject = nullptr;
//#pragma endregion
//
//#pragma region Value 라이트컨트롤
//	vector<tuple<char*, bool>> m_arrLightGroupCombo;
//	vector<tuple<char*, bool>> m_arrLightTypeCombo;
//#pragma endregion
//
//	TREE_DATA			m_MeshRootNode;
//	string				m_CurSelectedMeshFilePath;
//	string				m_CurSelectedMeshName;
//	vector<string>		m_vecSelectedMeshFilePath;
//	vector<string>		m_vecSelectedMeshName;
//
//	list<CGameObject*> m_CurObjectList;
//
//};
//END
