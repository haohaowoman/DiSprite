#pragma once

namespace DiControl
{
	interface IFontSize
	{
		virtual BYTE FontSize() const = 0;
		virtual void FontSize(BYTE val) = 0;
	};

	interface IFontFamily
	{
		virtual BYTE FontFamily() const = 0;
		virtual void FontFamily(BYTE val) = 0;
	};
}