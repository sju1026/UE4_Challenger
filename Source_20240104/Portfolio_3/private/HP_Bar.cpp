#include "HP_Bar.h"

void UHP_Bar::NativeConstruct() {

}

void UHP_Bar::set_bar_value_percent(float const value)
{
	hp_value->SetPercent(value);
}
