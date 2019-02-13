#pragma once
#include "HXCommon.h"
#include "HXVector.h"

namespace HX3D
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,		// ������
		LIGHT_DIRECTION	= 1,	// ƽ�й�
		LIGHT_POINT = 2,		// ���Դ
		LIGHT_SPOT = 3,			// �۹��
	};

	struct HXVertex;
	struct HXLightInfo;
	class HXLight
	{
	public:
		HXLight();
		HXLight(HXLightInfo* lightInfo);
		~HXLight();
		
		//HXColor VertexLighting(HXVertex* pVertex);

		std::string name;

		bool enable;
		LIGHT_TYPE lightType;
		HXColor color;
		HXVector3D direct;
		float shininess;
		float strength;

		HXVector3D position;
		float constantAttenuation;
		float LinearAttenuation;
		float QuadraticAttenuation;

		HXVector3D ConeDirection;
		float SpotCosCutoff;
		float SpotExponent;
	};
}