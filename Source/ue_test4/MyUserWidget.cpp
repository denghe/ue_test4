#include "MyUserWidget.h"

#include "Components/TextBlock.h"

void UMyUserWidget::SetFps(int fps)
{
	fpsTextBlock->SetText(FText::FromString(FString::FromInt(fps)));
}
