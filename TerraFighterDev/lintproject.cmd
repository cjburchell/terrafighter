@echo off
REM   NT COMMAND FILE:     LINTPROJECT.CMD
REM                        ===============
REM 
REM   Abstract:         Lint the tvnt\src\tvg_5_0 project
REM 
REM   iMPath restricted and confidential. Copyright iMPath 2005. All rights reserved.
REM 
REM   Last update: Thursday, April 7, 2005. @ 17:55:09.
REM 
REM   File: PC503::D:/dev/tvnt/src/tvg_5_0/lintproject.cmd
REM 
REM   RCS $Header: //Software/Official/MKS/MKS_SI/TV_NT/dev/tvnt/src/tvg_5_0/rcs/lintproject.cmd 1.4 2005/04/07 17:55:09 PRouleau Exp $
REM 
REM   Update history:
REM 
REM   - File created: Automatically by /dev/python/pclintset.py
REM   - 25/02/05 rcs : RCS Revision 1.1  2005/02/09 16:29:41  PRouleau
REM   - 25/02/05 rcs : RCS Initial revision
REM   - 07/04/05 rcs : RCS Revision 1.2  2005/02/25 16:26:16  PRouleau
REM   - 07/04/05 rcs : RCS Revision 1.3  2005/04/07 10:46:58  PRouleau
REM 
REM   RCS $Log: lintproject.cmd $
REM   RCS Revision 1.4  2005/04/07 17:55:09  PRouleau
REM   RCS .
REM 
REM -----------------------------------------------------------------------------
call setenv_wincmd redpoint.vs7
set __LINT_DSP=Win32 Release
call lint_dsp D:\dev2\TerraFighterDev TerraFighter
@echo off
set __LINT_DSP=
