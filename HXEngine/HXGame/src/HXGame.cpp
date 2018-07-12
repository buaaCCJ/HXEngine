#include "..\include\HXGame.h"
#include "HXVector.h"
#include "HXSceneManager.h"
#include "HXGameObject.h"
#include "HXMesh.h"
#include "HXICamera.h"
#include "HXRenderSystem.h"
#include "HXStatus.h"

HXGame::HXGame()
{
	CreateGameScene();
}

HXGame::~HXGame()
{
}

////HXGLTest* pGLTest = NULL;
//void Test()
//{
//	//// for test begin
//	/*pGLTest = new HXGLTest();
//	pGLTest->InitTriangle();*/
//
//	/*HXQuaternion q;
//	q.FromAngleAxis(90, HXVector3D(0,1,0));
//	HXVector3D P(1,0,1);
//	HXVector3D destP = q.Transform(P);
//	int nTest = 0;*/
//	//// for test end
//}

//#include "HXGLMaterial.h"
//void Test()
//{
//	HXGLMaterial* mat = new HXGLMaterial(NULL);
//	delete mat;
//}

void HXGame::CreateGameScene()
{
	//HXSceneManager::GetInstance()->LoadScene("./scene/Simple.scene");
	HXSceneManager::GetInstance()->LoadScene("./scene/DuKangCun.scene");
	//Test();
}

void HXGame::OnDisplay(bool shadow)
{
	HXSceneManager::GetInstance()->OnDisplay(shadow);
}

void HXGame::OnViewPortResize(int nScreenWidth, int nScreenHeight)
{
	if (HXSceneManager::GetInstance()->mainCamera)
	{
		HXSceneManager::GetInstance()->mainCamera->OnViewPortResize(nScreenWidth, nScreenHeight);
	}
}

void HXGame::OnKeyboard(unsigned char key, int x, int y)
{
	if (key == 'f' || key == 'F')
	{
		HXSceneManager::GetInstance()->UseFog(!HXSceneManager::GetInstance()->fog->useFog);
	}
	if (key == 's' || key == 'S')
	{
		HXStatus::GetInstance()->showStatus = !HXStatus::GetInstance()->showStatus;
	}
}

void HXGame::OnMouseMove(int nButton, int deltaX, int deltaY)
{
	if (HXSceneManager::GetInstance()->mainCamera == NULL)
	{
		return;
	}
	if (nButton == 0)
	{
		// �������״̬
		HXSceneManager::GetInstance()->mainCamera->move(HXVector3D((deltaX) / 10.0f, -float(deltaY) / 10.0f, 0));
	}
	else if (nButton == 2)
	{
		// �Ҽ�����״̬
		//std::cout << "x = " << deltaX << ";     y = " << deltaY << std::endl;
		HXSceneManager::GetInstance()->mainCamera->yaw(float(deltaX) / 10.0f);
		HXSceneManager::GetInstance()->mainCamera->pitch(float(deltaY) / 10.0f);
	}
}

void HXGame::OnMouseWheel(float fDistance)
{
	if (HXSceneManager::GetInstance()->mainCamera == NULL)
	{
		return;
	}
	HXSceneManager::GetInstance()->mainCamera->move(HXVector3D(0, 0, -fDistance));
}