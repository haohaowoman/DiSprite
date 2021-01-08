#pragma once
#include "DiControl.h"
#include "IVariable.h"
#include "IData.h"
#include "IKeyBordSupport.h"
#include "IFont.h"
#include "IText.h"

namespace DiControl
{
	class DiTextInput :
		public DiControlPri,
		public IVariable,
		public IKeyBordSupport,
		public IDataDisplayPos,
		public ITextInput,
		public ITextDisplay,
		public IDataInput,
		public IFontFamily
	{
	public:
		DiTextInput(void);
		virtual ~DiTextInput(void);

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

		virtual SHORT DisplayX() const;

		virtual void DisplayX(SHORT val);

		virtual SHORT DisplayY() const;

		virtual void DisplayY(SHORT val);

		virtual BYTE TextLength() const;

		virtual void TextLength(BYTE val);

		virtual BYTE InputMode() const;

		virtual void InputMode(BYTE val);

		virtual BYTE XPointCount() const;

		virtual void XPointCount(BYTE val);

		virtual BYTE YPointCount() const;

		virtual void YPointCount(BYTE val);

		virtual SHORT InputDisplayAreaX() const;

		virtual void InputDisplayAreaX(SHORT val);

		virtual SHORT InputDisplayAreaY() const;

		virtual void InputDisplayAreaY(SHORT val);

		virtual bool IsSelInputStateReturn() const;

		virtual void IsSelInputStateReturn(bool val);

		virtual SHORT InputDisplayAreaX_End() const;

		virtual void InputDisplayAreaX_End(SHORT val);

		virtual SHORT InputDisplayAreaY_End() const;

		virtual void InputDisplayAreaY_End(SHORT val);

		virtual BYTE CursorColor() const;

		virtual void CursorColor(BYTE val);

		virtual BYTE DisplayType() const;

		virtual void DisplayType(BYTE val);

		virtual CString Name() const;

		virtual USHORT VariableAddr() const;

		virtual void VariableAddr(USHORT val);

		virtual BYTE FontFamily() const;

		virtual void FontFamily(BYTE val);

		virtual long GetDataLength() const;

	};
}


