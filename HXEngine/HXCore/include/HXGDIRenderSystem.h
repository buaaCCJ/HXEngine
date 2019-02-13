#pragma once
#include "HXCommon.h"
#include "HXRenderSystem.h"
#include <windows.h>

namespace HX3D
{
	class HXITransform;
	class HXGDIRenderSystem : public HXRenderSystem
	{
	public:
		HXGDIRenderSystem();
		~HXGDIRenderSystem();

		virtual void CreateRenderWindow(std::string strName, int nWidth, int nHeight, bool bFullScreen);
		virtual void MainLoop();
		static void Display(HDC hdc);
		static void Reshape(int nWidth, int nHeight);
		static bool mouseLDown;
		static bool mouseRDown;

		virtual HXRenderable* GenerateRenderable(HXSubMesh* pSubMesh);
		virtual void RenderSingle(HXRenderable* pRenderable, bool shadow);

		virtual HXICamera* CreateCamera(const HXCameraInfo* cameraInfo);

		//virtual void BeginText();
		virtual void _DrawText(float x, float y, std::string text, const HXColor& color = HXColor(255,255,255,255));
		//virtual void EndText();

		virtual HXITransform* CreateTransform();
		virtual HXMaterial* CreateMaterial(HXMaterialInfo* info);

	private:
		// 2�����ڹ��̺���(����Ҫ��ȫ�ֺ���)
		static LRESULT CALLBACK MyWindowProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);
		// 3��ע�ᴰ������
		BOOL RegisterMyWindow(LPCSTR lpcWndName, HINSTANCE hInstance);
		// 4���������ڣ�lpClassName һ�����Ѿ�ע����Ĵ������ͣ�
		HWND CreateMyWindow(LPCTSTR lpClassName, LPCTSTR lpWinName, HINSTANCE hInstance);
		// 5�� ��ʾ����
		void DisplayMyWnd(HWND hWnd);
		// ˢ�´���
		void UpdateMyWnd(HWND hWnd);

		HWND m_hWnd;
	};
}


