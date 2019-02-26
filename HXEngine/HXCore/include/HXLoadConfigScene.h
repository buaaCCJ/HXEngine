#pragma once
#include "HXCommon.h"
#include "HXVector.h"
#include "HXFog.h"
#include "HXLight.h"
#include "tinyxml.h"
#include "HXLoadConfigPrefab.h"

namespace HX3D
{
	struct HXCameraInfo
	{
		std::string name;
		bool enable;
		EClearFlag clearFlag;
		HXColor background;
		ECullingMask cullingMask;
		ECameraProjection projection;
		float feild;
		float size;
		float nearZ;
		float farZ;
		float viewportRectX;
		float viewportRectY;
		float viewportRectW;
		float viewportRectH;
		int depth;
		HXVector3D position;
		HXVector3D rotation;
	};
	struct HXFogInfo
	{
		bool bUse;
		HXFogType type;
		HXColor color;
		float start;
		float end;
		float density;
		float gradiant;
	};
	struct HXLightInfo
	{
		std::string name;

		bool isEnable;
		LIGHT_TYPE lightType;
		HXColor lightColor;
		HXVector3D lightDir;
		float shininess;	// �߹����̶�ָ��
		float strength;		// ���ڵ�������ȵ�����

		HXVector3D lightPos;
		float constantAttenuation;	// ˥��ϵ��
		float LinearAttenuation;
		float QuadraticAttenuation;

		HXVector3D ConeDirection;	// �۹��
		float SpotCosCutoff;
		float SpotExponent;
	};
	struct HXSceneInfo
	{
		HXSceneInfo() {};
		~HXSceneInfo()
		{
			// ��������������ݹ�ɾ��
			for (std::vector<HXGameObjectInfo*>::iterator itr = vctGameObjInfo.begin(); itr != vctGameObjInfo.end(); ++itr)
			{
				delete (*itr);
			}
		}
		// TODO: Add other etc.
		std::vector<HXGameObjectInfo*> vctGameObjInfo;
		HXCameraInfo cameraInfo;
		HXFogInfo fogInfo;
		HXColor ambient;
		std::vector<HXLightInfo> vctLight;
		std::vector<HXCameraInfo> vctCamera;
	};

	class HXLoadConfigScene
	{
	public:
		HXLoadConfigScene();
		~HXLoadConfigScene();

		bool ReadGameObjectRecurve(TiXmlElement* gameobjElement, std::vector<HXGameObjectInfo*>& list);
		bool LoadFile(std::string strFileName);

		HXSceneInfo mSceneInfo;
	};
}