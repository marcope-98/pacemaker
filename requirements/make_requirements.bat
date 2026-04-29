@echo off
setlocal
set "cwd=%cd%"
cd /d %~dp0
mkdir latexmk-\chapters\REQ latexmk-\chapters\TC latexmk-\lib
latexmk -synctex=1 -file-line-error -pdf -interaction=nonstopmode -outdir="%~dp0\latexmk-build" "%~dp0\pacemaker_SRS.tex"
latexmk -synctex=1 -file-line-error -pdf -interaction=nonstopmode -outdir="%~dp0\latexmk-build" "%~dp0\pacemaker_TS.tex"
cd /d "%cwd%"