#include "Stdafx.h"
#include "DiControl.h"

using namespace DiControl;

BOOL DiControl::DiControlPri::WriteControls(const CString& path, DiControlData* controls, int count)
{
	if (NULL == controls || count <= 0)
	{
		return FALSE;
	}

	CFile file;
	BOOL bsuc = FALSE;
	if (file.Open(path, CFile::modeWrite|CFile::modeCreate))
	{
		for (int i = 0; i < count; i++)
		{
			file.Write((void*)&controls[i].Data, sizeof(DiControlData));
		}
		file.Close();
	}
	return bsuc;
}

BOOL DiControl::DiControlPri::ReadControls(const CString& path, std::vector<DiControlData> controls)
{
	CFile file;
	BOOL bsuc = FALSE;
	if (file.Open(path, CFile::modeRead))
	{
		ULONGLONG len = file.GetLength();
		int count = (int)len / sizeof(DiControlData);
		if (count >= 1 && len % sizeof(DiControlData) == 0)
		{
			for (int i = 0; i < count; i++)
			{
				DiControlData data;
				if (sizeof(DiControlData) != file.Read((void*)&data.Data, sizeof(DiControlData)))
				{
					break;
				}
				controls.push_back(data);
			}
		}
		file.Close();
	}

	return bsuc;
}

BOOL DiControl::DiControlPri::ReadControl(const CString& path, DiControlData& control, long validDataLen)
{
	CFile file;
	BOOL bsuc = FALSE;
	if (file.Open(path, CFile::modeRead|CFile::typeBinary) && file.GetLength() >= validDataLen)
	{
		UINT rl = file.Read((void*)&control.Data, validDataLen);
		bsuc = (rl == validDataLen && ValidDiControlData(control));
		file.Close();
	}
	return bsuc;
}

BOOL DiControl::DiControlPri::WriteControl(const CString& path, DiControlData& control, long validDataLen)
{
	CFile file;
	BOOL bsuc = FALSE;
	if (file.Open(path, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary))
	{
		file.Write((void *)&control.Data, validDataLen);
		file.Close();
		bsuc = TRUE;
	}
	return bsuc;
}

DiControl::DiControlPri::DiControlPri()
{
	
}

DiControl::DiControlPri::~DiControlPri()
{

}

void DiControl::DiControlPri::Path(const CString& path)
{
	m_path = path;
}

long DiControl::DiControlPri::GetDataLength() const
{
	return sizeof(DiControlData);
}

BOOL DiControl::DiControlPri::Read()
{
	return DiControlPri::ReadControl(Path(), *this, GetDataLength());
}

BOOL DiControl::DiControlPri::Write()
{
	return DiControlPri::WriteControl(Path(), *this, GetDataLength());
}

const CString& DiControl::DiControlPri::Path()
{
	return m_path;
}

DiControl::DiDisplayCtrlPri::DiDisplayCtrlPri()
{
	memset(Data, 0, GetDataLength());
	Header.CF = 0x5A;
}

DiControl::DiDisplayCtrlPri::~DiDisplayCtrlPri()
{
	
}

long DiControl::DiDisplayCtrlPri::GetDataLength() const
{
	return sizeof(PageData) * 2;
}

USHORT DiControl::DiDisplayCtrlPri::ScreenIndex() const
{
	return m_screenIndex;
}

int DiControl::DiDisplayCtrlPri::Top() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[10]);
}

int DiControl::DiDisplayCtrlPri::Left() const
{
	return (int)HighBitLowConverter::ToShort(&Header.Page[8]);
}

int DiControl::DiDisplayCtrlPri::Bottom() const
{
	if (m_defaultBottom < Top())
	{
		return Top() + 60;
	}
	return m_defaultBottom;
}

int DiControl::DiDisplayCtrlPri::Right() const
{
	if (m_defaultRight < Left())
	{
		return Left() + 120;
	}
	return m_defaultRight;
}

BYTE DiControl::DiDisplayCtrlPri::ControlType() const
{
	return Header.Page[1];
}

USHORT DiControl::DiDisplayCtrlPri::VariableAddr() const
{
	return HighBitLowConverter::ToUShort(&Header.Page[6]);
}

USHORT DiControl::DiDisplayCtrlPri::DescriptionPtr() const
{
	return HighBitLowConverter::ToUShort(&Header.Page[2]);
}

CString DiControl::DiDisplayCtrlPri::Name() const
{
	return _T("Display Control");
}

bool DiControl::DiDisplayCtrlPri::IsVoiceSwitch() const
{
	return false;
}

SHORT DiControl::DiDisplayCtrlPri::TogglePageIndex() const
{
	return 0;
}

SHORT DiControl::DiDisplayCtrlPri::KeyEffect() const
{
	return 0;
}

bool DiControl::DiDisplayCtrlPri::IsAutoUpdateData() const
{
	return false;
}

void DiControl::DiDisplayCtrlPri::IsAutoUpdateData(bool isAuto)
{
}

void DiControl::DiDisplayCtrlPri::KeyEffect(SHORT key)
{
}

void DiControl::DiDisplayCtrlPri::TogglePageIndex(SHORT pageIndex)
{
}

void DiControl::DiDisplayCtrlPri::IsVoiceSwitch(bool check)
{
}

void DiControl::DiDisplayCtrlPri::DescriptionPtr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Header.Page[2]);
}

void DiControl::DiDisplayCtrlPri::VariableAddr(USHORT val)
{
	HighBitLowConverter::ToBytes(val, &Header.Page[6]);
}

void DiControl::DiDisplayCtrlPri::ControlType(BYTE type)
{
	Header.Page[1] = type;
}

void DiControl::DiDisplayCtrlPri::Right(int right)
{
	m_defaultRight = right;
}

void DiControl::DiDisplayCtrlPri::Bottom(int bottom)
{
	m_defaultBottom = bottom;
}

void DiControl::DiDisplayCtrlPri::Left(int left)
{
	HighBitLowConverter::ToBytes((SHORT)left, &Header.Page[8]);
}

void DiControl::DiDisplayCtrlPri::Top(int top)
{
	HighBitLowConverter::ToBytes((SHORT)top, &Header.Page[10]);
}

void DiControl::DiDisplayCtrlPri::ScreenIndex(USHORT index)
{
	m_screenIndex = index;
}
