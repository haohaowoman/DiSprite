#pragma once
#include "DiControl.h"
#include "DiAssist.h"
namespace DiControl
{
	class DiControFactory
	{
	public:
		static DiControlPri* CreateControl(DiCtrlType type, const DiControlData* data = nullptr);
	};

}