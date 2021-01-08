#pragma once

namespace DiControl
{
	interface ITextInput
	{
		virtual BYTE InputMode() const = 0;
		virtual void InputMode(BYTE val) = 0;
		
		virtual SHORT InputDisplayAreaX() const = 0;
		virtual void InputDisplayAreaX(SHORT val) = 0;

		virtual SHORT InputDisplayAreaY() const = 0;
		virtual void InputDisplayAreaY(SHORT val) = 0;

		virtual bool IsSelInputStateReturn() const = 0;
		virtual void IsSelInputStateReturn(bool val) = 0;

		virtual SHORT InputDisplayAreaX_End() const = 0;
		virtual void InputDisplayAreaX_End(SHORT val) = 0;

		virtual SHORT InputDisplayAreaY_End() const = 0;
		virtual void InputDisplayAreaY_End(SHORT val) = 0;
	};

	interface ITextDisplay
	{
		virtual BYTE TextLength() const = 0;
		virtual void TextLength(BYTE val) = 0;

		virtual BYTE XPointCount() const = 0;
		virtual void XPointCount(BYTE val) = 0;

		virtual BYTE YPointCount() const = 0;
		virtual void YPointCount(BYTE val) = 0;

	};
}