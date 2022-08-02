echo ######input FILE PATH###########
echo ######input DST NAME###########
echo ######input CFA TYPE###########
set /p path=
set /p name=
set /p type=



@if "%type%"=="rggb" (
    @REM cls
    echo.
    @REM echo rggb
    .\raw2tiff.exe -M -d short -c none -w 1920 %path%  tmp.tiff
    RENAME tmp.tiff tmp.dng
    .\exiftool.exe  -PhotometricInterpretation="Color Filter Array" -IFD0:CFAPattern2="0 1 1 2" -IFD0:CFARepeatPatternDim="2 2" tmp.dng
    RENAME tmp.dng %name%
)
@if "%type%"=="bggr" (
    @REM cls
    echo.
    @REM echo rggb
    .\raw2tiff.exe -M -d short -c none -w 1920 %path%  tmp.tiff
    RENAME tmp.tiff tmp.dng
    .\exiftool.exe  -PhotometricInterpretation="Color Filter Array" -IFD0:CFAPattern2="2 1 1 0" -IFD0:CFARepeatPatternDim="2 2" tmp.dng
    RENAME tmp.dng %name%
)
@if "%type%"=="grbg" (
    @REM cls
    echo.
    @REM echo rggb
    .\raw2tiff.exe -M -d short -c none -w 1920 %path%  tmp.tiff
    RENAME tmp.tiff tmp.dng
    .\exiftool.exe  -PhotometricInterpretation="Color Filter Array" -IFD0:CFAPattern2="1 0 2 1" -IFD0:CFARepeatPatternDim="2 2" tmp.dng
    RENAME tmp.dng %name%
)
@if "%type%"=="gbrg" (
    @REM cls
    echo.
    @REM echo rggb
    .\raw2tiff.exe -M -d short -c none -w 1920 %path%  tmp.tiff
    RENAME tmp.tiff tmp.dng
    .\exiftool.exe  -PhotometricInterpretation="Color Filter Array" -IFD0:CFAPattern2="1 2 0 1" -IFD0:CFARepeatPatternDim="2 2" tmp.dng
    RENAME tmp.dng %name%
)