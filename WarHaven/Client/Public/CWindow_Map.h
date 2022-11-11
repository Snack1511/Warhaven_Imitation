#pragma once
#define HASHING(Type, Value) hash<Type>()(Value)
#include "CImGui_Window.h"
BEGIN(Engine)
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CTerrain;
class CDrawable_Terrain;
class CWindow_Map final
	: public CImGui_Window
{
public:
	enum TUPLEDATA { Tuple_CharPtr, Tuple_Bool, Tuple_Index };
	enum CONTROLTYPE { CONTROL_SCALING, CONTROL_ROTATE, CONTROL_MOVE };
	typedef struct MAPDATA
	{
		wstring TerrainDataPath;
		wstring ObjectDataPath;
		wstring NavDataPath;
		wstring LightDataPath;
	public:
		void Initialize();
		void Make_Path(string BasePath, string DataName);
		HRESULT SaveData(ofstream& rhsWriteFile, string BasePath, string DataName);
		HRESULT LoadData(ifstream& rhsReadFile, string FilePath);
		HRESULT SavePath(ofstream& rhsWriteFile, wstring strPath);
		HRESULT LoadPath(ifstream& rhsReadFile, wstring& strPath);
	};



	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

	typedef struct tagMapToolTerrainData
	{
		wstring strTileTexturePath;
		_uint iNumVerticesX;
		_uint iNumVerticesZ;
		_float3* pCurTerrainVertPos;
	public:
		typedef tuple<wstring, _uint, _uint, _float3*> Terrain_TUPLE;
		enum TupleType
		{
			Tuple_TileTexture,
			Tuple_VerticesX,
			Tuple_VerticesZ,
			Tuple_TerrainPosPtr,
		};
	public:
		void Initialize();
		void Make_Data(Terrain_TUPLE& tTerrainData);
	}MTT_DATA;

	typedef struct tagMapToolObjectData
	{
		wstring strMeshName;
		//wstring strGroupName;
		wstring strMeshPath;
		_float4x4 ObjectStateMatrix;
		_float4 vScale;
		_byte byteLightFlag;

	public:
		void Initialize();
	}MTO_DATA;

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
	// CImGui_Window��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
protected:

protected:

private:
	typedef vector<tuple<char*, bool>> DataComboArr;
	typedef tuple<char*, bool> SelectComboData;
	typedef vector<CGameObject*> OBJVECTOR;
	typedef vector<MTO_DATA> DATAVECTOR;
	typedef map<size_t, vector<CGameObject*>> OBJGROUPING;
	typedef map<size_t, vector<MTO_DATA>> DATAGROUPING;
private:
#pragma region Private ������Ʈ��
	void Func_FileControl();
	void Ready_FileArray();
	void Ready_ObjectGroupID();

	void Update_FileArray();
	void SetUp_FilePath(string& strFilePath, char* szData, string strExt = "");
	void SetUp_CurPickingGroup();

	void Add_MeshGroup(char* pMeshGroupName);
	void Delete_MeshGroup(char* pMeshGroupName);

	void Add_Object(string MeshGroup, string Meshpath, string MeshName);
	void Add_Object(string MeshGroup, MTO_DATA& tData);
	void Delete_Object(string MeshName, vector<CGameObject*>& ObjList, vector<MTO_DATA>& DataList);

	void Clear_MeshGroup(char* pMeshGroupName);
#pragma endregion


#pragma region Private ����Ÿ��Ʈ��
	void Func_DataControl();
	void SetUp_CurSelectObject();
	void Confirm_Data();
	void Show_ObjectData();
	void Set_ObjectSpeed();

	void Select_DataControlFlag();
	void Object_Control();

	void Object_Scale();
	void Object_Rotate();
	void Object_Position();
	_bool Object_Place();

	void Update_Data();
#pragma endregion


#pragma region Private ����Ʈ��Ʈ��
	void Func_LightControl();

	void Ready_LightGroup();
	void Ready_LightType();

	void Add_LightGroupList(char* pLightGroupName);
	void Delete_LightGroupList(char* pLightGroupName);

	void Add_Light(char* pLightName);
	void Delete_Light(char* pLightName);
#pragma endregion

#pragma region Priavate ���� ����� �Լ�
	void Save_MapData(string BasePath, string SaveName);
	void Load_MapData(string FilePath);

	void Save_TerrainData(string BasePath, string SaveName);
	void Load_TerrainData(string FilePath);

	void Save_ObjectGroup(string BasePath, string SaveName);
	void Load_ObjectGroup(string FilePath);

	void Save_NavGroup(string BasePath, string SaveName);
	void Load_NavGroup(string FilePath);

	void Save_LightGroup(string BasePath, string SaveName);
	void Load_LightGroup(string FilePath);
#pragma endregion

private:
	HRESULT Disable_DefaultTerrain();
	void Generate_Terrain();

#pragma region Private ��Ÿ �Լ�
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Map&)> func);
	_bool		Make_Combo(const char* szLabel, vector<tuple<char*, bool>>& szDataArr, int* pCurIndex, function<void()> SelectFunction);
	void		Clear_TupleData(vector<tuple<char*, bool>>& ArrData);
	void		DebugData(const char* szTitleName, string& strData);
	list<string>Read_Folder_ToStringList(const char* pFolderPath);
	void		Read_Folder_ForTree(const char* pFolderPath, TREE_DATA& tRootTree);
	void		Show_TreeData(TREE_DATA& tTree);
	string		CutOut_Ext(string& Origin, string& Ext);
	void		EmptyFunction() {}
#pragma endregion

private:
#pragma region Value ������Ʈ��
	vector<tuple<char*, bool>> m_arrSaveFilesCombo;
	string m_strPath;

	_int m_SelectObjectGroupIDIndex = 0;
	_int m_SelectMeshGroupIndex = 0;
	string m_strCurSelectObjectName = "Test....";
	_int m_iCurSelectObjectIndex = 0;
	vector<tuple<char*, bool>> m_arrObjectGroupId;
	vector<tuple<char*, bool>> m_arrMeshGroupName;
	map<size_t, vector<CGameObject*>> m_ObjectGroupMap;
	map<size_t, vector<MTO_DATA>> m_ObjectDataGroupMap;
	map<size_t, _int> m_ObjNameCallStack;
#pragma endregion
	CTerrain* m_pDefaultTerrain = nullptr;
	CDrawable_Terrain* m_pCurTerrain = nullptr;
	MTT_DATA m_CurTerrainData;
	vector<tuple<char*, bool>> m_arrTileTextureName;

#pragma region Value ������Ʈ ��Ʈ��
	CGameObject* m_pCurSelectGameObject = nullptr;
	CTransform* m_pObjTransform = nullptr;
	MTO_DATA* m_pCurSelectData;
	CONTROLTYPE m_eControlType = CONTROL_MOVE;

	_float m_fTickPerScalingSpeed = 1.f;
	_float m_fTickPerRotSpeed = 1.f;
	_float m_fTickPerMoveSpeed = 1.f;
#pragma endregion

#pragma region Value ����Ʈ��Ʈ��
	vector<tuple<char*, bool>> m_arrLightGroupCombo;
	vector<tuple<char*, bool>> m_arrLightTypeCombo;
#pragma endregion

	TREE_DATA			m_MeshRootNode;
	string				m_CurSelectedMeshFilePath;
	string				m_CurSelectedMeshName;
	vector<string>		m_vecSelectedMeshFilePath;
	vector<string>		m_vecSelectedMeshName;

	list<CGameObject*>* m_pCurObjectList = nullptr;

	_bool m_bPickable = false;

};
END
