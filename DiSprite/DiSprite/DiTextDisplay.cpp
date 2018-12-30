#include "StdAfx.h"
#include "DiTextDisplay.h"
#include "DiAssist.h"

DiControl::DiRTCText::DiRTCText()
{
	ControlType(static_cast<BYTE>(DiCtrlType::RTC_Text));
	SIZE sz = {120, 60};
	Size(sz);
}

DiControl::DiRTCText::~DiRTCText()
{

}

BYTE DiControl::DiRTCText::XPointCount() const
{
	return Header.Page[15];
}

CString DiControl::DiRTCText::Name() const
{
	return _T("RTC Text");
}

BYTE DiControl::DiRTCText::FontFamily() const
{
	return Header.Page[14];
}

USHORT DiControl::DiRTCText::RenderColor() const
{
	return HighBitLowConverter::ToUShort(&Header.Page[12]);
}

void DiControl::DiRTCText::RenderColor(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Header.Page[12]);
}

void DiControl::DiRTCText::FontFamily(BYTE val)
{
	Header.Page[14] = val;
}

void DiControl::DiRTCText::XPointCount(BYTE val)
{
	Header.Page[15] = val;
}

DiControl::DiDataDisplay::DiDataDisplay()
{
	ControlType(static_cast<BYTE>(DiCtrlType::Edit));
	SIZE sz = {120, 60};
	Size(sz);
}

DiControl::DiDataDisplay::~DiDataDisplay()
{

}

CString DiControl::DiDataDisplay::Name() const
{
	return _T("Edit");
}

USHORT DiControl::DiDataDisplay::RenderColor() const
{
	return HighBitLowConverter::ToUShort(&Header.Page[12]);
}

BYTE DiControl::DiDataDisplay::FontFamily() const
{
	return Header.Page[14];
}

BYTE DiControl::DiDataDisplay::FontSize() const
{
	return Header.Page[15];
}

BYTE DiControl::DiDataDisplay::DataType() const
{
	return Second.Page[3];
}

BYTE DiControl::DiDataDisplay::IntegerCount() const
{
	return Second.Page[1];
}

BYTE DiControl::DiDataDisplay::DecimalCount() const
{
	return Second.Page[2];
}

BYTE DiControl::DiDataDisplay::Agliment() const
{
	return Second.Page[0];
}

void DiControl::DiDataDisplay::Agliment(BYTE val)
{
	Second.Page[0] = val;
}

void DiControl::DiDataDisplay::DecimalCount(BYTE val)
{
	Second.Page[2] = val;
}

void DiControl::DiDataDisplay::IntegerCount(BYTE val)
{
	Second.Page[1] = val;
}

void DiControl::DiDataDisplay::DataType(BYTE val)
{
	Second.Page[3] = val;
}

void DiControl::DiDataDisplay::FontSize(BYTE val)
{
	Header.Page[15] = val;
}

void DiControl::DiDataDisplay::FontFamily(BYTE val)
{
	Header.Page[14] = val;
}

void DiControl::DiDataDisplay::RenderColor(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Header.Page[12]);
}

DiControl::DiTextBlock::DiTextBlock()
{
	ControlType(static_cast<BYTE>(DiCtrlType::Text));
	SIZE sz = {120, 60};
	Size(sz);
	IsNotAutoAdjustCharsetDis(true);
}

DiControl::DiTextBlock::~DiTextBlock()
{

}

int DiControl::DiTextBlock::Bottom() const
{
	return (int)HighBitLowConverter::ToShort(&Second.Page[4]);
}

int DiControl::DiTextBlock::Right() const
{
	return (int)HighBitLowConverter::ToShort(&Second.Page[2]);
}

CString DiControl::DiTextBlock::Name() const
{
	return _T("Text block");
}

USHORT DiControl::DiTextBlock::RenderColor() const
{
	return HighBitLowConverter::ToUShort(&Header.Page[12]);
}

BYTE DiControl::DiTextBlock::FontFamily() const
{
	return Second.Page[8];
}

BYTE DiControl::DiTextBlock::FontFamily1() const
{
	return Second.Page[9];
}

void DiControl::DiTextBlock::FontFamily1(BYTE val)
{
	Second.Page[9] = val;
}

BYTE DiControl::DiTextBlock::TextLength() const
{
	return Second.Page[7];
}

BYTE DiControl::DiTextBlock::XPointCount() const
{
	return Second.Page[10];
}

BYTE DiControl::DiTextBlock::YPointCount() const
{
	return Second.Page[11];
}

bool DiControl::DiTextBlock::IsNotAutoAdjustCharsetDis() const
{
	return (Second.Page[12] & 0x80) == 0x80;
}

BYTE DiControl::DiTextBlock::CodingStyle() const
{
	return Second.Page[12] & 0x7F;
}

BYTE DiControl::DiTextBlock::HorizontalMargin() const
{
	return Second.Page[13];
}

BYTE DiControl::DiTextBlock::VerticalMargin() const
{
	return Second.Page[14];
}

void DiControl::DiTextBlock::Left(int left)
{
	__super::Left(left);
	HighBitLowConverter::ToBytes((SHORT)left, &Header.Page[14]);
}

void DiControl::DiTextBlock::Top(int top)
{
	__super::Top(top);
	HighBitLowConverter::ToBytes((SHORT)top, &Second.Page[0]);
}

void DiControl::DiTextBlock::VerticalMargin(BYTE val)
{
	Second.Page[14] = val;
}

void DiControl::DiTextBlock::HorizontalMargin(BYTE val)
{
	Second.Page[13] = val;
}

void DiControl::DiTextBlock::CodingStyle(BYTE val)
{
	Second.Page[12] |= val & 0x7F;
}

void DiControl::DiTextBlock::IsNotAutoAdjustCharsetDis(bool val)
{
	Second.Page[12] |= val ? 0x80 : 0;
}

void DiControl::DiTextBlock::YPointCount(BYTE val)
{
	Second.Page[11] = val;
}

void DiControl::DiTextBlock::XPointCount(BYTE val)
{
	Second.Page[10] = val;
}

void DiControl::DiTextBlock::TextLength(BYTE val)
{
	Second.Page[7] = val;
}

void DiControl::DiTextBlock::FontFamily(BYTE val)
{
	Second.Page[8] = val;
}

void DiControl::DiTextBlock::RenderColor(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Header.Page[12]);
}

void DiControl::DiTextBlock::Right(int right)
{
	HighBitLowConverter::ToBytes((SHORT)right, &Second.Page[2]);
}

void DiControl::DiTextBlock::Bottom(int bottom)
{
	HighBitLowConverter::ToBytes((SHORT)bottom, &Second.Page[4]);
}
