@echo off
set /p input=�������ļ�������ֱ�ӻس���ֻ����ģ��: 

if "%input%"=="" (
    echo  ��ѡ�в������ļ��У�ֱ�Ӹ���ģ�塣
) else (
    pushd ..
    mkdir %input%
    cd %input%
    echo  �ļ����Ѵ���: %input% ���������ģ�� 
    popd
)

@echo off
setlocal enabledelayedexpansion

set sourceFolder=%CD%
set targetFolder=%CD%\..

set scriptName=%~nx0

for /r "%sourceFolder%" %%i in (*.c) do (
    set "sourceFile=%%i"
    set "targetFile=%targetFolder%!sourceFile:%sourceFolder%=!"

    if not exist "!targetFile!" (
        copy "!sourceFile!" "!targetFile!"
        echo �����ļ�: "!targetFile!"
    )  
)


for /d %%i in ("%targetFolder%\*") do (
    set "subFolder=%%i"
    for %%j in (!sourceFolder!\*.c) do (
        set "sourceFile=%%j"
        set "targetFile=!subFolder!\!sourceFile:%sourceFolder%\=!"

        if not exist "!targetFile!" (
            copy "!sourceFile!" "!targetFile!"
            echo �����ļ�: "!targetFile!"
        ) 
    )
)

echo ģ��ͬ�����,������˳�
pause
