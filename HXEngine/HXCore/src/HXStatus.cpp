#include "..\include\HXStatus.h"
#include <Windows.h>
#include "HXUtil.h"
#include "HXRenderSystem.h"
#include "HXRoot.h"

namespace HX3D
{
	HXStatus* HXStatus::pInstance = NULL;

	HXStatus::HXStatus()
	{
		mLastFPS = 0.0;
		mLastTime = 0.0;
		mFrameCount = 0;
		showStatus = true;
	}

	HXStatus::~HXStatus()
	{
	}

	void HXStatus::ResetStatus()
	{
		nVertexCount = 0;
		nTriangleCount = 0;
		nDrawCall = 0;
		nBatchCall = 0;
	}

	void HXStatus::ShowStatusInfo()
	{
		if (showStatus)
		{
			/*std::string strFPS = "FPS: " + FloatToString(CalcLastFPS());
			HXGraphics::GetInstance()->DrawString(strFPS, SCREEN_WIDTH / 2, 0);

			std::string strTriangleCount = "TriangleCount: " + FloatToString(nTriangleCount);
			HXGraphics::GetInstance()->DrawString(strTriangleCount, SCREEN_WIDTH / 2, 18);

			std::string strVertexCount = "VertexCount: " + FloatToString(nVertexCount);
			HXGraphics::GetInstance()->DrawString(strVertexCount, SCREEN_WIDTH / 2, 36);*/

			HXColor color = HXColor(255, 255, 255, 255);

			HXRenderSystem* rs = HXRoot::GetInstance()->GetRenderSystem();
			if (NULL == rs)
			{
				return;
			}
			rs->BeginText();

			std::string strFPS = "FPS: " + FloatToString(CalcLastFPS());
			rs->_DrawText(/*SCREEN_WIDTH / 2*/4, 8, strFPS.c_str(), color);

			std::string strTriangleCount = "TriangleCount: " + FloatToString(nTriangleCount);
			rs->_DrawText(/*SCREEN_WIDTH / 2*/4, 24, strTriangleCount.c_str(), color);

			std::string strVertexCount = "VertexCount: " + FloatToString(nVertexCount);
			rs->_DrawText(/*SCREEN_WIDTH / 2*/4, 40, strVertexCount.c_str(), color);

			std::string strDrawCall = "DrawCall: " + FloatToString(nDrawCall);
			rs->_DrawText(/*SCREEN_WIDTH / 2*/4, 56, strDrawCall.c_str(), color);

			std::string strBatchCall = "DynamicBatch: " + FloatToString(nBatchCall);
			rs->_DrawText(/*SCREEN_WIDTH / 2*/4, 72, strBatchCall.c_str(), color);

			rs->EndText();
		}
	}

	float HXStatus::CalcLastFPS()
	{
		mFrameCount++;
		double curTime = ::GetTickCount();
		if (curTime - mLastTime > 1000.0)
		{
			mLastFPS = (double)mFrameCount / (double)(curTime - mLastTime) * 1000.0;
			mLastTime = curTime;
			mFrameCount = 0;
		}
		return (float)mLastFPS;
	}
}
