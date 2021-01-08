#pragma once
#include "Visaul.h"
#include "DiControl.h"
#include "DiImagePage.h"
#include <map>
#include <list>

#include <memory>

using namespace std;

namespace DiControl
{
    class DiControlVisual : public Visaul
    {
    public:
        static DiControlVisual* CreateDiControlVisual(DiControlPri* ctrl);

        DiControlVisual(void);
        ~DiControlVisual(void);

        void SetControl(DiControlPri* ctrl);
        DiControlPri* GetControl() const;

        void Selected(bool selected);
        bool Selected() const;

        void UpdateControl();

		const Gdiplus::Color& BackgroundClr() const { return m_backgroundClr; }
		void BackgroundClr(Gdiplus::Color val) { m_backgroundClr = val; }
	protected:
        void OnRender(Graphics& dc) override;
        void OnSizeChanged(const Size& nSize) override;
        void OnMoved(const Point& pt) override;
    private:
        DiControlPri* m_control;
        bool m_selected;
		Gdiplus::Color m_backgroundClr;
    };


    class DiImagePageVisual : public ImageVisaul
    {
    public:
        DiImagePageVisual();
        virtual ~DiImagePageVisual();

        void SetImagePage(DiImagePage* imagePage);
        DiImagePage* GetImagePage() const;

        DiControlVisual* GetControlVisual(DiControlPri* ctrl) const;
        int GetControlVisualCount() const;

        int GetControlVisuals(std::list<DiControlVisual*>& visuals) const;

        void SelectedControl(DiControlPri* selCtrl, bool bMultiSel = false);
        DiControlVisual* GetSelectedVisual() const;

		int GetSelectedVisuals(std::list<DiControlVisual*>& visuals) const;
		int GetSelCount() const;

        void UpdateCtrlVisual(DiControlPri* ctrl) const;

        void UpdateImagePage();
    private:
        void Dispose();
    protected:
        void OnRender(Graphics& dc) override;
    private:
        DiImagePage* m_imagePage;
        map<DiControlPri*, std::shared_ptr<DiControlVisual>> m_ctrlVisuals;
		DiControlVisual* m_curSelDCtrlVisual;
    };
}

