#pragma once

#include <vector>
#include "DiImagePage.h"

using namespace DiControl;

namespace DiDocument
{
	class DiProject
	{
	public:
		DiProject(void);
		virtual ~DiProject(void);

		const CString& GetProjectFilePath() const;
		void SetProjectFilePath(const CString& path);

		CString Name() const;

		int GetImagePagesCount() const;
		DiImagePage* GetImagePage(int index) const;
		int AddImagePage(DiImagePage* pImage);
		int InsertImagePage(int index, DiImagePage* pImage);
		int RemoveImagePage(int index);
		int RemoveImagePage(DiImagePage* pImage);
		void RemoveAll();

		const std::vector<DiImagePage*>& GetImagePages() const;

		BOOL OpenFile(const CString& projectFilePath);
		BOOL SaveFile();

		BOOL Reset(const CString& path);

		void Dispose();

		BOOL SaveProjectFile(const CString& rootPath);
	private:
		std::vector<DiImagePage*> m_images;
		CString m_projectFilePath;

		CString m_inputCtrlsFileName;
		CString m_displayCtrlsFileName;
	};
}


