
#ifndef MENU_ITEM_UTILITY_HHH
#define MENU_ITEM_UTILITY_HHH

namespace DuiLib
{
    class CMenuElementUI;
}

void updateMaxPixelLengthForMenuItemText(HDC hDC, LPCTSTR szText, DWORD &uiMaximum);

// 'itemtextpadding' only set on first layer item (that defined in xml file). 
// So this value should be obtained from the CMenuElementUI object of that item.
void adjustPixelLengthOfDirectMenuItems(
    DuiLib::CMenuElementUI* pElement, LONG lPadding, DWORD uiMaxPixelLength);


#endif
