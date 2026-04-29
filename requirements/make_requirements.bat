@echo off
setlocal
set "cwd=%cd%"
cd /d %~dp0
mkdir build\chapters build\lib
latexmk -synctex=1 -file-line-error -pdf -interaction=nonstopmode -outdir="%~dp0\build" "%~dp0\pacemaker_SRS.tex"
@REM latexmk -synctex=1 -file-line-error -pdf -interaction=nonstopmode -outdir="%~dp0\build" "%~dp0\pacemaker_tests.tex"
cd /d "%cwd%"