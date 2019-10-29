::@echo off
SET OBC_PYTHON=python
SET OBC_PERL_CHKPATH_FILE=%CD%\..\..\..\tools\upg\sha256.py
"%OBC_PYTHON%" "%OBC_PERL_CHKPATH_FILE%" "%1" "%1.result"