#pragma once
#include "DiControlData.h"
#include <vector>
#include "IVariable.h"

namespace DiControl
{
	/*
	* Indicate the primitive di control.
	* The control data can access a bin file.
	*/
	class DiControlPri : public DiControlBase
	{
	public:
		// Write Control data to a bin file.

		static BOOL WriteControls(const CString& path, DiControlData* controls, int count);
		static BOOL ReadControls(const CString& path, std::vector<DiControlData> controls);

		static BOOL ReadControl(const CString& path, DiControlData& control, long validDataLen);
		static BOOL WriteControl(const CString& path, DiControlData& control, long validDataLen);

		DiControlPri();
		virtual~ DiControlPri();

		void Path(const CString& path);
		const CString& Path();

		virtual long GetDataLength() const;

		BOOL Read();
		BOOL Write();

	private:
		CString m_path;

	};

	class DiDisplayCtrlPri : public DiControlPri, public IVariable
	{
	public:
		DiDisplayCtrlPri();
		virtual ~DiDisplayCtrlPri();

		virtual long GetDataLength() const;

		virtual USHORT ScreenIndex() const;

		virtual void ScreenIndex(USHORT index);

		virtual int Top() const;

		virtual void Top(int top);

		virtual int Left() const;

		virtual void Left(int left);

		virtual int Bottom() const;

		virtual void Bottom(int bottom);

		virtual int Right() const;

		virtual void Right(int right);

		virtual BYTE ControlType() const;

		virtual void ControlType(BYTE type);

		virtual USHORT VariableAddr() const;

		virtual void VariableAddr(USHORT val);

		virtual USHORT DescriptionPtr() const;
		virtual void DescriptionPtr(USHORT val);

		virtual CString Name() const;

		virtual bool IsVoiceSwitch() const;

		virtual void IsVoiceSwitch(bool check);

		virtual SHORT TogglePageIndex() const;

		virtual void TogglePageIndex(SHORT pageIndex);

		virtual SHORT KeyEffect() const;

		virtual void KeyEffect(SHORT key);

		virtual bool IsAutoUpdateData() const;

		virtual void IsAutoUpdateData(bool isAuto);

	private:
		USHORT m_screenIndex;
		int m_defaultRight;
		int m_defaultBottom;
	};
}