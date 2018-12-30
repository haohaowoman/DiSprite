#pragma once

namespace DiControl
{

#define PROP_DATA_LENGTH 0x80
#define PROP_DATA_PAGE_LENGTH 16
#define PROP_DATA_PAGE_COUNT 8

	struct PageData
	{
		union
		{
			BYTE Page[PROP_DATA_PAGE_LENGTH];
			BYTE CF;
		};
	};

	struct DiControlData
	{
		union
		{
			BYTE Data[PROP_DATA_LENGTH];
			struct
			{
				PageData Header;
				PageData Second;
				PageData Third;
				PageData Fourth;
			};
			
			PageData Pages[PROP_DATA_PAGE_COUNT];
		};

		BYTE End1;
		BYTE End2;
	};

	inline BOOL ValidDiControlData(const DiControlData& data)
	{
		BOOL bv = data.End1 == 0xFF;
		bv &= data.End2 == 0xFF;
		return bv || data.Header.CF == 0x5A;
	}

	class HighBitLowConverter
	{
	public:
		static SHORT ToShort(const BYTE* src);
		static USHORT ToUShort(const BYTE* src);
		static int ToInt(const BYTE* src);

		static void ToBytes(SHORT value, BYTE* dst);
		static void ToBytes(USHORT value, BYTE* dst);
		static void ToBytes(int value, BYTE* dst);

	};

	interface IControlBasic
	{
		virtual USHORT ScreenIndex() const = 0;
		virtual void ScreenIndex(USHORT index) = 0;

		virtual int Top() const = 0;
		virtual void Top(int top) = 0;

		virtual int Left() const = 0;
		virtual void Left(int left) = 0;

		virtual int Bottom() const = 0;
		virtual void Bottom(int bottom) = 0;

		virtual int Right() const = 0;
		virtual void Right(int right) = 0;

		virtual POINT Location() const = 0;
		virtual void Location(const POINT& pt) = 0;

		virtual SIZE Size() const = 0;
		virtual void Size(const SIZE& sz) = 0;

		virtual bool IsVoiceSwitch() const = 0;
		virtual void IsVoiceSwitch(bool check) = 0;


		virtual SHORT TogglePageIndex() const = 0;
		virtual void TogglePageIndex(SHORT pageIndex) = 0;

		virtual SHORT KeyEffect() const = 0;
		virtual void KeyEffect(SHORT key) = 0;

		virtual bool IsAutoUpdateData() const = 0;
		virtual void IsAutoUpdateData(bool isAuto) = 0;

		virtual BYTE ControlType() const = 0;
		virtual void ControlType(BYTE type) = 0;

	};

	class DiControlBase : public DiControlData, public IControlBasic
	{
	public:
		DiControlBase();
		virtual ~DiControlBase();

		explicit DiControlBase(const DiControlData& val);

		void operator()(const DiControlData& val);

		void From(const DiControlData& val);

		virtual CString Name() const = 0;

		// Overrides
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

		virtual bool IsVoiceSwitch() const;
		virtual void IsVoiceSwitch(bool check);

		virtual SHORT TogglePageIndex() const;
		virtual void TogglePageIndex(SHORT pageIndex);

		virtual SHORT KeyEffect() const;
		virtual void KeyEffect(SHORT key);

		virtual bool IsAutoUpdateData() const;
		virtual void IsAutoUpdateData(bool isAuto);

		virtual BYTE ControlType() const;
		virtual void ControlType(BYTE type);

		virtual POINT Location() const;

		virtual void Location(const POINT& pt);

		virtual SIZE Size() const;

		virtual void Size(const SIZE& sz);

	};
}


