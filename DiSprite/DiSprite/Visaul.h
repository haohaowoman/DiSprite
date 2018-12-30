#pragma once

namespace DiControl
{
	enum /*class*/ HitArea
	{
        NotHit,
		Tranprant,
		Client,
		LeftTopCorner,
		LeftBottomCorner,
		RightTopCorner,
		RightBottomCorner,
		LeftMargin,
		TopMargin,
		RightMargin,
		BottomMargin
	};
	
	struct Thickness
	{
		int Left;
		int Top;
		int Right;
		int Bottom;

		Thickness()
		{
			Left = Top = Right = Bottom = 0;
		}

        Thickness(int t)
		{
			Left = Top = Right = Bottom = t;
		}

        Thickness(int lt, int rb)
		{
			Left = Top = lt;
		    Right = Bottom = rb;
		}
	};

	class Visaul
	{
	public:
		Visaul(void);
		virtual ~Visaul(void);

		void Render(Graphics& dc);

		Size GetSize() const;
		void SetSize(const Size& size);
		Point GetPosition() const;
		void SetPostion(const Point& pos);
		const Rect& GetLayout() const;
		void SetLayout(const Rect& layout); 

		void SetMargin(const Thickness& margin);
		const Thickness& GetMargin() const;

		Rect GetClientArea() const;

		HitArea HitTest(const Point& pt) const;
	protected:
		virtual void OnRender(Graphics& dc) = 0;

		virtual void OnSizeChanged(const Size& nSize);
		virtual void OnMoved(const Point& pt);
		virtual void OnMarginChanged(const Thickness& margin);


		virtual HitArea OnHitTest(const Point& pt) const;

        Rect AdjustHitArea(HitArea area) const;
	private:
		Rect m_layout;
		Thickness m_margin;
	};

	class ImageVisaul : public Visaul
	{
	public:
		ImageVisaul();
		virtual ~ImageVisaul();

		void SetImageUri(const CString& uri);
		const CString& GetImageUri() const;

		Size GetImageSize() const;

		
	protected:
		virtual void OnRender(Graphics& dc);
	private:
		Gdiplus::Image* m_image;
		CString m_imagePath;
	};
}


