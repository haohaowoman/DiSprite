#pragma once
#include "DiControl.h"

namespace DiControl
{
	class DiQRCode : public DiDisplayCtrlPri
	{
	public:
		DiQRCode(void);
		virtual ~DiQRCode(void);

		virtual BYTE UnitPixel() const;
		virtual void UnitPixel(BYTE val);

		virtual CString Name() const;

	};

}

