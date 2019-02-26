#pragma once
#include "HXCommon.h"
#include "HXVector.h"

namespace HX3D
{
	struct HXRenderList;
	class HXISkeleton;
	struct HXAnimationInstance;

	struct HXVertex
	{
		//HXVertex() {}
		//HXVertex(const HXVector3D& pos_, const HXColor& color_): pos(pos_),color(color_) {}
		HXVector3D pos;
		int ctrlPointIndex;	// ������������ʱ���Ƶ�ӳ�䵽������
		HXVector3D normal;
		HXVector3D tangent;
		HXColor	color;
		float u;
		float v;

		HXVertex interp(const HXVertex& rhs, float factor)
		{
			HXVertex vertex;
			//vertex.pos.x = pos.x * (1 - factor) + rhs.pos.x * factor;
			vertex.pos.x = pos.x + (rhs.pos.x - pos.x)*factor;
			vertex.pos.y = pos.y + (rhs.pos.y - pos.y)*factor;
			vertex.pos.z = pos.z + (rhs.pos.z - pos.z)*factor;

			vertex.color.r = color.r + (rhs.color.r - color.r)*factor;
			vertex.color.g = color.g + (rhs.color.g - color.g)*factor;
			vertex.color.b = color.b + (rhs.color.b - color.b)*factor;
			vertex.color.a = color.a + (rhs.color.a - color.a)*factor;

			vertex.u = u + (rhs.u - u)*factor;
			vertex.v = v + (rhs.v - v)*factor;

			return vertex;
		}

		inline HXVertex& operator+=(const HXVertex& rhs)
		{
			pos += rhs.pos;
			color += rhs.color;
			u += rhs.u;
			v += rhs.v;
			return *this;
		}

		inline HXVertex operator-(const HXVertex& rhs) const
		{
			HXVertex vertex;
			vertex.pos = pos - rhs.pos;
			vertex.color = color - rhs.color;
			vertex.u = u - rhs.u;
			vertex.v = v - rhs.v;
			return vertex;
		}

		inline HXVertex operator/(float rhs) const
		{
			HXVertex vertex;
			vertex.pos = pos / rhs;
			vertex.color = color / rhs;
			vertex.u = u / rhs;
			vertex.v = v / rhs;
			return vertex;
		}
	};

	struct HXScanline 
	{
		HXScanline(HXVertex v_, const HXVertex& step_, int x_, int y_, float width_)
			:v(v_), step(step_), x(x_), y(y_), width(width_){}
		HXVertex v;		// ��ǰ��
		HXVertex step;	// ��֮��Ĳ���
		int x;	// ��ʼX
		int y;	// ɨ����Y
		float width;	// ɨ���߿��
	};

	class HXRenderable;
	class HXRenderSystem;
	class HXSubMesh
	{
	public:
		HXSubMesh();
		~HXSubMesh();

		HXSubMesh* Clone(HXRenderSystem* pRenderSystem);

		/*void Insert_To_RenderList(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList);*/
		void Insert_To_RenderList(HXMatrix44& mat, HXRenderList* pRenderList);

		bool useIndex;
		bool IsStaticMesh;
		bool IsCastShadow;
		ELayer layer;

		std::vector<HXVertex>	vertexList;
		std::vector<int>	indexList;

		std::string materialName;

		int triangleCount;

		HXRenderable* renderable;
		

	private:
		//void Insert_To_RenderList_Without_Index(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList);
		void Insert_To_RenderList_Without_Index(HXMatrix44& mat, HXRenderList* pRenderList);
		//void Insert_To_RenderList_With_Index(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList);
		void Insert_To_RenderList_With_Index(HXMatrix44& mat ,HXRenderList* pRenderList);
	};

	class HXMesh
	{
	public:
		HXMesh();
		virtual ~HXMesh();

		void PlayDefaultAnimation();
		void PlayAnimation(std::string strAnimName, int nSpeed = 1);
		void UpdateAnimation();

		virtual HXMesh* Clone(HXRenderSystem* pRenderSystem);

		// For Test
		void CreateCubeForTest();
		void CreateTriangleForTest();
		void CreateQuadForTest();

		//void CreateSkyBox();

		//void Insert_To_RenderList(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList);

		void SetMeshNotStatic();

		std::vector<HXSubMesh*> subMeshList;

		int triangleCount;
		int vertexCount;
		HXISkeleton* skeleton;
		HXAnimationInstance* animInst;	// ֻ��ʵ������������MESH�Ż�new

		// �µĳ�Ա����cloneʱҪ��ֵ
	};
}


