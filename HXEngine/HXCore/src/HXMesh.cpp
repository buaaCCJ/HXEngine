#include "..\include\HXMesh.h"
#include "HXCore.h"
#include "HXMatrix.h"
#include "HXMath.h"
#include "HXISkeleton.h"
#include "HXRenderable.h"
#include "HXRenderSystem.h"

namespace HX3D
{

	//----------------------------------SubMesh-------------------------------------------
	HXSubMesh::HXSubMesh()
		: useIndex(false)
		, IsStaticMesh(true)
		, IsCastShadow(false)
		, layer(L_DEFAULT)
	{

	}
	HXSubMesh::~HXSubMesh()
	{
		if (renderable)
		{
			delete renderable;
		}
	}

	HXSubMesh* HXSubMesh::Clone(HXRenderSystem* pRenderSystem)
	{
		HXSubMesh* pHXSubMesh = new HXSubMesh();
		for (std::vector<HXVertex>::iterator itr = vertexList.begin(); itr != vertexList.end(); ++itr)
		{
			HXVertex vertex(*itr);
			pHXSubMesh->vertexList.push_back(vertex);
		}
		for (std::vector<int>::iterator itr = indexList.begin(); itr != indexList.end(); ++itr)
		{
			int index = *itr;
			pHXSubMesh->indexList.push_back(index);
		}

		pHXSubMesh->useIndex = useIndex;
		pHXSubMesh->IsStaticMesh = IsStaticMesh;
		pHXSubMesh->IsCastShadow = IsCastShadow;
		pHXSubMesh->layer = layer;

		pHXSubMesh->materialName = materialName;
		pHXSubMesh->triangleCount = triangleCount;

		// renderable clone
		if (pRenderSystem)
		{
			pHXSubMesh->renderable = pRenderSystem->GenerateRenderable(pHXSubMesh);
		}

		return pHXSubMesh;
	}

	/*void HXSubMesh::Insert_To_RenderList(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList)
	{
		if (useIndex)
		{
			Insert_To_RenderList_With_Index(pos, eulerDegree, scale, pRenderList);
		}
		else
		{
			Insert_To_RenderList_Without_Index(pos, eulerDegree, scale, pRenderList);
		}
	}*/

	void HXSubMesh::Insert_To_RenderList(HXMatrix44& mat, HXRenderList* pRenderList)
	{
		if (useIndex)
		{
			//Insert_To_RenderList_With_Index(pos, eulerDegree, scale, pRenderList);
			Insert_To_RenderList_With_Index(mat, pRenderList);
		}
		else
		{
			//Insert_To_RenderList_Without_Index(pos, eulerDegree, scale, pRenderList);
			Insert_To_RenderList_Without_Index(mat, pRenderList);
		}
	}

	//void HXSubMesh::Insert_To_RenderList_Without_Index(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList)
	//{
	//	for (std::vector<HXVertex>::iterator itr = vertexList.begin(); itr != vertexList.end(); ++itr)
	//	{
	//		HXTriangle triangle;
	//		triangle.vertexList[0] = *itr;
	//		triangle.vertexList[1] = *++itr;
	//		triangle.vertexList[2] = *++itr;

	//		// ���Ŵ���S
	//		HXMatrix44 matS = GetScaleMatrix44(scale.x, scale.y, scale.z);
	//		// ��ת����Q
	//		HXMatrix44 matX = GetRotateMatrix44X(eulerDegree.x);
	//		HXMatrix44 matY = GetRotateMatrix44Y(eulerDegree.y);
	//		HXMatrix44 matZ = GetRotateMatrix44Z(eulerDegree.z);
	//		// ƽ��һ��Ҫ�����T
	//		HXMatrix44 matT = GetTranslateMatrix44(pos.x, pos.y, pos.z);

	//		// ģ�Ϳռ䵽����ռ�ת�� SQT
	//		// HXMatrix44 mat = matS*matX*matY*matZ*matT;
	//		HXMatrix44 mat = matS*matY*matZ*matX*matT;
	//		triangle.vertexList[0].pos = GetVector3DMulMatrix44(triangle.vertexList[0].pos, mat);
	//		triangle.vertexList[1].pos = GetVector3DMulMatrix44(triangle.vertexList[1].pos, mat);
	//		triangle.vertexList[2].pos = GetVector3DMulMatrix44(triangle.vertexList[2].pos, mat);
	//		// TODO: ��ͬһ���ż��㷨��Ҫ����ת�þ���(����ֻ�з��򣬲���ƽ��)
	//		triangle.vertexList[0].normal = GetVector3DMulMatrix33(triangle.vertexList[0].normal, HXMatrix33(mat));
	//		triangle.vertexList[1].normal = GetVector3DMulMatrix33(triangle.vertexList[1].normal, HXMatrix33(mat));
	//		triangle.vertexList[2].normal = GetVector3DMulMatrix33(triangle.vertexList[2].normal, HXMatrix33(mat));

	//		pRenderList->triangleList.push_back(triangle);
	//	}
	//}

	void HXSubMesh::Insert_To_RenderList_Without_Index(HXMatrix44& mat, HXRenderList* pRenderList)
	{
		for (std::vector<HXVertex>::iterator itr = vertexList.begin(); itr != vertexList.end(); ++itr)
		{
			HXTriangle triangle;
			triangle.vertexList[0] = *itr;
			triangle.vertexList[1] = *++itr;
			triangle.vertexList[2] = *++itr;

			//// ���Ŵ���S
			//HXMatrix44 matS = GetScaleMatrix44(scale.x, scale.y, scale.z);
			//// ��ת����Q
			//HXMatrix44 matX = GetRotateMatrix44X(eulerDegree.x);
			//HXMatrix44 matY = GetRotateMatrix44Y(eulerDegree.y);
			//HXMatrix44 matZ = GetRotateMatrix44Z(eulerDegree.z);
			//// ƽ��һ��Ҫ�����T
			//HXMatrix44 matT = GetTranslateMatrix44(pos.x, pos.y, pos.z);

			//// ģ�Ϳռ䵽����ռ�ת�� SQT
			//// HXMatrix44 mat = matS*matX*matY*matZ*matT;
			//HXMatrix44 mat = matS*matY*matZ*matX*matT;
			triangle.vertexList[0].pos = GetVector3DMulMatrix44(triangle.vertexList[0].pos, mat);
			triangle.vertexList[1].pos = GetVector3DMulMatrix44(triangle.vertexList[1].pos, mat);
			triangle.vertexList[2].pos = GetVector3DMulMatrix44(triangle.vertexList[2].pos, mat);
			// TODO: ��ͬһ���ż��㷨��Ҫ����ת�þ���(����ֻ�з��򣬲���ƽ��)
			triangle.vertexList[0].normal = GetVector3DMulMatrix33(triangle.vertexList[0].normal, HXMatrix33(mat));
			triangle.vertexList[1].normal = GetVector3DMulMatrix33(triangle.vertexList[1].normal, HXMatrix33(mat));
			triangle.vertexList[2].normal = GetVector3DMulMatrix33(triangle.vertexList[2].normal, HXMatrix33(mat));

			triangle.vertexList[0].tangent = GetVector3DMulMatrix33(triangle.vertexList[0].tangent, HXMatrix33(mat));
			triangle.vertexList[1].tangent = GetVector3DMulMatrix33(triangle.vertexList[1].tangent, HXMatrix33(mat));
			triangle.vertexList[2].tangent = GetVector3DMulMatrix33(triangle.vertexList[2].tangent, HXMatrix33(mat));

			pRenderList->triangleList.push_back(triangle);
		}
	}

	//void HXSubMesh::Insert_To_RenderList_With_Index(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList)
	//{
	//	for (std::vector<int>::iterator itr = indexList.begin(); itr != indexList.end(); ++itr)
	//	{
	//		HXTriangle triangle;
	//		triangle.vertexList[0] = vertexList[*itr];
	//		triangle.vertexList[1] = vertexList[*++itr];
	//		triangle.vertexList[2] = vertexList[*++itr];

	//		// ���Ŵ���S
	//		HXMatrix44 matS = GetScaleMatrix44(scale.x, scale.y, scale.z);
	//		// ��ת����Q
	//		HXMatrix44 matX = GetRotateMatrix44X(eulerDegree.x);
	//		HXMatrix44 matY = GetRotateMatrix44Y(eulerDegree.y);
	//		HXMatrix44 matZ = GetRotateMatrix44Z(eulerDegree.z);
	//		// ƽ��һ��Ҫ�����T
	//		HXMatrix44 matT = GetTranslateMatrix44(pos.x, pos.y, pos.z);

	//		// ģ�Ϳռ䵽����ռ�ת�� SQT
	//		HXMatrix44 mat = matS*matX*matY*matZ*matT;
	//		triangle.vertexList[0].pos = GetVector3DMulMatrix44(triangle.vertexList[0].pos, mat);
	//		triangle.vertexList[1].pos = GetVector3DMulMatrix44(triangle.vertexList[1].pos, mat);
	//		triangle.vertexList[2].pos = GetVector3DMulMatrix44(triangle.vertexList[2].pos, mat);
	//		// TODO: ��ͬһ���ż��㷨��Ҫ����ת�þ���
	//		triangle.vertexList[0].normal = GetVector3DMulMatrix44(triangle.vertexList[0].normal, mat);
	//		triangle.vertexList[1].normal = GetVector3DMulMatrix44(triangle.vertexList[1].normal, mat);
	//		triangle.vertexList[2].normal = GetVector3DMulMatrix44(triangle.vertexList[2].normal, mat);

	//		pRenderList->triangleList.push_back(triangle);
	//	}
	//}

	void HXSubMesh::Insert_To_RenderList_With_Index(HXMatrix44& mat, HXRenderList* pRenderList)
	{
		for (std::vector<int>::iterator itr = indexList.begin(); itr != indexList.end(); ++itr)
		{
			HXTriangle triangle;
			triangle.vertexList[0] = vertexList[*itr];
			triangle.vertexList[1] = vertexList[*++itr];
			triangle.vertexList[2] = vertexList[*++itr];

			//// ���Ŵ���S
			//HXMatrix44 matS = GetScaleMatrix44(scale.x, scale.y, scale.z);
			//// ��ת����Q
			//HXMatrix44 matX = GetRotateMatrix44X(eulerDegree.x);
			//HXMatrix44 matY = GetRotateMatrix44Y(eulerDegree.y);
			//HXMatrix44 matZ = GetRotateMatrix44Z(eulerDegree.z);
			//// ƽ��һ��Ҫ�����T
			//HXMatrix44 matT = GetTranslateMatrix44(pos.x, pos.y, pos.z);

			//// ģ�Ϳռ䵽����ռ�ת�� SQT
			//HXMatrix44 mat = matS*matX*matY*matZ*matT;
			triangle.vertexList[0].pos = GetVector3DMulMatrix44(triangle.vertexList[0].pos, mat);
			triangle.vertexList[1].pos = GetVector3DMulMatrix44(triangle.vertexList[1].pos, mat);
			triangle.vertexList[2].pos = GetVector3DMulMatrix44(triangle.vertexList[2].pos, mat);
			// TODO: ��ͬһ���ż��㷨��Ҫ����ת�þ���
			triangle.vertexList[0].normal = GetVector3DMulMatrix44(triangle.vertexList[0].normal, mat);
			triangle.vertexList[1].normal = GetVector3DMulMatrix44(triangle.vertexList[1].normal, mat);
			triangle.vertexList[2].normal = GetVector3DMulMatrix44(triangle.vertexList[2].normal, mat);

			triangle.vertexList[0].tangent = GetVector3DMulMatrix44(triangle.vertexList[0].tangent, mat);
			triangle.vertexList[1].tangent = GetVector3DMulMatrix44(triangle.vertexList[1].tangent, mat);
			triangle.vertexList[2].tangent = GetVector3DMulMatrix44(triangle.vertexList[2].tangent, mat);

			pRenderList->triangleList.push_back(triangle);
		}
	}

	//----------------------------------Mesh-------------------------------------------
	HXMesh::HXMesh() : skeleton(NULL), animInst(NULL)
	{

	}

	HXMesh::~HXMesh()
	{
		for (std::vector<HXSubMesh*>::iterator itr = subMeshList.begin(); itr != subMeshList.end(); itr++)
		{
			delete (*itr);
		}
		if (skeleton && skeleton->ReduceReference(this))
		{
			delete skeleton;
		}
		skeleton = NULL;
	}

	void HXMesh::PlayDefaultAnimation()
	{
		if (animInst && skeleton)
		{
			std::vector<std::string> lt = skeleton->GetAnimNameList();
			if (lt.size() > 0)
			{
				animInst->nCurKeyframe = 0;
				animInst->nSpeed = 1;
				//animInst->strCurPlayAnim = lt[0];
				animInst->nCurPlayAnimIndex = 0;
				return;
			}
		}
	}

	void HXMesh::PlayAnimation(std::string strAnimName, int nSpeed)
	{
		if (animInst && skeleton)
		{
			std::vector<std::string> lt = skeleton->GetAnimNameList();

			for (int i = 0; i < lt.size(); ++i)
			{
				if (lt[i] == strAnimName)
				{
					animInst->nCurKeyframe = 0;
					animInst->nSpeed = nSpeed;
					//animInst->strCurPlayAnim = strAnimName;
					animInst->nCurPlayAnimIndex = i;
				}
			}
		}
	}

	void HXMesh::UpdateAnimation()
	{
		if (skeleton)
		{
			skeleton->UpdateAnimation(animInst);
		}
	}

	HXMesh* HXMesh::Clone(HXRenderSystem* pRenderSystem)
	{
		HXMesh* pMesh = new HXMesh();
		for (std::vector<HXSubMesh*>::iterator itr = subMeshList.begin(); itr != subMeshList.end(); ++itr)
		{
			HXSubMesh* pHXSubMesh = (*itr)->Clone(pRenderSystem);
			pMesh->subMeshList.push_back(pHXSubMesh);
		}
		pMesh->triangleCount = triangleCount;
		pMesh->vertexCount = vertexCount;
		
		// ������ͬһ�ף��������(�統ǰ����Ƭ�Σ�ʱ���)�Լ�����
		if (NULL != skeleton)
		{
			pMesh->skeleton = skeleton;
			skeleton->AddReference(pMesh);
			HXAnimationInstance* pAnimInst = new HXAnimationInstance();
			pMesh->animInst = pAnimInst;
			pMesh->animInst->mMesh = pMesh;
		}

		return pMesh;
	}

	void HXMesh::CreateCubeForTest()
	{
		HXSubMesh* subMesh = new HXSubMesh();

		subMesh->useIndex = false;
		subMesh->materialName = "terrain_texture.bmp";

		// ������
		// reserve���pushbackʹ��	resize���[]ʹ��
		subMesh->vertexList.reserve(36);
		subMesh->triangleCount = 12;

		// --------------------��--012--023--------------------------
		HXVertex vertex0up;
		vertex0up.pos = HXVector3D(-1, 1, 1);
		vertex0up.color = HXColor(255, 255, 255, 255);
		vertex0up.u = 0;
		vertex0up.v = 1;
		vertex0up.normal = HXVector3D(0, 1, 0);
		subMesh->vertexList.push_back(vertex0up);

		HXVertex vertex1up;
		vertex1up.pos = HXVector3D(1, 1, 1);
		vertex1up.color = HXColor(255, 0, 0, 255);
		vertex1up.u = 1;
		vertex1up.v = 1;
		vertex1up.normal = HXVector3D(0, 1, 0);
		subMesh->vertexList.push_back(vertex1up);

		HXVertex vertex2up;
		vertex2up.pos = HXVector3D(1, 1, -1);
		vertex2up.color = HXColor(0, 255, 0, 255);
		vertex2up.u = 1;
		vertex2up.v = 0;
		vertex2up.normal = HXVector3D(0, 1, 0);
		subMesh->vertexList.push_back(vertex2up);

		subMesh->vertexList.push_back(vertex0up);
		subMesh->vertexList.push_back(vertex2up);

		HXVertex vertex3up;
		vertex3up.pos = HXVector3D(-1, 1, -1);
		vertex3up.color = HXColor(0, 0, 255, 255);
		vertex3up.u = 0;
		vertex3up.v = 0;
		vertex3up.normal = HXVector3D(0, 1, 0);
		subMesh->vertexList.push_back(vertex3up);

		//---------------------��--765--754----------------------
		HXVertex vertex7bottom;
		vertex7bottom.pos = HXVector3D(-1, -1, -1);
		vertex7bottom.color = HXColor(255, 255, 255, 255);
		vertex7bottom.u = 0;
		vertex7bottom.v = 1;
		vertex7bottom.normal = HXVector3D(0, -1, 0);
		subMesh->vertexList.push_back(vertex7bottom);

		HXVertex vertex6bottom;
		vertex6bottom.pos = HXVector3D(1, -1, -1);
		vertex6bottom.color = HXColor(255, 0, 0, 255);
		vertex6bottom.u = 1;
		vertex6bottom.v = 1;
		vertex6bottom.normal = HXVector3D(0, -1, 0);
		subMesh->vertexList.push_back(vertex6bottom);

		HXVertex vertex5bottom;
		vertex5bottom.pos = HXVector3D(1, -1, 1);
		vertex5bottom.color = HXColor(0, 255, 0, 255);
		vertex5bottom.u = 1;
		vertex5bottom.v = 0;
		vertex5bottom.normal = HXVector3D(0, -1, 0);
		subMesh->vertexList.push_back(vertex5bottom);

		subMesh->vertexList.push_back(vertex7bottom);
		subMesh->vertexList.push_back(vertex5bottom);

		HXVertex vertex4bottom;
		vertex4bottom.pos = HXVector3D(-1, -1, 1);
		vertex4bottom.color = HXColor(0, 0, 255, 255);
		vertex4bottom.u = 0;
		vertex4bottom.v = 0;
		vertex4bottom.normal = HXVector3D(0, -1, 0);
		subMesh->vertexList.push_back(vertex4bottom);

		//---------------------��--037--074----------------------------
		HXVertex vertex0left;
		vertex0left.pos = HXVector3D(-1, 1, 1);
		vertex0left.color = HXColor(255, 255, 255, 255);
		vertex0left.u = 0;
		vertex0left.v = 1;
		vertex0left.normal = HXVector3D(-1, 0, 0);
		subMesh->vertexList.push_back(vertex0left);

		HXVertex vertex3left;
		vertex3left.pos = HXVector3D(-1, 1, -1);
		vertex3left.color = HXColor(0, 0, 255, 255);
		vertex3left.u = 1;
		vertex3left.v = 1;
		vertex3left.normal = HXVector3D(-1, 0, 0);
		subMesh->vertexList.push_back(vertex3left);

		HXVertex vertex7left;
		vertex7left.pos = HXVector3D(-1, -1, -1);
		vertex7left.color = HXColor(255, 255, 255, 255);
		vertex7left.u = 1;
		vertex7left.v = 0;
		vertex7left.normal = HXVector3D(-1, 0, 0);
		subMesh->vertexList.push_back(vertex7left);

		subMesh->vertexList.push_back(vertex0left);
		subMesh->vertexList.push_back(vertex7left);

		HXVertex vertex4left;
		vertex4left.pos = HXVector3D(-1, -1, 1);
		vertex4left.color = HXColor(0, 0, 255, 255);
		vertex4left.u = 0;
		vertex4left.v = 0;
		vertex4left.normal = HXVector3D(-1, 0, 0);
		subMesh->vertexList.push_back(vertex4left);

		//-----------------------��--215--256------------------------------------
		HXVertex vertex2right;
		vertex2right.pos = HXVector3D(1, 1, -1);
		vertex2right.color = HXColor(0, 255, 0, 255);
		vertex2right.u = 0;
		vertex2right.v = 1;
		vertex2right.normal = HXVector3D(1, 0, 0);
		subMesh->vertexList.push_back(vertex2right);

		HXVertex vertex1right;
		vertex1right.pos = HXVector3D(1, 1, 1);
		vertex1right.color = HXColor(255, 0, 0, 255);
		vertex1right.u = 1;
		vertex1right.v = 1;
		vertex1right.normal = HXVector3D(1, 0, 0);
		subMesh->vertexList.push_back(vertex1right);

		HXVertex vertex5right;
		vertex5right.pos = HXVector3D(1, -1, 1);
		vertex5right.color = HXColor(0, 255, 0, 255);
		vertex5right.u = 1;
		vertex5right.v = 0;
		vertex5right.normal = HXVector3D(1, 0, 0);
		subMesh->vertexList.push_back(vertex5right);

		subMesh->vertexList.push_back(vertex2right);
		subMesh->vertexList.push_back(vertex5right);

		HXVertex vertex6right;
		vertex6right.pos = HXVector3D(1, -1, -1);
		vertex6right.color = HXColor(255, 0, 0, 255);
		vertex6right.u = 0;
		vertex6right.v = 0;
		vertex6right.normal = HXVector3D(1, 0, 0);
		subMesh->vertexList.push_back(vertex6right);

		//--------------------------ǰ--326--367----------------------------
		HXVertex vertex3front;
		vertex3front.pos = HXVector3D(-1, 1, -1);
		vertex3front.color = HXColor(0, 0, 255, 255);
		vertex3front.u = 0;
		vertex3front.v = 1;
		vertex3front.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex3front);

		HXVertex vertex2front;
		vertex2front.pos = HXVector3D(1, 1, -1);
		vertex2front.color = HXColor(0, 255, 0, 255);
		vertex2front.u = 1;
		vertex2front.v = 1;
		vertex2front.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex2front);

		HXVertex vertex6front;
		vertex6front.pos = HXVector3D(1, -1, -1);
		vertex6front.color = HXColor(255, 0, 0, 255);
		vertex6front.u = 1;
		vertex6front.v = 0;
		vertex6front.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex6front);

		subMesh->vertexList.push_back(vertex3front);
		subMesh->vertexList.push_back(vertex6front);

		HXVertex vertex7front;
		vertex7front.pos = HXVector3D(-1, -1, -1);
		vertex7front.color = HXColor(255, 255, 255, 255);
		vertex7front.u = 0;
		vertex7front.v = 0;
		vertex7front.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex7front);

		//----------------------------��--104--145-----------------------------------------
		HXVertex vertex1back;
		vertex1back.pos = HXVector3D(1, 1, 1);
		vertex1back.color = HXColor(255, 0, 0, 255);
		vertex1back.u = 0;
		vertex1back.v = 1;
		vertex1back.normal = HXVector3D(0, 0, 1);
		subMesh->vertexList.push_back(vertex1back);

		HXVertex vertex0back;
		vertex0back.pos = HXVector3D(-1, 1, 1);
		vertex0back.color = HXColor(255, 255, 255, 255);
		vertex0back.u = 1;
		vertex0back.v = 1;
		vertex0back.normal = HXVector3D(0, 0, 1);
		subMesh->vertexList.push_back(vertex0back);

		HXVertex vertex4back;
		vertex4back.pos = HXVector3D(-1, -1, 1);
		vertex4back.color = HXColor(0, 0, 255, 255);
		vertex4back.u = 1;
		vertex4back.v = 0;
		vertex4back.normal = HXVector3D(0, 0, 1);
		subMesh->vertexList.push_back(vertex4back);

		subMesh->vertexList.push_back(vertex1back);
		subMesh->vertexList.push_back(vertex4back);

		HXVertex vertex5back;
		vertex5back.pos = HXVector3D(1, -1, 1);
		vertex5back.color = HXColor(0, 255, 0, 255);
		vertex5back.u = 0;
		vertex5back.v = 0;
		vertex5back.normal = HXVector3D(0, 0, 1);
		subMesh->vertexList.push_back(vertex5back);

		subMeshList.push_back(subMesh);

		//subMesh->useIndex = true;

		//subMesh->vertexList.reserve(8);

		//HXVertex vertex0;
		//vertex0.pos = HXVector3D(-1, 1, 1);
		//vertex0.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex0);

		//HXVertex vertex1;
		//vertex1.pos = HXVector3D(1, 1, 1);
		//vertex1.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex1);

		//HXVertex vertex2;
		//vertex2.pos = HXVector3D(-1, 1, -1);
		//vertex2.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex2);

		//HXVertex vertex3;
		//vertex3.pos = HXVector3D(1, 1, -1);
		//vertex3.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex3);

		//HXVertex vertex4;
		//vertex4.pos = HXVector3D(-1, -1, 1);
		//vertex4.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex4);

		//HXVertex vertex5;
		//vertex5.pos = HXVector3D(1, -1, 1);
		//vertex5.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex5);

		//HXVertex vertex6;
		//vertex6.pos = HXVector3D(-1, -1, -1);
		//vertex6.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex6);

		//HXVertex vertex7;
		//vertex7.pos = HXVector3D(1, -1, -1);
		//vertex7.color = HXColor(255, 0, 0, 255);
		//subMesh->vertexList.push_back(vertex7);

		//// ��0,1,3	0,3,2	ǰ2,3,7	2,7,6	��3,1,5	3,5,7	��0,2,6	0,6,4	��1,0,4	1,4,5	��6,7,5	6,5,4
		//subMesh->indexList.reserve(36);
		//subMesh->indexList.push_back(0);
		//subMesh->indexList.push_back(1);
		//subMesh->indexList.push_back(3);

		//subMesh->indexList.push_back(0);
		//subMesh->indexList.push_back(3);
		//subMesh->indexList.push_back(2);

		//subMesh->indexList.push_back(2);
		//subMesh->indexList.push_back(3);
		//subMesh->indexList.push_back(7);

		//subMesh->indexList.push_back(2);
		//subMesh->indexList.push_back(7);
		//subMesh->indexList.push_back(6);

		//subMesh->indexList.push_back(3);
		//subMesh->indexList.push_back(1);
		//subMesh->indexList.push_back(5);

		//subMesh->indexList.push_back(3);
		//subMesh->indexList.push_back(5);
		//subMesh->indexList.push_back(7);

		//subMesh->indexList.push_back(0);
		//subMesh->indexList.push_back(2);
		//subMesh->indexList.push_back(6);

		//subMesh->indexList.push_back(0);
		//subMesh->indexList.push_back(6);
		//subMesh->indexList.push_back(4);

		//subMesh->indexList.push_back(1);
		//subMesh->indexList.push_back(0);
		//subMesh->indexList.push_back(4);

		//subMesh->indexList.push_back(1);
		//subMesh->indexList.push_back(4);
		//subMesh->indexList.push_back(5);

		//subMesh->indexList.push_back(6);
		//subMesh->indexList.push_back(7);
		//subMesh->indexList.push_back(5);

		//subMesh->indexList.push_back(6);
		//subMesh->indexList.push_back(5);
		//subMesh->indexList.push_back(4);

		//subMeshList.push_back(subMesh);
	}

	void HXMesh::CreateTriangleForTest()
	{
		HXSubMesh* subMesh = new HXSubMesh();

		subMesh->useIndex = false;

		subMesh->vertexList.reserve(3);
		subMesh->triangleCount = 1;

		HXVertex vertex0;
		vertex0.pos = HXVector3D(0, 1, 0);
		vertex0.color = HXColor(255, 0, 0, 255);
		vertex0.u = 0.5f;
		vertex0.v = 1;
		vertex0.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex0);

		HXVertex vertex1;
		vertex1.pos = HXVector3D(1, 0, 0);
		vertex1.color = HXColor(0, 255, 0, 255);
		vertex1.u = 1;
		vertex1.v = 0;
		vertex1.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex1);

		HXVertex vertex2;
		vertex2.pos = HXVector3D(-1, 0, 0);
		vertex2.color = HXColor(0, 0, 255, 255);
		vertex2.u = 0;
		vertex2.v = 0;
		vertex2.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex2);

		// ˳ʱ�뷽��Ϊ����
		/*subMesh->indexList.reserve(3);
		subMesh->indexList.push_back(0);
		subMesh->indexList.push_back(1);
		subMesh->indexList.push_back(2);*/

		/*subMesh->indexList.push_back(2);
		subMesh->indexList.push_back(1);*/

		subMesh->materialName = "icon.bmp";

		subMeshList.push_back(subMesh);
	}

	void HXMesh::CreateQuadForTest()
	{
		HXSubMesh* subMesh = new HXSubMesh();
		
		subMesh->useIndex = false;

		// ����
		subMesh->vertexList.reserve(6);
		subMesh->triangleCount = 2;

		HXVertex vertex0;
		vertex0.pos = HXVector3D(-1, 1, 0);
		vertex0.color = HXColor(255, 0, 0, 255);
		vertex0.u = 0;
		vertex0.v = 1;
		vertex0.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex0);

		HXVertex vertex1;
		vertex1.pos = HXVector3D(1, 1, 0);
		vertex1.color = HXColor(0, 255, 0, 255);
		vertex1.u = 1;
		vertex1.v = 1;
		vertex1.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex1);

		HXVertex vertex2;
		vertex2.pos = HXVector3D(1, -1, 0);
		vertex2.color = HXColor(255, 0, 0, 255);
		vertex2.u = 1;
		vertex2.v = 0;
		vertex2.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex2);

		subMesh->vertexList.push_back(vertex0);
		subMesh->vertexList.push_back(vertex2);

		HXVertex vertex3;
		vertex3.pos = HXVector3D(-1, -1, 0);
		vertex3.color = HXColor(0, 0, 255, 255);
		vertex3.u = 0;
		vertex3.v = 0;
		vertex3.normal = HXVector3D(0, 0, -1);
		subMesh->vertexList.push_back(vertex3);

		// ����
		/*subMesh->indexList.reserve(6);

		subMesh->indexList.push_back(0);
		subMesh->indexList.push_back(1);
		subMesh->indexList.push_back(2);

		subMesh->indexList.push_back(0);
		subMesh->indexList.push_back(2);
		subMesh->indexList.push_back(3);*/
		
		subMesh->materialName = "icon.bmp";

		subMeshList.push_back(subMesh);
	}

	//void HXMesh::CreateSkyBox()
	//{
	//	HXSubMesh* subMesh = new HXSubMesh();

	//	subMesh->useIndex = false;
	//	subMesh->materialName = "";

	//	// ������
	//	// reserve���pushbackʹ��	resize���[]ʹ��
	//	subMesh->vertexList.reserve(36);
	//	subMesh->triangleCount = 12;
	//	//subMesh->vertexList.reserve(6);
	//	//subMesh->triangleCount = 2;

	//	// SKYBOX ���߳���  ��ʱ��Ϊ����????
	//	// --------------------��--012--023--------------------------
	//	HXVertex vertex0up;
	//	vertex0up.pos = HXVector3D(-1, 1, 1);
	//	vertex0up.color = HXColor(255, 255, 255, 255);
	//	vertex0up.u = 1;
	//	vertex0up.v = 0;
	//	vertex0up.normal = HXVector3D(0, -1, 0);
	//	subMesh->vertexList.push_back(vertex0up);

	//	HXVertex vertex2up;
	//	vertex2up.pos = HXVector3D(1, 1, -1);
	//	vertex2up.color = HXColor(0, 255, 0, 255);
	//	vertex2up.u = 0;
	//	vertex2up.v = 1;
	//	vertex2up.normal = HXVector3D(0, -1, 0);
	//	subMesh->vertexList.push_back(vertex2up);

	//	HXVertex vertex1up;
	//	vertex1up.pos = HXVector3D(1, 1, 1);
	//	vertex1up.color = HXColor(255, 0, 0, 255);
	//	vertex1up.u = 0;
	//	vertex1up.v = 0;
	//	vertex1up.normal = HXVector3D(0, -1, 0);
	//	subMesh->vertexList.push_back(vertex1up);

	//	subMesh->vertexList.push_back(vertex0up);

	//	HXVertex vertex3up;
	//	vertex3up.pos = HXVector3D(-1, 1, -1);
	//	vertex3up.color = HXColor(0, 0, 255, 255);
	//	vertex3up.u = 1;
	//	vertex3up.v = 1;
	//	vertex3up.normal = HXVector3D(0, -1, 0);
	//	subMesh->vertexList.push_back(vertex3up);

	//	subMesh->vertexList.push_back(vertex2up);

	//	//---------------------��--765--754----------------------
	//	HXVertex vertex7bottom;
	//	vertex7bottom.pos = HXVector3D(-1, -1, -1);
	//	vertex7bottom.color = HXColor(255, 255, 255, 255);
	//	vertex7bottom.u = 1;
	//	vertex7bottom.v = 0;
	//	vertex7bottom.normal = HXVector3D(0, 1, 0);
	//	subMesh->vertexList.push_back(vertex7bottom);

	//	HXVertex vertex5bottom;
	//	vertex5bottom.pos = HXVector3D(1, -1, 1);
	//	vertex5bottom.color = HXColor(0, 255, 0, 255);
	//	vertex5bottom.u = 0;
	//	vertex5bottom.v = 1;
	//	vertex5bottom.normal = HXVector3D(0, 1, 0);
	//	subMesh->vertexList.push_back(vertex5bottom);

	//	HXVertex vertex6bottom;
	//	vertex6bottom.pos = HXVector3D(1, -1, -1);
	//	vertex6bottom.color = HXColor(255, 0, 0, 255);
	//	vertex6bottom.u = 0;
	//	vertex6bottom.v = 0;
	//	vertex6bottom.normal = HXVector3D(0, 1, 0);
	//	subMesh->vertexList.push_back(vertex6bottom);

	//	subMesh->vertexList.push_back(vertex7bottom);

	//	HXVertex vertex4bottom;
	//	vertex4bottom.pos = HXVector3D(-1, -1, 1);
	//	vertex4bottom.color = HXColor(0, 0, 255, 255);
	//	vertex4bottom.u = 1;
	//	vertex4bottom.v = 1;
	//	vertex4bottom.normal = HXVector3D(0, 1, 0);
	//	subMesh->vertexList.push_back(vertex4bottom);

	//	subMesh->vertexList.push_back(vertex5bottom);

	//	//---------------------��--037--074----------------------------
	//	HXVertex vertex0left;
	//	vertex0left.pos = HXVector3D(-1, 1, 1);
	//	vertex0left.color = HXColor(255, 255, 255, 255);
	//	vertex0left.u = 0;
	//	vertex0left.v = 1;
	//	vertex0left.normal = HXVector3D(1, 0, 0);
	//	subMesh->vertexList.push_back(vertex0left);

	//	HXVertex vertex7left;
	//	vertex7left.pos = HXVector3D(-1, -1, -1);
	//	vertex7left.color = HXColor(255, 255, 255, 255);
	//	vertex7left.u = 1;
	//	vertex7left.v = 0;
	//	vertex7left.normal = HXVector3D(1, 0, 0);
	//	subMesh->vertexList.push_back(vertex7left);

	//	HXVertex vertex3left;
	//	vertex3left.pos = HXVector3D(-1, 1, -1);
	//	vertex3left.color = HXColor(0, 0, 255, 255);
	//	vertex3left.u = 1;
	//	vertex3left.v = 1;
	//	vertex3left.normal = HXVector3D(1, 0, 0);
	//	subMesh->vertexList.push_back(vertex3left);

	//	subMesh->vertexList.push_back(vertex0left);

	//	HXVertex vertex4left;
	//	vertex4left.pos = HXVector3D(-1, -1, 1);
	//	vertex4left.color = HXColor(0, 0, 255, 255);
	//	vertex4left.u = 0;
	//	vertex4left.v = 0;
	//	vertex4left.normal = HXVector3D(1, 0, 0);
	//	subMesh->vertexList.push_back(vertex4left);

	//	subMesh->vertexList.push_back(vertex7left);

	//	//-----------------------��--215--256------------------------------------
	//	HXVertex vertex2right;
	//	vertex2right.pos = HXVector3D(1, 1, -1);
	//	vertex2right.color = HXColor(0, 255, 0, 255);
	//	vertex2right.u = 0;
	//	vertex2right.v = 1;
	//	vertex2right.normal = HXVector3D(-1, 0, 0);
	//	subMesh->vertexList.push_back(vertex2right);

	//	HXVertex vertex5right;
	//	vertex5right.pos = HXVector3D(1, -1, 1);
	//	vertex5right.color = HXColor(0, 255, 0, 255);
	//	vertex5right.u = 1;
	//	vertex5right.v = 0;
	//	vertex5right.normal = HXVector3D(-1, 0, 0);
	//	subMesh->vertexList.push_back(vertex5right);

	//	HXVertex vertex1right;
	//	vertex1right.pos = HXVector3D(1, 1, 1);
	//	vertex1right.color = HXColor(255, 0, 0, 255);
	//	vertex1right.u = 1;
	//	vertex1right.v = 1;
	//	vertex1right.normal = HXVector3D(-1, 0, 0);
	//	subMesh->vertexList.push_back(vertex1right);

	//	subMesh->vertexList.push_back(vertex2right);

	//	HXVertex vertex6right;
	//	vertex6right.pos = HXVector3D(1, -1, -1);
	//	vertex6right.color = HXColor(255, 0, 0, 255);
	//	vertex6right.u = 0;
	//	vertex6right.v = 0;
	//	vertex6right.normal = HXVector3D(-1, 0, 0);
	//	subMesh->vertexList.push_back(vertex6right);

	//	subMesh->vertexList.push_back(vertex5right);

	//	//--------------------------ǰ--326--367----------------------------
	//	HXVertex vertex3front;
	//	vertex3front.pos = HXVector3D(-1, 1, -1);
	//	vertex3front.color = HXColor(0, 0, 255, 255);
	//	vertex3front.u = 0;
	//	vertex3front.v = 1;
	//	vertex3front.normal = HXVector3D(0, 0, 1);
	//	subMesh->vertexList.push_back(vertex3front);

	//	HXVertex vertex6front;
	//	vertex6front.pos = HXVector3D(1, -1, -1);
	//	vertex6front.color = HXColor(255, 0, 0, 255);
	//	vertex6front.u = 1;
	//	vertex6front.v = 0;
	//	vertex6front.normal = HXVector3D(0, 0, 1);
	//	subMesh->vertexList.push_back(vertex6front);

	//	HXVertex vertex2front;
	//	vertex2front.pos = HXVector3D(1, 1, -1);
	//	vertex2front.color = HXColor(0, 255, 0, 255);
	//	vertex2front.u = 1;
	//	vertex2front.v = 1;
	//	vertex2front.normal = HXVector3D(0, 0, 1);
	//	subMesh->vertexList.push_back(vertex2front);

	//	subMesh->vertexList.push_back(vertex3front);
	//	
	//	HXVertex vertex7front;
	//	vertex7front.pos = HXVector3D(-1, -1, -1);
	//	vertex7front.color = HXColor(255, 255, 255, 255);
	//	vertex7front.u = 0;
	//	vertex7front.v = 0;
	//	vertex7front.normal = HXVector3D(0, 0, 1);
	//	subMesh->vertexList.push_back(vertex7front);

	//	subMesh->vertexList.push_back(vertex6front);

	//	//----------------------------��--104--145-----------------------------------------
	//	HXVertex vertex1back;
	//	vertex1back.pos = HXVector3D(1, 1, 1);
	//	vertex1back.color = HXColor(255, 0, 0, 255);
	//	vertex1back.u = 0;
	//	vertex1back.v = 1;
	//	vertex1back.normal = HXVector3D(0, 0, -1);
	//	subMesh->vertexList.push_back(vertex1back);

	//	HXVertex vertex4back;
	//	vertex4back.pos = HXVector3D(-1, -1, 1);
	//	vertex4back.color = HXColor(0, 0, 255, 255);
	//	vertex4back.u = 1;
	//	vertex4back.v = 0;
	//	vertex4back.normal = HXVector3D(0, 0, -1);
	//	subMesh->vertexList.push_back(vertex4back);

	//	HXVertex vertex0back;
	//	vertex0back.pos = HXVector3D(-1, 1, 1);
	//	vertex0back.color = HXColor(255, 255, 255, 255);
	//	vertex0back.u = 1;
	//	vertex0back.v = 1;
	//	vertex0back.normal = HXVector3D(0, 0, -1);
	//	subMesh->vertexList.push_back(vertex0back);

	//	subMesh->vertexList.push_back(vertex1back);
	//	
	//	HXVertex vertex5back;
	//	vertex5back.pos = HXVector3D(1, -1, 1);
	//	vertex5back.color = HXColor(0, 255, 0, 255);
	//	vertex5back.u = 0;
	//	vertex5back.v = 0;
	//	vertex5back.normal = HXVector3D(0, 0, -1);
	//	subMesh->vertexList.push_back(vertex5back);

	//	subMesh->vertexList.push_back(vertex4back);

	//	subMeshList.push_back(subMesh);
	//}

	/*void HXMesh::Insert_To_RenderList(const HXVector3D& pos, const HXVector3D& eulerDegree, const HXVector3D& scale, HXRenderList* pRenderList)
	{
		for (std::vector<HXSubMesh*>::iterator itr = subMeshList.begin(); itr != subMeshList.end(); ++itr)
		{
			(*itr)->Insert_To_RenderList(pos, eulerDegree, scale, pRenderList);
		}
	}*/

	void HXMesh::SetMeshNotStatic()
	{
		for (std::vector<HXSubMesh*>::iterator itr = subMeshList.begin(); itr != subMeshList.end(); ++itr)
		{
			(*itr)->IsStaticMesh = false;
		}
	}
}