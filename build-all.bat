@echo OFF

echo  building boobs

pushd engine
call build.bat
popd
if %ERRORLEVEL% neq 0 (echo   error: %ERRORLEVEL% && exit)
echo   built engine

pushd testbed
call build.bat
popd
if %ERRORLEVEL% neq 0 (echo   error: %ERRORLEVEL% && exit)
echo   built testbed

echo  built boobs