// Weichao Qiu @ 2017
#include "LitCamSensor.h"
#include "UnrealcvLog.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ULitCamSensor::ULitCamSensor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

// void ULitCamSensor::SetupRenderTarget()
// {
// 	bool bUseLinearGamma = false;
// 	TextureTarget->InitCustomFormat(FilmWidth, FilmHeight, EPixelFormat::PF_B8G8R8A8, bUseLinearGamma);
// }

// bool bUseLinearGamma = false;
// bool bUseLinearGamma = true;  // true by default
// bUseLinearGamma requires CaptureEveryFrame!
// TextureTarget->InitAutoFormat(Width, Height);
// TextureTarget->TargetGamma = GEngine->GetDisplayGamma();
// TextureTarget->TargetGamma = 1;

void ULitCamSensor::InitTextureTarget(int FilmWidthx, int FilmHeightx)
{
	TextureTarget = NewObject<UTextureRenderTarget2D>(this); 
	TextureTarget->InitAutoFormat(FilmWidthx, FilmHeightx);
	TextureTarget->TargetGamma = GEngine->GetDisplayGamma();
}

void ULitCamSensor::CaptureLit(TArray<FColor>& Image, int& Width, int& Height)
{
	if (!CheckTextureTarget())
	{
		InitTextureTarget(this->FilmWidth, this->FilmHeight);
		if (!CheckTextureTarget())
		{
			UE_LOG(LogUnrealCV, Error, TEXT("Failed to initialize TextureTarget."));
			return;
		}
	}
	this->CaptureScene();
	FReadSurfaceDataFlags ReadSurfaceDataFlags;
	ReadSurfaceDataFlags.SetLinearToGamma(false); 
	// TextureTarget->GetRenderTargetResource()->ReadPixels(Image, ReadSurfaceDataFlags);
	TextureTarget->GameThread_GetRenderTargetResource()->ReadPixels(Image, ReadSurfaceDataFlags);
	if (Image.Num() == 0)
	{
		UE_LOG(LogUnrealCV, Warning, TEXT("Captured lit data is empty."));
	}
	Width = GetFilmWidth();
	Height = GetFilmHeight();
}