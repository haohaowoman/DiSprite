// DiPropertyCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DiSprite.h"
#include "DiPropertyCtrl.h"
#include "DiAssist.h"
#include <algorithm>
#include "MainFrm.h"
#include "DiSpriteView.h"
#include "DiImagePage.h"
#include "DiControlVisual.h"
// CDiPropertyCtrl
#include "DiSpriteView.h"

IMPLEMENT_DYNAMIC(CDiPropertyCtrl, CMFCPropertyGridCtrl)


    CDiPropertyCtrl::CDiPropertyCtrl(): m_control(nullptr), m_propertyPack(nullptr)
{
}

CDiPropertyCtrl::~CDiPropertyCtrl()
{
}


BEGIN_MESSAGE_MAP(CDiPropertyCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()


// CDiPropertyCtrl 消息处理程序

void CDiPropertyCtrl::UpdateFormDiControl()
{
    if (nullptr == m_propertyPack || nullptr == m_propertyPack->Control())
    {
        return;
    }

    int gCount = GetPropertyCount();

    for (int i = 0; i < gCount; i++)
    {
        CMFCPropertyGridProperty* pGroup = GetProperty(i);

        recursionUpdate(pGroup);
    }
    Invalidate();
}

void CDiPropertyCtrl::UpdateLayoutProperties()
{
    if (nullptr == m_propertyPack || nullptr == m_propertyPack->Control())
    {
        return;
    }

    int gCount = GetPropertyCount();
    for (int g = 0; g < gCount; g++)
    {
		CMFCPropertyGridProperty* pGroup = GetProperty(g);
		if (nullptr != pGroup && StrCmpC(_T("基本"), pGroup->GetName()) == 0)
		{
			if (pGroup->GetSubItemsCount() < 5)
			{
				return;
			}

			for (int i = 1; i < 5; i++)
			{
				CMFCPropertyGridProperty* prop = pGroup->GetSubItem(i);
				if (nullptr != prop)
				{
					_variant_t val;
					if (m_propertyPack->GetProperty(prop->GetName(), val))
					{
						_variant_t pval = prop->GetValue();
						if (val.vt != pval.vt)
						{
							val.ChangeType(pval.vt);
						}

						prop->SetValue(val);
					}
				}
			}
			break;
		}
    }

    Invalidate();
}

void CDiPropertyCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
    __super::OnPropertyChanged(pProp);
    if (nullptr != pProp && m_propertyPack)
    {
        BOOL bsuc = m_propertyPack->SetProperty(pProp->GetName(), pProp->GetValue());

        if (bsuc && NULL != theApp.GetDiView())
        {
            theApp.GetDiView()->UpdateEditControVisual();
        }
    }
	else
	{
		//Image prop update.
		if (nullptr != pProp && _T("图像路径") == (CString)pProp->GetName())
		{
			CString nPath = pProp->GetValue();
			CDiSpriteView* pView = theApp.GetDiView();
			if (::PathFileExists(nPath) && NULL != pView)
			{
				DiImagePage* page = pView->GetEditImagePage();
				if (nullptr != page)
				{
					page->ImagePath(nPath);
					pView->SetEditImagePage(page);
				}
			}
		}
	}
}

void CDiPropertyCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
    __super::OnChangeSelection(pNewSel, pOldSel);
}

BOOL CDiPropertyCtrl::EditItem(CMFCPropertyGridProperty* pProp, LPPOINT lptClick)
{
    return __super::EditItem(pProp, lptClick);
}

void CDiPropertyCtrl::OnClickButton(CPoint point)
{
    __super::OnClickButton(point);
}

BOOL CDiPropertyCtrl::EndEditItem(BOOL bUpdateData)
{
    return __super::EndEditItem(bUpdateData);
}

BOOL CDiPropertyCtrl::ValidateItemData(CMFCPropertyGridProperty* pProp)
{
	/*if (pProp->m_nMinValue > pProp->GetValue() || pProp->m_nMaxValue < pProp->GetValue())
	{
		return FALSE;
	}*/
	
    return __super::ValidateItemData(pProp);
}

void CDiPropertyCtrl::recursionUpdate(CMFCPropertyGridProperty* prop) const
{
    if (nullptr != prop && nullptr != m_propertyPack)
    {
        if (prop->IsGroup())
        {
            int pCount = prop->GetSubItemsCount();

            for (int i = 0; i < pCount; i++)
            {
                recursionUpdate(prop->GetSubItem(i));
            }
        }
        else
        {
            _variant_t val;
            if (m_propertyPack->GetProperty(prop->GetName(), val))
            {
                if (val.vt != prop->GetValue().vt)
                {
                    _variant_t pval = prop->GetValue();
                    val.ChangeType(prop->GetValue().vt, &pval);
                }

                prop->SetValue(val);
            }

        }
    }
}

void CDiPropertyCtrl::Control(DiControlPri* val)
{
    if (val != m_control || (nullptr == val && nullptr == m_control))
    {

        if (nullptr != val)
        {
            BOOL shouldNew = FALSE;
            BOOL justRefresh = FALSE;
            if (nullptr != m_control)
            {
                shouldNew = m_control->ControlType() != val->ControlType();
                justRefresh = shouldNew ? FALSE : TRUE;
            }
            else
            {
                shouldNew = TRUE;
            }

            if (shouldNew)
            {
                RemoveAll();
                SafeDelete(&m_propertyPack);

                m_propertyPack = DiControlPropertyPack::CreatePropertyPack(val);
                vector<CMFCPropertyGridProperty*> groups;
                m_propertyPack->GetPropertyGrids(groups);

                int count = groups.size();

                for (int i = 0; i < count; i++)
                {
                    if (nullptr != groups[i])
                    {
                        AddProperty(groups[i], FALSE, FALSE);
						SetCurSel(groups[i]);
                    }
                }
				
                AdjustLayout();
                Invalidate();
            }
            else
            {
                if (justRefresh)
                {
                    m_propertyPack->Control(val);
                    UpdateFormDiControl();
                }
            }

        }
        else
        {
            RemoveAll();
            Invalidate();
            SafeDelete(&m_propertyPack);

			// Show the Image prop.
			CDiSpriteView* pView = theApp.GetDiView();
			if (NULL != pView)
			{
				DiImagePage* page = pView->GetEditImagePage();
				if (nullptr != page)
				{
					CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty(_T("图像"));
					pGroup->AddSubItem(
						new CMFCPropertyGridFileProperty(_T("图像路径"), 
						TRUE, 
						page->ImagePath(), 
						_T("bmp"), 
						0, 
						_T("PNG (*.png)|*.png|JPEG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
						_T("指定图像的路径。")));
					AddProperty(pGroup);
					this->SetCurSel(pGroup->GetSubItem(0));
				}
			}
			
        }

        m_control = val;
    }
    else
    {
        UpdateLayoutProperties();
    }
}

// Property grid pack.

#define SCREEN_INDEX_PROXY (1001)
#define TOP_PROXY (1002)
#define LEFT_PROXY (1003)
#define BOTTOM_PROXY (1004)
#define RIGHT_PROXY  (1005)
#define IS_VOICE_SWITCH_PROXY  (1006)
#define VOICE_COUNT_PROXY  (1007)
#define VOICE_ID_PROXY  (1008)
#define TOGGEL_PAGEINDEX_PROXY  (1009)
#define KEY_EFFECT_PROXY  (1010)
#define IS_AUTO_UPDATA_DATA_PROXY  (1011)
#define CONTROL_TYPE_PROXY  (1012)

/*
Properties define:
{_T("Screen Index"), SCREEN_INDEX_PROXY, VT_UI2, _T("第几屏")},
{_T("Top"), TOP_PROXY, VT_I4, _T("左上角Y坐标")},
{_T("Left"), LEFT_PROXY, VT_I4, _T("左上角X坐标")},
{_T("Bottom"), BOTTOM_PROXY, VT_I4, _T("右下角Y坐标")},
{_T("Right"), RIGHT_PROXY, VT_I4, _T("右下角X坐标")},
{_T("切换界面"), TOGGEL_PAGEINDEX_PROXY, VT_I2, _T("有值表示要切换到界面")},
{_T("按键效果"), KEY_EFFECT_PROXY, VT_I2, _T("按键界面的效果")},
{_T("自动上传"), IS_AUTO_UPDATA_DATA_PROXY, VT_BOOL, _T("是否自动上传数据")},
{_T("控件类型"), CONTROL_TYPE_PROXY, VT_UI1, _T("控件类型")},

*/

DiControlPropertyPack* DiControlPropertyPack::CreatePropertyPack(DiControlPri* control)
{
    DiControlPropertyPack* rp = nullptr;
    if (nullptr != control)
    {
        DiCtrlType ctrlType = static_cast<DiCtrlType>(control->ControlType());
        switch (ctrlType)
        {
        case DataInput: 
			rp = new DiDataInputPropertyPack();
            break;
        case Button: 
            rp = new DiButtonPropertyPack();
            break;
        case TextInput: 
			rp = new DiTextInputPropertyPack();
            break;
        case GBKInput: 
			rp = new DiControlPropertyPack();
            break;
        default: 
			rp = new DiControlPropertyPack();
			break;
		case DiControl::Unkown:
			break;
		case DiControl::Edit:
			rp = new DiDataDisplayPropertyPack();
			break;
		case DiControl::Text:
			rp = new DiTextPropertyPack();
			break;
		case DiControl::RTC_Text:
			rp = new DiRTCTextPropertyPack();
			break;
		case DiControl::QR_Code:
			rp = new QRCodePropertyPack();
			break;
        }

        if (rp != nullptr)
        {
            rp->Control(control);
        }
    }
    return rp;
}

DiControlPropertyPack::DiControlPropertyPack() : m_control(nullptr)
{

}

DiControlPropertyPack::~DiControlPropertyPack()
{
}

BOOL DiControlPropertyPack::GetPropertyGrids(vector<CMFCPropertyGridProperty*>& groups)
{
    if (nullptr != m_control/* && m_propMap.size() > 0*/)
    {
        QueryPropertyGrids(groups);
        return TRUE;
    }
    return FALSE;
}

BOOL DiControlPropertyPack::SetProperty(const CString & propName, const _variant_t& val)
{
    if (nullptr == m_control)
        return FALSE;

    vector<PropProxyInfo>::iterator it = std::find_if(m_propMap.begin(), m_propMap.end(), [&](const PropProxyInfo& i){ return propName == i.PropName;});
    if (it == m_propMap.end())
    {
        return FALSE;
    }
	
    return OnSetPropertyValue(*it, val);
}

BOOL DiControlPropertyPack::GetProperty(const CString & propName, _variant_t & val)
{
    if (nullptr == m_control)
        return FALSE;

    vector<PropProxyInfo>::iterator it = std::find_if(m_propMap.begin(), m_propMap.end(), [&](const PropProxyInfo& i){ return propName == i.PropName;});
    if (it == m_propMap.end())
    {
        return FALSE;
    }
    return OnGetPropertyValue(*it, val);
}

void DiControlPropertyPack::Control(DiControlPri * val)
{
    if (val != m_control)
    {
        m_control = val;
        OnOwerControlChanged(val);
    }

    if (0 == m_propMap.size())
    {
        ProducePropertiesMap(m_propMap);
    }
}

BOOL DiControlPropertyPack::RegisterProperty(const CString& propName, VARTYPE valueType, UINT proxyIP, const CString& description /*= _T("")*/, BOOL enable /*= TRUE*/)
{
    PropProxyInfo nPPI = { propName, valueType, proxyIP, description, TRUE};
    m_propMap.push_back(nPPI);
    return TRUE;
}

BOOL DiControlPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
    BOOL bSucc = FALSE;

    ////if (val.vt == propProxy.ValueType)
    //{
    //    switch (propProxy.ProxyIP)
    //    {
    //    case SCREEN_INDEX_PROXY:
    //        m_control->ScreenIndex(static_cast<USHORT>(val));
    //        break;
    //    case TOP_PROXY:
    //        m_control->Top(static_cast<int>(val));
    //        break;
    //    case LEFT_PROXY:
    //        m_control->Left(static_cast<int>(val));
    //        break;
    //    case BOTTOM_PROXY:
    //        m_control->Bottom(static_cast<int>(val));
    //        break;
    //    case RIGHT_PROXY:
    //        m_control->Right(static_cast<int>(val));
    //        break;
    //    case TOGGEL_PAGEINDEX_PROXY:
    //        m_control->TogglePageIndex(static_cast<SHORT>(val));
    //        break;
    //    case KEY_EFFECT_PROXY:
    //        m_control->KeyEffect(static_cast<SHORT>(val));
    //        break;
    //    case IS_AUTO_UPDATA_DATA_PROXY:
    //        m_control->IsAutoUpdateData(static_cast<bool>(val));
    //        break;
    //    case CONTROL_TYPE_PROXY:
    //        // Can't edit;
    //        m_control->ControlType(static_cast<int>(val));
    //        break;
    //    default:
    //        bSucc = FALSE;
    //        break;
    //    }
    //}

    return bSucc;
}

BOOL DiControlPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t & val)
{
    BOOL bSucc = FALSE;
    //switch (propProxy.ProxyIP)
    //{
    //case SCREEN_INDEX_PROXY:
    //    val = m_control->ScreenIndex();
    //    break;
    //case TOP_PROXY:
    //    val = m_control->Top();
    //    break;
    //case LEFT_PROXY:
    //    val = m_control->Left();
    //    break;
    //case BOTTOM_PROXY:
    //    val = m_control->Bottom();
    //    break;
    //case RIGHT_PROXY:
    //    val = m_control->Right();
    //    break;
    //case TOGGEL_PAGEINDEX_PROXY:
    //    val = m_control->TogglePageIndex();
    //    break;
    //case KEY_EFFECT_PROXY:
    //    val = m_control->KeyEffect();
    //    break;
    //case IS_AUTO_UPDATA_DATA_PROXY:
    //    val = m_control->IsAutoUpdateData();
    //    break;
    //case CONTROL_TYPE_PROXY:
    //    val = static_cast<int>(m_control->ControlType());
    //    break;
    //default:
    //    bSucc = FALSE;
    //    break;
    //}

    return bSucc;
}

void DiControlPropertyPack::OnOwerControlChanged(DiControlPri * newControl)
{
}

void DiControlPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
    
}

void DiControlPropertyPack::QueryPropertyGrids(vector<CMFCPropertyGridProperty*>& groups)
{
    CMFCPropertyGridProperty* prop = nullptr;

    CMFCPropertyGridProperty* pGroup = nullptr;

    pGroup = new CMFCPropertyGridProperty(GroupName());
    pGroup->Expand();

    vector<PropProxyInfo>::iterator it = m_propMap.begin();
    int i = 0;
    for (; it != m_propMap.end(); ++it, i++)
    {
        _variant_t val;
        if (GetProperty(it->PropName, val))
        {
            prop = new CMFCPropertyGridProperty(it->PropName, val, it->Description);
            prop->SetData(reinterpret_cast<DWORD_PTR>(&it));
            /*if (i < 5)
            {
                prop->EnableSpinControl(TRUE, 0, 65536);
            }*/
			
			POINT rpt = { 0 };

			BOOL nUp = FALSE;
			switch(it->ValueType)
			{
			case VT_I4:
			case VT_INT:

				break;
			case VT_UI1:
				rpt.y = 255;
				nUp = TRUE;
				break;
			case VT_I2:
				rpt.x = -32767;
				rpt.y = 32768;
				nUp = TRUE;
				break;
			case VT_UI2:
				rpt.y = 65535;
				nUp = TRUE;
				break;
			default:
				break;
			}

			CDiSpriteView* pView = theApp.GetDiView();
			if (nullptr != pView && nullptr != pView->GetEditImagePage())
			{
				if (_T("Left") == it->PropName || _T("Width") == it->PropName)
				{
					nUp = TRUE;
					rpt.y = pView->GetImageView()->GetLayout().Width;
				}
				else if (_T("Top") == it->PropName || _T("Height") == it->PropName)
				{
					nUp = TRUE;
					rpt.y = pView->GetImageView()->GetLayout().Height;
				}
			}

			if (nUp)
			{
				CString tempStr;
				prop->EnableSpinControl(TRUE, rpt.x, rpt.y);
				tempStr.Format(_T("%s\n属性范围：%d~%d"), prop->GetDescription(), rpt.x, rpt.y);
				prop->SetDescription(tempStr);
			}


			prop->Enable(it->Enable);
            pGroup->AddSubItem(prop);
        }

    }

    groups.push_back(pGroup);

}

#define VARIABLE_ADDR_PROXY (SCREEN_INDEX_PROXY + 100)
#define WRITE_STYLE_PROXY (VARIABLE_ADDR_PROXY + 1)
#define KEY_VALUE_PROXY (WRITE_STYLE_PROXY + 1)

void DiButtonPropertyPack::Control(DiButton* val)
{
	__super::Control(val);
}

DiButtonPropertyPack::DiButtonPropertyPack()
{
	AddPack(new BasicCtrlPropertyPack());
	AddPack(new VariableProperyPack());
	AddPack(new WriteKeyProperyPack());
	GroupName(_T("BUTTON"));
}

DiButtonPropertyPack::~ DiButtonPropertyPack()
{

}

BOOL DiButtonPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	return FALSE;
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		switch (propProxy.ProxyIP)
		{
		case VARIABLE_ADDR_PROXY:
			Control()->VariableAddr(static_cast<USHORT>(val));
			break;
		case WRITE_STYLE_PROXY:
			Control()->WriteStyle(static_cast<int>(val));
			break;
		case KEY_VALUE_PROXY:
			Control()->KeyValue(static_cast<SHORT>(val));
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL DiButtonPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	return FALSE;
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case VARIABLE_ADDR_PROXY:
			val = Control()->VariableAddr();
			break;
		case WRITE_STYLE_PROXY:
			val = static_cast<int>(Control()->WriteStyle());
			break;
		case KEY_VALUE_PROXY:
			val = Control()->KeyValue();
			break;
		default:
			bsuc = FALSE;
			break;
		}
		
	}
	return bsuc;
}

void DiButtonPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
/*
	PropProxyInfo tempProps[] = 
	{
		{_T("变量地址"), VARIABLE_ADDR_PROXY, VT_UI2, _T(""), TRUE},
		{_T("变量写入方式"), WRITE_STYLE_PROXY, VT_UI2, _T("为控制选择时为0x10后4位比表示写入位0x"), TRUE},
		{_T("键值"), KEY_VALUE_PROXY, VT_I2, _T("控件类型"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
*/
}

#define DATA_TYPE_PROXY (VARIABLE_ADDR_PROXY + 101)
#define INTEGER_COUNT_PROXY (DATA_TYPE_PROXY + 1)
#define DECIMAL_COUNT_PROXY (DATA_TYPE_PROXY + 2)
#define DISPLAY_X_PROXY (DATA_TYPE_PROXY + 3)
#define DISPLAY_Y_PROXY (DATA_TYPE_PROXY + 4)
#define FONTFAMILY_PROXY (DATA_TYPE_PROXY + 5)
#define FONTSIZE_PROXY (DATA_TYPE_PROXY + 6)
#define CURSOR_TYPE_PROXY (DATA_TYPE_PROXY + 7)
#define DISPLAY_TYPE_PROXY (DATA_TYPE_PROXY + 8)
#define KEYBORD_POSITION_PROXY (DATA_TYPE_PROXY + 9)
#define KEYBORD_PAGE_PROXY (DATA_TYPE_PROXY + 10)
#define KEYBORD_AREA_BEGIN_X_PROXY (DATA_TYPE_PROXY + 11)
#define KEYBORD_AREA_BEGIN_Y_PROXY (DATA_TYPE_PROXY + 12)
#define KEYBORD_AREA_END_X_PROXY (DATA_TYPE_PROXY + 13)
#define KEYBORD_AREA_END_Y_PROXY (DATA_TYPE_PROXY + 14)
#define KEYBORD_DISPLAY_X_PROXY (DATA_TYPE_PROXY + 15)
#define KEYBORD_DISPLAY_Y_PROXY (DATA_TYPE_PROXY + 16)
#define ENABLE_RANGE_PROXY (DATA_TYPE_PROXY + 17)
#define RANGE_LOW_PROXY (DATA_TYPE_PROXY + 18)
#define RANGE_HIGH_PROXY (DATA_TYPE_PROXY + 19)
#define LOADDATA_STYLE_PROXY (DATA_TYPE_PROXY + 20)
#define LOADDATA_ADDR_PROXY (DATA_TYPE_PROXY + 21)
#define LOADDATA_PROXY (DATA_TYPE_PROXY + 22)

DiDataInputPropertyPack::DiDataInputPropertyPack()
{
	GroupName(_T("data Input"));
	AddPack(new BasicCtrlPropertyPack());
	AddPack(new VariableProperyPack());
	AddPack(new RenderColorPack());
	AddPack(new DataDisplayPack());
	AddPack(new DataDisplayPosPack());
	AddPack(new FontFamilyPosPack());
	AddPack(new FontSizePack());
	AddPack(new KeyBordProperyPack());
	AddPack(new DataInputProperyPack());
	AddPack(new RangeSourportPack());
	AddPack(new DataLoadPack());
}

DiDataInputPropertyPack::~DiDataInputPropertyPack()
{

}

BOOL DiDataInputPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	return FALSE;
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DATA_TYPE_PROXY:
			Control()->DataType((USHORT)val);
			break;
		case INTEGER_COUNT_PROXY:
			Control()->IntegerCount((USHORT)val);
			break;
		case DECIMAL_COUNT_PROXY:
			Control()->DecimalCount((USHORT)val);
			break;
		case DISPLAY_X_PROXY:
			Control()->DisplayX((SHORT)val);
			break;
		case DISPLAY_Y_PROXY:
			Control()->DisplayY((SHORT)val);
			break;
		case FONTFAMILY_PROXY:
			Control()->FontFamily((USHORT)val);
			break;
		case FONTSIZE_PROXY:
			Control()->FontSize((USHORT)val);
			break;
		case CURSOR_TYPE_PROXY:
			Control()->CursorColor((USHORT)val);
			break;
		case DISPLAY_TYPE_PROXY:
			Control()->DisplayType((USHORT)val);
			break;
		case KEYBORD_POSITION_PROXY:
			Control()->KeyBordPosition((USHORT)val);
			break;
		case KEYBORD_PAGE_PROXY:
			Control()->KeyBordPage((USHORT)val);
			break;
		case KEYBORD_AREA_BEGIN_X_PROXY:
			Control()->KeyBordAreaBeginX((SHORT)val);
			break;
		case KEYBORD_AREA_BEGIN_Y_PROXY:
			Control()->KeyBordAreaBeginY((SHORT)val);
			break;
		case KEYBORD_AREA_END_X_PROXY:
			Control()->KeyBordAreaEndX((SHORT)val);
			break;
		case KEYBORD_AREA_END_Y_PROXY:
			Control()->KeyBordAreaEndY((SHORT)val);
			break;
		case KEYBORD_DISPLAY_X_PROXY:
			Control()->KeyBordDisplayX((SHORT)val);
			break;
		case KEYBORD_DISPLAY_Y_PROXY:
			Control()->KeyBordDisplayY((SHORT)val);
			break;
		case ENABLE_RANGE_PROXY:
			Control()->EnableRange((bool)val);
			break;
		case RANGE_LOW_PROXY:
			Control()->RangeLow((int)val);
			break;
		case RANGE_HIGH_PROXY:
			Control()->RangeHigh((int)val);
			break;
		case LOADDATA_STYLE_PROXY:
			Control()->LoadDataStyle((USHORT)val);
			break;
		case LOADDATA_ADDR_PROXY:
			Control()->LoadDataAddr((USHORT)val);
			break;
		case LOADDATA_PROXY:
			Control()->LoadData((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DiDataInputPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	return FALSE;
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DATA_TYPE_PROXY:
			val = (USHORT)Control()->DataType();
			break;
		case INTEGER_COUNT_PROXY:
			val = (USHORT)Control()->IntegerCount();
			break;
		case DECIMAL_COUNT_PROXY:
			val = (USHORT)Control()->DecimalCount();
			break;
		case DISPLAY_X_PROXY:
			val = (SHORT)Control()->DisplayX();
			break;
		case DISPLAY_Y_PROXY:
			val = (SHORT)Control()->DisplayY();
			break;
		case FONTFAMILY_PROXY:
			val = (USHORT)Control()->FontFamily();
			break;
		case FONTSIZE_PROXY:
			val = (USHORT)Control()->FontSize();
			break;
		case CURSOR_TYPE_PROXY:
			val = (USHORT)Control()->CursorColor();
			break;
		case DISPLAY_TYPE_PROXY:
			val = (USHORT)Control()->DisplayType();
			break;
		case KEYBORD_POSITION_PROXY:
			val = (USHORT)Control()->KeyBordPosition();
			break;
		case KEYBORD_PAGE_PROXY:
			val = (USHORT)Control()->KeyBordPage();
			break;
		case KEYBORD_AREA_BEGIN_X_PROXY:
			val = (SHORT)Control()->KeyBordAreaBeginX();
			break;
		case KEYBORD_AREA_BEGIN_Y_PROXY:
			val = (SHORT)Control()->KeyBordAreaBeginY();
			break;
		case KEYBORD_AREA_END_X_PROXY:
			val = (SHORT)Control()->KeyBordAreaEndX();
			break;
		case KEYBORD_AREA_END_Y_PROXY:
			val = (SHORT)Control()->KeyBordAreaEndY();
			break;
		case KEYBORD_DISPLAY_X_PROXY:
			val = (SHORT)Control()->KeyBordDisplayX();
			break;
		case KEYBORD_DISPLAY_Y_PROXY:
			val = (SHORT)Control()->KeyBordDisplayY();
			break;
		case ENABLE_RANGE_PROXY:
			val = (bool)Control()->EnableRange();
			break;
		case RANGE_LOW_PROXY:
			val = (int)Control()->RangeLow();
			break;
		case RANGE_HIGH_PROXY:
			val = (int)Control()->RangeHigh();
			break;
		case LOADDATA_STYLE_PROXY:
			val = (USHORT)Control()->LoadDataStyle();
			break;
		case LOADDATA_ADDR_PROXY:
			val = (USHORT)Control()->LoadDataAddr();
			break;
		case LOADDATA_PROXY:
			val = (SHORT)Control()->LoadData();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DiDataInputPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
/*
	PropProxyInfo tempProps[] = 
	{
		{_T("变量地址"), VARIABLE_ADDR_PROXY, VT_UI2, _T(""), TRUE},
		{_T("数据类型"), DATA_TYPE_PROXY, VT_UI2, _T("0x00,整数（字），0x01，长整数（双子）0x02，变量指针高字节，0x03 变量指针低字节，0x04 超长整数（8字节）"), TRUE},
		{_T("整数位数"), INTEGER_COUNT_PROXY, VT_UI2, _T("控件类型"), TRUE},
		{_T("小数位数"), DECIMAL_COUNT_PROXY, VT_UI2, _T("控件类型"), TRUE},
		{_T("显示位置X"), DISPLAY_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("显示位置Y"), DISPLAY_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("字库位置"), FONTFAMILY_PROXY, VT_UI2, _T(""), TRUE},
		{_T("字库大小"), FONTSIZE_PROXY, VT_UI2, _T(""), TRUE},
		{_T("光标颜色"), CURSOR_TYPE_PROXY, VT_UI2, _T(""), TRUE},
		{_T("输入显示方式"), DISPLAY_TYPE_PROXY, VT_UI2, _T("00-显示*，01-直接显示"), TRUE},
		{_T("键盘位置"), KEYBORD_POSITION_PROXY, VT_UI2, _T("00-当前页面，01-其他页面"), TRUE},
		{_T("键盘所在页面"), KEYBORD_PAGE_PROXY, VT_UI2, _T(""), TRUE},
		{_T("键盘区域开始X"), KEYBORD_AREA_BEGIN_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域开始Y"), KEYBORD_AREA_BEGIN_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域结束X"), KEYBORD_AREA_END_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域结束Y"), KEYBORD_AREA_END_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘显示位置X"), KEYBORD_DISPLAY_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘显示位置Y"), KEYBORD_DISPLAY_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("启用范围限制"), ENABLE_RANGE_PROXY, VT_BOOL, _T(""), TRUE},
		{_T("上限"), RANGE_LOW_PROXY, VT_INT, _T(""), TRUE},
		{_T("下限"), RANGE_HIGH_PROXY, VT_INT, _T(""), TRUE},
		{_T("加载数据方式"), LOADDATA_STYLE_PROXY, VT_UI2, _T("0x5A-录入过程中加载数据"), TRUE},
		{_T("数据地址"), LOADDATA_ADDR_PROXY, VT_UI2, _T(""), TRUE},
		{_T("加载数据"), LOADDATA_PROXY, VT_I2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}*/
}

MultiCtrlPropertyPack::~MultiCtrlPropertyPack()
{
	RemoveAll();
}

void MultiCtrlPropertyPack::AddPack(DiControlPropertyPack* pPack)
{
	shared_ptr<DiControlPropertyPack> np(pPack);
	m_packs.push_back(np);
}

void MultiCtrlPropertyPack::RemoveAll()
{
	m_packs.clear();
}

void MultiCtrlPropertyPack::OnOwerControlChanged(DiControlPri* newControl)
{
	__super::OnOwerControlChanged(newControl);
	
	for (auto pack = m_packs.begin(); pack != m_packs.end(); ++pack)
	{
		pack->get()->Control(newControl);
	}
}

void MultiCtrlPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
	/*for (auto pack = m_packs.begin(); pack != m_packs.end(); ++pack)
	{
		pack->get()->ProducePropertiesMap(propMap);
	}*/
}

void MultiCtrlPropertyPack::QueryPropertyGrids(vector<CMFCPropertyGridProperty*> &groups)
{
	__super::QueryPropertyGrids(groups);
	for (auto pack = m_packs.begin(); pack != m_packs.end(); ++pack)
	{
		if (nullptr != pack->get())
		{
			pack->get()->GetPropertyGrids(groups);
		}
	}
}

BOOL MultiCtrlPropertyPack::SetProperty(const CString& propName, const _variant_t& val)
{
	if (nullptr == Control())
	{
		return FALSE;
	}
	BOOL bsuc = FALSE;
	for (auto pack = m_packs.begin(); pack != m_packs.end(); ++pack)
	{
		if (nullptr != pack->get())
		{
			bsuc = pack->get()->SetProperty(propName, val);
			if (bsuc)
			{
				break;
			}
		}
	}
	if (!bsuc)
	{
		bsuc = __super::SetProperty(propName, val);
	}
	return bsuc;
}

BOOL MultiCtrlPropertyPack::GetProperty(const CString& propName, _variant_t& val)
{
	if (nullptr == Control())
	{
		return FALSE;
	}
	BOOL bsuc = FALSE;
	for (auto pack = m_packs.begin(); pack != m_packs.end(); ++pack)
	{
		if (nullptr != pack->get())
		{
			bsuc = pack->get()->GetProperty(propName, val);
			if (bsuc)
			{
				break;
			}
		}
	}
	if (!bsuc)
	{
		bsuc = __super::GetProperty(propName, val);
	}
	return bsuc;
}

BOOL BasicCtrlPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case SCREEN_INDEX_PROXY:
			Control()->ScreenIndex(static_cast<USHORT>(val));
			break;
		case TOP_PROXY:
			{
				int top = static_cast<int>(val);
				CDiSpriteView* pView = theApp.GetDiView();
				if (nullptr != pView && nullptr != pView->GetEditImagePage())
				{
					int ttop = max(0, top);
					ttop = min(pView->GetImageView()->GetLayout().Height - Control()->Height(), ttop);

					POINT pt = {Control()->Left(), ttop};
					Control()->Location(pt);
					if (ttop != top)
					{
						theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
					}
				}
			}
			break;
		case LEFT_PROXY:
			{
				int left = static_cast<int>(val);
				CDiSpriteView* pView = theApp.GetDiView();
				if (nullptr != pView && nullptr != pView->GetEditImagePage())
				{
					int tleft = max(0, left);
					tleft = min(pView->GetImageView()->GetLayout().Width - Control()->Width(), tleft);

					POINT pt = {tleft, Control()->Top()};
					Control()->Location(pt);
					if (tleft != left)
					{
						theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
					}
				}
			}
			break;
		case BOTTOM_PROXY:
			Control()->Height(static_cast<int>(val));
			break;
		case RIGHT_PROXY:
			Control()->Width(static_cast<int>(val));
			break;
		case TOGGEL_PAGEINDEX_PROXY:
			Control()->TogglePageIndex(static_cast<SHORT>(val));
			break;
		case KEY_EFFECT_PROXY:
			Control()->KeyEffect(static_cast<SHORT>(val));
			break;
		case IS_AUTO_UPDATA_DATA_PROXY:
			Control()->IsAutoUpdateData(static_cast<bool>(val));
			break;
		case CONTROL_TYPE_PROXY:
			// Can't edit;
			Control()->ControlType(static_cast<int>(val));
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	
	return bsuc;
}

BOOL BasicCtrlPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case SCREEN_INDEX_PROXY:
			val = Control()->ScreenIndex();
			break;
		case TOP_PROXY:
			val = Control()->Top();
			break;
		case LEFT_PROXY:
			val = Control()->Left();
			break;
		case BOTTOM_PROXY:
			val = Control()->Height();
			break;
		case RIGHT_PROXY:
			val = Control()->Width();
			break;
		case TOGGEL_PAGEINDEX_PROXY:
			val = Control()->TogglePageIndex();
			break;
		case KEY_EFFECT_PROXY:
			val = Control()->KeyEffect();
			break;
		case IS_AUTO_UPDATA_DATA_PROXY:
			val = Control()->IsAutoUpdateData();
			break;
		case CONTROL_TYPE_PROXY:
			val = static_cast<int>(Control()->ControlType());
			break;
		default:
			bsuc = FALSE;
			break;
		}

	}
	
	return bsuc;
}

void BasicCtrlPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
	PropProxyInfo tempProps[] = 
	{
		{_T("Screen Index"), SCREEN_INDEX_PROXY, VT_UI2, _T("第几屏"), FALSE},
		{_T("Top"), TOP_PROXY, VT_UI2, _T("左上角Y坐标"), TRUE},
		{_T("Left"), LEFT_PROXY, VT_UI2, _T("左上角X坐标"), TRUE},
		{_T("Width"), RIGHT_PROXY, VT_UI2, _T("控件宽度"), TRUE},
		{_T("Height"), BOTTOM_PROXY, VT_UI2, _T("控件高度"), TRUE},
		{_T("切换界面"), TOGGEL_PAGEINDEX_PROXY, VT_I2, _T("有值表示要切换到界面"), TRUE},
		{_T("按键效果"), KEY_EFFECT_PROXY, VT_I2, _T("按键界面的效果"), TRUE},
		{_T("自动上传"), IS_AUTO_UPDATA_DATA_PROXY, VT_BOOL, _T("是否自动上传数据"), TRUE},
		{_T("控件类型"), CONTROL_TYPE_PROXY, VT_UI1, _T("控件类型"), FALSE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}

}

BOOL KeyBordProperyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case KEYBORD_POSITION_PROXY:
			Control()->KeyBordPosition((USHORT)val);
			break;
		case KEYBORD_PAGE_PROXY:
			Control()->KeyBordPage((USHORT)val);
			break;
		case KEYBORD_AREA_BEGIN_X_PROXY:
			Control()->KeyBordAreaBeginX((SHORT)val);
			break;
		case KEYBORD_AREA_BEGIN_Y_PROXY:
			Control()->KeyBordAreaBeginY((SHORT)val);
			break;
		case KEYBORD_AREA_END_X_PROXY:
			Control()->KeyBordAreaEndX((SHORT)val);
			break;
		case KEYBORD_AREA_END_Y_PROXY:
			Control()->KeyBordAreaEndY((SHORT)val);
			break;
		case KEYBORD_DISPLAY_X_PROXY:
			Control()->KeyBordDisplayX((SHORT)val);
			break;
		case KEYBORD_DISPLAY_Y_PROXY:
			Control()->KeyBordDisplayY((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL KeyBordProperyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case KEYBORD_POSITION_PROXY:
			val = (USHORT)Control()->KeyBordPosition();
			break;
		case KEYBORD_PAGE_PROXY:
			val = (USHORT)Control()->KeyBordPage();
			break;
		case KEYBORD_AREA_BEGIN_X_PROXY:
			val = (SHORT)Control()->KeyBordAreaBeginX();
			break;
		case KEYBORD_AREA_BEGIN_Y_PROXY:
			val = (SHORT)Control()->KeyBordAreaBeginY();
			break;
		case KEYBORD_AREA_END_X_PROXY:
			val = (SHORT)Control()->KeyBordAreaEndX();
			break;
		case KEYBORD_AREA_END_Y_PROXY:
			val = (SHORT)Control()->KeyBordAreaEndY();
			break;
		case KEYBORD_DISPLAY_X_PROXY:
			val = (SHORT)Control()->KeyBordDisplayX();
			break;
		case KEYBORD_DISPLAY_Y_PROXY:
			val = (SHORT)Control()->KeyBordDisplayY();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void KeyBordProperyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("键盘位置"), KEYBORD_POSITION_PROXY, VT_UI1, _T("00-当前页面，01-其他页面"), TRUE},
		{_T("键盘所在页面"), KEYBORD_PAGE_PROXY, VT_UI1, _T(""), TRUE},
		{_T("键盘区域开始X"), KEYBORD_AREA_BEGIN_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域开始Y"), KEYBORD_AREA_BEGIN_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域结束X"), KEYBORD_AREA_END_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘区域结束Y"), KEYBORD_AREA_END_Y_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘显示位置X"), KEYBORD_DISPLAY_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("键盘显示位置Y"), KEYBORD_DISPLAY_Y_PROXY, VT_I2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL DataDisplayPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DATA_TYPE_PROXY:
			Control()->DataType((USHORT)val);
			break;
		case INTEGER_COUNT_PROXY:
			Control()->IntegerCount((USHORT)val);
			break;
		case DECIMAL_COUNT_PROXY:
			Control()->DecimalCount((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DataDisplayPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DATA_TYPE_PROXY:
			val = (USHORT)Control()->DataType();
			break;
		case INTEGER_COUNT_PROXY:
			val = (USHORT)Control()->IntegerCount();
			break;
		case DECIMAL_COUNT_PROXY:
			val = (USHORT)Control()->DecimalCount();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DataDisplayPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("数据类型"), DATA_TYPE_PROXY, VT_UI1, _T("0x00,整数（字），0x01，长整数（双子）0x02，变量指针高字节，0x03 变量指针低字节，0x04 超长整数（8字节）"), TRUE},
		{_T("整数位数"), INTEGER_COUNT_PROXY, VT_UI2, _T("控件类型"), TRUE},
		{_T("小数位数"), DECIMAL_COUNT_PROXY, VT_UI2, _T("控件类型"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL DataDisplayPosPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DISPLAY_X_PROXY:
			Control()->DisplayX((SHORT)val);
			break;
		case DISPLAY_Y_PROXY:
			Control()->DisplayY((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DataDisplayPosPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case DISPLAY_X_PROXY:
			val = (SHORT)Control()->DisplayX();
			break;
		case DISPLAY_Y_PROXY:
			val = (SHORT)Control()->DisplayY();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DataDisplayPosPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("显示位置X"), DISPLAY_X_PROXY, VT_I2, _T(""), TRUE},
		{_T("显示位置Y"), DISPLAY_Y_PROXY, VT_I2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL VariableProperyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		switch (propProxy.ProxyIP)
		{
		case VARIABLE_ADDR_PROXY:
			Control()->VariableAddr(static_cast<USHORT>(val));
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL VariableProperyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case VARIABLE_ADDR_PROXY:
			val = Control()->VariableAddr();
			break;
		default:
			bsuc = FALSE;
			break;
		}

	}
	return bsuc;
}

void VariableProperyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("变量地址"), VARIABLE_ADDR_PROXY, VT_UI2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}

}

BOOL WriteKeyProperyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		switch (propProxy.ProxyIP)
		{
		case WRITE_STYLE_PROXY:
			Control()->WriteStyle(static_cast<int>(val));
			break;
		case KEY_VALUE_PROXY:
			Control()->KeyValue(static_cast<SHORT>(val));
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL WriteKeyProperyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case WRITE_STYLE_PROXY:
			val = static_cast<int>(Control()->WriteStyle());
			break;
		case KEY_VALUE_PROXY:
			val = Control()->KeyValue();
			break;
		default:
			bsuc = FALSE;
			break;
		}

	}
	return bsuc;
}

void WriteKeyProperyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("变量写入方式"), WRITE_STYLE_PROXY, VT_UI1, _T("为控制选择时为0x10后4位比表示写入位0x"), TRUE},
		{_T("键值"), KEY_VALUE_PROXY, VT_UI2, _T("控件类型"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}

}

DataInputProperyPack::DataInputProperyPack()
{
	GroupName(_T("数据输入"));
}

DataInputProperyPack::~ DataInputProperyPack()
{

}

BOOL DataInputProperyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case CURSOR_TYPE_PROXY:
			Control()->CursorColor((USHORT)val);
			break;
		case DISPLAY_TYPE_PROXY:
			Control()->DisplayType((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DataInputProperyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case CURSOR_TYPE_PROXY:
			val = (USHORT)Control()->CursorColor();
			break;
		case DISPLAY_TYPE_PROXY:
			val = (USHORT)Control()->DisplayType();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DataInputProperyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("光标颜色"), CURSOR_TYPE_PROXY, VT_UI1, _T(""), TRUE},
		{_T("输入显示方式"), DISPLAY_TYPE_PROXY, VT_UI1, _T("00-显示*，01-直接显示"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL FontFamilyPosPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTFAMILY_PROXY:
			Control()->FontFamily((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL FontFamilyPosPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTFAMILY_PROXY:
			val = (USHORT)Control()->FontFamily();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void FontFamilyPosPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("字库位置"), FONTFAMILY_PROXY, VT_UI1, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL FontSizePack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTSIZE_PROXY:
			Control()->FontSize((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL FontSizePack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTSIZE_PROXY:
			val = (USHORT)Control()->FontSize();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void FontSizePack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("字体大小"), FONTSIZE_PROXY, VT_UI1, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL RangeSourportPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case ENABLE_RANGE_PROXY:
			Control()->EnableRange((bool)val);
			break;
		case RANGE_LOW_PROXY:
			Control()->RangeLow((int)val);
			break;
		case RANGE_HIGH_PROXY:
			Control()->RangeHigh((int)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL RangeSourportPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case ENABLE_RANGE_PROXY:
			val = (bool)Control()->EnableRange();
			break;
		case RANGE_LOW_PROXY:
			val = (int)Control()->RangeLow();
			break;
		case RANGE_HIGH_PROXY:
			val = (int)Control()->RangeHigh();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void RangeSourportPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("启用范围限制"), ENABLE_RANGE_PROXY, VT_BOOL, _T(""), TRUE},
		{_T("上限"), RANGE_LOW_PROXY, VT_INT, _T(""), TRUE},
		{_T("下限"), RANGE_HIGH_PROXY, VT_INT, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL DataLoadPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case LOADDATA_STYLE_PROXY:
			Control()->LoadDataStyle((USHORT)val);
			break;
		case LOADDATA_ADDR_PROXY:
			Control()->LoadDataAddr((USHORT)val);
			break;
		case LOADDATA_PROXY:
			Control()->LoadData((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DataLoadPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case LOADDATA_STYLE_PROXY:
			val = (USHORT)Control()->LoadDataStyle();
			break;
		case LOADDATA_ADDR_PROXY:
			val = (USHORT)Control()->LoadDataAddr();
			break;
		case LOADDATA_PROXY:
			val = (SHORT)Control()->LoadData();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DataLoadPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("加载数据方式"), LOADDATA_STYLE_PROXY, VT_UI1, _T("0x5A-录入过程中加载数据"), TRUE},
		{_T("数据地址"), LOADDATA_ADDR_PROXY, VT_UI2, _T(""), TRUE},
		{_T("加载数据"), LOADDATA_PROXY, VT_UI2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

DiTextInputPropertyPack::DiTextInputPropertyPack()
{
	GroupName(_T("文本录入"));
	AddPack(new BasicCtrlPropertyPack);
	AddPack(new VariableProperyPack);
	AddPack(new TextDisplayPack);
	AddPack(new TextInputPack);
	AddPack(new KeyBordProperyPack);
	AddPack(new DataDisplayPosPack);
	AddPack(new DataInputProperyPack);
}

DiTextInputPropertyPack::~ DiTextInputPropertyPack()
{

}

#define TEXT_LENGTH_PROXY (DATA_TYPE_PROXY + 100)
#define INPUT_MODE_PROXY (TEXT_LENGTH_PROXY + 1)
#define X_POINT_COUNT_PROXY (TEXT_LENGTH_PROXY + 2)
#define Y_POINT_COUNT_PROXY (TEXT_LENGTH_PROXY + 3)
#define INPUT_DISPLAY_AREA_X (TEXT_LENGTH_PROXY + 4)
#define INPUT_DISPLAY_AREA_Y (TEXT_LENGTH_PROXY + 5)

#define IS_SEL_INPUT_STATE_RETURN_PROXY (TEXT_LENGTH_PROXY + 6)

#define INPUT_DISPLAY_AREA_X_END (TEXT_LENGTH_PROXY + 7)
#define INPUT_DISPLAY_AREA_Y_END (TEXT_LENGTH_PROXY + 8)

BOOL TextInputPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		ITextInput* pCtrl = Control();
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case INPUT_MODE_PROXY:
			pCtrl->InputMode((USHORT)val);
			break;
		case INPUT_DISPLAY_AREA_X:
			pCtrl->InputDisplayAreaX((SHORT)val);
			break;
		case INPUT_DISPLAY_AREA_Y:
			pCtrl->InputDisplayAreaY((SHORT)val);
			break;
		case IS_SEL_INPUT_STATE_RETURN_PROXY:
			pCtrl->IsSelInputStateReturn((bool)val);
			break;
		case INPUT_DISPLAY_AREA_X_END:
			pCtrl->InputDisplayAreaX_End((SHORT)val);
			break;
		case INPUT_DISPLAY_AREA_Y_END:
			pCtrl->InputDisplayAreaY_End((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL TextInputPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		ITextInput* pCtrl = Control();
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case INPUT_MODE_PROXY:
			val = (USHORT)pCtrl->InputMode();
			break;
		case INPUT_DISPLAY_AREA_X:
			val = (SHORT)pCtrl->InputDisplayAreaX();
			break;
		case INPUT_DISPLAY_AREA_Y:
			val = (SHORT)pCtrl->InputDisplayAreaY();
			break;
		case IS_SEL_INPUT_STATE_RETURN_PROXY:
			val = (bool)pCtrl->IsSelInputStateReturn();
			break;
		case INPUT_DISPLAY_AREA_X_END:
			val = (SHORT)pCtrl->InputDisplayAreaX_End();
			break;
		case INPUT_DISPLAY_AREA_Y_END:
			val = (SHORT)pCtrl->InputDisplayAreaY_End();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void TextInputPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("录入模式"), INPUT_MODE_PROXY, VT_UI1, _T("0x00：从新录入 0x01：修改文本"), TRUE},
		{_T("录入显示区域X"), Y_POINT_COUNT_PROXY, VT_I2, _T(""), TRUE},
		{_T("录入显示区域Y"), Y_POINT_COUNT_PROXY, VT_I2, _T(""), TRUE},
		{_T("是否选输入状态返回"), Y_POINT_COUNT_PROXY, VT_BOOL, _T("选：0x55"), TRUE},
		{_T("录入显示区域X_End"), Y_POINT_COUNT_PROXY, VT_I2, _T(""), TRUE},
		{_T("录入显示区域Y_End"), Y_POINT_COUNT_PROXY, VT_I2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

BOOL TextDisplayPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		ITextDisplay* pCtrl = Control();
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case TEXT_LENGTH_PROXY:
			pCtrl->TextLength((USHORT)val);
			break;
		case X_POINT_COUNT_PROXY:
			pCtrl->XPointCount((SHORT)val);
			break;
		case Y_POINT_COUNT_PROXY:
			pCtrl->YPointCount((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL TextDisplayPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		ITextDisplay* pCtrl = Control();
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case TEXT_LENGTH_PROXY:
			val = (USHORT)pCtrl->TextLength();
			break;
		case X_POINT_COUNT_PROXY:
			val = (SHORT)pCtrl->XPointCount();
			break;
		case Y_POINT_COUNT_PROXY:
			val = (SHORT)pCtrl->YPointCount();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void TextDisplayPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("文本长度"), TEXT_LENGTH_PROXY, VT_UI2, _T(""), TRUE},
		{_T("X方向点阵数"), X_POINT_COUNT_PROXY, VT_UI2, _T("4--255"), TRUE},
		{_T("Y方向点阵数"), Y_POINT_COUNT_PROXY, VT_UI2, _T("4--255"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

#define DESCRIPTION_PTR_PROXY (TEXT_LENGTH_PROXY + 100)

BOOL DisplayBasicCtrlPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case SCREEN_INDEX_PROXY:
			Control()->ScreenIndex(static_cast<USHORT>(val));
			break;
		case TOP_PROXY:
			{
				int top = static_cast<int>(val);
				CDiSpriteView* pView = theApp.GetDiView();
				if (nullptr != pView && nullptr != pView->GetEditImagePage())
				{
					int ttop = max(0, top);
					ttop = min(pView->GetImageView()->GetLayout().Height - Control()->Height(), ttop);
					
					POINT pt = {Control()->Left(), ttop};
					Control()->Location(pt);
					if (ttop != top)
					{
						theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
					}
				}
			}
			break;
		case LEFT_PROXY:
			{
				int left = static_cast<int>(val);
				CDiSpriteView* pView = theApp.GetDiView();
				if (nullptr != pView && nullptr != pView->GetEditImagePage())
				{
					int tleft = max(0, left);
					tleft = min(pView->GetImageView()->GetLayout().Width - Control()->Width(), tleft);

					POINT pt = {tleft, Control()->Top()};
					Control()->Location(pt);
					if (tleft != left)
					{
						theApp.GetMainFarme()->GetPropertysView()->RefreshProperty();
					}
				}
			}
			break;
		case BOTTOM_PROXY:
			Control()->Height(static_cast<int>(val));
			break;
		case RIGHT_PROXY:
			Control()->Width(static_cast<int>(val));
			break;
		case DESCRIPTION_PTR_PROXY:
			Control()->DescriptionPtr((USHORT)val);
			break;
		case VARIABLE_ADDR_PROXY:
			Control()->VariableAddr((USHORT)val);
			break;
		case CONTROL_TYPE_PROXY:
			// Can't edit;
			Control()->ControlType(static_cast<int>(val));
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DisplayBasicCtrlPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case SCREEN_INDEX_PROXY:
			val = Control()->ScreenIndex();
			break;
		case TOP_PROXY:
			val = Control()->Top();
			break;
		case LEFT_PROXY:
			val = Control()->Left();
			break;
		case BOTTOM_PROXY:
			val = (INT)Control()->Height();
			break;
		case RIGHT_PROXY:
			val = (INT)Control()->Width();
			break;
		case DESCRIPTION_PTR_PROXY:
			val = Control()->DescriptionPtr();
			break;
		case VARIABLE_ADDR_PROXY:
			val	= Control()->VariableAddr();
			break;
		case CONTROL_TYPE_PROXY:
			val = static_cast<int>(Control()->ControlType());
			break;
		default:
			bsuc = FALSE;
			break;
		}

	}

	return bsuc;
}

void DisplayBasicCtrlPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
	PropProxyInfo tempProps[] = 
	{
		{_T("Screen Index"), SCREEN_INDEX_PROXY, VT_UI2, _T("第几屏"), FALSE},
		{_T("Top"), TOP_PROXY, VT_INT, _T("左上角Y坐标"), TRUE},
		{_T("Left"), LEFT_PROXY, VT_INT, _T("左上角X坐标"), TRUE},
		{_T("Width"), RIGHT_PROXY, VT_INT, _T("控件宽度"), TRUE},
		{_T("Height"), BOTTOM_PROXY, VT_INT, _T("控件高度"), TRUE},
		{_T("描述指针"), DESCRIPTION_PTR_PROXY, VT_UI2, _T("有值表示要切换到界面"), TRUE},
		{_T("变量地址"), VARIABLE_ADDR_PROXY, VT_UI2, _T("按键界面的效果"), TRUE},
		{_T("控件类型"), CONTROL_TYPE_PROXY, VT_UI1, _T("控件类型"), FALSE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}

}
#define UNIT_PIXEL_PROXY (TEXT_LENGTH_PROXY + 200)

QRCodePropertyPack::QRCodePropertyPack()
{
	GroupName(_T("二维码"));
	AddPack(new BasicCtrlPropertyPack);
}

QRCodePropertyPack::~QRCodePropertyPack()
{

}

BOOL QRCodePropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case UNIT_PIXEL_PROXY:
			Control()->UnitPixel((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL QRCodePropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case UNIT_PIXEL_PROXY:
			val = (USHORT)Control()->UnitPixel();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

void QRCodePropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);
	PropProxyInfo tempProps[] = 
	{
		{_T("unit_pixels"), UNIT_PIXEL_PROXY, VT_UI2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}

}

#define RENDER_COLOR_PROXY (TEXT_LENGTH_PROXY + 300)

BOOL RenderColorPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case RENDER_COLOR_PROXY:
			Control()->RenderColor((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL RenderColorPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case RENDER_COLOR_PROXY:
			val = (USHORT)Control()->RenderColor();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

void RenderColorPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("显示颜色"), RENDER_COLOR_PROXY, VT_UI2, _T("显示颜色"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

DiRTCTextPropertyPack::DiRTCTextPropertyPack()
{
	GroupName(_T("文本RTC显示"));
	AddPack(new DisplayBasicCtrlPropertyPack);
	AddPack(new RenderColorPack);
	AddPack(new FontFamilyPosPack);
	AddPack(new RTCFormatPropertyPack);
}

DiRTCTextPropertyPack::~DiRTCTextPropertyPack()
{

}

BOOL DiRTCTextPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case X_POINT_COUNT_PROXY:
			Control()->XPointCount((SHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL DiRTCTextPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case X_POINT_COUNT_PROXY:
			val = (SHORT)Control()->XPointCount();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void DiRTCTextPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("X方向点阵数"), X_POINT_COUNT_PROXY, VT_UI2, _T("4--255"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

#define AGLIMENT_PROXY (X_POINT_COUNT_PROXY + 100)

BOOL AglimentPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case AGLIMENT_PROXY:
			Control()->Agliment((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

BOOL AglimentPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case AGLIMENT_PROXY:
			val = (USHORT)Control()->Agliment();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}
	return bsuc;
}

void AglimentPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("对齐方式"), AGLIMENT_PROXY, VT_UI2, _T("0:左对齐，1：右对齐，2：居中"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

DiDataDisplayPropertyPack::DiDataDisplayPropertyPack()
{
	GroupName(_T("数据变量显示"));
	AddPack(new DisplayBasicCtrlPropertyPack);
	AddPack(new RenderColorPack);
	AddPack(new FontFamilyPosPack);
	AddPack(new FontSizePack);
	AddPack(new DataDisplayPack);
	AddPack(new AglimentPack);
}

DiDataDisplayPropertyPack::~DiDataDisplayPropertyPack()
{

}

DiTextPropertyPack::DiTextPropertyPack()
{
	GroupName(_T("文本显示"));
	AddPack(new DisplayBasicCtrlPropertyPack);
	AddPack(new RenderColorPack);
	AddPack(new TextDisplayPack);
	AddPack(new FontFamilyPosPack);

}

DiTextPropertyPack::~DiTextPropertyPack()
{

}

#define IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY (UNIT_PIXEL_PROXY + 100)
#define FONTFAMILY_1_PROXY (IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY + 4)
#define CODING_STYLE_PROXY (IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY + 1)
#define HORIZONTAL_MARGIN_PROXY (IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY + 2)
#define VERTICAL_MARGIN_PROXY (IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY + 3)

BOOL DiTextPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTFAMILY_1_PROXY:
			Control()->FontFamily1((USHORT)val);
			break;
		case IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY:
			Control()->IsNotAutoAdjustCharsetDis((bool)val);
			break;
		case CODING_STYLE_PROXY:
			Control()->CodingStyle((USHORT)val);
			break;
		case HORIZONTAL_MARGIN_PROXY:
			Control()->HorizontalMargin((USHORT)val);
			break;
		case VERTICAL_MARGIN_PROXY:
			Control()->VerticalMargin((USHORT)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL DiTextPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case FONTFAMILY_1_PROXY:
			val = (USHORT)Control()->FontFamily1();
			break;
		case IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY:
			val = (bool)Control()->IsNotAutoAdjustCharsetDis();
			break;
		case CODING_STYLE_PROXY:
			val = (USHORT)Control()->CodingStyle();
			break;
		case HORIZONTAL_MARGIN_PROXY:
			val = (USHORT)Control()->HorizontalMargin();
			break;
		case VERTICAL_MARGIN_PROXY:
			val = (USHORT)Control()->VerticalMargin();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

void DiTextPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("字库位置1"), FONTFAMILY_1_PROXY, VT_UI2, _T(""), TRUE},
		{_T("不自动调节字符间距"), IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY, VT_BOOL, _T(""), TRUE},
		{_T("编码方式"), CODING_STYLE_PROXY, VT_UI2, _T("0x00-8bit编码；0x01-gb2312内码；0x02-GBK；0X03-BIG5；0X04-SJIS；0X05-UNICODE"), TRUE},
		{_T("水平间隔"), HORIZONTAL_MARGIN_PROXY, VT_UI2, _T(""), TRUE},
		{_T("垂直间隔"), HORIZONTAL_MARGIN_PROXY, VT_UI2, _T(""), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}

#define RTCFORMAT_PROXY (IS_NOTAUTO_ADJUST_CHARSET_DIS_PROXY + 100)

RTCFormatPropertyPack::RTCFormatPropertyPack()
{
	GroupName(_T("RTC格式"));
}

RTCFormatPropertyPack::~RTCFormatPropertyPack()
{

}

BOOL RTCFormatPropertyPack::OnSetPropertyValue(const PropProxyInfo& propProxy, const _variant_t& val)
{
	BOOL bsuc = __super::OnSetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case RTCFORMAT_PROXY:
			Control()->RTCFormat((CString)val);
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

BOOL RTCFormatPropertyPack::OnGetPropertyValue(const PropProxyInfo& propProxy, _variant_t& val)
{
	BOOL bsuc = __super::OnGetPropertyValue(propProxy, val);
	if (!bsuc)
	{
		bsuc = TRUE;
		switch (propProxy.ProxyIP)
		{
		case RTCFORMAT_PROXY:
			val = Control()->RTCFormat();
			break;
		default:
			bsuc = FALSE;
			break;
		}
	}

	return bsuc;
}

void RTCFormatPropertyPack::ProducePropertiesMap(vector<PropProxyInfo>& propMap)
{
	__super::ProducePropertiesMap(propMap);

	PropProxyInfo tempProps[] = 
	{
		{_T("RTC格式"), RTCFORMAT_PROXY, VT_LPWSTR, _T("RTC 格式，请输入小于等于16字节个ASCII码。"), TRUE},
	};

	const int count = sizeof(tempProps) / sizeof(PropProxyInfo);
	for (int i = 0; i < count; ++i)
	{
		propMap.push_back(tempProps[i]);
	}
}
