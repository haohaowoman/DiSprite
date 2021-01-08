#include "StdAfx.h"
#include "DiControlData.h"

DiControl::DiControlBase::DiControlBase()
{
	memset(Data, 0, sizeof(DiControlData));
	IsAutoUpdateData(true);
	TogglePageIndex(-1);
	KeyEffect(-1);
	Top(0);
	Left(0);
	Right(120);
	Bottom(60);

	Data[64] = Data[65] = 0xFF;

	End1 = 0xFF;
	End2 = 0xFF;
}

DiControl::DiControlBase::~DiControlBase()
{
	
}

DiControl::DiControlBase::DiControlBase(const DiControlData& val)
{
	memcpy(Data, val.Data, sizeof(DiControlData));
}

USHORT DiControl::DiControlBase::ScreenIndex() const
{
	return HighBitLowConverter::ToUShort(Header.Page);
}

void DiControl::DiControlBase::ScreenIndex(USHORT index)
{
	HighBitLowConverter::ToBytes(index, Header.Page);
}

int DiControl::DiControlBase::Top() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[4]);
}

void DiControl::DiControlBase::Top(int top)
{
	HighBitLowConverter::ToBytes((SHORT)top, &Header.Page[4]);
}

int DiControl::DiControlBase::Left() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[2]);
}

void DiControl::DiControlBase::Left(int left)
{
	HighBitLowConverter::ToBytes((SHORT)(left), &Header.Page[2]);
}

int DiControl::DiControlBase::Bottom() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[8]);
}

void DiControl::DiControlBase::Bottom(int bottom)
{
	HighBitLowConverter::ToBytes((SHORT)bottom, &Header.Page[8]);
}

int DiControl::DiControlBase::Right() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[6]);
}

void DiControl::DiControlBase::Right(int right)
{
	HighBitLowConverter::ToBytes((SHORT)right, &Header.Page[6]);
}

bool DiControl::DiControlBase::IsVoiceSwitch() const
{
	return (Header.Page[2] >> 4) == 5;
}

void DiControl::DiControlBase::IsVoiceSwitch(bool check)
{
	if (check)
	{
		Header.Page[2] &= 0x5f;
	}
	else
	{
		Header.Page[2] &= 0xaf;
	}
}

SHORT DiControl::DiControlBase::TogglePageIndex() const
{
	return HighBitLowConverter::ToShort(&Header.Page[10]);
}

void DiControl::DiControlBase::TogglePageIndex(SHORT pageIndex)
{
	HighBitLowConverter::ToBytes(pageIndex, &Header.Page[10]);
}

SHORT DiControl::DiControlBase::KeyEffect() const
{
	return HighBitLowConverter::ToShort(&Header.Page[12]);
}

void DiControl::DiControlBase::KeyEffect(SHORT key)
{
	HighBitLowConverter::ToBytes(key, &Header.Page[12]);
}

bool DiControl::DiControlBase::IsAutoUpdateData() const
{
	return Header.Page[14] == 0xFE;
}

void DiControl::DiControlBase::IsAutoUpdateData(bool isAuto)
{
	Header.Page[14] = isAuto ? 0xFE : 0xFD;
}

BYTE DiControl::DiControlBase::ControlType() const
{
	return Header.Page[15];
}

POINT DiControl::DiControlBase::Location() const
{
	POINT pt = {(LONG)Left(), (LONG)Top()};
	return pt;
}

SIZE DiControl::DiControlBase::Size() const
{
	SIZE sz;
	sz.cx = Right() - Left();
	sz.cy = Bottom() - Top();
	return sz;
}

int DiControl::DiControlBase::Width() const
{
	return Right() - Left();
}

int DiControl::DiControlBase::Height() const
{
	return Bottom() - Top();
}

void DiControl::DiControlBase::Height(int val)
{
	Bottom(Top() + val);
}

void DiControl::DiControlBase::Width(int val)
{
	Right(Left() + val);
}

void DiControl::DiControlBase::Size(const SIZE& sz)
{
	Right(Left() + sz.cx);
	Bottom(Top() + sz.cy);
}

void DiControl::DiControlBase::Location(const POINT& pt)
{
	SIZE sz = Size();
	Left(pt.x);
	Top(pt.y);
	Size(sz);
}

void DiControl::DiControlBase::ControlType(BYTE type)
{
	Header.Page[15] = type;
}

void DiControl::DiControlBase::operator()(const DiControlData& val)
{
	memcpy(Data, val.Data, sizeof(DiControlData));
}

void DiControl::DiControlBase::From(const DiControlData& val)
{
	this->operator()(val);
}

SHORT DiControl::HighBitLowConverter::ToShort(const BYTE* src)
{
	return (SHORT)((src[0] << 8)|(src[1]));
}

USHORT DiControl::HighBitLowConverter::ToUShort(const BYTE* src)
{
	return (USHORT)((src[0] << 8)|(src[1]));
}

int DiControl::HighBitLowConverter::ToInt(const BYTE* src)
{
	return (int)((src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3]);
}

void DiControl::HighBitLowConverter::ToBytes(SHORT value, BYTE* dst)
{
	dst[0] = (value >> 8) & 0xff;
	dst[1] = value & 0xff;
}

void DiControl::HighBitLowConverter::ToBytes(int value, BYTE* dst)
{
	dst[0] = (value >> 24) & 0xff;
	dst[1] = (value >> 16) & 0xff;
	dst[2] = (value >> 8) & 0xff;
	dst[3] = (value) & 0xff;
}

void DiControl::HighBitLowConverter::ToBytes(USHORT value, BYTE* dst)
{
	dst[0] = (value >> 8) & 0xff;
	dst[1] = value & 0xff;
}
