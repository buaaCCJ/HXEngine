#include "..\include\HXSceneManager.h"
#include "HXMesh.h"
#include "HXGameObject.h"
#include "HXCore.h"
#include "HXFrustum.h"
#include "HXResourceManager.h"
#include "HXRenderState.h"
#include "HXStatus.h"
#include "HXRoot.h"
#include "HXRenderSystem.h"
#include "HXRenderable.h"
#include "HXICamera.h"
#include "HXLoadConfigModel.h"
#include "HXLoadConfigMat.h"
#include <algorithm>
#include "HXFog.h"
#include "HXLoadConfigScene.h"
#include "HXLoadConfigPrefab.h"
#include "HXMaterial.h"
#include "HXGLERMap.h"
#include "HXGLCubeMap.h"
#include "HXGLConvolutionCubeMap.h"

namespace HX3D
{
	HXSceneManager* HXSceneManager::m_pInstance = NULL;

	HXSceneManager::HXSceneManager():/*mainCamera(NULL), */fog(NULL)
	{
		ambient = HXColor(0,0,0,255);
		gameObjectTreeRoot = new HXGameObject(NULL, HXRoot::GetInstance()->GetRenderSystem());
		gameObjectTreeRoot->SetName("_GameObjectTreeRoot");
		gameObjectTreeRoot->GetTransform()->CaculateModelMatrix();
	}

	HXSceneManager::~HXSceneManager()
	{
		// ��HXGameObject���������н����˵ݹ�ж��
		delete gameObjectTreeRoot;
		gameObjectTreeRoot = NULL;
		
		opaqueMap.clear();
		transparentMap.clear();

		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); itr++)
		{
			delete *itr;
		}
		lightVct.clear();

		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); itr++)
		{
			delete *itr;
		}
		cameraVct.clear();

		/*if (mainCamera)
		{
			delete mainCamera;
		}*/

		if (fog)
		{
			delete fog;
		}
	}

	void HXSceneManager::LoadScene(std::string strSceneCfgFile)
	{
		// GDI��������ԭ��ʹ�ó����򵥰汾
		if (HXRoot::GetInstance()->GetRenderSystem() && 
			HXRoot::GetInstance()->GetRenderSystem()->GetRenderSystemType() == RenderSystem_GDI)
		{
			//strSceneCfgFile.insert(strSceneCfgFile.length() - 6, "_GDI");
			strSceneCfgFile += ".gdi";
		}

		HXLoadConfigScene cfg;
		cfg.LoadFile(strSceneCfgFile);
		// camera
		// CreateMainCamera(&cfg.mSceneInfo.cameraInfo);
		// fog
		CreateFog(&cfg.mSceneInfo.fogInfo);
		// ambient
		ambient = cfg.mSceneInfo.ambient;
		// camera
		for (std::vector<HXCameraInfo>::iterator itr = cfg.mSceneInfo.vctCamera.begin(); itr != cfg.mSceneInfo.vctCamera.end(); ++itr)
		{
			HXCameraInfo& info = *itr;
			CreateCamera(&info);
		}
		SortCameraList();
		// light
		for (std::vector<HXLightInfo>::iterator itr = cfg.mSceneInfo.vctLight.begin(); itr != cfg.mSceneInfo.vctLight.end(); ++itr)
		{
			HXLightInfo& info = *itr;
			CreateLight(&info);
		}

		// GameObject
		CreateGameObjectRecurve(cfg.mSceneInfo.vctGameObjInfo, gameObjectTreeRoot);
	}

	void HXSceneManager::UnLoadScene()
	{
		/*if (mainCamera)
		{
			delete mainCamera;
			mainCamera = NULL;
		}*/
		if (fog)
		{
			delete fog;
			fog = NULL;
		}
		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); ++itr)
		{
			delete *itr;
		}
		cameraVct.clear();
		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); ++itr)
		{
			delete *itr;
		}
		lightVct.clear();
		// ��HXGameObject���������н����˵ݹ�ж��
		/*for (std::vector<HXGameObject*>::iterator itr = gameObjectTreeRoot->GetChildren().begin(); itr != gameObjectTreeRoot->GetChildren().end(); ++itr)
		{
			delete *itr;
		}
		gameObjectTreeRoot->GetChildren().clear();*/
		// ��HXGameObject���������н����˵ݹ�ж��
		for (std::vector<HXITransform*>::iterator itr = gameObjectTreeRoot->GetTransform()->GetChildren().begin(); itr != gameObjectTreeRoot->GetTransform()->GetChildren().end(); ++itr)
		{
			delete (*itr)->gameObject;
		}
		gameObjectTreeRoot->GetTransform()->GetChildren().clear();

		opaqueMap.clear();
		transparentMap.clear();
	}

	void HXSceneManager::CreateGameObjectRecurve(std::vector<HXGameObjectInfo*>& list, HXGameObject* father)
	{
		for (std::vector<HXGameObjectInfo*>::iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			HXGameObjectInfo* gameobjectinfo = *itr;
			HXGameObject* gameobject = CreateGameObject(father, gameobjectinfo);
			if (NULL == gameobject)
			{
				continue;
			}
			CreateGameObjectRecurve(gameobjectinfo->children, gameobject);
		}
	}

	HXGameObject* HXSceneManager::CreateGameObject(HXGameObject* pFather, HXGameObjectInfo* gameobjectinfo)
	{
		HXGameObject* gameObject = new HXGameObject(pFather, HXRoot::GetInstance()->GetRenderSystem());
		gameObject->Initialize(gameobjectinfo);
		InsertGameObjectToOrderQueueRecurve(gameObject);
		return gameObject;
	}

	HXGameObject* HXSceneManager::GetGameObjectTreeRoot()
	{
		return gameObjectTreeRoot;
	}

	void HXSceneManager::InsertGameObjectToOrderQueueRecurve(HXGameObject* gameobject)
	{
		if (!gameobject->GetActivity())
		{
			return;
		}

		HXMesh* pMesh = gameobject->GetMesh();
		if (pMesh)
		{
			for (std::vector<HXSubMesh*>::iterator itr = pMesh->subMeshList.begin(); itr != pMesh->subMeshList.end(); itr++)
			{
				HXSubMesh* subMesh = (*itr);
				int renderQueue = subMesh->renderable->m_pMaterial->mMatInfo->nRenderQueue;
				if (renderQueue < ERenderQueue::RQ_TRANSPARENT)
				{
					// opaque
					std::string materialName = subMesh->materialName;
					std::map<int, mapStringVector>::iterator itr1 = opaqueMap.find(renderQueue);
					if (itr1 != opaqueMap.end())
					{
						mapStringVector::iterator itr2 = itr1->second.find(materialName);
						if (itr2 != itr1->second.end())
						{
							itr2->second.push_back(subMesh->renderable);
						}
						else
						{
							vectorRenderable vct;
							vct.push_back(subMesh->renderable);
							itr1->second.insert(std::make_pair(materialName, vct));
						}
					}
					else
					{
						mapStringVector sv;
						vectorRenderable vct;
						vct.push_back(subMesh->renderable);
						sv.insert(std::make_pair(materialName, vct));
						opaqueMap.insert(std::make_pair(renderQueue, sv));
					}
					subMesh->renderable->m_pTransform = gameobject->GetTransform();
				}
				else
				{
					// transparent
					std::map<int, vectorRenderable>::iterator itr1 = transparentMap.find(renderQueue);
					if (itr1 != transparentMap.end())
					{
						itr1->second.push_back(subMesh->renderable);
					}
					else
					{
						vectorRenderable vct;
						vct.push_back(subMesh->renderable);
						transparentMap.insert(std::make_pair(renderQueue, vct));
					}
					subMesh->renderable->m_pTransform = gameobject->GetTransform();
				}
			}
		}
		
		for (std::vector<HXITransform*>::iterator itr = gameobject->GetTransform()->GetChildren().begin(); itr != gameobject->GetTransform()->GetChildren().end(); ++itr)
		{
			InsertGameObjectToOrderQueueRecurve((*itr)->gameObject);
		}
	}

	HXLight* HXSceneManager::CreateLight(LIGHT_TYPE type)
	{
		HXLight* pLight = new HXLight();
		pLight->enable = true;
		pLight->lightType = type;
		pLight->shininess = 10;
		pLight->strength = 0.01;
		pLight->constantAttenuation = 0.05;
		pLight->LinearAttenuation = 0.05;
		pLight->QuadraticAttenuation = 0.01;
		pLight->SpotCosCutoff = 0;
		pLight->SpotExponent = 10;
		pLight->color = HXColor(255, 255, 255, 255);
		pLight->position = HXVector3D(0,0,0);
		pLight->direct = HXVector3D(1, 1, 1);
		pLight->ConeDirection = HXVector3D(1,1,1);
		if (type == LIGHT_DIRECTION)
		{
			pLight->name = "Directional Light";
		}
		else if (type == LIGHT_POINT)
		{
			pLight->name = "Point Light";
		}
		else if (type == LIGHT_SPOT)
		{
			pLight->name = "Spot Light";
		}
		lightVct.push_back(pLight);
		return pLight;
	}

	bool HXSceneManager::DeleteLight(HXLight* light)
	{
		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); ++itr)
		{
			if ((*itr) == light)
			{
				lightVct.erase(itr);
				delete light;
				light = NULL;
				return true;
			}
		}
		return false;
	}

	HXLight* HXSceneManager::CreateLight(HXLightInfo* lightInfo)
	{
		HXLight* pLight = new HXLight(lightInfo);
		lightVct.push_back(pLight);
		return pLight;
	}

	HXLight* HXSceneManager::GetMainLight()
	{
		for (std::vector<HXLight*>::iterator itr = lightVct.begin(); itr != lightVct.end(); ++itr)
		{
			if ((*itr)->lightType == LIGHT_DIRECTION && (*itr)->enable)
			{
				return *itr;
			}
		}
		return NULL;
	}

	/*HXICamera* HXSceneManager::CreateMainCamera(const HXCameraInfo* cameraInfo)
	{
		if (HXRoot::GetInstance()->GetRenderSystem())
		{
			mainCamera = HXRoot::GetInstance()->GetRenderSystem()->CreateCamera(cameraInfo);
		}
		return mainCamera;
	}*/

	HXICamera* HXSceneManager::CreateCamera(const HXCameraInfo* cameraInfo)
	{
		if (HXRoot::GetInstance()->GetRenderSystem())
		{
			HXICamera* camera = HXRoot::GetInstance()->GetRenderSystem()->CreateCamera(cameraInfo);
			cameraVct.push_back(camera);
			return camera;
		}
		return NULL;
	}

	HXICamera* HXSceneManager::CreateDefaultCamera()
	{
		HXCameraInfo cameraInfo;
		cameraInfo.name = "Camera";
		cameraInfo.enable = true;
		cameraInfo.clearFlag = CF_SOLIDCOLOR;

		cameraInfo.cullingMask = CM_EVERYTHING;
		cameraInfo.projection = CP_PERSPECTIVE;
		cameraInfo.feild = 60;
		cameraInfo.size = 5;
		cameraInfo.nearZ = 0.01f;
		cameraInfo.farZ = 1000;
		cameraInfo.viewportRectX = 0;
		cameraInfo.viewportRectY = 0;
		cameraInfo.viewportRectW = 1;
		cameraInfo.viewportRectH = 1;
		cameraInfo.depth = -1;

		return CreateCamera(&cameraInfo);
	}

	bool Depthcompare(HXICamera* i, HXICamera* j)
	{
		return i->depth < j->depth;
	}

	void HXSceneManager::SortCameraList()
	{
		std::sort(cameraVct.begin(), cameraVct.end(), Depthcompare);
	}

	HXICamera* HXSceneManager::GetMainCamera()
	{
		if (cameraVct.size() > 0)
		{
			return cameraVct[0];
		}
		else
		{
			return NULL;
		}
	}

	bool HXSceneManager::DeleteCamera(HXICamera* camera)
	{
		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); ++itr)
		{
			if ((*itr) == camera)
			{
				cameraVct.erase(itr);
				delete camera;
				camera = NULL;
				return true;
			}
		}
		return false;
	}

	void HXSceneManager::CreateFog(HXFogInfo* info)
	{
		fog = new HXFog(info);
	}

	void HXSceneManager::UseFog(bool useFog)
	{
		if (fog)
		{
			fog->useFog = useFog;
		}
	}

	void HXSceneManager::PushSortListRecurve(HXGameObject* src, std::vector<HXGameObject*>& dest)
	{
		dest.push_back(src);
		for (std::vector<HXITransform*>::iterator itr = src->GetTransform()->GetChildren().begin(); itr != src->GetTransform()->GetChildren().end(); itr++)
		{
			PushSortListRecurve((*itr)->gameObject, dest);
		}
	}

	bool Zcompare(HXRenderable* i, HXRenderable* j)
	{
		return i->GetZDepth() < j->GetZDepth();
	}

	void HXSceneManager::OnDisplay()
	{
		HXRenderSystem* pRenderSystem = HXRoot::GetInstance()->GetRenderSystem();
		if (NULL == pRenderSystem)
		{
			return;
		}

		HXStatus::GetInstance()->ResetStatus();
		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); ++itr)
		{
			(*itr)->Update();
		}
		gameObjectTreeRoot->Update();

		static HXGLERMap* ermap = NULL;
		static HXGLCubeMap* cubemap = NULL;
		static HXGLConvolutionCubeMap* convolutionCubeMap = NULL;
		if (!ermap)
		{
			ermap = new HXGLERMap();
			ermap->Initialize();
			ermap->PreRender();
			ermap->Render();
			ermap->PostRender();
			ermap->GenerateMipmap();

			convolutionCubeMap = new HXGLConvolutionCubeMap();
			convolutionCubeMap->Initialize();
			convolutionCubeMap->PreRender();
			convolutionCubeMap->Render(ermap->GetCubeMapTexture());
			convolutionCubeMap->PostRender();
			convolutionCubeMap->GenerateMipmap();

			cubemap = new HXGLCubeMap();
			cubemap->Initialize();
		}
		cubemap->Render(ermap->GetCubeMapTexture());
		return;

		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); ++itr)
		{
			HXICamera* curCamera = *itr;
			if (false == curCamera->enable)
			{
				continue;
			}

			/************************************ render shadowmap ****************************************/
			if (curCamera->PreRenderShadowMap())
			{
				// render opaque
				HXMaterial* curMaterial = NULL;
				for (std::map<int, mapStringVector>::iterator itr = opaqueMap.begin(); itr != opaqueMap.end(); ++itr)
				{
					for (mapStringVector::iterator itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1)
					{
						for (vectorRenderable::iterator itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
						{
							HXRenderable* renderable = *itr2;
							if (!renderable->m_pSubMesh->IsCastShadow)
							{
								continue;
							}
							if (IsCulled(renderable->m_pSubMesh->layer, curCamera->cullingMask))
							{
								continue;
							}
							if (curMaterial != renderable->m_pMaterial)
							{
								curMaterial = renderable->m_pMaterial;
								
								curMaterial->SetShadowMapMaterialRenderStateAllRenderable();
							}
							HXStatus::GetInstance()->nVertexCount += renderable->m_pSubMesh->vertexList.size();
							HXStatus::GetInstance()->nTriangleCount += renderable->m_pSubMesh->triangleCount;
							renderable->SetModelMatrix(renderable->m_pTransform->mGlobalModelMatrix);
							//renderable->SetViewMatrix(curCamera);
							//renderable->SetProjectionMatrix(curCamera);
							
							curMaterial->SetShadowMapMaterialRenderStateEachRenderable(renderable);
							
							pRenderSystem->RenderSingle(renderable, true);
						}
					}
				}
				curMaterial = NULL;

				// render transparent
				// ��͸����Ͷ����Ӱ

				curCamera->PostRenderShadowMap();
			}

			/*************************************** render scene ****************************************/

			if (curCamera->PreRender())
			{
				// render opaque
				HXMaterial* curMaterial = NULL;
				for (std::map<int, mapStringVector>::iterator itr = opaqueMap.begin(); itr != opaqueMap.end(); ++itr)
				{
					for (mapStringVector::iterator itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1)
					{
						for (vectorRenderable::iterator itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
						{
							HXRenderable* renderable = *itr2;
							if (IsCulled(renderable->m_pSubMesh->layer, curCamera->cullingMask))
							{
								continue;
							}
							if (curMaterial != renderable->m_pMaterial)
							{
								curMaterial = renderable->m_pMaterial;
								
								curMaterial->SetMaterialRenderStateAllRenderable(curCamera);
							}
							HXStatus::GetInstance()->nVertexCount += renderable->m_pSubMesh->vertexList.size();
							HXStatus::GetInstance()->nTriangleCount += renderable->m_pSubMesh->triangleCount;
							renderable->SetModelMatrix(renderable->m_pTransform->mGlobalModelMatrix);
							renderable->SetViewMatrix(curCamera);
							renderable->SetProjectionMatrix(curCamera);
							
							curMaterial->SetMaterialRenderStateEachRenderable(renderable);
							
							pRenderSystem->RenderSingle(renderable, false);
						}
					}
				}
				curMaterial = NULL;
				// render transparent
				// Z����
				for (std::map<int, vectorRenderable>::iterator itr = transparentMap.begin(); itr != transparentMap.end(); ++itr)
				{
					for (vectorRenderable::iterator itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1)
					{
						HXRenderable* renderable = *itr1;
						renderable->SetModelMatrix(renderable->m_pTransform->mGlobalModelMatrix);
						renderable->SetViewMatrix(curCamera);
						renderable->SetProjectionMatrix(curCamera);
					}
				}
				for (std::map<int, vectorRenderable>::iterator itr = transparentMap.begin(); itr != transparentMap.end(); ++itr)
				{
					std::sort(itr->second.begin(), itr->second.end(), Zcompare);
				}
				
				curMaterial = NULL;
				for (std::map<int, vectorRenderable>::iterator itr = transparentMap.begin(); itr != transparentMap.end(); ++itr)
				{
					for (vectorRenderable::iterator itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1)
					{
						HXRenderable* renderable = *itr1;
						if (IsCulled(renderable->m_pSubMesh->layer, curCamera->cullingMask))
						{
							continue;
						}
						if (curMaterial != renderable->m_pMaterial)
						{
							curMaterial = renderable->m_pMaterial;
							
							curMaterial->SetMaterialRenderStateAllRenderable(curCamera);
						}
						HXStatus::GetInstance()->nVertexCount += renderable->m_pSubMesh->vertexList.size();
						HXStatus::GetInstance()->nTriangleCount += renderable->m_pSubMesh->triangleCount;
						
						curMaterial->SetMaterialRenderStateEachRenderable(renderable);
						
						pRenderSystem->RenderSingle(renderable, false);
					}
				}
				curMaterial = NULL;

				curCamera->PostRender();
			}
		}

		HXStatus::GetInstance()->ShowStatusInfo();
	}

	void HXSceneManager::OnViewPortResize(int nScreenWidth, int nScreenHeight)
	{
		for (std::vector<HXICamera*>::iterator itr = cameraVct.begin(); itr != cameraVct.end(); ++itr)
		{
			(*itr)->OnViewPortResize(nScreenWidth, nScreenHeight);
		}
	}

	HXGameObject* HXSceneManager::CreateGameObjectFromPrefab(HXGameObject* father, std::string strPrefabPath)
	{
		HXLoadConfigPrefab cfgPrefab;
		cfgPrefab.LoadFile(strPrefabPath);
		if (cfgPrefab.vctGameObjInfo.size() > 0)
		{
			// ���ϲ�һ��HXGameObjectInfo�͹���
			HXGameObjectInfo* gameobjectinfo = cfgPrefab.vctGameObjInfo[0];
			if (NULL == gameobjectinfo)
			{
				return NULL;
			}
			HXGameObject* gameobject = CreateGameObject(father, gameobjectinfo);
			if (gameobject)
			{
				CreateGameObjectRecurve(gameobjectinfo->children, gameobject);
			}
			return gameobject;
		}
		return NULL;
	}

	bool HXSceneManager::DeleteGameObject(HXGameObject* gameobject)
	{
		if (DeleteGameObjectRecurve(gameObjectTreeRoot->GetTransform()->GetChildren(), gameobject))
		{
			UpdateRenderableQueue();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool HXSceneManager::DeleteGameObjectRecurve(std::vector<HXITransform*>& list, HXGameObject* gameobject)
	{
		for (std::vector<HXITransform*>::iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			if ((*itr)->gameObject == gameobject)
			{
				list.erase(itr);
				delete gameobject;
				gameobject = NULL;
				return true;
			}
			if (DeleteGameObjectRecurve((*itr)->GetChildren(), gameobject))
			{
				return true;
			}
		}
		return false;
	}

	void HXSceneManager::UpdateRenderableQueue()
	{
		opaqueMap.clear();
		transparentMap.clear();
		for (std::vector<HXITransform*>::iterator itr = gameObjectTreeRoot->GetTransform()->GetChildren().begin(); itr != gameObjectTreeRoot->GetTransform()->GetChildren().end(); ++itr)
		{
			InsertGameObjectToOrderQueueRecurve((*itr)->gameObject);
		}
	}
}