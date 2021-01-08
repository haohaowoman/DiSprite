#pragma once

namespace DiControl
{
	interface IDataDisplayPos
	{
		virtual SHORT DisplayX() const = 0;
		virtual void DisplayX(SHORT val) = 0;

		virtual SHORT DisplayY() const = 0;
		virtual void DisplayY(SHORT val) = 0;
	};

	interface IRenderColor
	{
		virtual USHORT RenderColor() const = 0;
		virtual void RenderColor(USHORT val) = 0;
	};

	interface IDataDisplay
	{
		virtual BYTE DataType() const = 0;
		virtual void DataType(BYTE val) = 0;

		virtual BYTE IntegerCount() const = 0;
		virtual void IntegerCount(BYTE val) = 0;

		virtual BYTE DecimalCount() const = 0;
		virtual void DecimalCount(BYTE val) = 0;
	};

	interface IDataInput
	{
		virtual BYTE CursorColor() const = 0;
		virtual void CursorColor(BYTE val) = 0;

		virtual BYTE DisplayType() const = 0;
		virtual void DisplayType(BYTE val) = 0;
	};

	interface IDataLoad
	{
		virtual BYTE LoadDataStyle() = 0;
		virtual void LoadDataStyle(BYTE val) = 0;

		virtual USHORT LoadDataAddr() const = 0;
		virtual void LoadDataAddr(USHORT val) = 0;

		virtual SHORT LoadData() const = 0;
		virtual void LoadData(SHORT val) = 0;

	};

	interface IAgliment
	{
		virtual BYTE Agliment() const = 0;
		virtual void Agliment(BYTE val) = 0;
	};

	interface IRangeSourport
	{
		virtual bool EnableRange() const = 0;
		virtual void EnableRange(bool val) = 0;

		virtual int RangeLow() const = 0;
		virtual void RangeLow(int val) = 0;

		virtual int RangeHigh() const = 0;
		virtual void RangeHigh(int val) = 0;

	};
}