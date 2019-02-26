#pragma once
#include "HXCommon.h"
#include "HXIFreeTypeFont.h"

namespace HX3D
{
	class HXGLFontBG;
	class HXGLFreeTypeFont : public HXIFreeTypeFont
	{
	public:
		HXGLFreeTypeFont();
		~HXGLFreeTypeFont();

		virtual void Initialize(const char* fontFile, int fontSize);
		virtual void BeginText();
		virtual void _DrawText(float x, float y, const wchar_t* text, const HXColor& color);
		virtual void EndText();

	private:
		HXCharacter* GetCharacter(wchar_t ch);
	
		unsigned    _textureId;
		//unsigned	_samplerId;	ʹ��Ĭ�ϲ�����
		unsigned	_vaoId;
		// TODO: �Ƿ�Ҫÿ֡��ֵ���䲻�������,�����_DrawTextʱ��renderlist��������,���ɾ�����ü�ֵ�ԣ���
		unsigned	_vboId;
		unsigned	_programId;

		HXGLFontBG* bg;
	};
}


