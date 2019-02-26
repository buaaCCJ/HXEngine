#pragma once
#include "HXCommon.h"
#include <map>
#include "HXLight.h"
#include "HXFog.h"
#include "HXITransform.h"

namespace HX3D
{
	class HXGameObject;
	class HXLight;
	struct HXRenderList;
	class HXICamera;
	struct HXFogInfo;
	struct HXGameObjectInfo;
	class HXRenderable;
	struct HXCameraInfo;
	class HXSceneManager
	{
	public:
		static HXSceneManager* GetInstance()
		{
			if (NULL == m_pInstance)
			{
				m_pInstance = new HXSceneManager;
			}
			return m_pInstance;
		}

		~HXSceneManager();

		void LoadScene(std::string strSceneCfgFile);
		void UnLoadScene();

		HXGameObject* GetGameObjectTreeRoot();
		void CreateGameObjectRecurve(std::vector<HXGameObjectInfo*>& list, HXGameObject* father);
		HXGameObject* CreateGameObjectFromPrefab(HXGameObject* father, std::string strPrefabPath);
		HXGameObject* CreateGameObject(HXGameObject* pFather, HXGameObjectInfo* gameobjectinfo);
		bool DeleteGameObject(HXGameObject* gameobject);
		bool DeleteGameObjectRecurve(std::vector<HXITransform*>& list, HXGameObject* gameobject);
		void InsertGameObjectToOrderQueueRecurve(HXGameObject* gameobject);

		// ƽ�й⡢���Դ���۹��
		HXLight* CreateLight(LIGHT_TYPE type);
		bool DeleteLight(HXLight* light);
		HXLight* CreateLight(HXLightInfo* lightInfo);
		// ���ڲ�����Ӱ����ƽ�й�
		HXLight* GetMainLight();

		//HXICamera* CreateMainCamera(const HXCameraInfo* cameraInfo);
		HXICamera* CreateCamera(const HXCameraInfo* cameraInfo);
		HXICamera* CreateDefaultCamera();
		void SortCameraList();
		// ������Ⱦ��(depth��С)cameraΪ��camera
		HXICamera* GetMainCamera();
		bool DeleteCamera(HXICamera* camera);

		void CreateFog(HXFogInfo* info);
		void UseFog(bool useFog);

		void PushSortListRecurve(HXGameObject* src, std::vector<HXGameObject*>& dest);
		void OnDisplay();
		void OnViewPortResize(int nScreenWidth, int nScreenHeight);
		
		// �ȽϺ����ܣ������ڱ༭���༭ʱ����
		void UpdateRenderableQueue();

		// ƽ�й⡢���Դ���۹��
		std::vector<HXLight*>		lightVct;
		// �����ⵥ������ ��֤Ψһ��
		HXColor ambient;
		HXFog* fog;
		//HXICamera* mainCamera;
		std::vector<HXICamera*>		cameraVct;

	private:
		static HXSceneManager* m_pInstance;
		HXSceneManager();

		// gameobject tree root
		HXGameObject* gameObjectTreeRoot;

		typedef std::vector<HXRenderable*> vectorRenderable;
		typedef std::map<std::string, vectorRenderable> mapStringVector;
		// opaque
		std::map<int, mapStringVector> opaqueMap;
		// transparent
		std::map<int, vectorRenderable> transparentMap;
	};
}