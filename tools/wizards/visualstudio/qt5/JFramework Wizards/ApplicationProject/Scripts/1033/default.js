var QtEngine;

function GetNameFromFile(strFile) {
    var nPos = strFile.lastIndexOf(".");
    return strFile.substr(0, nPos);
}

function OnFinish(selProj, selObj) {
    try {
        try {
            QtEngine = new ActiveXObject("Digia.Qt5ProjectEngine");
        } catch (ex) {
            wizard.ReportError("Cannot instantiate QtProjectEngine object!");
            return false;
        }

        // vs
        var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
        var solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
        var projectPath = wizard.FindSymbol("PROJECT_PATH");
        var projectName = wizard.FindSymbol("PROJECT_NAME");
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH") + "\\";
        var exclusive = wizard.FindSymbol("CLOSE_SOLUTION");
        // modules
        var applicationName = wizard.FindSymbol("APPLICATION_NAME");
        var applicationClassName = wizard.FindSymbol("APPLICATION_CLASS_NAME");

        //////////

        selProj = CreateQtProject(projectName, projectPath);
        AddCommonConfig(selProj, projectName);
        AddSpecificConfig(selProj, projectName, projectPath);
        SetCommonPchSettings(selProj);
        SetupFilters(selProj);
        SetResDlgFont();
        AddFilesToProjectWithInfFile(selProj, projectName);
        selProj.Object.Save();

    } catch (e) {
        var msg = "name: " + e.name + "\n" +
            "message: " + e.message + "\n" +
            "description: " + e.description;
        wizard.ReportError("OnFinish => \n" + msg);
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
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH");
        var projectTemplatePath = wizard.FindSymbol("PROJECT_TEMPLATE_PATH");
        var projectTemplate = templatePath + "\\source\\framework\\application\\Application140.vcxproj";
        var solutionPath = wizard.FindSymbol("SOLUTION_PATH");
        var solution = dte.Solution;
        var solutionName = "";

        if (wizard.FindSymbol("CLOSE_SOLUTION")) {
            solution.Close();
            solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
            if (solutionName.length) {
                solution.Create(solutionPath, solutionName);
            }
        }

        var projectNameWithExt = projectName + ".vcxproj";
        var target = wizard.FindSymbol("TARGET");
        var project;
        if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject) { // vsWizardAddSubProject
            var projectItem = target.AddFromTemplate(projectTemplate, projectPath + "\\" + projectNameWithExt);
            project = projectItem.SubProject;
        } else {
            project = target.AddFromTemplate(projectTemplate, projectPath, projectNameWithExt);
        }
        return project;
    } catch (e) {
        throw e;
    }
}

function AddSpecificConfig(project, projectName, projectPath) {
    try {
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
                additionalIncludeDirectories += ";";
            }
            additionalIncludeDirectories += ";$(JFRAME_DIR)\\lib\\3rdpart";
            compilerTool.AdditionalIncludeDirectories = additionalIncludeDirectories;

            //
            var preProcDefines = compilerTool.PreprocessorDefinitions;
            if (preProcDefines != "") {
                //
            }
            if (bDebug) {
                //
            } else {
                //
            }
            preProcDefines += GetPlatformDefine(config);
            //
            compilerTool.PreprocessorDefinitions = preProcDefines;

            // ------------linkerTool--------------

            var linkerTool = config.Tools("VCLinkerTool");

            //
            linkerTool.LinkIncremental = (bDebug ? linkIncrementalYes : linkIncrementalNo);

            // 
            var additionalLibraryDirectories = linkerTool.AdditionalLibraryDirectories;
            if (additionalLibraryDirectories != "") {
                additionalLibraryDirectories += ";";
            }
            additionalLibraryDirectories += "$(JFRAME_DIR)\\lib\\3rdpart";
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
        var msg = "name: " + e.name + "\n" +
            "message: " + e.message + "\n" +
            "description: " + e.description;
        wizard.ReportError("OnFinish => \n" + msg);
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
        // -- . --
        if (sourceName == "jframework140.sln") {
            targetName = solutionPath + "/" + wizard.FindSymbol("VS_SOLUTION_NAME") + ".sln";
        } else if (sourceName == "jframework.pro") {
            targetName = solutionPath + "/" + wizard.FindSymbol("VS_SOLUTION_NAME") + ".pro";
        }
        // -- bin --
        else if (sourceName == "bin/jframeworkdir.dll") {
            targetName = solutionPath + "/bin/jframeworkdir.dll";
        }
        // -- config --
        else if (sourceName == "config/config.pri") {
            targetName = solutionPath + "/config/config.pri";
        }
        // -- config/frame --
        else if (sourceName == "config/frame/jframe_global.xml") {
            targetName = solutionPath + "/config/frame/jframe_global.xm";
        } else if (sourceName == "config/frame/jframe_layout.xml") {
            targetName = solutionPath + "/config/frame/jframe_layout.xm";
        } else if (sourceName == "config/frame/jframe_logging.xml") {
            targetName = solutionPath + "/config/frame/jframe_logging.xm";
        }
        // -- config/resource --
        else if (sourceName == "config/resource/app.ico") {
            targetName = solutionPath + "/config/resource/app.ico";
        } else if (sourceName == "config/resource/finish.png") {
            targetName = solutionPath + "/config/resource/finish.png";
        } else if (sourceName == "config/resource/splash.png") {
            targetName = solutionPath + "/config/resource/splash.png";
        }
        // -- config/workmode_1 --
        else if (sourceName == "config/workmode_1/jframe_component.xml") {
            targetName = solutionPath + "/config/workmode_1/jframe_component.xml";
        }
        // -- source -- 
        else if (sourceName == "source/source.pro") {
            targetName = solutionPath + "/source/source.pro";
        }
        // -- source/common --
        else if (sourceName == "source/common/build.pri") {
            targetName = solutionPath + "/source/common/build.pri";
        } else if (sourceName == "source/common/copy.ignore") {
            targetName = solutionPath + "/source/common/copy.ignore";
        } else if (sourceName == "source/common/precomp.h") {
            targetName = solutionPath + "/source/common/precomp.h";
        }
        // -- source/component --
        else if (sourceName == "source/component/component.pro") {
            targetName = solutionPath + "/source/component/component.pro";
        }
        // -- source/framework --
        else if (sourceName == "source/framework/framework.pro") {
            targetName = solutionPath + "/source/framework/framework.pro";
        }
        // -- source/framework/application --
        else if (sourceName == "source/framework/application/res/Application.ico") {
            targetName = "source/framework/" + projectName + "/res/" + projectName + ".ico";
        } else if (sourceName == "source/framework/application/res/Application.rc2") {
            targetName = "source/framework/" + projectName + "/res/" + projectName + ".rc2";
        } else if (sourceName == "source/framework/application/Application.cpp") {
            targetName = "source/framework/" + projectName + "/" + projectName + ".cpp";
        } else if (sourceName == "source/framework/application/Application.h") {
            targetName = "source/framework/" + projectName + "/" + projectName + ".h";
        } else if (sourceName == "source/framework/application/Application.pri") {
            targetName = "source/framework/" + projectName + "/" + projectName + ".pri";
        } else if (sourceName == "source/framework/application/Application.pro") {
            targetName = "source/framework/" + projectName + "/" + projectName + ".pro";
        } else if (sourceName == "source/framework/application/Application.rc") {
            targetName = "source/framework/" + projectName + "/" + projectName + ".rc";
        } else if (sourceName == "source/framework/application/ChildView.cpp") {
            targetName = "source/framework/" + projectName + "/ChildView.cpp";
        } else if (sourceName == "source/framework/application/ChildView.h") {
            targetName = "source/framework/" + projectName + "/ChildView.h";
        } else if (sourceName == "source/framework/application/main.cpp") {
            targetName = "source/framework/" + projectName + "/main.cpp";
        } else if (sourceName == "source/framework/application/MainFrm.cpp") {
            targetName = "source/framework/" + projectName + "/MainFrm.cpp";
        } else if (sourceName == "source/framework/application/MainFrm.h") {
            targetName = "source/framework/" + projectName + "/MainFrm.h";
        } else if (sourceName == "source/framework/application/Resource.h") {
            targetName = "source/framework/" + projectName + "/Resource.h";
        } else if (sourceName == "source/framework/application/stdafx.cpp") {
            targetName = "source/framework/" + projectName + "/stdafx.cpp";
        } else if (sourceName == "source/framework/application/stdafx.h") {
            targetName = "source/framework/" + projectName + "/stdafx.h";
        } else if (sourceName == "source/framework/application/targetver.h") {
            targetName = "source/framework/" + projectName + "/targetver.h";
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

function SetFileProperties(projfile, fileName) {
    if (fileName == "dllmain.cpp") {
        var Configs = projfile.Object.FileConfigurations;
        for (var i = 1; i <= Configs.Count; ++i) {
            var Config = Configs(i);
            var CLTool = Config.Tool;
            CLTool.CompileAsManaged = 0; // Force no /CLR
            CLTool.UsePrecompiledHeader = 0; // No pre-compiled headers			
        }
    }
}