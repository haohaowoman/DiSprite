#pragma once
#include "DiControl.h"
#include "IVariable.h"
#include "IData.h"
#include "IKeyBordSupport.h"
#include "IFont.h"

namespace DiControl
{
	class DiDataInput : 
		public DiControlPri,
		public IDataDisplay, 
		public IDataDisplayPos,
		public IKeyBordSupport,
		public IVariable,
		public IDataInput,
		public IDataLoad,
		public IRangeSourport,
		public IFontSize,
		public IFontFamily,
		public IRenderColor
	{
	public:
		DiDataInput(void);
		virtual ~DiDataInput(void);

		virtual BYTE DataType() const;

		virtual void DataType(BYTE val);

		virtual BYTE IntegerCount() const;

		virtual void IntegerCount(BYTE val);

		virtual BYTE DecimalCount() const;

		virtual void DecimalCount(BYTE val);

		virtual SHORT DisplayX() const;

		virtual void DisplayX(SHORT val);

		virtual SHORT DisplayY() const;

		virtual void DisplayY(SHORT val);

		virtual USHORT RenderColor() const;

		virtual void RenderColor(USHORT val);

		virtual BYTE KeyBordPosition() const;

		virtual void KeyBordPosition(BYTE val);

		virtual BYTE KeyBordPage() const;

		virtual void KeyBordPage(BYTE val);

		virtual SHORT KeyBordAreaBeginX() const;

		virtual void KeyBordAreaBeginX(SHORT val);

		virtual SHORT KeyBordAreaBeginY() const;

		virtual void KeyBordAreaBeginY(SHORT val);

		virtual SHORT KeyBordAreaEndX() const;

		virtual void KeyBordAreaEndX(SHORT val);

		virtual SHORT KeyBordAreaEndY() const;

		virtual void KeyBordAreaEndY(SHORT val);

		virtual SHORT KeyBordDisplayX() const;

		virtual void KeyBordDisplayX(SHORT val);

		virtual SHORT KeyBordDisplayY() const;

		virtual void KeyBordDisplayY(SHORT val);

		virtual USHORT VariableAddr() const;

		virtual void VariableAddr(USHORT val);

		virtual BYTE CursorColor() const;

		virtual void CursorColor(BYTE val);

		virtual BYTE DisplayType() const;

		virtual void DisplayType(BYTE val);

		virtual BYTE LoadDataStyle();

		virtual void LoadDataStyle(BYTE val);

		virtual USHORT LoadDataAddr() const;

		virtual void LoadDataAddr(USHORT val);

		virtual SHORT LoadData() const;

		virtual void LoadData(SHORT val);

		virtual bool EnableRange() const;

		virtual void EnableRange(bool val);

		virtual int RangeLow() const;

		virtual void RangeLow(int val);

		virtual int RangeHigh() const;

		virtual void RangeHigh(int val);

		virtual BYTE FontFamily() const;

		virtual void FontFamily(BYTE val);

		virtual BYTE FontSize() const;

		virtual void FontSize(BYTE val);

		virtual CString Name() const;

	};
}


