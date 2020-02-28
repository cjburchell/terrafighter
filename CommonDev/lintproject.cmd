@echo off
call setenv_wincmd iaxvideo.vs6
call lint_dsp %DIR_DEV2%\CommonDev CommonDev.vcproj
