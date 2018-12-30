#include "StdAfx.h"
#include "DiButton.h"

using namespace DiControl;

DiControl::DiButton::DiButton()
{
	Second.CF = 0xFE;
	VariableAddr(0x0609);
	KeyValue(0x0608);
	Left(10);
	Top(10);
	Right(90);
	Bottom(60);
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
	return _T("Button");
}
