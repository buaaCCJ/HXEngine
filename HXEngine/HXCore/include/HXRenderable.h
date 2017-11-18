#pragma once
#include "HXCommon.h"

namespace HX3D
{
	class HXSubMesh;
	class HXICamera;
	class HXRenderable
	{
	public:
		HXRenderable();
		virtual ~HXRenderable();

		virtual void SetModelMatrix(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale) = 0;
		virtual void SetViewMatrix(HXICamera* pCamera) = 0;
		virtual void SetProjectionMatrix(HXICamera* pCamera) = 0;
		// ���ɾ�����Ⱦϵͳ��Ҫ�����ݣ����� OPENGL �� VBO�ȣ�
		virtual void GenerateArguments(HXSubMesh* pSubMesh);
		virtual void Render() = 0;

		HXSubMesh* m_pSubMesh;
	};
}