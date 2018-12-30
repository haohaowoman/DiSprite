#include "Stdafx.h"
#include "DiControlFactory.h"
#include "DiButton.h"
#include "DiDataInput.h"
#include "DiTextInput.h"
#include "DiQRCode.h"
#include "DiTextDisplay.h"
using namespace DiControl;

DiControlPri* DiControFactory::CreateControl(DiCtrlType type, const DiControlData* data)
{
	DiControlPri* dCtrl = nullptr;
	
	switch (type)
	{
	case DataInput:
		dCtrl = new DiDataInput();
		break;
	case Button:
		dCtrl = new DiButton();
		break;
	case TextInput:
		dCtrl = new DiTextInput();
		break;
	case GBKInput:
		break;
	case Edit:
		dCtrl = new DiDataDisplay();
		break;
	case Text:
		dCtrl = new DiTextBlock();
		break;
	case RTC_Text:
		dCtrl = new DiRTCText();
		break;
	case QR_Code:
		dCtrl = new DiQRCode();
		break;
	default:
		break;

	}

	if (nullptr != dCtrl && nullptr != data)
	{
		dCtrl->operator()(*data);
	}
	return dCtrl;
}
