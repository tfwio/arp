:: inkscape-gen.cmd
@ECHO OFF
SET INKSCAPE_PATH=c:\Program Files (x86)\Inkscape
SET INKSCAPE_PATH=c:\Program Files\Inkscape
SET PATH=%INKSCAPE_PATH%;%PATH%
SET FN=arpeggiator.svg
SET ROOT=%~dp0

::CALL:DO_SOMETHING "%FN%" transportation
CALL:DO_SOMETHING "%FN%" panic
CALL:DO_SOMETHING "%FN%" img_sync

CALL:DO_SOMETHING "%FN%" img_keyse
CALL:DO_SOMETHING "%FN%" img_keysi

CALL:DO_SOMETHING "%FN%" img_up
CALL:DO_SOMETHING "%FN%" img_dn
CALL:DO_SOMETHING "%FN%" img_ud
CALL:DO_SOMETHING "%FN%" img_du
CALL:DO_SOMETHING "%FN%" img_ru
CALL:DO_SOMETHING "%FN%" img_rd

CALL:DO_SOMETHING "%FN%" img_1_2
CALL:DO_SOMETHING "%FN%" img_1_3
CALL:DO_SOMETHING "%FN%" img_1_4
CALL:DO_SOMETHING "%FN%" img_1_6
CALL:DO_SOMETHING "%FN%" img_1_8
CALL:DO_SOMETHING "%FN%" img_1_16
CALL:DO_SOMETHING "%FN%" img_4_8
CALL:DO_SOMETHING "%FN%" img_4_6
CALL:DO_SOMETHING "%FN%" img_4_4
CALL:DO_SOMETHING "%FN%" img_4_3
CALL:DO_SOMETHING "%FN%" img_4_2
CALL:DO_SOMETHING "%FN%" img_4_1
CALL:DO_SOMETHING "%FN%" img_3_8
CALL:DO_SOMETHING "%FN%" img_3_6
CALL:DO_SOMETHING "%FN%" img_3_4
CALL:DO_SOMETHING "%FN%" img_3_3
CALL:DO_SOMETHING "%FN%" img_3_2
CALL:DO_SOMETHING "%FN%" img_3_1
::
GOTO:EOF

:: SET OUT_PATH=%~dp0onoffbtns-0
:: if NOT EXIST %OUT_PATH% mkdir %OUT_PATH%
:: CALL:DO_SOMETHING "%FN%" BtnTonOn
::LL:DO_SOMETHING "%~1" BtnTonOn

:DO_SOMETHING
  SET DOTHIS=inkscape -e "%OUT_PATH%%~2.png" --export-id="%~2" -f %FN%
  echo %DOTHIS%
  %DOTHIS%
  IF NOT ERRORLEVEL 0 (
    SET ERRORLEVEL=0
    echo whew
    pause
  )
  GOTO:EOF
:DO_DIR
  SET OUT_PATH=%ROOT%%~1
  if NOT EXIST %OUT_PATH% mkdir %OUT_PATH%
  GOTO:EOF

:: cmd /k /E:ON /V:ON
:: GOTO:EOF
:: --select=OBJECT-ID
:: -z - without GUI
:: -f - filename
:: -e, --export-png=FILENAME
:: -d, --export-dpi=DPI
:: -D --export-area-drawing
:: -b --export-background=COLOR
:: -y --export-background-opacity=VALUE
:: -l --export-plain-svg=FILENAME
:: -I --query-id WTF IS THIS
:: -i --export-id
:: -j --export-id-only
:: 
:: :: for EPS, PDF
:: -C --export-area-page
::    --export-margin=VALUE:: 