#include "..\include\HXBitmap.h"
#include "HXMath.h"

namespace HX3D
{
	HINSTANCE GInstance;

	HXBitmap::HXBitmap():mPixels(NULL),nWidth(0),nHeight(0),nPitch(0)
	{
		
	}

	HXBitmap::~HXBitmap()
	{
		if (mPixels)
		{
			delete mPixels;
			mPixels = NULL;
		}
	}

	bool HXBitmap::LoadBitmapFile(std::string strFileName)
	{
		hBitmap = (HBITMAP)::LoadImage(GInstance, strFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap != NULL)
		{
			BITMAP bitmap;
			hBitmapDC = ::CreateCompatibleDC(NULL);
			::SelectObject(hBitmapDC, hBitmap);
			::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			nWidth = bitmap.bmWidth;
			nHeight = bitmap.bmHeight;
			nPitch = bitmap.bmHeight;

			mPixels = new HXColor[nWidth*nHeight];
			// int nSize = sizeof(mPixels);
			for (int i = 0; i < nHeight; i++)
			{
				for (int j = 0; j < nWidth; j++)
				{
					COLORREF color = ::GetPixel(hBitmapDC, j, i);
					mPixels[i*nPitch + j] = HXColor(GetRValue(color), GetGValue(color), GetBValue(color));
					
					// alpha���Զ�����Ϊ0���޷���ȷ��ȡ
					/*float a = LOBYTE((color) >> 24);
					float r = LOBYTE(color);
					float g = LOBYTE(((WORD)(color)) >> 8);
					float b = LOBYTE((color) >> 16);
					int n = 0;*/
				}
			}

			::DeleteObject(hBitmap);
			::DeleteDC(hBitmapDC);

			return true;
		}
		else
		{
			return false;
		}
	}

	HXColor HXBitmap::GetPixel(int nX, int nY)
	{
		nX = Clamp(nX, nWidth - 1, 0);
		nY = Clamp(nY, nHeight - 1, 0);
		return mPixels[nY*nPitch + nX];
	}

	HXColor HXBitmap::GetPixelRatio(float fX, float fY)
	{
		int nX = fX * (float)(nWidth - 1.0f) + 0.5f;
		int nY = fY * (float)(nHeight -1.0f) + 0.5f;
		return GetPixel(nX, nY);
	}
}
