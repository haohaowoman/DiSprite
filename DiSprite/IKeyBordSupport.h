#pragma once

namespace DiControl
{
	interface IKeyBordSupport
	{
		virtual BYTE KeyBordPosition() const = 0;
		virtual void KeyBordPosition(BYTE val) = 0;

		virtual BYTE KeyBordPage() const = 0;
		virtual void KeyBordPage(BYTE val) = 0;

		virtual SHORT KeyBordAreaBeginX() const = 0;
		virtual void KeyBordAreaBeginX(SHORT val) = 0;

		virtual SHORT KeyBordAreaBeginY() const = 0;
		virtual void KeyBordAreaBeginY(SHORT val) = 0;

		virtual SHORT KeyBordAreaEndX() const = 0;
		virtual void KeyBordAreaEndX(SHORT val) = 0;

		virtual SHORT KeyBordAreaEndY() const = 0;
		virtual void KeyBordAreaEndY(SHORT val) = 0;

		virtual SHORT KeyBordDisplayX() const = 0;
		virtual void KeyBordDisplayX(SHORT val) = 0;

		virtual SHORT KeyBordDisplayY() const = 0;
		virtual void KeyBordDisplayY(SHORT val) = 0;

	};

}