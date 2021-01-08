#pragma once
#include "DiControl.h"
#include <vector>
using namespace std;
namespace DiControl
{
	class DiImagePage
	{
	public:
		DiImagePage(void);
		~DiImagePage(void);

		const CString& ImagePath() const { return m_imagePath; }
		void ImagePath(const CString& val) { m_imagePath = val; }

		int PageIndex() const { return m_pageIndex; }
		void PageIndex(int val) { m_pageIndex = val; }

		const vector<DiControlPri*>& GetControls() const;

		int GetControlCount() const;
		DiControlPri* GetControl(int index) const;
		int AddControl(DiControlPri* ctl);
		int InsertControl(int index, DiControlPri* ctl);

		int RemoveControl(int index);
		int RemoveControl(DiControlPri* ctrl);
		void RemoveAll();

	private:
		
		CString m_imagePath;

		int m_pageIndex;

		 vector<DiControlPri*> m_controls;
		
	};
}

