#pragma once
#include "DiControl.h"
#include <map>
#include <vector>
#include <memory>
#include <unordered_map>
#include "DiButton.h"
#include "DiDataInput.h"
#include "DiTextInput.h"
#include "DiQRCode.h"
#include "DiTextDisplay.h"
// CDiPropertyCtrl
using namespace DiControl;
using namespace std;


class DiControlPropertyPack
{
public:
    static DiControlPropertyPack* CreatePropertyPack(DiControlPri* control);

    struct PropProxyInfo
    {
        CString PropName;
        UINT ProxyIP;
        VARTYPE ValueType;
        CString Description;
		BOOL Enable;
    };

    DiControlPropertyPack();
    virtual ~DiControlPropertyPack();

    BOOL GetPropertyGrids(vector<CMFCPropertyGridProperty*> &groups);

    virtual BOOL SetProperty(const CString& propName, const _variant_t& val);

    virtual BOOL GetProperty(const CString& propName, _variant_t& val);

    DiControlPri* Control() const { return m_control; }
    virtual void Control(DiControlPri* val);
	
	const vector<PropProxyInfo>& PropertyInfo() const {return m_propMap;}
	
	BOOL RegisterProperty(const CString& propName, VARTYPE valueType, UINT proxyIP, const CString& description = _T(""), BOOL enable = TRUE);
	
	const CString& GroupName() const { return m_groupName; }
	void GroupName(const CString& val) { m_groupName = val; }
protected:

    virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);
    virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

    virtual void OnOwerControlChanged(DiControlPri* newControl);

    virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

    virtual void QueryPropertyGrids(vector<CMFCPropertyGridProperty*> &groups);

private:
    DiControlPri* m_control;
    vector<PropProxyInfo> m_propMap;
	CString m_groupName;
};

class BasicCtrlPropertyPack: public DiControlPropertyPack
{
public:
	BasicCtrlPropertyPack(){ GroupName(_T("基本"));}

protected:

	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DisplayBasicCtrlPropertyPack: public DiControlPropertyPack
{
public:

	DiDisplayCtrlPri* Control(){return static_cast<DiDisplayCtrlPri*>(__super::Control());}
	DisplayBasicCtrlPropertyPack(){ GroupName(_T("基本"));}

protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class QRCodePropertyPack: public DisplayBasicCtrlPropertyPack
{
public:
	DiQRCode* Control(){return static_cast<DiQRCode*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class MultiCtrlPropertyPack : public DiControlPropertyPack
{
public:
	virtual ~MultiCtrlPropertyPack();

	void AddPack(DiControlPropertyPack* pPack);

	void RemoveAll();

	const vector<shared_ptr<DiControlPropertyPack>>& Packs() { return m_packs;}

	virtual void OnOwerControlChanged(DiControlPri* newControl);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

	virtual void QueryPropertyGrids(vector<CMFCPropertyGridProperty*> &groups);

	virtual BOOL SetProperty(const CString& propName, const _variant_t& val);

	virtual BOOL GetProperty(const CString& propName, _variant_t& val);

private:
	vector<shared_ptr<DiControlPropertyPack>> m_packs;
};

class VariableProperyPack : public DiControlPropertyPack
{
public:
	VariableProperyPack(){ GroupName(_T("变量"));}
	IVariable* Control() { return dynamic_cast<IVariable*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};


class WriteKeyProperyPack : public DiControlPropertyPack
{
public:
	WriteKeyProperyPack(){ GroupName(_T("键值"));}
	IWriteKeyValue* Control() { return dynamic_cast<IWriteKeyValue*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class KeyBordProperyPack : public DiControlPropertyPack
{
public:
	KeyBordProperyPack(){ GroupName(_T("键盘"));}
	IKeyBordSupport* Control() { return dynamic_cast<IKeyBordSupport*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DataDisplayPack : public DiControlPropertyPack
{
public:
	DataDisplayPack(){ GroupName(_T("数据显示"));}
	IDataDisplay* Control() { return dynamic_cast<IDataDisplay*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};


class DataDisplayPosPack : public DiControlPropertyPack
{
public:
	DataDisplayPosPack(){ GroupName(_T("数据显示位置"));}
	IDataDisplayPos* Control() { return dynamic_cast<IDataDisplayPos*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};


class FontFamilyPosPack : public DiControlPropertyPack
{
public:
	FontFamilyPosPack(){ GroupName(_T("字体库位置"));}
	IFontFamily* Control() { return dynamic_cast<IFontFamily*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class FontSizePack : public DiControlPropertyPack
{
public:
	FontSizePack(){ GroupName(_T("字体大小"));}
	IFontSize* Control() { return dynamic_cast<IFontSize*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class RangeSourportPack : public DiControlPropertyPack
{
public:
	RangeSourportPack(){ GroupName(_T("范围"));}
	IRangeSourport* Control() { return dynamic_cast<IRangeSourport*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DataLoadPack : public DiControlPropertyPack
{
public:
	DataLoadPack(){ GroupName(_T("数据加载"));}
	IDataLoad* Control() { return dynamic_cast<IDataLoad*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class TextInputPack : public DiControlPropertyPack
{
public:
	TextInputPack(){ GroupName(_T("文本录入"));}
	ITextInput* Control() { return dynamic_cast<ITextInput*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);
};

class TextDisplayPack : public DiControlPropertyPack
{
public:
	TextDisplayPack(){ GroupName(_T("文本录入"));}
	ITextDisplay* Control() { return dynamic_cast<ITextDisplay*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);
};

class RenderColorPack : public DiControlPropertyPack
{
public:
	RenderColorPack(){ GroupName(_T("显示颜色"));}
	IRenderColor* Control() { return dynamic_cast<IRenderColor*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);
};

class AglimentPack : public DiControlPropertyPack
{
public:
	AglimentPack(){ GroupName(_T("对齐方式"));}
	IAgliment* Control() { return dynamic_cast<IAgliment*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);
};

class DataInputProperyPack : public DiControlPropertyPack
{
public:
	DataInputProperyPack();
	virtual~ DataInputProperyPack();

	IDataInput* Control() { return dynamic_cast<IDataInput*>(__super::Control());}
protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DiButtonPropertyPack : public /*BasicCtrlPropertyPack*/MultiCtrlPropertyPack
{
public:
	DiButton* Control() const { return static_cast<DiButton*>(__super::Control()); }
	virtual void Control(DiButton* val);

	DiButtonPropertyPack();
	virtual~ DiButtonPropertyPack();
protected:

	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DiDataInputPropertyPack : public /*BasicCtrlPropertyPack*/MultiCtrlPropertyPack
{
public:
	DiDataInputPropertyPack();
	virtual ~DiDataInputPropertyPack();
	DiDataInput* Control() const { return static_cast<DiDataInput*>(__super::Control()); }

protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DiTextInputPropertyPack : public MultiCtrlPropertyPack
{
public:
	DiTextInputPropertyPack();
	virtual~ DiTextInputPropertyPack();
};

class DiRTCTextPropertyPack : public /*BasicCtrlPropertyPack*/MultiCtrlPropertyPack
{
public:
	DiRTCTextPropertyPack();
	virtual ~DiRTCTextPropertyPack();
	DiRTCText* Control() const { return static_cast<DiRTCText*>(__super::Control()); }

protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class DiDataDisplayPropertyPack : public /*BasicCtrlPropertyPack*/MultiCtrlPropertyPack
{
public:
	DiDataDisplayPropertyPack();
	virtual ~DiDataDisplayPropertyPack();
	DiDataDisplay* Control() const { return static_cast<DiDataDisplay*>(__super::Control()); }
};

class DiTextPropertyPack : public MultiCtrlPropertyPack
{
public:
	DiTextPropertyPack();
	virtual ~DiTextPropertyPack();
	DiTextBlock* Control() const { return static_cast<DiTextBlock*>(__super::Control()); }

protected:
	virtual BOOL OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val);

	virtual BOOL OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val);

	virtual void ProducePropertiesMap(vector<PropProxyInfo>& propMap);

};

class CDiPropertyCtrl : public CMFCPropertyGridCtrl
{
    DECLARE_DYNAMIC(CDiPropertyCtrl)

public:
    CDiPropertyCtrl();
    virtual ~CDiPropertyCtrl();

    DiControlPri* Control() const { return m_control; }
    void Control(DiControlPri* val);

    void UpdateFormDiControl();
    void UpdateLayoutProperties();

    // Overrides
    virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
    virtual void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);
    virtual BOOL EditItem(CMFCPropertyGridProperty* pProp, LPPOINT lptClick = NULL);
	virtual void OnClickButton(CPoint point);
	virtual BOOL EndEditItem(BOOL bUpdateData = TRUE);
	virtual BOOL ValidateItemData(CMFCPropertyGridProperty* pProp);

protected:
    DECLARE_MESSAGE_MAP()

private:
    void recursionUpdate(CMFCPropertyGridProperty* prop) const;
private:
    DiControlPri* m_control;
    DiControlPropertyPack* m_propertyPack;
};


