#include "StdAfx.h"
#include "DiDataInput.h"
#include <stdexcept>

using namespace DiControl;

DiDataInput::DiDataInput(void)
{
	Second.CF = 0xFE;
	Third.CF = 0xFE;
	Fourth.CF = 0xFE;
	ControlType(0);
}


DiDataInput::~DiDataInput(void)
{
}

BYTE DiControl::DiDataInput::DataType() const
{
	return Second.Page[3];
}

BYTE DiControl::DiDataInput::IntegerCount() const
{
	return Second.Page[4];
}

BYTE DiControl::DiDataInput::DecimalCount() const
{
	return Second.Page[5];
}

SHORT DiControl::DiDataInput::DisplayX() const
{
	return HighBitLowConverter::ToShort(&Second.Page[6]);
}

SHORT DiControl::DiDataInput::DisplayY() const
{
	return HighBitLowConverter::ToShort(&Second.Page[8]);
}

USHORT DiControl::DiDataInput::RenderColor() const
{
	return HighBitLowConverter::ToUShort(&Second.Page[10]);
}

BYTE DiControl::DiDataInput::KeyBordPosition() const
{
	return Third.Page[1];
}

BYTE DiControl::DiDataInput::KeyBordPage() const
{
	return Third.Page[3];
}

SHORT DiControl::DiDataInput::KeyBordAreaBeginX() const
{
	return HighBitLowConverter::ToShort(&Third.Page[4]);
}

SHORT DiControl::DiDataInput::KeyBordAreaBeginY() const
{
	return HighBitLowConverter::ToShort(&Third.Page[6]);
}

SHORT DiControl::DiDataInput::KeyBordAreaEndX() const
{
	return HighBitLowConverter::ToShort(&Third.Page[8]);
}

SHORT DiControl::DiDataInput::KeyBordAreaEndY() const
{
	return HighBitLowConverter::ToShort(&Third.Page[10]);
}

SHORT DiControl::DiDataInput::KeyBordDisplayX() const
{
	return HighBitLowConverter::ToShort(&Third.Page[12]);
}

SHORT DiControl::DiDataInput::KeyBordDisplayY() const
{
	return HighBitLowConverter::ToShort(&Third.Page[14]);
}

USHORT DiControl::DiDataInput::VariableAddr() const
{
	return HighBitLowConverter::ToUShort(&Second.Page[1]);
}

BYTE DiControl::DiDataInput::CursorColor() const
{
	return Second.Page[14];
}

void DiControl::DiDataInput::CursorColor(BYTE val)
{
	Second.Page[14] = val;
}

BYTE DiControl::DiDataInput::DisplayType() const
{
	return Second.Page[15];
}

BYTE DiControl::DiDataInput::LoadDataStyle()
{
	return Fourth.Page[10];
}

USHORT DiControl::DiDataInput::LoadDataAddr() const
{
	return HighBitLowConverter::ToUShort(&Fourth.Page[11]);
}

SHORT DiControl::DiDataInput::LoadData() const
{
	return HighBitLowConverter::ToUShort(&Fourth.Page[13]);
}

bool DiControl::DiDataInput::EnableRange() const
{
	return Fourth.Page[1] == 0xFF;
}

int DiControl::DiDataInput::RangeLow() const
{
	return HighBitLowConverter::ToInt(&Fourth.Page[2]);
}

int DiControl::DiDataInput::RangeHigh() const
{
	return HighBitLowConverter::ToInt(&Fourth.Page[6]);
}

BYTE DiControl::DiDataInput::FontFamily() const
{
	return Second.Page[12];
}

BYTE DiControl::DiDataInput::FontSize() const
{
	return Second.Page[13];
}

CString DiControl::DiDataInput::Name() const
{
	return _T("Data Input");
}

void DiControl::DiDataInput::FontSize(BYTE val)
{
	Second.Page[13];
}

void DiControl::DiDataInput::FontFamily(BYTE val)
{
	Second.Page[12] = val;
}

void DiControl::DiDataInput::RangeHigh(int val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[6]);
}

void DiControl::DiDataInput::RangeLow(int val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[2]);
}

void DiControl::DiDataInput::EnableRange(bool val)
{
	Fourth.Page[1] = val ? 0xFF : 0;
}

void DiControl::DiDataInput::LoadData(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[13]);
}

void DiControl::DiDataInput::LoadDataAddr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Fourth.Page[11]);
}

void DiControl::DiDataInput::LoadDataStyle(BYTE val)
{
	Fourth.Page[10] = val;
}

void DiControl::DiDataInput::DisplayType(BYTE val)
{
	Second.Page[15] = val;
}

void DiControl::DiDataInput::VariableAddr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[1]);
}

void DiControl::DiDataInput::KeyBordDisplayY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[14]);
}

void DiControl::DiDataInput::KeyBordDisplayX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[12]);
}

void DiControl::DiDataInput::KeyBordAreaEndY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[10]);
}

void DiControl::DiDataInput::KeyBordAreaEndX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[8]);
}

void DiControl::DiDataInput::KeyBordAreaBeginY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[6]);
}

void DiControl::DiDataInput::KeyBordAreaBeginX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Third.Page[4]);
}

void DiControl::DiDataInput::KeyBordPage(BYTE val)
{
	Third.Page[3] = val;
}

void DiControl::DiDataInput::KeyBordPosition(BYTE val)
{
	Third.Page[1] = val;
}

void DiControl::DiDataInput::RenderColor(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[10]);
}

void DiControl::DiDataInput::DisplayY(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[8]);
}

void DiControl::DiDataInput::DisplayX(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[6]);
}

void DiControl::DiDataInput::DecimalCount(BYTE val)
{
	Second.Page[5] = val;
}

void DiControl::DiDataInput::IntegerCount(BYTE val)
{
	Second.Page[4] = val;
}

void DiControl::DiDataInput::DataType(BYTE val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[3]);
}
