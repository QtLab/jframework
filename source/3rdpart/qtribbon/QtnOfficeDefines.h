
#ifndef OFFICEDEFINES_H
#define OFFICEDEFINES_H

namespace QtRibbon
{
    #define QTN_VERSION_RIBBON_STR "3.0.0"
    #define QTN_VERSION_OFFICE_STR "3.0.0"

    #define qtn_Prop_Index          "Index"
    #define qtn_Prop_Group          "Group"
    #define qtn_Prop_Wrap           "Wrap"
    #define qtn_Prop_Wrap_Group     "WrapGroup"
    #define qtn_Prop_Begin_Group    "BeginGroup"
    #define qtn_Prop_Index_Group    "IndexGroup"
    #define qtn_Prop_Switch_Count   "SwitchCount"

    #define qtn_Prop_Key_Tip        "QtRibbon::KeyTip"

    #define qtn_PopupButtonGallery        "PopupButtonGallery"
    #define qtn_ScrollUpButtonGallery     "ScrollUpButtonGallery"
    #define qtn_ScrollDownButtonGallery   "ScrollDownButtonGallery"

    #define qtn_PopupBar                  "PopupBar"
    #define qtn_PopupLable                "PopupLable_"
    #define qtn_SplitActionPopup          "SplitActionPopup_"
    #define qtn_WidgetGallery             "WidgetGallery"

    /*! \internal */
    enum BarType
    {
        TypeMenuBar,
        TypeNormal,
        TypePopup,
        TypeRibbon,
        TypeSplitButtonPopup,
    };

    /*! \internal */
    enum BarPosition
    {
        BarTop,
        BarBottom,
        BarLeft,
        BarRight,
        BarFloating,
        BarPopup,
        BarListBox,
        BarNone
    };

} // namespace QtRibbon

#endif //OFFICEDEFINES_H

