// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdaptText.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "SlateGlobals.h"
#include "Framework/Text/PlainTextLayoutMarshaller.h"
#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Types/ReflectionMetadata.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"

class FAdaptTextEllipse : public FAdaptTextBase
{

public:
	FAdaptTextEllipse(const SAdaptText::FArguments& InArgs)
	{
		WrapTextAt = InArgs._WrapTextAt;
		AutoWrapText = InArgs._AutoWrapText;
		CurFont = InArgs._Font.Get();
		MaxLines = InArgs._TextMaxLines.Get();

		MySizeBox = SNew(SBox)
			.HAlign(InArgs._HAlign)
			.VAlign(InArgs._VAlign)
			.WidthOverride(InArgs._WidthOverride)
			.HeightOverride(InArgs._HeightOverride)
			.MinDesiredWidth(InArgs._MinDesiredWidth)
			.MinDesiredHeight(InArgs._MinDesiredHeight)
			.MaxDesiredWidth(InArgs._MaxDesiredWidth)
			.MaxDesiredHeight(InArgs._MaxDesiredHeight)
			.Padding(FMargin())
			[
				SAssignNew(MyTextBlock, STextBlock)
				.Text(InArgs._Text)
				.TextStyle(InArgs._TextStyle)
				.Font(InArgs._Font)
				.StrikeBrush(InArgs._StrikeBrush)
				.ColorAndOpacity(InArgs._ColorAndOpacity)
				.ShadowOffset(InArgs._ShadowOffset)
				.ShadowColorAndOpacity(InArgs._ShadowColorAndOpacity)
				.HighlightColor(InArgs._HighlightColor)
				.HighlightShape(InArgs._HighlightShape)
				.HighlightText(InArgs._HighlightText)
				.WrapTextAt(InArgs._WrapTextAt)
				.AutoWrapText(InArgs._AutoWrapText)
				.WrappingPolicy(InArgs._WrappingPolicy)
				.Margin(InArgs._Margin)
				.LineHeightPercentage(InArgs._LineHeightPercentage)
				.Justification(InArgs._Justification)
				.MinDesiredWidth(InArgs._TextMinDesiredWidth)
				.TextShapingMethod(InArgs._TextShapingMethod)
				.TextFlowDirection(InArgs._TextFlowDirection)
				.LineBreakPolicy(InArgs._LineBreakPolicy)
				.SimpleTextMode(InArgs._SimpleTextMode)
				.OnDoubleClicked(InArgs._OnDoubleClicked)
				.Clipping(EWidgetClipping::Inherit)
			];
	}

	void SetText(const TAttribute<FText>& InText) override
	{
		SetText(InText.Get());
	}

	void SetText(const FText& InText) override
	{
		FAdaptTextBase::SetText(InText);
		TextStr = InText.ToString();
		TextLength = TextStr.Len();
		EllipseIndex = TextLength;
	}

	void SetFont(const TAttribute<FSlateFontInfo>& InFont) override
	{
		FAdaptTextBase::SetFont(InFont);
		CurFont = InFont.Get();
		//SetFont后字号和阴影等都会影响行高，这里也尽量给到一个比较大的高度，所以闪烁会比较严重
		UpdateMaxDesiredHeight(true);
	}

	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt) override
	{
		FAdaptTextBase::SetWrapTextAt(InWrapTextAt);
		if (!WrapTextAt.IdenticalTo(InWrapTextAt))
		{
			WrapTextAt = InWrapTextAt;
		}
	}

	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText) override
	{
		FAdaptTextBase::SetAutoWrapText(InAutoWrapText);
		if (!AutoWrapText.IdenticalTo(InAutoWrapText))
		{
			AutoWrapText = InAutoWrapText;
		}
	}

	void SetMinDesiredWidth(TAttribute<FOptionalSize> InMinDesiredWidth) override
	{
		FAdaptTextBase::SetMinDesiredWidth(InMinDesiredWidth);
		CacheBoxMinWidth = InMinDesiredWidth.Get();
	}

	void SetMaxDesiredHeight(TAttribute<FOptionalSize> InMaxDesiredHeight) override
	{
		CacheBoxMaxHeight = InMaxDesiredHeight.Get();
		//为了缓解闪烁，这里一开始就给文本限制一个大概的高度，这样可以把闪烁大小控制在一行以内
		UpdateMaxDesiredHeight();
	}

	TSharedRef<SWidget> GetWidget() override
	{
		return GetSizeBox();
	}

	void SetTextMaxLines(const TAttribute<FOptionalSize>& InMaxLines)
	{
		if (MaxLines == InMaxLines.Get())
		{
			return;
		}

		MaxLines = InMaxLines.Get();
		InvalidateAdaptText(EInvalidateAdaptTextReason::Layout);
		UpdateMaxDesiredHeight();
		//UE_LOG(LogTemp, Log, TEXT("SetTextMaxLines  IsSet:%d  MaxLines:%f"), MaxLines.IsSet(), MaxLines.Get());
	}

protected:
	TSharedRef<SBox> GetSizeBox() override
	{
		return MySizeBox.ToSharedRef();
	}

	TSharedRef<STextBlock> GetTextBlock() override
	{
		return MyTextBlock.ToSharedRef();
	}

	void OnInvalidate(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime, const EInvalidateAdaptTextReason& Reason) override
	{
		if (!(EnumHasAnyFlags(EInvalidateAdaptTextReason::SetText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetFont, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetAutoWrapText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetWrapTextAt, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::Layout, Reason)))
		{
			return;
		}
		bIsFirstTimeToEllipse = true;
		bMaxLinesHeightInvalidate = MaxLines.IsSet() && (EnumHasAnyFlags(EInvalidateAdaptTextReason::SetFont, Reason) || EnumHasAnyFlags(EInvalidateAdaptTextReason::Layout, Reason));
		bLineHeightInvalidate = EnumHasAnyFlags(EInvalidateAdaptTextReason::SetFont, Reason);
		//加个裁切，缓解下闪烁
		MySizeBox->SetClipping(EWidgetClipping::ClipToBounds);
		UE_LOG(LogTemp, Warning, TEXT("FAdaptTextEllipse  OnInvalidate  BoxSize:%s"), *AllottedGeometry.GetLocalSize().ToString());
	}

	void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		if (!bIsFirstTimeToEllipse && MySizeBox->GetClipping() != EWidgetClipping::Inherit)
		{
			MySizeBox->SetClipping(EWidgetClipping::Inherit);
		}
		CacheBoxSize = AllottedGeometry.GetLocalSize();
		CacheTextSize = MyTextBlock->GetDesiredSize();
		if (CacheGeometryScale != AllottedGeometry.Scale)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnTick  CacheGeometryScale != GeometryScale  CacheGeometryScale:%f  GeometryScale:%f  BoxSize:%s  TextSize:%s"), CacheGeometryScale, AllottedGeometry.Scale, *CacheBoxSize.ToString(), *CacheTextSize.ToString());
			bLineHeightInvalidate = true;
			bMaxLinesHeightInvalidate = true;
		}

		if (bLineHeightInvalidate)
		{
			ComputeSuffixSize(AllottedGeometry.Scale);
			if (GetIsWrapText())
			{
				//GeometryScale变化的时候，文本大小却不是等比例缩放，这时候文本可能会换行，需要重新计算
				bForceResize = true;
			}
			bLineHeightInvalidate = false;
		}

		if (bMaxLinesHeightInvalidate)
		{
			//重新计算 MaxLinesHeight
			ComputeMaxLinesHeight(CacheTextSize.Y, !GetIsWrapText());
			bMaxLinesHeightInvalidate = false;
		}

		//UE_LOG(LogTemp, Warning, TEXT("GeometryScale:%f GetLocalSize:%s  TextSize:%s"), AllottedGeometry.Scale, *AllottedGeometry.GetLocalSize().ToString(), *CacheTextSize.ToString());
		EllipseText(AllottedGeometry.Scale, CacheBoxSize, CacheTextSize);
		CacheGeometryScale = AllottedGeometry.Scale;
	}

private:
	bool GetIsWrapText() const
	{
		return AutoWrapText.Get() || WrapTextAt.Get() > 0;
	}

	void UpdateMaxDesiredHeight(bool bUseCachePriority = true)
	{
		if (MaxLines.IsSet())
		{
			float MaxHeight = CacheLineHeight;
			if (!bUseCachePriority || LineHeight <= 0.f)
			{
				ComputeSuffixSize(0.15f);
				GetRetRealLineHeight(MaxHeight);
				UE_LOG(LogTemp, Log, TEXT("UpdateMaxDesiredHeight111   LineHeight:%f  MaxHeight:%f"), LineHeight, MaxHeight);
			}
			MaxHeight = FMath::CeilToInt(MaxHeight * MaxLines.Get());

			if (CacheBoxMaxHeight.IsSet())
			{
				MaxHeight = FMath::Min(CacheBoxMaxHeight.Get(), MaxHeight);
			}
			
			FAdaptTextBase::SetMaxDesiredHeight(MaxHeight);
			UE_LOG(LogTemp, Log, TEXT("UpdateMaxDesiredHeight   LineHeight:%f  MaxHeight:%f"), LineHeight, MaxHeight);
		}
		else
		{
			FAdaptTextBase::SetMaxDesiredHeight(CacheBoxMaxHeight);
		}
	}

	float GetLineHeightOffset(const float& FontScale)
	{
		static float OffsetTable[12][2] = {
			{1.f, 1.f},        //>1  (1.0, 0.0)
			{0.875f, 1.15f},   //-1  1.142856
			{0.75f, 1.34f},    //-2  1.333332
			{0.675f, 1.49f},   //-3  1.481483
			{0.5f, 2.f},       //-4  2.0
			{0.375f, 2.67 },   //-5  2.666664
			{0.25f, 4.f },     //-6  4.0
			{0.225f, 4.45f },  //-7  4.444443
			{0.2f, 5.f},       //-8  5.0
			{0.175f, 5.72f },  //-9  5.714287
			{0.15f, 6.67f },   //10  6.666664
			{0.f, 8.f}         //-Infinite
		};
		for (int32 Index = 0; Index < 12; Index++)
		{
			if (FontScale >= OffsetTable[Index][0])
			{
				return OffsetTable[Index][1];
			}
		}

		return 10.f;
	}

	bool GetRetRealLineHeight(float& OutLineHeight)
	{
		if (LineHeight <= 0.f)
		{
			return false;
		}
		OutLineHeight = LineHeight + LineHeightOffset;
		return true;
	}

	void ComputeSuffixSize(const float& FontScale)
	{
		//可以给Measure的FontScale传1.f，但通过FontScale换算回来的结果更加精确
		SuffixSize = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(Suffix, CurFont, FontScale);
		SuffixSize = SuffixSize / FontScale;
		//几乎都是偏差了1.f
		SuffixSize.X += 1.f;
		//LineHeight偏差明显(但总会比实际的偏小)，可以通过GetLineHeightOffset获取大概的偏差值
		LineHeight = SuffixSize.Y;
		LineHeightOffset = GetLineHeightOffset(FontScale);
		UE_LOG(LogTemp, Warning, TEXT("SuffixSize:%s  TextSize:%s   FontScale:%f"), *SuffixSize.ToString(), *CacheTextSize.ToString(), FontScale);
	}

	void ComputeMaxLinesHeight(const float& TextHeight, const bool&& bIsSingleLine = false)
	{
		if (!MaxLines.IsSet())
		{
			return;
		}

		float MaxLinesNum = FMath::Max(MaxLines.Get(), 0.f);
		float LinesNum = 1;
		if (!bIsSingleLine)
		{
			LinesNum = TextHeight / LineHeight;
			UE_LOG(LogTemp, Log, TEXT("ComputeMaxLinesHeight  TextHeight:%f  LineHeight:%f  LinesNum:%f"), TextHeight, LineHeight, LinesNum);
			LinesNum = FMath::FloorToInt(LinesNum);
		}
		float RealLineHeight = TextHeight / LinesNum;
		float RetRealLineHeight;
		GetRetRealLineHeight(RetRealLineHeight);
		float Offset = RetRealLineHeight - RealLineHeight;
		//OffsetTable中每个区间的偏差不超过2.0
		if (Offset < 0 || Offset > 2.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("ComputeMaxLinesHeight  The Offset between the RealLineHeight and the RetRealLineHeight is too large!!!"));
			return;
		}

		CacheLineHeight = RealLineHeight;
		CacheMaxLinesHeight = FMath::CeilToInt(RetRealLineHeight * MaxLinesNum);
		UpdateMaxDesiredHeight();
		UE_LOG(LogTemp, Log, TEXT("ComputeMaxLinesHeight   MaxLinesNum:%f   CacheMaxLinesHeight:%f, CacheLineHeight:%f,  LinesNum:%f  TextHeight:%f   LineHeight:%f"), MaxLinesNum, CacheMaxLinesHeight, CacheLineHeight, LinesNum, TextHeight, LineHeight);
	}

	void UpdateSizeWithMaxLines(FVector2D& BoxSize)
	{
		if (MaxLines.IsSet())
		{
			BoxSize.Y = FMath::Min(BoxSize.Y, CacheMaxLinesHeight);
		}
	}

	bool ResizeWrapText(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		bool bHasResize = false;

		static float LastReduceBoxWidth = 0.f;
		static float LastReduceTextHeight = 0.f;

		if (BoxSize.Y >= TextSize.Y && EllipseIndex < TextLength && (bForceResize || BoxSize.X >= LastReduceBoxWidth + 2 || BoxSize.Y >= LastReduceTextHeight))
		{
			//UE_LOG(LogTemp, Log, TEXT("ResizeWrapText ++   BoxSize.Y:%f  TextSize.Y:%f"), BoxSize.Y, TextSize.Y);
			EllipseIndex++;
			bHasResize = true;
		}
		else if (BoxSize.Y < TextSize.Y && EllipseIndex > 0)
		{
			//UE_LOG(LogTemp, Log, TEXT("ResizeWrapText --   BoxSize.Y:%f  TextSize.Y:%f"), BoxSize.Y, TextSize.Y);
			LastReduceBoxWidth = BoxSize.X;
			LastReduceTextHeight = TextSize.Y;
			EllipseIndex--;
			bHasResize = true;
			bForceResize = false;
		}

		if (bHasResize)
		{
			FString InString = TextStr.Left(EllipseIndex);
			if (EllipseIndex != TextLength)
			{
				InString.Append(Suffix);
			}
			MyTextBlock->SetText(FText::FromString(InString));
		}

		return bHasResize;
	}

	bool EllipseWrapText(const float& GeometryScale, FVector2D BoxSize, FVector2D TextSize)
	{
		bool bHasResize = false;
		static FVector2D CacheWrapBoxSize = FVector2D::ZeroVector;

		UpdateSizeWithMaxLines(BoxSize);

		bool bCanFastEllipse = bIsFirstTimeToEllipse;
		if (!bCanFastEllipse && CacheWrapBoxSize != BoxSize)
		{
			//当文本高度与容器高度相差大于1行时使用快速计算
			float DiffSize = BoxSize.Y - TextSize.Y;
			bCanFastEllipse = (DiffSize > CacheLineHeight && EllipseIndex < TextLength) || DiffSize < 0;
		}
		
		if (bCanFastEllipse)
		{
			CacheWrapBoxSize = BoxSize;
			float LinesNum = BoxSize.Y / LineHeight;
			LinesNum = FMath::FloorToInt(LinesNum);
			//AutoWrapText首次加载时TextSize.X是单行的宽度，而不是换行之后的宽度，就算是调用ComputeDesiredSize也一样。
			float FastSize = TextSize.X;  
			if (AutoWrapText.Get())
			{
				FastSize = FMath::Min(BoxSize.X, TextSize.X);
			}
			FastSize = LinesNum * FastSize - SuffixSize.X;
			int32 FastIndex = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->FindCharacterIndexAtOffset(TextStr, CurFont, FastSize);
			FastIndex = FMath::Clamp(FastIndex, 0, TextLength);

			UE_LOG(LogTemp, Warning, TEXT("EllipseWrapText FastEllipse    LinesNum:%f  TextLength:%d  EllipseIndex:%d  FastIndex:%d"), LinesNum, TextLength, EllipseIndex, FastIndex);
			if (FastIndex != EllipseIndex)
			{
				EllipseIndex = FastIndex;
				FString Str = TextStr.Left(EllipseIndex);
				if (EllipseIndex != TextLength)
				{
					Str.Append(Suffix);
				}
				MyTextBlock->SetText(FText::FromString(Str));
				TextSize = MyTextBlock->ComputeDesiredSize(GeometryScale);
				//FastEllipse会有偏差，所以还得Resize
				bForceResize = true;
				bHasResize = true;
			}
			//这里直接return掉，因为首帧得到的TextSize都不太正常，AutoWrapText下得到的是一个单行Size，WrapTextAt下得到的高度是一个很大的值（在PlayInEditor下貌似没问题）
			//在第二帧开始都没问题，但这样就很难避免闪烁
			if (bIsFirstTimeToEllipse)
			{
				bIsFirstTimeToEllipse = false;
				return bHasResize;
			}
		}

		//微调
		int32 ResizeCount = 0;  //计算次数
		while (ResizeWrapText(BoxSize, TextSize))
		{
			TextSize = MyTextBlock->ComputeDesiredSize(GeometryScale);
#if WITH_EDITOR
			//在Editor中打开蓝图时，首帧得到的Scale是1:1，TextSize也是1:1下的到结果
			//但是次帧开始Scale为0.5(打开蓝图默认缩放为-4)，然后TextSize还是1:1下的结果，除非手动点编译或缩放屏幕才会重新计算
			//是不是应该InvalidateText
			TextSize.Y = FMath::FloorToInt(TextSize.Y / CacheLineHeight) * CacheLineHeight;
#endif
			//UE_LOG(LogTemp, Log, TEXT("while BoxSize:%s  TextSize:%s   ========================"), *BoxSize.ToString(), *TextSize.ToString());
			bHasResize = true;
			ResizeCount++;
		}
		if (ResizeCount > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResizeCount:%d  EllipseIndex:%d"), ResizeCount, EllipseIndex);
		}

		return bHasResize;
	}

	bool EllipseNoWrapText(const float& GeometryScale, const FVector2D& BoxSize, FVector2D TextSize)
	{
		bool bHasResize = false;
		static float CacheTextDesiredWidth = 0.f;
		static float CacheBoxWidth = 0.f;
		
		if (bIsFirstTimeToEllipse || CacheBoxWidth != BoxSize.X)
		{
			CacheBoxWidth = BoxSize.X;
			int32 FastIndex = 0;
			//SizeToContent的情况下，BoxSize等于TextSize，但是TextSize会根据GeometryScale变化，所以BoxSize也会变化
			if (bIsFirstTimeToEllipse || CacheGeometryScale != GeometryScale)
			{
				CacheTextDesiredWidth = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(TextStr, CurFont, GeometryScale).X;
				CacheTextDesiredWidth = CacheTextDesiredWidth / GeometryScale + 1.f;
				//Ellipse后文本会变短，这期间要是SizeToContent的话就得不到原来的宽度，所以提前加个MinDesiredWidth
				MySizeBox->SetMinDesiredWidth(CacheBoxMinWidth.IsSet() ? FMath::Min(CacheBoxMinWidth.Get(), CacheTextDesiredWidth) : CacheTextDesiredWidth);
			}

			if (BoxSize.X >= CacheTextDesiredWidth)
			{
				FastIndex = TextLength;
			}
			else
			{
				FastIndex = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->FindCharacterIndexAtOffset(TextStr, CurFont, BoxSize.X - SuffixSize.X);
				FastIndex = FMath::Clamp(FastIndex, 0, TextLength);
				UE_LOG(LogTemp, Warning, TEXT("EllipseNoWrapText  FastIndex:%d  Str:%s"), FastIndex, *TextStr.Left(FastIndex));
			}

			if (FastIndex != EllipseIndex)
			{
				EllipseIndex = FastIndex;
				FString Str = TextStr.Left(EllipseIndex);
				if (EllipseIndex != TextLength)
				{
					Str.Append(Suffix);
				}
				MyTextBlock->SetText(FText::AsCultureInvariant(Str));
				bHasResize = true;
			}

			if (bIsFirstTimeToEllipse)
			{
				bIsFirstTimeToEllipse = false;
				return bHasResize;
			}
		}

		return bHasResize;
	}

	void EllipseText(const float& GeometryScale, const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		if (GetIsWrapText())
		{
			EllipseWrapText(GeometryScale, BoxSize, TextSize);
		}
		else
		{
			EllipseNoWrapText(GeometryScale, BoxSize, TextSize);
		}
	}

private:
	FSlateFontInfo CurFont;
	TAttribute<float> WrapTextAt;
	TAttribute<bool> AutoWrapText;
	FOptionalSize CacheBoxMinWidth;
	FOptionalSize CacheBoxMaxHeight;
	FOptionalSize MaxLines;                //最大行数，限制到合适的行数可以缓解闪烁
	float CacheMaxLinesHeight = 0.f;       //缓存根据MaxLines计算到的MaxLinesHeight值
	float CacheLineHeight = 0.f;           //缓存行高，每次计算MaxLinesHeight都能得到一个相对真实的行高
	bool bMaxLinesHeightInvalidate = false;//是否重新计算MaxLinesHeight
	float LineHeight = -1;                 //通过Measure得到的行高，并不准确
	float LineHeightOffset = 0.f;          //LineHeight与实际行高的大致偏差
	bool bLineHeightInvalidate = false;    //是否重新计算LineHeight

	TSharedPtr<STextBlock> MyTextBlock;
	TSharedPtr<SBox> MySizeBox;

	FString TextStr;       //每次SetText后都会保存一份，用于接下来的Ellipse
	int32 TextLength = 0;  //文本的最大长度，SetText时重置
	int32 EllipseIndex = 0;//当前Ellipse的Index

	const FString Suffix = TEXT("...");//省略符号
	FVector2D SuffixSize;              //省略符号的Size

	FVector2D CacheBoxSize;       //缓存Geometry的Size，考虑到要是SetText等可以提前Ellipse，可以缓解闪烁
	FVector2D CacheTextSize;      //缓存TextSize
	float CacheGeometryScale = -1;//缓存Geometry的Scale
	bool bIsFirstTimeToEllipse = false;
	bool bForceResize = false;
};

class FAdaptTextBestFit : public FAdaptTextBase
{

public:
	FAdaptTextBestFit(const SAdaptText::FArguments& InArgs)
	{
		BoundText = InArgs._Text;
		WrapTextAt = InArgs._WrapTextAt;
		AutoWrapText = InArgs._AutoWrapText;
		SetTextMinFontSize(InArgs._TextMinFontSize);
		SetCurFont(InArgs._Font);
		OnTextFontSizeChangedCallback = InArgs._OnTextFontSizeChanged;

		MySizeBox = SNew(SBox)
			.HAlign(InArgs._HAlign)
			.VAlign(InArgs._VAlign)
			.WidthOverride(InArgs._WidthOverride)
			.HeightOverride(InArgs._HeightOverride)
			.MinDesiredWidth(InArgs._MinDesiredWidth)
			.MinDesiredHeight(InArgs._MinDesiredHeight)
			.MaxDesiredWidth(InArgs._MaxDesiredWidth)
			.MaxDesiredHeight(InArgs._MaxDesiredHeight)
			.Padding(FMargin())
			[
				SAssignNew(MyTextBlock, STextBlock)
				.Text(InArgs._Text)
				.TextStyle(InArgs._TextStyle)
				.Font(InArgs._Font)
				.StrikeBrush(InArgs._StrikeBrush)
				.ColorAndOpacity(InArgs._ColorAndOpacity)
				.ShadowOffset(InArgs._ShadowOffset)
				.ShadowColorAndOpacity(InArgs._ShadowColorAndOpacity)
				.HighlightColor(InArgs._HighlightColor)
				.HighlightShape(InArgs._HighlightShape)
				.HighlightText(InArgs._HighlightText)
				.WrapTextAt(InArgs._WrapTextAt)
				.AutoWrapText(InArgs._AutoWrapText)
				.WrappingPolicy(InArgs._WrappingPolicy)
				.Margin(InArgs._Margin)
				.LineHeightPercentage(InArgs._LineHeightPercentage)
				.Justification(InArgs._Justification)
				.MinDesiredWidth(InArgs._TextMinDesiredWidth)
				.TextShapingMethod(InArgs._TextShapingMethod)
				.TextFlowDirection(InArgs._TextFlowDirection)
				.LineBreakPolicy(InArgs._LineBreakPolicy)
				.SimpleTextMode(InArgs._SimpleTextMode)
				.OnDoubleClicked(InArgs._OnDoubleClicked)
				.Clipping(EWidgetClipping::Inherit)
			];
	}

	void SetText(const TAttribute<FText>& InText) override
	{
		FAdaptTextBase::SetText(InText);
		BoundText = InText;
	}

	void SetText(const FText& InText) override
	{
		FAdaptTextBase::SetText(InText);
		BoundText = InText;
	}

	void SetTextMinFontSize(const TAttribute<int32>& InTextMinFontSize)
	{
		int32 Temp = InTextMinFontSize.Get();
		TextMinFontSize = Temp < 0 ? TextMinFontSize : Temp;
	}

	void SetFont(const TAttribute<FSlateFontInfo>& InFont) override
	{
		FAdaptTextBase::SetFont(InFont);
		SetCurFont(InFont);
	}

	TSharedRef<SWidget> GetWidget() override
	{
		return GetSizeBox();
	}

	int32 GetCurFontSize() const
	{
		return CurFontSize;
	}

protected:
	TSharedRef<SBox> GetSizeBox() override
	{
		return MySizeBox.ToSharedRef();
	}

	TSharedRef<STextBlock> GetTextBlock() override
	{
		return MyTextBlock.ToSharedRef();
	}

	void SetWrapTextAt(const TAttribute<float>& InWrapTextAt) override
	{
		FAdaptTextBase::SetWrapTextAt(InWrapTextAt);
		if (!WrapTextAt.IdenticalTo(InWrapTextAt))
		{
			WrapTextAt = InWrapTextAt;
		}
	}

	void SetAutoWrapText(const TAttribute<bool>& InAutoWrapText) override
	{
		FAdaptTextBase::SetAutoWrapText(InAutoWrapText);
		if (!AutoWrapText.IdenticalTo(InAutoWrapText))
		{
			AutoWrapText = InAutoWrapText;
		}
	}

	void OnInvalidate(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime, const EInvalidateAdaptTextReason& Reason) override
	{
		//UE_LOG(LogTemp, Warning, TEXT("FAdaptTextBestFit OnInvalidate"));
		if (!(EnumHasAnyFlags(EInvalidateAdaptTextReason::SetText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetAutoWrapText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetWrapTextAt, Reason)))
		{
			return;
		}
		LastReduceTextSize = FVector2D::ZeroVector;
		LastReduceBoxSize = FVector2D::ZeroVector;
		LineHeight = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(BoundText.Get(), CurFont, 1.0f).Y;
	}

	void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		CacheBoxSize = AllottedGeometry.GetLocalSize();
		CacheTextSize = MyTextBlock->GetDesiredSize();
		CacheGeometryScale = AllottedGeometry.Scale;
		BestFitText(CacheBoxSize, CacheTextSize);
	}

private:
	void SetCurFont(const TAttribute<FSlateFontInfo>& InFont)
	{
		CurFont = InFont.Get();
		MaxFontSize = CurFont.Size;
		CurFontSize = CurFont.Size;
	}

	bool GetIsWrapText() const
	{
		return AutoWrapText.Get() || WrapTextAt.Get() > 0;
	}

	bool ResizeWrapText(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		bool bHasResize = false;
		if (BoxSize.Y > TextSize.Y && CurFontSize < MaxFontSize && (BoxSize.X > LastReduceBoxSize.X + 2 || BoxSize.Y > LastReduceTextSize.Y))
		{
			CurFontSize++;
			bHasResize = true;
		}
		else if (BoxSize.Y < TextSize.Y && CurFontSize > TextMinFontSize)
		{
			//��С�ֺ�
			LastReduceTextSize = TextSize;
			LastReduceBoxSize = BoxSize;
			CurFontSize--;
			bHasResize = true;
		}

		if (bHasResize)
		{
			CurFont.Size = CurFontSize;
			MyTextBlock->SetFont(CurFont);
		}

		return bHasResize;
	}

	bool ResizeNoWrapText(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		bool bHasResize = false;
		if (BoxSize > TextSize && CurFontSize < MaxFontSize && BoxSize >= LastReduceTextSize)
		{
			CurFontSize++;
			bHasResize = true;
		}
		else if ((TextSize.X > BoxSize.X || TextSize.Y > BoxSize.Y) && CurFontSize > TextMinFontSize)
		{
			LastReduceTextSize = TextSize;
			CurFontSize--;
			bHasResize = true;
		}

		if (bHasResize)
		{
			CurFont.Size = CurFontSize;
			MyTextBlock->SetFont(CurFont);
		}

		return bHasResize;
	}

	bool BestFitWrapText(const FVector2D& BoxSize, FVector2D TextSize, bool bFontSizeCallback = true)
	{
		bool bHasResize = false;
		//换行后会不准确，所以这里只用于快速调整
		float DiffSize = BoxSize.Y - TextSize.Y;
		if (BoxSize != CacheWrapBoxSize && ((DiffSize > LineHeight && CurFontSize < MaxFontSize) || (DiffSize < -LineHeight && CurFontSize > TextMinFontSize)))
		{
			CacheWrapBoxSize = BoxSize;
			float Ratio = (BoxSize.Y / FMath::Max(TextSize.Y, 1.f));
			int32 TempSize = FMath::FloorToInt(CurFontSize * Ratio);
			TempSize = FMath::Clamp(TempSize, TextMinFontSize, MaxFontSize);
			//UE_LOG(LogTemp, Warning, TEXT("MaxFontSize:%d  TextMinFontSize:%d  CurFontSize:%d  TempSize:%d  Ratio:%f"), MaxFontSize, TextMinFontSize, CurFontSize, TempSize, Ratio);

			if (TempSize != CurFontSize)
			{
				CurFontSize = TempSize;
				CurFont.Size = CurFontSize;
				MyTextBlock->SetFont(CurFont);
				LastReduceBoxSize = FVector2D::ZeroVector;
				LastReduceTextSize = FVector2D::ZeroVector;
				TextSize = MyTextBlock->ComputeDesiredSize(CacheGeometryScale);
				bHasResize = true;
			}
		}

		//微调
		//int32 ResizeCount = 0;  //计算次数
		while (ResizeWrapText(BoxSize, TextSize))
		{
			TextSize = MyTextBlock->ComputeDesiredSize(CacheGeometryScale);
			//UE_LOG(LogTemp, Warning, TEXT("while BoxSize:%s  TextSize:%s   ========================"), *BoxSize.ToString(), *TextSize.ToString());
			bHasResize = true;
			//ResizeCount++;
		}
		/*if (ResizeCount > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResizeCount:%d  CurFontSize:%d"), ResizeCount, CurFontSize);
		}*/

		if (bHasResize && bFontSizeCallback)
		{
			OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
		}

		return bHasResize;
	}

	bool BestFitNoWrapText(const FVector2D& BoxSize, FVector2D TextSize, bool bFontSizeCallback = true)
	{
		bool bHasResize = false;
		float DiffSize = BoxSize.X - TextSize.X;
		if (BoxSize != CacheWrapBoxSize && ((DiffSize > 5 * LineHeight && CurFontSize < MaxFontSize) || (DiffSize < -5 * LineHeight && CurFontSize > TextMinFontSize)))
		{
			CacheWrapBoxSize = BoxSize;
			float TempY = BoxSize.Y - TextSize.Y;
			float Ratio = DiffSize < TempY ? BoxSize.X / FMath::Max(TextSize.X, 1.f) : BoxSize.Y / FMath::Max(TextSize.Y, 1.f);
			int32 TempSize = FMath::FloorToInt(CurFontSize * Ratio);
			TempSize = FMath::Clamp(TempSize, TextMinFontSize, MaxFontSize);
			if (TempSize != CurFontSize)
			{
				//UE_LOG(LogTemp, Warning, TEXT("MaxFontSize:%d  TextMinFontSize:%d  CurFontSize:%d  TempSize:%d  Ratio:%f"), MaxFontSize, TextMinFontSize, CurFontSize, TempSize, Ratio);
				CurFontSize = TempSize;
				CurFont.Size = CurFontSize;
				MyTextBlock->SetFont(CurFont);
				TextSize = MyTextBlock->ComputeDesiredSize(CacheGeometryScale);
				LastReduceTextSize = FVector2D::ZeroVector;
				bHasResize = true;
			}
		}

		//这里微调
		//int32 ResizeCount = 0; //计算次数
		while (ResizeNoWrapText(BoxSize, TextSize))
		{
			TextSize = MyTextBlock->ComputeDesiredSize(CacheGeometryScale);
			bHasResize = true;
			//ResizeCount++;
		}

		/*if (ResizeCount > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ResizeCount:%d  CurFontSize:%d"), ResizeCount, CurFontSize);
		}*/

		if (bHasResize && bFontSizeCallback)
		{
			OnTextFontSizeChangedCallback.ExecuteIfBound(CurFontSize);
		}

		return bHasResize;
	}

	bool BestFitText(const FVector2D& BoxSize, const FVector2D& TextSize, bool bFontSizeCallback = true)
	{
		if (GetIsWrapText())
		{
			return BestFitWrapText(BoxSize, TextSize, bFontSizeCallback);
		}
		else
		{
			return BestFitNoWrapText(BoxSize, TextSize, bFontSizeCallback);
		}
	}

private:
	TAttribute<FText> BoundText;
	TAttribute<float> WrapTextAt;
	TAttribute<bool> AutoWrapText;
	int32 TextMinFontSize;
	FOnTextFontSizeChanged OnTextFontSizeChangedCallback;

	TSharedPtr<STextBlock> MyTextBlock;
	TSharedPtr<SBox> MySizeBox;

	int32 MaxFontSize;
	int32 CurFontSize;
	FSlateFontInfo CurFont;

	FVector2D LastReduceTextSize;
	FVector2D LastReduceBoxSize;
	FVector2D CacheBoxSize;
	FVector2D CacheTextSize;
	float CacheGeometryScale;
	FVector2D CacheWrapBoxSize;
	float LineHeight;
};

class FAdaptTextMarquee : public FAdaptTextBase
{

public:
	FAdaptTextMarquee(const SAdaptText::FArguments& InArgs)
	{
		SetTextMarqueeDirection(InArgs._TextMarqueeDirection);
		SetTextMarqueeType(InArgs._TextMarqueeType);
		SetTextMarqueePolicy(InArgs._TextMarqueePolicy);
		SetTextMarqueeSpeed(InArgs._TextMarqueeSpeed);
		SetTextMarqueeStartDelay(InArgs._TextMarqueeStartDelay);
		SetTextMarqueeStartDelayOnlyFirst(InArgs._TextMarqueeStartDelayOnlyFirst);
		OnTextMarqueeEndCallback = InArgs._OnTextMarqueeEnd;

		MySizeBox = SNew(SBox)
			.HAlign(InArgs._HAlign)
			.VAlign(InArgs._VAlign)
			.WidthOverride(InArgs._WidthOverride)
			.HeightOverride(InArgs._HeightOverride)
			.MinDesiredWidth(InArgs._MinDesiredWidth)
			.MinDesiredHeight(InArgs._MinDesiredHeight)
			.MaxDesiredWidth(InArgs._MaxDesiredWidth)
			.MaxDesiredHeight(InArgs._MaxDesiredHeight)
			.Padding(FMargin())
			[
				
				SNew(SOverlay).Clipping(EWidgetClipping::Inherit)
				+ SOverlay::Slot().HAlign(EHorizontalAlignment::HAlign_Fill).VAlign(EVerticalAlignment::VAlign_Fill).Expose(MyOverlaySlot)
				[
					SAssignNew(MyTextBlock, STextBlock)
					.Text(InArgs._Text)
					.TextStyle(InArgs._TextStyle)
					.Font(InArgs._Font)
					.StrikeBrush(InArgs._StrikeBrush)
					.ColorAndOpacity(InArgs._ColorAndOpacity)
					.ShadowOffset(InArgs._ShadowOffset)
					.ShadowColorAndOpacity(InArgs._ShadowColorAndOpacity)
					.HighlightColor(InArgs._HighlightColor)
					.HighlightShape(InArgs._HighlightShape)
					.HighlightText(InArgs._HighlightText)
					.WrapTextAt(InArgs._WrapTextAt)
					.AutoWrapText(InArgs._AutoWrapText)
					.WrappingPolicy(InArgs._WrappingPolicy)
					.Margin(InArgs._Margin)
					.LineHeightPercentage(InArgs._LineHeightPercentage)
					.Justification(InArgs._Justification)
					.MinDesiredWidth(InArgs._TextMinDesiredWidth)
					.TextShapingMethod(InArgs._TextShapingMethod)
					.TextFlowDirection(InArgs._TextFlowDirection)
					.LineBreakPolicy(InArgs._LineBreakPolicy)
					.SimpleTextMode(InArgs._SimpleTextMode)
					.OnDoubleClicked(InArgs._OnDoubleClicked)
					.Clipping(EWidgetClipping::Inherit)
				]
			];
	}

	void SetTextMarqueeDirection(const TAttribute<ETextMarqueeDirection>& InTextMarqueeDirection)
	{
		TextMarqueeDirection = InTextMarqueeDirection.Get();
		InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextMarquee);
	}

	void SetTextMarqueeType(const TAttribute<ETextMarqueeType>& InTextMarqueeType)
	{
		TextMarqueeType = InTextMarqueeType.Get();
		InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextMarquee);
	}

	void SetTextMarqueePolicy(const TAttribute<ETextMarqueePolicy>& InTextMarqueePolicy)
	{
		TextMarqueePolicy = InTextMarqueePolicy.Get();
		InvalidateAdaptText(EInvalidateAdaptTextReason::SetTextMarquee);
	}

	void SetTextMarqueeSpeed(const TAttribute<int32>& InTextMarqueeSpeed)
	{
		int32 Temp = InTextMarqueeSpeed.Get();
		TextMarqueeSpeed = Temp < 0 ? TextMarqueeSpeed : Temp;
	}

	void SetTextMarqueeStartDelay(const TAttribute<float>& InTextMarqueeStartDelay)
	{
		float Temp = InTextMarqueeStartDelay.Get();
		TextMarqueeStartDelay = Temp < 0 ? TextMarqueeStartDelay : Temp;
	}

	void SetTextMarqueeStartDelayOnlyFirst(const TAttribute<bool>& InTextMarqueeStartDelayOnlyFirst)
	{
		TextMarqueeStartDelayOnlyFirst = InTextMarqueeStartDelayOnlyFirst.Get();
	}

	TSharedRef<SWidget> GetWidget() override
	{
		return GetSizeBox();
	}

protected:
	TSharedRef<SBox> GetSizeBox() override
	{
		return MySizeBox.ToSharedRef();
	}

	TSharedRef<STextBlock> GetTextBlock() override
	{
		return MyTextBlock.ToSharedRef();
	}

	void OnInvalidate(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime, const EInvalidateAdaptTextReason& Reason) override
	{
		//UE_LOG(LogTemp, Warning, TEXT("FAdaptTextMarquee OnInvalidate"));
		if (!(EnumHasAnyFlags(EInvalidateAdaptTextReason::SetText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetAutoWrapText, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetWrapTextAt, Reason)
			|| EnumHasAnyFlags(EInvalidateAdaptTextReason::SetTextMarquee, Reason)))
		{
			return;
		}
		bIsFirstTimeToMarquee = true;
		bIsStartToMarquee = true;
		CacheTime = InCurrentTime;
		CacheBoxSize = AllottedGeometry.GetLocalSize();
		CacheTextSize = MyTextBlock->GetDesiredSize();
		switch (TextMarqueeType)
		{
		case ETextMarqueeType::Default:
			InitMarqueeDefault(CacheBoxSize, CacheTextSize);
			break;
		case ETextMarqueeType::DelayStart:
			InitMarqueeDelayStart(CacheBoxSize, CacheTextSize);
			break;
		}
	}

	void OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		CacheBoxSize = AllottedGeometry.GetLocalSize();
		CacheTextSize = MyTextBlock->GetDesiredSize();
		if (!ShouldMarquee(CacheBoxSize, CacheTextSize))
		{
			return;
		}
		switch (TextMarqueeType)
		{
		case ETextMarqueeType::Default:
			MarqueeText(CacheBoxSize, CacheTextSize, InDeltaTime);
			break;
		case ETextMarqueeType::DelayStart:
		{
			//UE_LOG(LogTemp, Warning, TEXT("InCurrentTime:%lf   InDeltaTime:%lf   MyDeltaTime:%lf   "), InCurrentTime, InDeltaTime, InCurrentTime - CacheTime);
			if (bIsStartToMarquee && InCurrentTime - CacheTime > TextMarqueeStartDelay)
			{
				//UE_LOG(LogTemp, Warning, TEXT("DelayStart"));
				bIsStartToMarquee = false;
				CacheTime = InCurrentTime;
			}

			if (!bIsStartToMarquee)
			{
				MarqueeText(CacheBoxSize, CacheTextSize, InDeltaTime);
			}
			break;
		}
		}
	}

private:
	bool ShouldMarquee(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		if ((TextMarqueePolicy == ETextMarqueePolicy::OverflowX && TextSize.X <= BoxSize.X)
			|| (TextMarqueePolicy == ETextMarqueePolicy::OverflowY && TextSize.Y <= BoxSize.Y)
			|| (TextMarqueePolicy == ETextMarqueePolicy::OverflowBoth && (TextSize.X <= BoxSize.X || TextSize.Y <= BoxSize.Y)))
		{
			MarqueeOffset = 0;
			SBoxPadding = FMargin();
			SetPadding(SBoxPadding);
			return false;
		}

		return true;
	}

	void SetPadding(const FMargin& InPadding)
	{
		MyOverlaySlot->Padding(InPadding);
	}

	void InitMarqueeDefault(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		switch (TextMarqueeDirection)
		{
		case ETextMarqueeDirection::LeftToRight: MarqueeOffset = -TextSize.X; break;
		case ETextMarqueeDirection::RightToLeft: MarqueeOffset = BoxSize.X; break;
		case ETextMarqueeDirection::TopToBottom: MarqueeOffset = -TextSize.Y; break;
		case ETextMarqueeDirection::BottomToTop: MarqueeOffset = BoxSize.Y; break;
		}
		SBoxPadding = FMargin();
	}

	void InitMarqueeDelayStart(const FVector2D& BoxSize, const FVector2D& TextSize)
	{
		MarqueeOffset = 0;
		SBoxPadding = FMargin();
		SetPadding(SBoxPadding);
	}

	void MarqueeText(const FVector2D& BoxSize, const FVector2D& TextSize, const float& InDeltaTime)
	{
		switch (TextMarqueeDirection)
		{
		case ETextMarqueeDirection::LeftToRight: 
		{
			if (MarqueeOffset > BoxSize.X)
			{
				MarqueeOffset = -TextSize.X;
				OnTextMarqueeEndCallback.ExecuteIfBound();
			}
			MarqueeOffset += TextMarqueeSpeed * InDeltaTime;
			SBoxPadding.Left = MarqueeOffset;
			SBoxPadding.Right = -MarqueeOffset;
			SBoxPadding.Top = 0;
			SBoxPadding.Bottom = 0;
			break;
		}
		case ETextMarqueeDirection::RightToLeft: 
		{
			if (MarqueeOffset < -TextSize.X)
			{
				MarqueeOffset = BoxSize.X;
				OnTextMarqueeEndCallback.ExecuteIfBound();
			}
			MarqueeOffset -= TextMarqueeSpeed * InDeltaTime;
			SBoxPadding.Left = MarqueeOffset;
			SBoxPadding.Right = -MarqueeOffset;
			SBoxPadding.Top = 0;
			SBoxPadding.Bottom = 0;
			break;
		}
		case ETextMarqueeDirection::TopToBottom: 
		{
			if (MarqueeOffset > BoxSize.Y)
			{
				MarqueeOffset = -TextSize.Y;
				OnTextMarqueeEndCallback.ExecuteIfBound();
			}
			MarqueeOffset += TextMarqueeSpeed * InDeltaTime;
			SBoxPadding.Left = 0;
			SBoxPadding.Right = 0;
			SBoxPadding.Top = MarqueeOffset;
			SBoxPadding.Bottom = -MarqueeOffset;
			break;
		}
		case ETextMarqueeDirection::BottomToTop: 
		{
			if (MarqueeOffset < -TextSize.Y)
			{
				MarqueeOffset = BoxSize.Y;
				OnTextMarqueeEndCallback.ExecuteIfBound();
			}
			MarqueeOffset -= TextMarqueeSpeed * InDeltaTime;
			SBoxPadding.Left = 0;
			SBoxPadding.Right = 0;
			SBoxPadding.Top = MarqueeOffset;
			SBoxPadding.Bottom = -MarqueeOffset;
			break;
		}
		}
		SetPadding(SBoxPadding);
	}

private:
	ETextMarqueeDirection TextMarqueeDirection;
	ETextMarqueeType TextMarqueeType;
	ETextMarqueePolicy TextMarqueePolicy;
	int32 TextMarqueeSpeed;
	float TextMarqueeStartDelay;
	bool TextMarqueeStartDelayOnlyFirst;
	FOnTextMarqueeEnd OnTextMarqueeEndCallback;

	TSharedPtr<STextBlock> MyTextBlock;
	TSharedPtr<SBox> MySizeBox;
	SOverlay::FOverlaySlot* MyOverlaySlot;

	bool bIsFirstTimeToMarquee;
	bool bIsStartToMarquee;
	FMargin SBoxPadding;
	float MarqueeOffset;
	double CacheTime;

	FVector2D CacheBoxSize;
	FVector2D CacheTextSize;
};


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
SAdaptText::SAdaptText()
{
	SetCanTick(true);
	bCanSupportFocus = false;
}

SAdaptText::~SAdaptText()
{

}

void SAdaptText::Construct(const FArguments& InArgs)
{
	SetOnMouseDoubleClick(InArgs._OnDoubleClicked);
	CacheArgs = InArgs;
	AdaptType = InArgs._TextAdaptType.Get(ETextAdaptType::Ellipsis);
	BuildChildWidget();
}

const FText& SAdaptText::GetText() const
{
	return CacheArgs._Text.Get();
}

const int32 SAdaptText::GetCurrentFontSize() const
{
	if (AdaptType != ETextAdaptType::BestFit)
	{
		return CacheArgs._Font.Get().Size;
	}
	return StaticCastSharedPtr<FAdaptTextBestFit>(MyAdaptText)->GetCurFontSize();
}

void SAdaptText::SetText(const TAttribute< FText >& InText)
{
	MyAdaptText->SetText(InText);
	CacheArgs._Text = InText;
}

void SAdaptText::SetText(const FText& InText)
{
	MyAdaptText->SetText(InText);
	CacheArgs._Text = InText;
}

void SAdaptText::SetHighlightText(TAttribute<FText> InHighlightText)
{
	MyAdaptText->SetHighlightText(InHighlightText);
	CacheArgs._HighlightText = InHighlightText;
}

void SAdaptText::SetFont(const TAttribute<FSlateFontInfo>& InFont)
{
	MyAdaptText->SetFont(InFont);
	CacheArgs._Font = InFont;
}

void SAdaptText::SetStrikeBrush(const TAttribute<const FSlateBrush*>& InStrikeBrush)
{
	MyAdaptText->SetStrikeBrush(InStrikeBrush);
	CacheArgs._StrikeBrush = InStrikeBrush;
}

void SAdaptText::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	MyAdaptText->SetColorAndOpacity(InColorAndOpacity);
	CacheArgs._ColorAndOpacity = InColorAndOpacity;
}

void SAdaptText::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	MyAdaptText->SetTextStyle(InTextStyle);
	CacheArgs._TextStyle = InTextStyle;
}

void SAdaptText::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	MyAdaptText->SetTextShapingMethod(InTextShapingMethod);
	CacheArgs._TextShapingMethod = InTextShapingMethod;
}

void SAdaptText::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	MyAdaptText->SetTextFlowDirection(InTextFlowDirection);
	CacheArgs._TextFlowDirection = InTextFlowDirection;
}

void SAdaptText::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	MyAdaptText->SetWrapTextAt(InWrapTextAt);
	CacheArgs._WrapTextAt = InWrapTextAt;
}

void SAdaptText::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	MyAdaptText->SetAutoWrapText(InAutoWrapText);
	CacheArgs._AutoWrapText = InAutoWrapText;
}

void SAdaptText::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	MyAdaptText->SetWrappingPolicy(InWrappingPolicy);
	CacheArgs._WrappingPolicy = InWrappingPolicy;
}

void SAdaptText::SetShadowOffset(const TAttribute<FVector2D>& InShadowOffset)
{
	MyAdaptText->SetShadowOffset(InShadowOffset);
	CacheArgs._ShadowOffset = InShadowOffset;
}

void SAdaptText::SetShadowColorAndOpacity(const TAttribute<FLinearColor>& InShadowColorAndOpacity)
{
	MyAdaptText->SetShadowColorAndOpacity(InShadowColorAndOpacity);
	CacheArgs._ShadowColorAndOpacity = InShadowColorAndOpacity;
}

void SAdaptText::SetTextMinDesiredWidth(const TAttribute<float>& InTextMinDesiredWidth)
{
	MyAdaptText->SetTextMinDesiredWidth(InTextMinDesiredWidth);
	CacheArgs._TextMinDesiredWidth = InTextMinDesiredWidth;
}

void SAdaptText::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	MyAdaptText->SetLineHeightPercentage(InLineHeightPercentage);
	CacheArgs._LineHeightPercentage = InLineHeightPercentage;
}

void SAdaptText::SetMargin(const TAttribute<FMargin>& InMargin)
{
	MyAdaptText->SetMargin(InMargin);
	CacheArgs._Margin = InMargin;
}

void SAdaptText::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	MyAdaptText->SetJustification(InJustification);
	CacheArgs._Justification = InJustification;
}

//SizeBox
void SAdaptText::SetHAlign(EHorizontalAlignment InHAlign)
{
	MyAdaptText->SetHAlign(InHAlign);
	CacheArgs._HAlign = InHAlign;
}

void SAdaptText::SetVAlign(EVerticalAlignment InVAlign)
{
	MyAdaptText->SetVAlign(InVAlign);
	CacheArgs._VAlign = InVAlign;
}

void SAdaptText::SetWidthOverride(TAttribute<FOptionalSize> InWidthOverride)
{
	MyAdaptText->SetWidthOverride(InWidthOverride);
	CacheArgs._WidthOverride = InWidthOverride;
}

void SAdaptText::SetHeightOverride(TAttribute<FOptionalSize> InHeightOverride)
{
	MyAdaptText->SetHeightOverride(InHeightOverride);
	CacheArgs._HeightOverride = InHeightOverride;
}

void SAdaptText::SetMinDesiredWidth(TAttribute < FOptionalSize> InMinDesiredWidth)
{
	MyAdaptText->SetMinDesiredWidth(InMinDesiredWidth);
	CacheArgs._MinDesiredWidth = InMinDesiredWidth;
}

void SAdaptText::SetMinDesiredHeight(TAttribute<FOptionalSize> InMinDesiredHeight)
{
	MyAdaptText->SetMinDesiredHeight(InMinDesiredHeight);
	CacheArgs._MinDesiredHeight = InMinDesiredHeight;
}

void SAdaptText::SetMaxDesiredWidth(TAttribute<FOptionalSize> InMaxDesiredWidth)
{
	MyAdaptText->SetMaxDesiredWidth(InMaxDesiredWidth);
	CacheArgs._MaxDesiredWidth = InMaxDesiredWidth;
}

void SAdaptText::SetMaxDesiredHeight(TAttribute<FOptionalSize> InMaxDesiredHeight)
{
	MyAdaptText->SetMaxDesiredHeight(InMaxDesiredHeight);
	CacheArgs._MaxDesiredHeight = InMaxDesiredHeight;
}

//SAdapter Text
void SAdaptText::SetTextAdaptType(const TAttribute<ETextAdaptType>& InTextAdaptType)
{
	if (!CacheArgs._TextAdaptType.IsSet() || !CacheArgs._TextAdaptType.IdenticalTo(InTextAdaptType))
	{
		CacheArgs._TextAdaptType = InTextAdaptType;
		AdaptType = InTextAdaptType.Get();
		BuildChildWidget();
	}
}

void SAdaptText::SetTextMaxLines(const TAttribute<FOptionalSize>& InTextMaxLines)
{
	if (AdaptType != ETextAdaptType::Ellipsis)
	{
		return;
	}
	if (!CacheArgs._TextMaxLines.IsSet() || !CacheArgs._TextMaxLines.IdenticalTo(InTextMaxLines))
	{
		CacheArgs._TextMaxLines = InTextMaxLines;
		StaticCastSharedPtr<FAdaptTextEllipse>(MyAdaptText)->SetTextMaxLines(InTextMaxLines);
	}
}

void SAdaptText::SetTextMinFontSize(const TAttribute<int32>& InTextMinFontSize)
{
	if (AdaptType != ETextAdaptType::BestFit)
	{
		return;
	}

	if (!CacheArgs._TextMinFontSize.IsSet() || !CacheArgs._TextMinFontSize.IdenticalTo(InTextMinFontSize))
	{
		CacheArgs._TextMinFontSize = InTextMinFontSize;
		StaticCastSharedPtr<FAdaptTextBestFit>(MyAdaptText)->SetTextMinFontSize(InTextMinFontSize);
	}
}

void SAdaptText::SetTextMarqueeDirection(const TAttribute<ETextMarqueeDirection>& InTextMarqueeDireciton)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueeDirection.IsSet() || !CacheArgs._TextMarqueeDirection.IdenticalTo(InTextMarqueeDireciton))
	{
		CacheArgs._TextMarqueeDirection = InTextMarqueeDireciton;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueeDirection(InTextMarqueeDireciton);
	}
}

void SAdaptText::SetTextMarqueePolicy(const TAttribute<ETextMarqueePolicy>& InTextMarqueePolicy)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueePolicy.IsSet() || !CacheArgs._TextMarqueePolicy.IdenticalTo(InTextMarqueePolicy))
	{
		CacheArgs._TextMarqueePolicy = InTextMarqueePolicy;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueePolicy(InTextMarqueePolicy);
	}
}

void SAdaptText::SetTextMarqueeSpeed(const TAttribute<int32>& InTextMarqueeSpeed)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueeSpeed.IsSet() || !CacheArgs._TextMarqueeSpeed.IdenticalTo(InTextMarqueeSpeed))
	{
		CacheArgs._TextMarqueeSpeed = InTextMarqueeSpeed;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueeSpeed(InTextMarqueeSpeed);
	}
}

void SAdaptText::SetTextMarqueeType(const TAttribute<ETextMarqueeType>& InTextMarqueeType)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueeType.IsSet() || !CacheArgs._TextMarqueeType.IdenticalTo(InTextMarqueeType))
	{
		CacheArgs._TextMarqueeType = InTextMarqueeType;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueeType(InTextMarqueeType);
	}
}

void SAdaptText::SetTextMarqueeStartDelay(const TAttribute<float>& InTextMarqueeStartDelay)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueeStartDelay.IsSet() || !CacheArgs._TextMarqueeStartDelay.IdenticalTo(InTextMarqueeStartDelay))
	{
		CacheArgs._TextMarqueeStartDelay = InTextMarqueeStartDelay;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueeStartDelay(InTextMarqueeStartDelay);
	}
}

void SAdaptText::SetTextMarqueeStartDelayOnlyFirst(const TAttribute<bool>& InTextMarqueeStartDelayOnlyFirst)
{
	if (AdaptType != ETextAdaptType::Marquee)
	{
		return;
	}

	if (!CacheArgs._TextMarqueeStartDelayOnlyFirst.IsSet() || !CacheArgs._TextMarqueeStartDelayOnlyFirst.IdenticalTo(InTextMarqueeStartDelayOnlyFirst))
	{
		CacheArgs._TextMarqueeStartDelayOnlyFirst = InTextMarqueeStartDelayOnlyFirst;
		StaticCastSharedPtr<FAdaptTextMarquee>(MyAdaptText)->SetTextMarqueeStartDelayOnlyFirst(InTextMarqueeStartDelayOnlyFirst);
	}
}

void SAdaptText::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	MyAdaptText->Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SAdaptText::BuildChildWidget()
{
	this->ChildSlot.DetachWidget();
	switch (AdaptType)
	{
	case ETextAdaptType::Ellipsis:
		MyAdaptText = MakeShared<FAdaptTextEllipse>(CacheArgs);
		this->ChildSlot.AttachWidget(MyAdaptText->GetWidget());
		break;
	case ETextAdaptType::BestFit:
		MyAdaptText = MakeShared<FAdaptTextBestFit>(CacheArgs);
		this->ChildSlot.AttachWidget(MyAdaptText->GetWidget());
		break;
	case ETextAdaptType::Marquee:
		MyAdaptText = MakeShared<FAdaptTextMarquee>(CacheArgs);
		this->ChildSlot.AttachWidget(MyAdaptText->GetWidget());
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unrecognized AdaptType!"));
		break;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
