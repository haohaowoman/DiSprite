#include "StdAfx.h"
#include "DiQRCode.h"
#include "DiAssist.h"
using namespace DiControl;

DiControl::DiQRCode::DiQRCode(void)
{
	SIZE sz = {120, 120};
	Size(sz);
	ControlType(static_cast<BYTE>(DiCtrlType::QR_Code));
}

DiControl::DiQRCode::~DiQRCode(void)
{

}

BYTE DiControl::DiQRCode::UnitPixel() const
{
	return Header.Page[13];
}

CString DiControl::DiQRCode::Name() const
{
	return _T("QR Code");
}

void DiControl::DiQRCode::UnitPixel(BYTE val)
{
	Header.Page[13] = val;
}
