@setlocal
@echo off

call :cleardir .build
call :cleardir .vs
call :cleardir .\src\ipch
call :clearfile *.ncb
call :clearfile *.pdb
call :clearfile *.rpt
call :clearfile *.plg
call :clearfile *.opt
call :clearfile *.ilk
call :clearfile *.tlh
call :clearfile *.tli
call :clearfile *.suo
call :clearfile *.aps
call :clearfile *.dep
call :clearfile *.idb
call :clearfile *.user
call :clearfile *.tmp
call :clearfile *.sdf

pause
goto :eof

:: ===========================================================================
:: Remove a directory
:: ===========================================================================
:cleardir
echo [Remove Directory: %1]
for /D /R %%i in (%1) do (
	if exist %%i (
		echo rmdir "%%i"
		rmdir "%%i" /s /q
	)
)
goto :eof

:: ===========================================================================
:: Remove a file
:: ===========================================================================
:clearfile
echo [Remove file: %1]
for /R %%i in (%1) do (
	if exist %%i (
		echo del "%%i"
		del "%%i" /q
	)
)
goto :eof

