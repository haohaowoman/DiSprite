#include "StdAfx.h"
#include "DiImagePage.h"

using namespace DiControl;

DiImagePage::DiImagePage(void): m_pageIndex(0)
{
}


DiImagePage::~DiImagePage(void)
{
	RemoveAll();
}

const vector<DiControlPri*>& DiControl::DiImagePage::GetControls() const
{
	return m_controls;
}

int DiControl::DiImagePage::GetControlCount() const
{
	return m_controls.size();
}

DiControlPri* DiControl::DiImagePage::GetControl(int index) const
{
	if (0 <= index && index < (int)m_controls.size())
	{
		return m_controls[index];
	}
	return nullptr;
}

int DiControl::DiImagePage::AddControl(DiControlPri* ctl)
{
	if (NULL != ctl)
	{
		m_controls.push_back(ctl);
        ctl->ScreenIndex(PageIndex());
		return m_controls.size() - 1;
	}
	return -1;
}

int DiControl::DiImagePage::InsertControl(int index, DiControlPri* ctl)
{
	if (NULL != ctl && 0 <= index && index < (int)m_controls.size())
	{
		m_controls.insert(m_controls.begin() + index, ctl);
        ctl->ScreenIndex(PageIndex());
		return index;
	}
	return -1;
}

int DiControl::DiImagePage::RemoveControl(int index)
{
	if (0 <= index && index < (int)m_controls.size())
	{
		DiControlPri* temp = m_controls[index];

		m_controls.erase(m_controls.begin() + index);
		delete temp;
		return index;
	}
	return -1;
}

int DiControl::DiImagePage::RemoveControl(DiControlPri* ctrl)
{
	if (nullptr == ctrl)
	{
		return -1;
	}
	int index = -1;
	auto it = m_controls.begin();
	for (; it != m_controls.end(); ++it)
	{
		if (ctrl == *it)
		{
			break;
		}
	}
	if (it != m_controls.end())
	{
		index = it - m_controls.begin();
		delete *it;
		m_controls.erase(it);
	}
	
	return index;
}

void DiControl::DiImagePage::RemoveAll()
{
	for (int i = 0; i < (int)m_controls.size(); i++)
	{
		auto ptr = m_controls[i];
		delete ptr;
	}
	m_controls.clear();
}

