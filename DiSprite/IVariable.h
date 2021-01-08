#pragma once

namespace DiControl
{
	interface IVariable
	{
		virtual USHORT VariableAddr() const = 0;
		virtual void VariableAddr(USHORT val) = 0;
	};
}