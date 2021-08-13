;----------------------------------------------------------------------
; Name=Slideshow Mastery
; Author=Cadrell / Patrick M Livingston
; Version=0.11.1-20210812
; License=Creative Commons Attribution-Non-Commercial-Share Alike 3.0
;----------------------------------------------------------------------



[Rainmeter]
AccurateText=1
MiddleMouseUpAction=!Refresh
MouseOverAction=[!ToggleMeterGroup gButtons][!UpdateMeterGroup gButtons][!Redraw]
MouseLeaveAction=[!ToggleMeterGroup gButtons][!UpdateMeterGroup gButtons][!Redraw]

ContextTitle=Pause / Unpause
ContextAction=[!SetOption mPaused Formula (Abs(mPaused-1))][!UpdateMeasure mPaused]
ContextTitle2=Open Image
ContextAction2=["#vImageApp#" "[mImage]"]
ContextTitle3=Open Folder
ContextAction3=["#vFolderApp#" "[mFilePath]"]
ContextTitle4=-
ContextTitle5=Scroll Back 1
ContextAction5=[!SetOption mCycler Formula (((mCycler+mTally-2)%mTally)+1)][!UpdateMeasure mCycler]
ContextTitle6=Update Image
ContextAction6=[!UpdateMeasure mUpdating]
ContextTitle7=Scroll Forward 1
ContextAction7=[!UpdateMeasure mCycler]
ContextTitle8=-
ContextTitle9=Random / Sequential Order
ContextAction9=[!SetVariable vDisplayOrder (Abs(#vDisplayOrder#-1))][!WriteKeyValue Variables vDisplayOrder (Abs(#vDisplayOrder#-1))][!Redraw]
ContextTitle10=-
ContextTitle11=Refresh Skin
ContextAction11=!Refresh



[Variables]
;----------------------------------------------------
; DANGER!!! - Notes for Controls & Code Segments
;----------------------------------------------------

NtLeftClick=""
NtHalfEdge=(#vOutlineSize#/2)
NtFrameSize=(#vShadowSize#+#vBorderSize#)
NtLinedFrameSize=(#NtHalfEdge#+#NtFrameSize#)
NtHalfButton=(#vButtonSize#/2)
NtButtonScale=(#vButtonSize#/30)



;----------------------------------------------------
; Visual Styles
;----------------------------------------------------

[VslImageStyle]
X=#NtFrameSize#+#vOutlineSize#
Y=#NtFrameSize#+#vOutlineSize#
W=#vImageWidth#
H=#vImageHeight#

[VslFrameStyle]
W=((#NtLinedFrameSize#*2)+#vImageWidth#+#vOutlineSize#)
H=((#NtLinedFrameSize#*2)+#vImageHeight#+#vOutlineSize#)
Group=gBorders

[VslButtonStyle]
X=(#NtFrameSize#+#vOutlineSize#+#NtHalfButton#)
Y=(#NtFrameSize#+#vOutlineSize#+#NtHalfButton#)
W=#vButtonSize#
H=#vButtonSize#
StringAlign=CenterCenter
ClipString=1
AntiAlias=1
Group=gButtons
Hidden=1



;----------------------------------------------------
; Core Slideshow Measures
;----------------------------------------------------

[mPath]
Measure=Plugin
Plugin=FileView
Path=#vBaseFolder#
Recursive=(#vIncludeSubfolders#*2)
Extensions=#vFileTypes#
ShowDotDot=0
ShowFolder=0
ShowHidden=0
FinishAction=[!EnableMeasure mTally][!UpdateMeasure mTally]

[mTally]
Measure=Plugin
Plugin=FileView
Path=[mPath]
Type=FileCount
OnUpdateAction=[!SetOption MtrImage MeasureName mImage][!EnableMeasureGroup gSlideControls][!SetOption mRandomizer HighBound [mTally]][!UpdateMeasure mRandomizer][!UpdateMeasure mPaused]
UpdateDivider=-1
Disabled=1

[mRandomizer]
Measure=Calc
Formula=Random
LowBound=1
HighBound=1
UpdateRandom=1
UniqueRandom=1
IfConditionMode=1
IfCondition=#vDisplayOrder#=1
IfTrueAction=!SetOption mCycler Formula [mRandomizer]
OnUpdateAction=!UpdateMeasure mCycler
Group=gSlideControls
DynamicVariables=1
Disabled=1

[mCycler]
Measure=Calc
Formula=1
OnUpdateAction=[!SetOptionGroup gChildren Index [mCycler]][!UpdateMeasureGroup gChildren][!UpdateMeter *][!Redraw][!SetOption mCycler Formula ((mCycler%mTally)+1)]
Group=gSlideControls
DynamicVariables=1
UpdateDivider=-1
Disabled=1

[mImage]
Measure=Plugin
Plugin=FileView
Type=FilePath
Path=[mPath]
IgnoreCount=1
Group=gChildren

;----------------------------------------------------
; Button Scaling Measure
;----------------------------------------------------

[mButtonScaling]
Measure=Calc
Formula=Min(#vImageWidth#,#vImageHeight#)
IfCondition=#vButtonSize#>mButtonScaling
IfTrueAction=[!SetVariable NtButtonScale ([mButtonScaling]/#vButtonSize#)][!SetVariable vButtonSize [mButtonScaling]][!SetVariable NtHalfButton ([mButtonScaling]/2)]
IfCondition2=#vButtonSize#<(mButtonScaling/10)
IfTrueAction2=[!SetVariable NtButtonScale (([mButtonScaling]/10)/#vButtonSize#)][!SetVariable vButtonSize ([mButtonScaling]/10)][!SetVariable NtHalfButton (([mButtonScaling]/10)/2)]
OnUpdateAction=[!SetOptionGroup gButtons MeterStyle VslButtonStyle][!UpdateMeterGroup gButtons]
DynamicVariables=1
UpdateDivider=-1

;----------------------------------------------------
; Tooltip Measures
;----------------------------------------------------

[mFileName]
Measure=Plugin
Plugin=FileView
Type=FileName
Path=[mPath]
IgnoreCount=1
Group=gChildren

[mFilePath]
Measure=Plugin
Plugin=FileView
Type=PathToFile
Path=[mPath]
IgnoreCount=1
Group=gChildren

[mSetToolTips]
Measure=Calc
IfCondition=#vShowToolTips#=0
IfTrueAction=[!SetOption MtrToolTip W 0][!SetOption MtrToolTip H 0]
IfCondition2=#vShowToolTips#=1
IfTrueAction2=!SetOption MtrToolTip MeterStyle VslImageStyle
IfCondition3=#vShowToolTips#=2
IfTrueAction3=!SetOption MtrToolTip MeterStyle VslButtonStyle
OnUpdateAction=[!UpdateMeter MtrToolTip]
DynamicVariables=1
UpdateDivider=-1

;----------------------------------------------------
; Pause Control Measures
;----------------------------------------------------

[mPaused]
Measure=Calc
Formula=#vStartPaused#
IfBelowValue=1
IfBelowAction=[!SetOption MtrToolTip ToolTipText "Index [*mCycler*] of [*mTally*]#CRLF##CRLF#[*mFilePath*]"][!UnpauseMeasure mRandomizer]
IfEqualValue=1
IfEqualAction=[!SetOption MtrToolTip ToolTipText "PAUSED - Index [*mCycler*] of [*mTally*]#CRLF##CRLF#[*mFilePath*]"][!PauseMeasure mRandomizer]
OnUpdateAction=!UpdateMeter MtrToolTip
Group=gSlideControls
UpdateDivider=-1
Disabled=1

[mUpdating]
Measure=Calc
IfConditionMode=1
IfCondition=mPaused=0
IfTrueAction=!Update
IfFalseAction=[!TogglePauseMeasure mRandomizer][!UpdateMeasure mRandomizer][!TogglePauseMeasure mRandomizer]
Group=gSlideControls
UpdateDivider=-1
Disabled=1

;----------------------------------------------------
; Left-Click Measure
;----------------------------------------------------

[mSetLeftClick]
Measure=Calc
IfCondition=#vLeftClick#=1
IfTrueAction=!SetVariable NtLeftClick "!UpdateMeasure mUpdating"
IfCondition2=#vLeftClick#=2
IfTrueAction2=!SetVariable NtLeftClick "[!SetOption mPaused Formula (Abs(mPaused-1))][!UpdateMeasure mPaused]"
IfCondition3=#vLeftClick#=3
IfTrueAction3=!SetVariable NtLeftClick """"#vImageApp#" "[*mImage*]""""
IfCondition4=#vLeftClick#=4
IfTrueAction4=!SetVariable NtLeftClick """"#vFolderApp#" "[*mFilePath*]""""
DynamicVariables=1
UpdateDivider=-1

;----------------------------------------------------
; Reflection Measure
;----------------------------------------------------

[mSetReflection]
Measure=String
String=#vReflectColor#
RegExpSubstitute=1
Substitute="(\d+),(\d+),(\d+)*":"\1,\2,\3,#vReflectEndOpacity#"
UpdateDivider=-1

;----------------------------------------------------
; Frame Shape Measure
;----------------------------------------------------

[mSetFrame]
Measure=String
String=#vShadowStartColor#
RegExpSubstitute=1
Substitute="(\d+),(\d+),(\d+)*":"\1,\2,\3,#vShadowEndOpacity#"
IfConditionMode=1
IfCondition=#vCornerType#=1
IfTrueAction=[!ShowMeter MtrFrameSharp][!HideMeter MtrFrameRound]
IfFalseAction=[!HideMeter MtrFrameSharp][!ShowMeter MtrFrameRound]
OnUpdateAction=[!UpdateMeterGroup gBorders][!Redraw]
DynamicVariables=1
UpdateDivider=-1



;----------------------------------------------------
; Core Slideshow Meter
;----------------------------------------------------

[MtrImage]
Meter=Image
MeterStyle=VslImageStyle
SolidColor=#vImageBackColor#
Greyscale=#vGreyscale#
ImageTint=#vImageTint#
ImageAlpha=#vImageOpacity#
PreserveAspectRatio=#vAspectRatio#
UseExifOrientation=#vExifRotation#
DynamicVariables=1
MouseScrollUpAction=[!SetOption mCycler Formula (((mCycler+mTally-2)%mTally)+1)][!UpdateMeasure mCycler]
MouseScrollDownAction=[!UpdateMeasure mCycler]
LeftMouseUpAction=#NtLeftClick#
MouseLeaveAction=!Redraw

;----------------------------------------------------
; ToolTip Meter
;----------------------------------------------------

[MtrToolTip]
Meter=String
DynamicVariables=1
ToolTipTitle=[mFileName]
ToolTipText=Index [mCycler] of [mTally]#CRLF##CRLF#[mFilePath]
ToolTipType=1
MouseOverAction=[!SetOption MtrToolTip ToolTipHidden 0][!UpdateMeter MtrToolTip][!Redraw]
MouseLeaveAction=[!SetOption MtrToolTip ToolTipHidden 1][!UpdateMeter MtrToolTip][!Redraw]

;----------------------------------------------------
; Settings Button Meter
;----------------------------------------------------

[MtrSettingsButton]
Meter=String
MeterStyle=VslButtonStyle
SolidColor=#vButtonBackColor#
FontColor=#vButtonColorOff#
FontSize=(15*#NtButtonScale#)
Text=[\x2699]
MouseOverAction=[!SetOption MtrSettingsButton FontColor #vButtonColorOn#][!UpdateMeter MtrSettingsButton][!Redraw]
MouseLeaveAction=[!SetOption MtrSettingsButton FontColor #vButtonColorOff#][!UpdateMeter MtrSettingsButton][!Redraw]
LeftMouseDownAction=[!SetOption MtrSettingsButton FontColor #vButtonColorDown#][!UpdateMeter MtrSettingsButton][!Redraw]
LeftMouseUpAction=[!SetOption MtrSettingsButton FontColor #vButtonColorOn#][!UpdateMeter MtrSettingsButton][!Redraw][!SkinCustomMenu]

;----------------------------------------------------
; Glass Reflection Meter
;----------------------------------------------------

[MtrReflection]
Meter=Shape
MeterStyle=VslImageStyle
Shape=Rectangle 0,0,#vImageWidth#,#vImageHeight# | StrokeWidth 0 | Fill LinearGradient GrdReflect
Shape2=Ellipse #vImageWidth#,#vImageHeight#,#vImageWidth#,#vImageHeight#
Shape3=Combine Shape | Exclude Shape2
GrdReflect=(-1*Deg(Atan(#vImageWidth#/#vImageHeight#))) | #vReflectColor# ; 0 | [mSetReflection] ; 1
Hidden=(Abs(#vShowGlass#-1))

;----------------------------------------------------
; Border & Shadow Meters
;----------------------------------------------------

[MtrFrameSharp]
Meter=Shape
MeterStyle=VslFrameStyle
;-- Shadow Sides ------------------------------------
Shape=Path PathTop | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradTop | Scale 1.0005,1
Shape2=Path PathTop | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradTop | Scale 1.0005,1 | Rotate 180 | Offset 0,(#NtFrameSize#+#vImageHeight#+#vOutlineSize#+#vBorderSize#)
Shape3=Path PathLeft | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradLeft | Scale 1,1.0005
Shape4=Path PathLeft | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradLeft | Scale 1,1.0005 | Rotate 180 | Offset (#NtFrameSize#+#vImageWidth#+#vOutlineSize#+#vBorderSize#),0
PathTop=#NtHalfEdge#,#NtHalfEdge# | LineTo (#NtHalfEdge#+#vShadowSize#),(#NtHalfEdge#+#vShadowSize#) | LineTo (#NtLinedFrameSize#+#vImageWidth#+#vOutlineSize#+#vBorderSize#),(#NtHalfEdge#+#vShadowSize#) | LineTo (#NtLinedFrameSize#+#vImageWidth#+#vOutlineSize#+#NtFrameSize#),#NtHalfEdge# | ClosePath 1
PathLeft=#NtHalfEdge#,#NtHalfEdge# | LineTo (#NtHalfEdge#+#vShadowSize#),(#NtHalfEdge#+#vShadowSize#) | LineTo (#NtHalfEdge#+#vShadowSize#),(#NtLinedFrameSize#+#vImageHeight#+#vOutlineSize#+#vBorderSize#) | LineTo #NtHalfEdge#,(#NtLinedFrameSize#+#vImageHeight#+#vOutlineSize#+#NtFrameSize#) | ClosePath 1
;-- Coloring ----------------------------------------
GradTop=90 | #vShadowStartColor# ; 0 | [mSetFrame] ; 1
GradLeft=0 | #vShadowStartColor# ; 0 | [mSetFrame] ; 1
;-- Border ------------------------------------------
Shape5=Rectangle (#NtHalfEdge#+#vShadowSize#),(#NtHalfEdge#+#vShadowSize#),((#vBorderSize#*2)+#vImageWidth#+#vOutlineSize#),((#vBorderSize#*2)+#vImageHeight#+#vOutlineSize#) | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill Color #vBorderColor#
Shape6=Rectangle #NtLinedFrameSize#,#NtLinedFrameSize#,(#vImageWidth#+#vOutlineSize#),(#vImageHeight#+#vOutlineSize#)
Shape7=Combine Shape5 | Exclude Shape6
Hidden=1

[MtrFrameRound]
Meter=Shape
MeterStyle=VslFrameStyle
;-- Corner Setup ------------------------------------
Shape=Ellipse #NtLinedFrameSize#,#NtLinedFrameSize#,#NtFrameSize# | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill RadialGradient GradCorner
Shape2=Rectangle #NtLinedFrameSize#,#NtHalfEdge#,#NtFrameSize#,(#NtFrameSize#*2)
Shape3=Rectangle #NtHalfEdge#,#NtLinedFrameSize#,#NtFrameSize#,#NtFrameSize#
Shape4=Rectangle (#vShadowSize#+#NtHalfEdge#),(#vShadowSize#+#NtHalfEdge#),((#vBorderSize#*2)+#vImageWidth#+#vOutlineSize#),((#vBorderSize#*2)+#vImageHeight#+#vOutlineSize#),#vBorderSize# | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill Color #vBorderColor#
;-- Corners -----------------------------------------
Shape5=Combine Shape | Exclude Shape2 | Exclude Shape3 | Exclude Shape4
Shape6=Combine Shape | Exclude Shape2 | Exclude Shape3 | Exclude Shape4 | Rotate 90 | Offset (#NtFrameSize#+#vImageWidth#+#vOutlineSize#),0
Shape7=Combine Shape | Exclude Shape2 | Exclude Shape3 | Exclude Shape4 | Rotate -90 | Offset 0,(#NtFrameSize#+#vImageHeight#+#vOutlineSize#)
Shape8=Combine Shape | Exclude Shape2 | Exclude Shape3 | Exclude Shape4 | Rotate 180 | Offset (#NtFrameSize#+#vImageWidth#+#vOutlineSize#),(#NtFrameSize#+#vImageHeight#+#vOutlineSize#)
;-- Sides -------------------------------------------
Shape9= Rectangle #NtLinedFrameSize#,#NtHalfEdge#,(#vImageWidth#+#vOutlineSize#),#vShadowSize# | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradTop
Shape10=Rectangle #NtLinedFrameSize#,#NtHalfEdge#,(#vImageWidth#+#vOutlineSize#),#vShadowSize# | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradTop | Rotate 180 | Offset 0,(#NtFrameSize#+#vImageHeight#+#vOutlineSize#+#vBorderSize#)
Shape11=Rectangle #NtHalfEdge#,#NtLinedFrameSize#,#vShadowSize#,(#vImageHeight#+#vOutlineSize#) | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradLeft
Shape12=Rectangle #NtHalfEdge#,#NtLinedFrameSize#,#vShadowSize#,(#vImageHeight#+#vOutlineSize#) | StrokeWidth #vOutlineSize# | Stroke Color #vOutlineColor# | Fill LinearGradient GradLeft | Rotate 180 | Offset (#NtFrameSize#+#vImageWidth#+#vOutlineSize#+#vBorderSize#),0
;-- Coloring ----------------------------------------
GradCorner=(#NtFrameSize#/2),(#NtFrameSize#/2),0,0,#NtFrameSize#,#NtFrameSize# | #vShadowStartColor# ; (#NtFrameSize#=0 ? 0 : #vBorderSize#/(#NtFrameSize#=0 ? 0.00001 : #NtFrameSize#)) | [mSetFrame] ; 1
GradTop=90 | #vShadowStartColor# ; 0 | [mSetFrame] ; 1
GradLeft=0 | #vShadowStartColor# ; 0 | [mSetFrame] ; 1
;-- Border ------------------------------------------
Shape13=Rectangle #NtLinedFrameSize#,#NtLinedFrameSize#,(#vImageWidth#+#vOutlineSize#),(#vImageHeight#+#vOutlineSize#)
Shape14=Combine Shape4 | Exclude Shape13
Hidden=1
