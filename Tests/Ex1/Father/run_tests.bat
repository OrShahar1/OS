@ECHO OFF 
:: This batch file runs all test cases found under input directory
TITLE Test Father Program
ECHO Please wait... Starting tests.
:: Section 1: Windows 10 information
ECHO ==========================
ECHO TEST INFO
ECHO ============================
SET FatherPath=%~dp0..\..\Debug\
ECHO FatherPath = %FatherPath% 
SET OutputFile=%FatherPath%output.txt
ECHO OutputFile = %OutputFile%
SET InputTestCasesPath=%~dp0input_files\ 
ECHO InputTestCasesPath = %InputTestCasesPath%
SET OutputTestCasesPath=%~dp0output_files\
ECHO OutputTestCasesPath = %OutputTestCasesPath%  
cd %~dp0
ECHO ==========================
ECHO Starting loop
ECHO ============================
FOR /R %%f in (input_files\*) do (
  CD %FatherPath%
  SET TestCaseName=%%~nf
  echo fullname: %%f
  echo Executing with test case: %%~nf.txt %TestCaseName%
  START /W "MyProg" %FatherPath%\Father.exe %%f
  ECHO %OutputFile%
  COPY %OutputFile% %OutputTestCasesPath%
  SET moved_file=%OutputTestCasesPath%output.txt
  ::ECHO Moved outputfile full path : %moved_file%
  RENAME "%OutputTestCasesPath%output.txt" %%~nf.txt
)

