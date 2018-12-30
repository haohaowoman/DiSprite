#include "StdAfx.h"
#include "DiTextInput.h"

using namespace DiControl;

DiTextInput::DiTextInput(void)
{
	Second.CF = 0xFE;
	Third.CF = 0xFE;
	Fourth.CF = 0xFE;
	ControlType(6);
}


DiTextInput::~DiTextInput(void)
{
}

BYTE DiControl::DiTextInput::KeyBordPosition() const
{
	return Third.Page[5];
}

BYTE DiControl::DiTextInput::KeyBordPage() const
{
	return Third.Page[7];
}

SHORT DiControl::DiTextInput::KeyBordAreaBeginX() const
{
	return HighBitLowConverter::ToShort(&Third.Page[8]);
}

SHORT DiControl::DiTextInput::KeyBordAreaBeginY() const
{
	return HighBitLowConverter::ToShort(&Third.Page[10]);
}

SHORT DiControl::DiTextInput::KeyBordAreaEndX() const
{
	return HighBitLowConverter::ToShort(&Third.Page[12]);
}

SHORT DiControl::DiTextInput::KeyBordAreaEndY() const
{
	return HighBitLowConverter::ToShort(&Third.Page[15]);
}

SHORT DiControl::DiTextInput::KeyBordDisplayX() const
{
	return -1;
}

SHORT DiControl::DiTextInput::KeyBordDisplayY() const
{
	return -1;
}

SHORT DiControl::DiTextInput::DisplayX() const
{
	return HighBitLowConverter::ToShort(&Fourth.Page[1]);
}

SHORT DiControl::DiTextInput::DisplayY() const
{
	return HighBitLowConverter::ToShort(&Fourth.Page[3]);
}

BYTE DiControl::DiTextInput::TextLength() const
{
	return Second.Page[3];
}

BYTE DiControl::DiTextInput::InputMode() const
{
	return Second.Page[4];
}

BYTE DiControl::DiTextInput::XPointCount() const
{
	return Second.Page[6];
}

BYTE DiControl::DiTextInput::YPointCount() const
{
	return Second.Page[7];
}

SHORT DiControl::DiTextInput::InputDisplayAreaX() const
{
	return HighBitLowConverter::ToShort(&Second.Page[11]);
}

SHORT DiControl::DiTextInput::InputDisplayAreaY() const
{
	return HighBitLowConverter::ToShort(&Second.Page[13]);
}

bool DiControl::DiTextInput::IsSelInputStateReturn() const
{
	return Second.Page[15] == 0x55;
}

SHORT DiControl::DiTextInput::InputDisplayAreaX_End() const
{
	return HighBitLowConverter::ToShort(&Third.Page[1]);
}

SHORT DiControl::DiTextInput::InputDisplayAreaY_End() const
{
	return HighBitLowConverter::ToShort(&Third.Page[3]);
}

BYTE DiControl::DiTextInput::CursorColor() const
{
	return 0;
}

BYTE DiControl::DiTextInput::DisplayType() const
{
	return Fourth.Page[5];
}

CString DiControl::DiTextInput::Name() const
{
	return _T("Text input");
}

USHORT DiControl::DiTextInput::VariableAddr() const
{
	return HighBitLowConverter::ToUShort(&Second.Page[1]);
}

BYTE DiControl::DiTextInput::FontFamily() const
{
	return Second.Page[5];
}

void DiControl::DiTextInput::FontFamily(BYTE val)
{
	Second.Page[5] = val;
}

void DiControl::DiTextInput::VariableAddr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[1]);
}

void DiControl::DiTextInput::DisplayType(BYTE val)
{
	Fourth.Page[5] = val;
}

void DiControl::DiTextInput::CursorColor(BYTE val)
{
	
}

void DiControl::DiTextInput::InputDisplayAreaY_End(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[3]);
}

void DiControl::DiTextInput::InputDisplayAreaX_End(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[1]);
}

void DiControl::DiTextInput::IsSelInputStateReturn(bool val)
{
	Second.Page[15] = val ? 0x55 : 0;
}

void DiControl::DiTextInput::InputDisplayAreaY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[13]);
}

void DiControl::DiTextInput::InputDisplayAreaX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[11]);
}

void DiControl::DiTextInput::YPointCount(BYTE val)
{
	Second.Page[7] = val;
}

void DiControl::DiTextInput::XPointCount(BYTE val)
{
	Second.Page[6] = val;
}

void DiControl::DiTextInput::InputMode(BYTE val)
{
	Second.Page[4] = val;
}

void DiControl::DiTextInput::TextLength(BYTE val)
{
	Second.Page[3] = val;
}

void DiControl::DiTextInput::DisplayY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[3]);
}

void DiControl::DiTextInput::DisplayX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[1]);
}

void DiControl::DiTextInput::KeyBordDisplayY(SHORT val)
{
	
}

void DiControl::DiTextInput::KeyBordDisplayX(SHORT val)
{
	
}

void DiControl::DiTextInput::KeyBordAreaEndY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[14]);
}

void DiControl::DiTextInput::KeyBordAreaEndX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[12]);
}

void DiControl::DiTextInput::KeyBordAreaBeginY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[10]);
}

void DiControl::DiTextInput::KeyBordAreaBeginX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[8]);
}

void DiControl::DiTextInput::KeyBordPage(BYTE val)
{
	Third.Page[7] = val;
}

void DiControl::DiTextInput::KeyBordPosition(BYTE val)
{
	Third.Page[5] = val;
}
