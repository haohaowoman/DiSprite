#pragma once

namespace DiControl
{
    template<class tPtr>
    inline void SafeDelete(tPtr** pptr)
    {
        if (nullptr != pptr && nullptr != *pptr)
        {
            delete *pptr;
            *pptr = nullptr;
        }
    }

    enum DiCtrlType
    {
        Unkown = static_cast<BYTE>(-1),
		// Input controls.
        DataInput = 0,
        Button = 5,
        TextInput = 6,
        GBKInput = 7,

		// Display controls.
		Edit = 0x10,
		Text = 0x11,
		RTC_Text = 0x12,
		QR_Code = 0x25
    };
}