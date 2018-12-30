#pragma once
#include "DiControl.h"
#include "IFont.h"
#include "IData.h"
#include "IText.h"
namespace DiControl
{
	class DiRTCText : 
		public DiDisplayCtrlPri,
		public IFontFamily,
		public IRenderColor
	{
	public:
		DiRTCText();
		virtual ~DiRTCText();

		virtual BYTE XPointCount() const;
		virtual void XPointCount(BYTE val);

		virtual CString Name() const;

		virtual BYTE FontFamily() const;

		virtual void FontFamily(BYTE val);

		virtual USHORT RenderColor() const;

		virtual void RenderColor(USHORT val);


	};

	class DiDataDisplay :
		public DiDisplayCtrlPri,
		public IRenderColor,
		public IFontFamily,
		public IFontSize,
		public IDataDisplay,
		public IAgliment
	{
	public:
		DiDataDisplay();
		virtual ~DiDataDisplay();

		virtual CString Name() const;

		virtual USHORT RenderColor() const;

		virtual void RenderColor(USHORT val);

		virtual BYTE FontFamily() const;

		virtual void FontFamily(BYTE val);

		virtual BYTE FontSize() const;

		virtual void FontSize(BYTE val);

		virtual BYTE DataType() const;

		virtual void DataType(BYTE val);

		virtual BYTE IntegerCount() const;

		virtual void IntegerCount(BYTE val);

		virtual BYTE DecimalCount() const;

		virtual void DecimalCount(BYTE val);

		virtual BYTE Agliment() const;

		virtual void Agliment(BYTE val);

	};

	class DiTextBlock :
		public DiDisplayCtrlPri,
		public IRenderColor,
		public IFontFamily,
		public ITextDisplay
	{
	public:
		DiTextBlock();
		virtual ~DiTextBlock();

		virtual int Bottom() const;

		virtual void Bottom(int bottom);

		virtual int Right() const;

		virtual void Right(int right);

		virtual CString Name() const;

		virtual USHORT RenderColor() const;

		virtual void RenderColor(USHORT val);

		virtual BYTE FontFamily() const;

		virtual void FontFamily(BYTE val);

		virtual BYTE FontFamily1() const;

		virtual void FontFamily1(BYTE val);


		virtual BYTE TextLength() const;

		virtual void TextLength(BYTE val);

		virtual BYTE XPointCount() const;

		virtual void XPointCount(BYTE val);

		virtual BYTE YPointCount() const;

		virtual void YPointCount(BYTE val);

		virtual bool IsNotAutoAdjustCharsetDis() const;
		virtual void IsNotAutoAdjustCharsetDis(bool val);

		virtual BYTE CodingStyle() const;
		virtual void CodingStyle(BYTE val);

		virtual BYTE HorizontalMargin() const;
		virtual void HorizontalMargin(BYTE val);

		virtual BYTE VerticalMargin() const;
		virtual void VerticalMargin(BYTE val);

		virtual void Top(int top);

		virtual void Left(int left);

	};

}
