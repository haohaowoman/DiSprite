#pragma once
#include "IVariable.h"
#include "DiControl.h"

namespace DiControl
{
	interface IWriteKeyValue
	{
		virtual BYTE WriteStyle() const = 0;
		virtual void WriteStyle(BYTE val) = 0;

		virtual SHORT KeyValue() const = 0;
		virtual void KeyValue(SHORT val) = 0;
	};

	class DiButton : public DiControlPri, public IVariable, public IWriteKeyValue
	{
	public:
		DiButton();
		virtual ~DiButton();

		virtual USHORT VariableAddr() const;
		virtual void VariableAddr(USHORT val);

		virtual BYTE WriteStyle() const;
		virtual void WriteStyle(BYTE val);

		virtual SHORT KeyValue() const;
		virtual void KeyValue(SHORT val);

		//Override

	public:
		virtual CString Name() const;

		virtual long GetDataLength() const;

	};
}