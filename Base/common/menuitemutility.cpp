
#include "stdafx.h"
#include "menuitemutility.h"
#include "Control\UIMenu.h"
#include "Common\CommonInterface.h"

void updateMaxPixelLengthForMenuItemText(HDC hDC, LPCTSTR szText, DWORD &uiMaximum)
{
    DWORD length = static_cast<CCommonInterface*>(0)->GetStringDClength(
        hDC, szText, _T("Î¢ÈíÑÅºÚ"), 13);
    if (uiMaximum < length)
    {
        uiMaximum = length;
    }
}

void adjustPixelLengthOfDirectMenuItems(
    DuiLib::CMenuElementUI* pElement, LONG lPadding, DWORD uiMaxPixelLength)
{
    if (NULL != pElement && pElement->GetInterface(_T("MenuElement")) != NULL)
    {
        for (int i = 0; i < pElement->GetCount(); ++i)
        {
            CControlUI* pControl = pElement->GetItemAt(i);
            pControl->SetFixedWidth(
                //pElement->GetOwner()->GetListInfo()->rcTextPadding.left +
                lPadding + 
                static_cast<int>(uiMaxPixelLength) +
                DuiLib::ITEM_DEFAULT_EXPLAND_ICON_WIDTH);
        }
    }
}
