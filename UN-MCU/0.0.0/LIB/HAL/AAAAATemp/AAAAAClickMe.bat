@echo off
set /p input=请输入文件夹名，直接回车则只更新模板: 

if "%input%"=="" (
    echo  已选中不创建文件夹，直接更新模板。
) else (
    pushd ..
    mkdir %input%
    cd %input%
    echo  文件夹已创建: %input% 任意键更新模板 
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
        echo 复制文件: "!targetFile!"
    )  
)


for /d %%i in ("%targetFolder%\*") do (
    set "subFolder=%%i"
    for %%j in (!sourceFolder!\*.c) do (
        set "sourceFile=%%j"
        set "targetFile=!subFolder!\!sourceFile:%sourceFolder%\=!"

        if not exist "!targetFile!" (
            copy "!sourceFile!" "!targetFile!"
            echo 复制文件: "!targetFile!"
        ) 
    )
)

echo 模板同步完成,任意键退出
pause
