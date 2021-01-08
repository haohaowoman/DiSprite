#include "StdAfx.h"
#include "DiProject.h"
#include <atlpath.h>
#include "DiControlFactory.h"

#import <msxml3.dll> named_guids

using namespace DiDocument;

const LPCTSTR Project_Node = _T("Project");
const LPCTSTR Project_InputCtrls_File_Node = _T("InputCtrls");
const LPCTSTR Project_DisplayCtrls_File_Node = _T("DisplayCtrls");
const LPCTSTR Pages_Node = _T("Pages");
const LPCTSTR Page_Node = _T("Page");
const LPCTSTR ImagePath_Node = _T("ImagePath");
const LPCTSTR Index_Node = _T("Index");
const LPCTSTR DiControl_Node = _T("DiControl");
const LPCTSTR Type_Node = _T("Type");
const LPCTSTR File_Node = _T("File");

const LPCTSTR ScWidth_Node = _T("ScWidth");
const LPCTSTR ScHeight_Node = _T("ScHeight");

const LPCTSTR InputControlsFileName = _T("_InputControls.bin");
const LPCTSTR DisplayControlsFileName = _T("_DisplayControls.bin");

#define DisplayPageDatalength (0x800)

DiProject::DiProject(void)
{
	m_screenSize.cx = 272;
	m_screenSize.cy = 480;
}


DiProject::~DiProject(void)
{
	Dispose();
}

const CString& DiDocument::DiProject::GetProjectFilePath() const
{
	return m_projectFilePath;
}

void DiDocument::DiProject::SetProjectFilePath(const CString& path)
{
	m_projectFilePath = path;
}

CString DiDocument::DiProject::Name() const
{

	CString tempStr = ::PathFindFileName(m_projectFilePath);

	if (tempStr.IsEmpty())
	{
		tempStr = _T("***ÐÂÏîÄ¿***");
	}
	else
	{
		tempStr = tempStr.Trim(::PathFindExtension(tempStr));
	}
	return tempStr;
}

int DiDocument::DiProject::GetImagePagesCount() const
{
	return m_images.size();
}

DiImagePage* DiDocument::DiProject::GetImagePage(int index) const
{
	int count = m_images.size();
	if (0 <= index && index < count)
	{
		return m_images[index];
	}
	return nullptr;
}

int DiDocument::DiProject::AddImagePage(DiImagePage* pImage)
{
	if (NULL != pImage)
	{
		m_images.push_back(pImage);
		return m_images.size() - 1;
	}
	return -1;
}

int DiDocument::DiProject::InsertImagePage(int index, DiImagePage* pImage)
{
	int count = m_images.size();
	if (0 <= index && index < count && NULL != pImage)
	{
		m_images.insert(m_images.begin() + index, pImage);
		return index;
	}
	return -1;
}

int DiDocument::DiProject::RemoveImagePage(int index)
{
	int count = m_images.size();
	if (0 <= index && index < count)
	{
		DiImagePage* iPage = m_images[index];
		m_images.erase(m_images.begin() + index);
		if (NULL != iPage)
		{
			delete iPage;
		}
	}
	return -1;
}

int DiDocument::DiProject::RemoveImagePage(DiImagePage* pImage)
{
	if (nullptr == pImage)
	{
		return -1;
	}
	int index = -1;
	auto it = m_images.begin();
	for (; it != m_images.end(); ++it)
	{
		if (pImage == *it)
		{
			break;
		}
	}
	if (it != m_images.end())
	{
		index = it - m_images.begin();
		delete *it;
		m_images.erase(it);
	}

	return index;
}

void DiDocument::DiProject::RemoveAll()
{
	for (int i = 0; i < (int)m_images.size(); i++)
	{
		auto ptr = m_images[i];
		delete ptr;
	}
	m_images.clear();
}

const std::vector<DiImagePage*>& DiDocument::DiProject::GetImagePages() const
{
	return m_images;
}

BOOL DiDocument::DiProject::OpenFile(const CString& projectFilePath)
{
	CPath path(projectFilePath);
	HRESULT hr = S_OK;
	if (path.FileExists() && path.GetExtension() == _T(".disln"))
	{
		RemoveAll();

		int index = path.FindFileName();
		CString tempDic = projectFilePath.Mid(0, index);

		CString setDir = tempDic + _T("QTOE_SET\\");
		
		MSXML2::IXMLDOMDocumentPtr doc;

		hr = doc.CreateInstance(__uuidof(MSXML2::DOMDocument30));

		if (SUCCEEDED(hr))
		{
			hr = doc->load(_variant_t(projectFilePath)) == VARIANT_TRUE ? S_OK : E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			doc->async = VARIANT_FALSE;
			MSXML2::IXMLDOMNodePtr project;

			project = doc->selectSingleNode(_bstr_t(Project_Node));

			hr = nullptr != project ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{
				auto filenames = project->attributes;
				auto filename = filenames->getNamedItem(_bstr_t(Project_InputCtrls_File_Node));
				if (nullptr != filename)
				{
					m_inputCtrlsFileName = (CString)filename->nodeValue;
				}
				
				filename = filenames->getNamedItem(_bstr_t(Project_DisplayCtrls_File_Node));
				if (nullptr != filename)
				{
					m_displayCtrlsFileName = (CString)filename->nodeValue;
				}

				filename = filenames->getNamedItem(_bstr_t(ScWidth_Node));
				if (nullptr != filename)
				{
					m_screenSize.cx = (LONG)filename->nodeValue;
				}
				else
				{
					m_screenSize.cx = 272;
				}

				filename = filenames->getNamedItem(_bstr_t(ScHeight_Node));
				if (nullptr != filename)
				{
					m_screenSize.cy = (LONG)filename->nodeValue;
				}
				else
				{
					m_screenSize.cy = 480;
				}
			}

			MSXML2::IXMLDOMNodePtr pages;
			CString str;
			str.Format(_T("/%s/%s"), Project_Node, Pages_Node);
			hr = NULL != (pages = doc->selectSingleNode(_bstr_t(str))) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				// Attributes

				// Element of pages.
				MSXML2::IXMLDOMNodeListPtr pageList = pages->selectNodes(_bstr_t(Page_Node));
				if (SUCCEEDED(hr = pageList != NULL ? S_OK : E_FAIL))
				{
					long count = 0;
					hr = pageList->get_length(&count);
					MSXML2::IXMLDOMNodePtr page;
					for (long i = 0; SUCCEEDED(hr) && i < count && NULL != (page = pageList->item[i]); i++)
					{
						// Create image page.
						DiImagePage* dIPage = new DiImagePage();
						
						auto ats = page->attributes;
						MSXML2::IXMLDOMNodePtr at = ats->getNamedItem(_bstr_t(ImagePath_Node));
						CString tipath = setDir + (NULL != at ? (CString)at->nodeValue : _T(""));
						dIPage->ImagePath(/*NULL != at ? at->nodeValue : _T("")*/tipath);
						
						dIPage->PageIndex(AddImagePage(dIPage));

						at = ats->getNamedItem(_bstr_t(Index_Node));

						dIPage->PageIndex(NULL != at ? at->nodeValue : -1);

						// Controls.
						auto nctrls = page->selectNodes(_bstr_t(DiControl_Node));

						if (SUCCEEDED(hr = nctrls != NULL ? S_OK : E_FAIL))
						{
							long cc = 0;
							nctrls->get_length(&cc);
							for (long j = 0; SUCCEEDED(hr) && j < cc; j++)
							{
								ats = nctrls->item[j]->attributes;

								DiCtrlType type = DiCtrlType::Button;

								at = ats->getNamedItem(_bstr_t(Type_Node));

								if (nullptr != at)
								{
									type = static_cast<DiCtrlType>((int)at->nodeValue);
								}

								int rlen = sizeof(DiControlData);
								if (type >= static_cast<BYTE>(DiCtrlType::Edit))
								{
									rlen = sizeof(PageData) * 2;
								}

								at = ats->getNamedItem(_bstr_t(File_Node));

								DiControlData data = {0};
								data.End1 = data.End2 = 0xFF;
								hr = DiControlPri::ReadControl(at->nodeValue, data, rlen) ? S_OK : E_FAIL;

								if (SUCCEEDED(hr))
								{
									
									DiControlPri* dCtrl = DiControFactory::CreateControl(type, &data);
									
									switch(type)
									{
									case DiControl::Edit:
									case DiControl::RTC_Text:
									case DiControl::QR_Code:
										{
											SIZE sz = {120, 60};
											dCtrl->Size(sz);
										}
										break;
									default:
										break;

									}

									if (NULL != dCtrl)
									{
										// Check The deferent type control invalid. 
										at = ats->getNamedItem(_bstr_t(Type_Node));
										// ............
										dCtrl->Path(at->nodeValue);
										dIPage->AddControl(dCtrl);
                                        dCtrl->ScreenIndex(dIPage->PageIndex());
									}

								}
								
							}
						}
					}
				}
			}
		}
	}
	else
	{
		hr = path.IsFileSpec() ? S_OK : E_FAIL;
	}
	
	if (SUCCEEDED(hr))
	{
		m_projectFilePath = projectFilePath;
	}
	
	return SUCCEEDED(hr);
}

BOOL DiDocument::DiProject::SaveFile()
{
	CPath p(GetProjectFilePath());
	int index = p.FindFileName();
	CString tempDic = GetProjectFilePath().Mid(0, index);
	
	if (!::PathFileExists(tempDic))
	{
		// Create it;
		BOOL bc = ::CreateDirectory(tempDic, NULL);
		ASSERT(bc);
	}

	CString setDir = tempDic + _T("QTOE_SET\\");
	if (!::PathFileExists(setDir))
	{
		// Create it;
		BOOL bc = ::CreateDirectory(setDir, NULL);
		ASSERT(bc);
	}

	MSXML2::IXMLDOMDocumentPtr doc;
	
	HRESULT hr = doc.CreateInstance(__uuidof(MSXML2::DOMDocument30));

	if (SUCCEEDED(hr))
	{
		MSXML2::IXMLDOMProcessingInstructionPtr pPi;

		pPi = doc->createProcessingInstruction(_T("xml"), _T("version=\"1.0\" encoding=\"utf-8\""));

		doc->appendChild(pPi);

		MSXML2::IXMLDOMElementPtr project;
		project = doc->createElement(_bstr_t(Project_Node));

		doc->appendChild(project);

		if (m_inputCtrlsFileName.IsEmpty())
		{
			m_inputCtrlsFileName = Name() + InputControlsFileName;
		}

		if (m_displayCtrlsFileName.IsEmpty())
		{
			m_displayCtrlsFileName = Name() + DisplayControlsFileName;
		}

		project->setAttribute(_bstr_t(Project_InputCtrls_File_Node), (_variant_t)m_inputCtrlsFileName);
		project->setAttribute(_bstr_t(Project_DisplayCtrls_File_Node), (_variant_t)m_displayCtrlsFileName);

		project->setAttribute(_bstr_t(ScWidth_Node), (_variant_t)m_screenSize.cx);
		project->setAttribute(_bstr_t(ScHeight_Node), (_variant_t)m_screenSize.cy);

		MSXML2::IXMLDOMElementPtr pages = doc->createElement(_bstr_t(Pages_Node));
		project->appendChild(pages);

		CString dicPath = tempDic + _T("\\ProFiles");

		if (!::PathFileExists(dicPath))
		{
			BOOL bcs = ::CreateDirectory(dicPath, NULL);
			ASSERT(bcs);
		}

		int count = m_images.size();
		for (int i = 0; i < count; i++)
		{
			DiImagePage* dIPage = m_images[i];
			if (NULL != dIPage)
			{
				MSXML2::IXMLDOMElementPtr page = project->appendChild(doc->createElement(_bstr_t(Page_Node)));
				
				CString ifname = ::PathFindFileName(dIPage->ImagePath());
				CString tipath = setDir + ifname;

				if (tipath != dIPage->ImagePath())
				{
					BOOL bc = ::CopyFile(dIPage->ImagePath(), tipath, FALSE);
					ASSERT(bc);
				}
				
				page->setAttribute(_bstr_t(ImagePath_Node), (_variant_t)/*dIPage->ImagePath()*/ifname);
				page->setAttribute(_bstr_t(Index_Node), (_variant_t)dIPage->PageIndex());
				
				CString nameStr = Name();
				const int ctlCount = dIPage->GetControlCount();
				for (int j = 0; j < ctlCount; j++)
				{
					DiControlPri* dICtrl = dIPage->GetControl(j);
					if (NULL != dICtrl)
					{
						// Control file.
						CString str;

						str.Format(_T("%s\\%s_%03d_%03d_%s.bin"), dicPath,(LPCTSTR)nameStr,i + 1, j + 1, dICtrl->Name());
						dICtrl->Path(str);
						if (dICtrl->Write())
						{
							MSXML2::IXMLDOMElementPtr ctl = doc->createElement(_bstr_t(DiControl_Node));
							ctl->setAttribute(_bstr_t(Type_Node), (_variant_t)dICtrl->ControlType());
							ctl->setAttribute(_bstr_t(File_Node), (_variant_t)str);
							page->appendChild(ctl);
						}
					}
				}

				pages->appendChild(page);
			}
		}

		hr = doc->save(_variant_t(m_projectFilePath));
	}

	return SUCCEEDED(hr) && SaveProjectFile(setDir);
}

BOOL DiDocument::DiProject::Reset(const CString& path)
{
	if (path != m_projectFilePath)
	{
		m_inputCtrlsFileName.Empty();
		m_displayCtrlsFileName.Empty();
		return OpenFile(path);
	}
	return FALSE;
}

void DiProject::NewProject(const CString& path)
{
    Dispose();
    m_projectFilePath = path;
}

void DiDocument::DiProject::Dispose()
{
	RemoveAll();
	m_projectFilePath = _T("");
	m_screenSize.cx = 272;
	m_screenSize.cy = 480;
	m_inputCtrlsFileName.Empty();
	m_displayCtrlsFileName.Empty();
}

BOOL DiDocument::DiProject::SaveProjectFile(const CString& rootPath)
{
	// Save project controls files.
	// Input controls.
	// Display controls.

	CFile inputFile;
	CFile displayFile;
	CString iPath, dPath;
	
	if (m_inputCtrlsFileName.IsEmpty())
	{
		m_inputCtrlsFileName = Name() + InputControlsFileName;
	}

	iPath = rootPath + m_inputCtrlsFileName;
	if (m_displayCtrlsFileName.IsEmpty())
	{
		m_displayCtrlsFileName = Name() + DisplayControlsFileName;
	}

	dPath = rootPath + m_displayCtrlsFileName;

	if (inputFile.Open(iPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) &&
		displayFile.Open(dPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		const int dclen = sizeof(PageData) * 2;
		int count = GetImagePagesCount();
		for (int i = 0; i < count; i++)
		{
			DiImagePage* page = GetImagePage(i);
			BYTE dPageData[DisplayPageDatalength] = {0};
			if (nullptr != page)
			{
				int cc = page->GetControlCount();
				int tc = 0;
				for (int c = 0; c < cc; c++)
				{
					DiControlPri* ctrl = page->GetControl(c);
					if (nullptr != ctrl)
					{
						if (ctrl->ControlType() >= static_cast<BYTE>(DiCtrlType::Edit))
						{
							memcpy(dPageData + tc * dclen, &ctrl->Data, dclen);
							tc++;
						}
						else
						{
							inputFile.Write(ctrl->Data, ctrl->GetDataLength());
						}
					}
				}
			}
			displayFile.Write(dPageData, DisplayPageDatalength);
		}
		
		const BYTE inputFileEnd[] = {0xFF, 0xFF};
		inputFile.Write(inputFileEnd, sizeof(inputFileEnd) / sizeof(BYTE));
		inputFile.Close();
		displayFile.Close();
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
	
}

