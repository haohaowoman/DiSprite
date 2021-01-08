#include "StdAfx.h"
#include "DiButton.h"

using namespace DiControl;

DiControl::DiButton::DiButton()
{
	Second.CF = 0xFE;
	Left(10);
	Top(10);
	Right(130);
	Bottom(60);
	Data[32] = Data[33] = 0xFF;
	__super::ControlType(0x05);
}

DiControl::DiButton::~DiButton()
{
}

USHORT DiControl::DiButton::VariableAddr() const
{
	return HighBitLowConverter::ToUShort(&Second.Page[1]);
}

BYTE DiControl::DiButton::WriteStyle() const
{
	return Second.Page[3];
}

void DiControl::DiButton::WriteStyle(BYTE val)
{
	Second.Page[3] = val;
}

void DiControl::DiButton::VariableAddr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[1]);
}

SHORT DiControl::DiButton::KeyValue() const
{
	return HighBitLowConverter::ToShort(&Second.Page[4]);
}

void DiControl::DiButton::KeyValue(SHORT val)
{
	HighBitLowConverter::ToBytes(val, &Second.Page[4]);
}

CString DiControl::DiButton::Name() const
{
	return _T("°´Å¥");
}

long DiControl::DiButton::GetDataLength() const
{
	return 32;
}
