var fso;

function GetNameFromFile(fileName) {
    var pos = fileName.lastIndexOf(".");
    return fileName.substr(0, pos);
}

function OnFinish(selProj, selObj) {
    try {
        try {
            fso = new ActiveXObject("Scripting.FileSystemObject");
        } catch (ex) {
            wizard.ReportError("Cannot instantiate QtProjectEngine object!");
            return false;
        }

        // vs
        //var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
        //var solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
        var projectPath = wizard.FindSymbol("PROJECT_PATH");
        var projectName = wizard.FindSymbol("PROJECT_NAME");
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH");
        //var exclusive = wizard.FindSymbol("CLOSE_SOLUTION");
        // modules
        var appName = wizard.FindSymbol("APP_NAME");
        //var appClassName = wizard.FindSymbol("APP_CLASS_NAME");
        var workModeName = wizard.FindSymbol("WORK_MODE_NAME");

        //////////

        if (dte.version == "9.0") {
            //
        } else if (dte.version == "10.0") {
            wizard.AddSymbol("TOOLS_VERSION", "4.0");
        } else {
            wizard.AddSymbol("TOOLS_VERSION", dte.version);
        }

        // platform version
        wizard.AddSymbol("PLATFORM_TOOLSET", "v" + dte.version.replace(/\./g, ''));

        //////////

        selProj = CreateQtProject(projectName, projectPath);

        AddCommonConfig(selProj, projectName);
        AddSpecificConfig(selProj, projectName, projectPath);

        SetupFilters(selProj);

        SetResDlgFont();

        AddFilesToProjectWithInfFile(selProj, projectName);
        SetCommonPchSettings(selProj);

        selProj.Object.Save();

        //
        CopyCustomFiles(selProj);
        //
        AddSolutionFolders();

        //////////

    } catch (e) {
        //var msg = "name: " + e.name + "\n" +
        //    "message: " + e.message + "\n";
        //wizard.ReportError("OnFinish => \n" + msg);
        if (e.description.length != 0)
            SetErrorInfo(e);
        return e.number
    }
}

/******************************************************************************
Description: Creates a Qt project
strProjectName: Project Name
strProjectPath: The path that the project will be created in
******************************************************************************/
function CreateQtProject(projectName, projectPath) {
    try {
        var appName = wizard.FindSymbol("APP_NAME");
        var workModeName = wizard.FindSymbol("WORK_MODE_NAME");
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH") + "\\";
        var projectTemplatePath = templatePath + "source\\framework\\app\\";
        var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
        var solution = dte.Solution;
        var solutionName = "";
        var projectFileSuffix = "";

        // 
        if (dte.version == "9.0") {
            projectFileSuffix = ".vcproj";
        } else {
            projectFileSuffix = ".vcxproj";
        }

        if (wizard.FindSymbol("CLOSE_SOLUTION")) {
            solution.Close();
            solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
            if (solutionName.length) {
                solution.Create(solutionPath, solutionName);
            }
        }
        //
        projectTemplatePath += "default" + dte.version.replace(/\./g, '') + projectFileSuffix;
        var projectNameWithExt = projectName + projectFileSuffix;
        var target = wizard.FindSymbol("TARGET");
        var project;
        if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject) { // vsWizardAddSubProject
            var projectItem = target.AddFromTemplate(projectTemplatePath, projectPath + "\\" + projectNameWithExt);
            project = projectItem.SubProject;
        } else {
            // -- solution - framework folder --
            var frameworkProj = solution.AddSolutionFolder("framework");
            project = frameworkProj.Object.AddFromTemplate(projectTemplatePath, projectPath, projectNameWithExt);
        }
        // 
        return project;
    } catch (e) {
        if (e.description.length != 0)
            SetErrorInfo(e);
        throw e;
    }
}

function AddSolutionFolders() {
    var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
    var projectName = wizard.FindSymbol("PROJECT_NAME");
    var templatePath = wizard.FindSymbol("TEMPLATES_PATH") + "\\";
    var solution = dte.Solution;
    var appName = wizard.FindSymbol("APP_NAME");
    var workModeName = wizard.FindSymbol("WORK_MODE_NAME");
    /// ---------------------------------------
    // -- config --
    var configProjItem = solution.AddSolutionFolder("config");
    // -- config/app --
    var appProjItem = configProjItem.Object.AddSolutionFolder(appName);
    AddSolutionFile(appProjItem, GetTargetName("config/" + appName + "/jframe_global.xml"), true);
    AddSolutionFile(appProjItem, GetTargetName("config/" + appName + "/jframe_logging.conf"), true);
    // -- config/workmode -- 
    var workModeProjItem = appProjItem.Object.AddSolutionFolder("workmode_1");
    AddSolutionFile(workModeProjItem, GetTargetName("config/" + appName + "/workmode_1/jframe_component.xml"), true);
    AddSolutionFile(workModeProjItem, GetTargetName("config/" + appName + "/workmode_1/jframe_layout.xml"), true);
    // -- 3rdpart --
    var thirdPartProjItem = solution.AddSolutionFolder("3rdpart");
    // -- core --
    var coreProjItem = solution.AddSolutionFolder("core");
    // -- component --
    var componentProjItem = solution.AddSolutionFolder("component");
    // -- component/app --
    var componentAppProjItem = componentProjItem.AddSolutionFolder(appName);
    /// ---------------------------------------

    /// remove folders
    if (fso.FolderExists(solutionPath + projectName)) {
        fso.DeleteFolder(solutionPath + projectName);
    }
}

function AddSpecificConfig(project, projectName, projectPath) {
    try {
        //
        //project.Object.ToolsVersion = wizard.FindSymbol("TOOLS_VERSION");
        //project.Object.PlatformToolset = wizard.FindSymbol("PLATFORM_TOOLSET");
        //
        var configs = project.Object.Configurations;
        for (var i = 1; i <= configs.Count; i++) {
            var config = configs(i);
            var bDebug = false;
            if (-1 != config.Name.indexOf("Debug"))
                bDebug = true;

            // Default to unicode
            config.CharacterSet = charSetUNICODE;
            //
            config.UseOfMFC = useMfcDynamic;

            // ------------compilerTool--------------

            var compilerTool = config.Tools("VCCLCompilerTool");

            //
            var additionalIncludeDirectories = compilerTool.AdditionalIncludeDirectories;
            if (additionalIncludeDirectories != "") {
                //additionalIncludeDirectories += ";";
            }
            //additionalIncludeDirectories += ";$(JFRAME_DIR)\\lib\\3rdpart";
            compilerTool.AdditionalIncludeDirectories = additionalIncludeDirectories;

            //
            var preProcDefines = compilerTool.PreprocessorDefinitions;
            if (preProcDefines != "") {
                //preProcDefines += ";";
            }
            if (bDebug) {
                //
            } else {
                //
            }
            //preProcDefines += GetPlatformDefine(config);
            //
            compilerTool.PreprocessorDefinitions = preProcDefines;

            // ------------linkerTool--------------

            var linkerTool = config.Tools("VCLinkerTool");

            //
            linkerTool.LinkIncremental = (bDebug ? linkIncrementalYes : linkIncrementalNo);

            // 
            var additionalLibraryDirectories = linkerTool.AdditionalLibraryDirectories;
            if (additionalLibraryDirectories != "") {
                //additionalLibraryDirectories += ";";
            }
            //additionalLibraryDirectories += "$(JFRAME_DIR)\\lib\\3rdpart";
            linkerTool.AdditionalLibraryDirectories = additionalLibraryDirectories;

            //
            var additionalDependencies = linkerTool.AdditionalDependencies;
            if (additionalDependencies != "") {
                //
            }
            if (bDebug) {
                //
            } else {
                //
            }
            linkerTool.AdditionalDependencies = additionalDependencies;

            // ------------resourceCompilerTool--------------

            var resourceCompilerTool = config.Tools("VCResourceCompilerTool");
            resourceCompilerTool.Culture = wizard.FindSymbol("LCID");
            resourceCompilerTool.PreprocessorDefinitions = (bDebug ? "_DEBUG" : "NDEBUG");
            resourceCompilerTool.AdditionalIncludeDirectories = "$(IntDir)";
        } //for
    } catch (e) {
        if (e.description.length != 0)
            SetErrorInfo(e);
        return e.number
    }
}

function GetTargetName(sourceName, projectName, resourcePath, helpPath) {
    try {
        var targetName = sourceName;
        var solutionPath = wizard.FindSymbol('SOLUTION_PATH');
        var projectName = wizard.FindSymbol('PROJECT_NAME');
        var appName = wizard.FindSymbol("APP_NAME");
        var workModeName = wizard.FindSymbol("WORK_MODE_NAME");
        // -- . --
        if (sourceName == "app.pro") {
            targetName = solutionPath + "" + wizard.FindSymbol("VS_SOLUTION_NAME") + ".pro";
        }
        // -- bin --
        else if (sourceName == "bin/jframeworkdir.dll") {
            targetName = solutionPath + "bin/jframeworkdir.dll";
        }
        // -- config --
        else if (sourceName == "config/config.pri") {
            targetName = solutionPath + "config/config.pri";
        }
        // -- config/app --
        else if (sourceName == "config/app/app.pri") {
            targetName = solutionPath + "config/" + appName + "/" + appName + ".pri";
        } else if (sourceName == "config/app/jframe_global.xml") {
            targetName = solutionPath + "config/" + appName + "/jframe_global.xml";
        } else if (sourceName == "config/app/jframe_logging.conf") {
            targetName = solutionPath + "config/" + appName + "/jframe_logging.conf";
        }
        // -- config/app/resource --
        else if (sourceName == "config/app/resource/app.ico") {
            targetName = solutionPath + "config/" + appName + "/resource/app.ico";
        } else if (sourceName == "config/app/resource/finish.png") {
            targetName = solutionPath + "config/" + appName + "/resource/finish.png";
        } else if (sourceName == "config/app/resource/splash.png") {
            targetName = solutionPath + "config/" + appName + "/resource/splash.png";
        }
        // -- config/workmode_1 --
        else if (sourceName == "config/app/workmode_1/jframe_component.xml") {
            targetName = solutionPath + "config/" + appName + "/" + workModeName + "/jframe_component.xml";
        } else if (sourceName == "config/app/workmode_1/jframe_layout.xml") {
            targetName = solutionPath + "config/" + appName + "/" + workModeName + "/jframe_layout.xml";
        }
        // -- source -- 
        else if (sourceName == "source/source.pro") {
            targetName = solutionPath + "source/source.pro";
        }
        // -- source/3rdpart --
        else if (sourceName == "source/3rdpart/3rdpart.pro") {
            targetName = solutionPath + "source/3rdpart/3rdpart.pro";
        }
        // -- source/common --
        else if (sourceName == "source/common/build.pri") {
            targetName = solutionPath + "source/common/build.pri";
        } else if (sourceName == "source/common/copy.ignore") {
            targetName = solutionPath + "source/common/copy.ignore";
        } else if (sourceName == "source/common/precomp.h") {
            targetName = solutionPath + "source/common/precomp.h";
        }
        // -- source/component --
        else if (sourceName == "source/component/component.pro") {
            targetName = solutionPath + "source/component/component.pro";
        }
        // -- source/component/app --
        else if (sourceName == "source/component/app/app.pro") {
            targetName = solutionPath + "source/component/" + appName + "/" + appName + ".pro";
        }
        // -- source/core --
        else if (sourceName == "source/core/core.pro") {
            targetName = solutionPath + "source/core/core.pro";
        }
        // -- source/framework --
        else if (sourceName == "source/framework/framework.pro") {
            targetName = solutionPath + "source/framework/framework.pro";
        }
        // -- source/framework/application --
        else if (sourceName == "source/framework/app/app.pri") {
            targetName = appName + ".pri";
        } else if (sourceName == "source/framework/app/app.pro") {
            targetName = appName + ".pro";
        } else if (sourceName == "source/framework/app/res/app.ico") {
            targetName = "res/" + projectName + ".ico";
        } else if (sourceName == "source/framework/app/res/Application.rc2") {
            targetName = "res/" + projectName + ".rc2";
        } else if (sourceName == "source/framework/app/Application.cpp") {
            targetName = projectName + ".cpp";
        } else if (sourceName == "source/framework/app/Application.h") {
            targetName = projectName + ".h";
        } else if (sourceName == "source/framework/app/Application.rc") {
            targetName = projectName + ".rc";
        } else if (sourceName == "source/framework/app/ChildView.cpp") {
            targetName = "ChildView.cpp";
        } else if (sourceName == "source/framework/app/ChildView.h") {
            targetName = "ChildView.h";
        } else if (sourceName == "source/framework/app/main.cpp") {
            targetName = "main.cpp";
        } else if (sourceName == "source/framework/app/MainFrm.cpp") {
            targetName = "MainFrm.cpp";
        } else if (sourceName == "source/framework/app/MainFrm.h") {
            targetName = "MainFrm.h";
        } else if (sourceName == "source/framework/app/Resource.h") {
            targetName = "Resource.h";
        } else if (sourceName == "source/framework/app/stdafx.cpp") {
            targetName = "stdafx.cpp";
        } else if (sourceName == "source/framework/app/stdafx.h") {
            targetName = "stdafx.h";
        } else if (sourceName == "source/framework/app/targetver.h") {
            targetName = "targetver.h";
        }
        return targetName;
    } catch (e) {
        var msg = "name: " + e.name + "\n" +
            "message: " + e.message + "\n" +
            "description: " + e.description;
        wizard.ReportError("GetTargetName => \n" + msg);
        throw e;
    }
}

function CopyCustomFiles(selProj) {
    // vs
    var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
    var solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
    var projectPath = wizard.FindSymbol("PROJECT_PATH");
    var projectName = wizard.FindSymbol("PROJECT_NAME");
    var templatePath = wizard.FindSymbol("TEMPLATES_PATH") + "\\";
    var exclusive = wizard.FindSymbol("CLOSE_SOLUTION");

    try {
        // -- . --
        CopyFileWithTargetName("app.pro", templatePath, projectName);
        // -- bin --
        CopyFileWithTargetName("bin/jframeworkdir.dll", templatePath, projectName);
        // -- config --
        CopyFileWithTargetName("config/config.pri", templatePath, projectName);
        // -- config/app --
        CopyFileWithTargetName("config/app/app.pri", templatePath, projectName);
        CopyFileWithTargetName("config/app/jframe_global.xml", templatePath, projectName);
        CopyFileWithTargetName("config/app/jframe_logging.conf", templatePath, projectName);
        // -- config/app/resource --
        CopyFileWithTargetName("config/app/resource/app.ico", templatePath, projectName);
        CopyFileWithTargetName("config/app/resource/finish.png", templatePath, projectName);
        CopyFileWithTargetName("config/app/resource/splash.png", templatePath, projectName);
        // -- config/app/workmode_1 --
        CopyFileWithTargetName("config/app/workmode_1/jframe_component.xml", templatePath, projectName);
        CopyFileWithTargetName("config/app/workmode_1/jframe_layout.xml", templatePath, projectName);
        // -- source --
        CopyFileWithTargetName("source/source.pro", templatePath, projectName);
        // -- source/3rdpart --
        CopyFileWithTargetName("source/3rdpart/3rdpart.pro", templatePath, projectName);
        // -- source/comon --
        CopyFileWithTargetName("source/common/build.pri", templatePath, projectName);
        CopyFileWithTargetName("source/common/copy.ignore", templatePath, projectName);
        CopyFileWithTargetName("source/common/precomp.h", templatePath, projectName);
        // -- source/component --
        CopyFileWithTargetName("source/component/component.pro", templatePath, projectName);
        // -- source/component/app --
        CopyFileWithTargetName("source/component/app/app.pro", templatePath, projectName);
        // -- source/core --
        CopyFileWithTargetName("source/core/core.pro", templatePath, projectName);
        // -- source/framework --
        CopyFileWithTargetName("source/framework/framework.pro", templatePath, projectName);
        // -- source/framework/app --
        CopyFileWithTargetName("source/framework/app/app.pri", templatePath, projectName);
        CopyFileWithTargetName("source/framework/app/app.pro", templatePath, projectName);
        CopyFileWithTargetName("source/framework/app/main.cpp", templatePath, projectName);
    } catch (e) {
        if (e.description.length != 0)
            SetErrorInfo(e);
        throw e;
    }
}

function AddSolutionFile(parentProj, fileName, bClose) {
    var fileProjItem = parentProj.ProjectItems.AddFromFile(fileName);
    if (bClose && fileProjItem) {
        //fileProjItem.Object.Close();
    }
}

function SetFileProperties(projfile, sourceName) {
    if (sourceName == "dllmain.cpp") {
        var configs = projfile.Object.FileConfigurations;
        for (var i = 1; i <= configs.Count; ++i) {
            var config = configs(i);
            var clTool = config.Tool;
            clTool.CompileAsManaged = 0; // Force no /CLR
            clTool.UsePrecompiledHeader = 0; // No pre-compiled headers			
        }
    }
}

//
function CopyFileWithTargetName(sourceName, sourceBase, projectName) {
    var targetName = GetTargetName(sourceName, projectName);
    if (targetName != "") {
        CopyFile(sourceName, sourceBase, targetName);
    }
}

//
function CopyFile(sourceName, sourceBase, dest) {
    sourceBase = sourceBase.replace(/\//g, '\\');
    if (sourceBase.charAt(sourceBase.length - 1) != '\\') {
        sourceBase += '\\';
    }
    sourceName = sourceName.replace(/\//g, '\\');
    dest = dest.replace(/\//g, '\\');
    try {
        var destdir = dest;
        var lastIndex = dest.lastIndexOf('\\');
        if (lastIndex != -1) {
            destdir = dest.substr(0, lastIndex).substring();
        } else {
            return false;
        }
        //
        if (!fso.FolderExists(destdir)) {
            fso.CreateFolder(destdir);
        }
        fso.CopyFile(sourceBase + sourceName, dest);
    } catch (e) {
        if (e.description.length != 0)
            SetErrorInfo(e);
        throw e;
    }
}