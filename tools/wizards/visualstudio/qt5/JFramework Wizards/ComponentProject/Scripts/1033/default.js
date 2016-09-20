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
        var solutionPath = wizard.FindSymbol("VS_SOLUTION_PATH");
        var solutionName = wizard.FindSymbol("VS_SOLUTION_LOCATION");
        var projectPath = wizard.FindSymbol("PROJECT_PATH");
        var projectName = wizard.FindSymbol("PROJECT_NAME");
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH") + "\\";
        var exclusive = wizard.FindSymbol("CLOSE_SOLUTION");
        // modules
        var componentName = wizard.FindSymbol("COMPONENT_NAME");
        var componentDesc = wizard.FindSymbol("COMPONENT_DESC");
        var includeCheckLoginUser = wizard.FindSymbol("INCLUDE_CHECK_LOGIN_USER");
        var includeDynamicLayout = wizard.FindSymbol("INCLUDE_DYNAMIC_LAYOUT");
        var includeIJComponentUi = wizard.FindSymbol("INCLUDE_IJCOMPONENTUI");
        var includeIJCommandSink = wizard.FindSymbol("INCLUDE_IJCOMMANDSINK");
        var includeIJMessageSink = wizard.FindSymbol("INCLUDE_IJMESSAGESINK");
        var includeJObserver = wizard.FindSymbol("INCLUDE_JOBSERVER");
        var includeConfigFile = wizard.FindSymbol("INCLUDE_CONFIG_FILE");
        var includePrecompiled = wizard.FindSymbol("INCLUDE_PRECOMPILED");
        // component_class
        var componentClassName = wizard.FindSymbol("COMPONENT_CLASS_NAME");
        var headerFileName = wizard.FindSymbol("HEADER_FILE_NAME");
        var sourceFileName = wizard.FindSymbol("SOURCE_FILE_NAME");
        // component_ui
        var componentUiClassName = wizard.FindSymbol("COMPONENT_UI_CLASS_NAME");
        var uiBaseClassName = wizard.FindSymbol("UI_BASE_CLASS_NAME");
        var includeQWidget = wizard.FindSymbol("INCLUDE_QWIDGET");
        var includeQDialog = wizard.FindSymbol("INCLUDE_QDIALOG");
        var includeQMainWindow = wizard.FindSymbol("INCLUDE_QMAINWINDOW");
        var uiHeaderFileName = wizard.FindSymbol("UI_HEADER_FILE_NAME");
        var uiSourceFileName = wizard.FindSymbol("UI_SOURCE_FILE_NAME");

        // 
        var regexp = /\W/g;
        var defComponentHeader = headerFileName.toUpperCase().replace(regexp, "_");
        var defComponentUiHeader = uiHeaderFileName.toUpperCase().replace(regexp, "_");
        wizard.AddSymbol("COMPONENT_HEADER_GUARD", defComponentHeader);
        wizard.AddSymbol("COMPONENT_UI_HEADER_GUARD", defComponentUiHeader);

        var index = 0; //["QWidget", "QDialog", "QMainWindow"].indexOf(uiBaseClassName);
        wizard.AddSymbol("BASECLASS_INSTANCEOF_QOBJECT", (index >= 0));

        wizard.AddSymbol("UI_BASE_CLASS_NAME_IS_EMPTY", uiBaseClassName === "");
        wizard.AddSymbol("UI_BASE_CLASS_IS_QOBJECT", uiBaseClassName === "QObject");
        wizard.AddSymbol("UI_BASE_CLASS_INERITS_QWIDGET", uiBaseClassName === "QWidget" ||
            uiBaseClassName === "QDialog" || uiBaseClassName === "QMainWindow");

        //////////

        selProj = CreateQtProject(projectName, projectPath);

        //QtEngine.CreateLibraryProject(wizard.dte, projectName,
        //    projectPath, solutionName, exclusive, false, includePrecompiled);

        AddCommonConfig(selProj, projectName, /*unicode*/ true);
        AddSpecificConfig(selProj, projectName, projectPath);

        SetCommonPchSettings(selProj);

        //
        SetupFilters(selProj);

        SetResDlgFont();

        AddFilesToProjectWithInfFile(selProj, projectName);

        SetCommonPchSettings(selProj);

        selProj.Object.Save();

        SetAllConfigCharset(selProj, /*unicode*/ true);

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
function CreateQtProject(strProjectName, strProjectPath) {
    try {
        var templatePath = wizard.FindSymbol("TEMPLATES_PATH");
        var strProjTemplatePath = wizard.FindSymbol("PROJECT_TEMPLATE_PATH");
        var strProjTemplate = templatePath + "\\default_140.vcxproj";

        var Solution = dte.Solution;
        var strSolutionName = "";
        if (wizard.FindSymbol("CLOSE_SOLUTION")) {
            Solution.Close();
            strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
            if (strSolutionName.length) {
                var strSolutionPath = strProjectPath.substr(0, strProjectPath.length - strProjectName.length);
                Solution.Create(strSolutionPath, strSolutionName);
            }
        }

        var strProjectNameWithExt = strProjectName + ".vcxproj";
        var oTarget = wizard.FindSymbol("TARGET");
        var oProj;
        if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject) // vsWizardAddSubProject
        {
            var prjItem = oTarget.AddFromTemplate(strProjTemplate, strProjectPath + "\\" + strProjectNameWithExt);
            oProj = prjItem.SubProject;
        } else {
            oProj = oTarget.AddFromTemplate(strProjTemplate, strProjectPath, strProjectNameWithExt);
        }
        return oProj;
    } catch (e) {
        throw e;
    }
}

function AddSpecificConfig(proj, strProjectName, strProjectPath) {
    try {
        var configs = proj.Object.Configurations;
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
            additionalIncludeDirectories += ".\\GeneratedFiles";
            additionalIncludeDirectories += ";.\\GeneratedFiles\\$(ConfigurationName)";
            additionalIncludeDirectories += ";.";
            additionalIncludeDirectories += ";$(QTDIR)\\include";
            additionalIncludeDirectories += ";$(QTDIR)\\include\\QtCore";
            additionalIncludeDirectories += ";$(QTDIR)\\include\\QtGui";
            additionalIncludeDirectories += ";$(QTDIR)\\include\\QtWidgets";
            additionalIncludeDirectories += ";$(JFRAME_DIR)\\include";
            additionalIncludeDirectories += ";$(JFRAME_DIR)\\include\\3rdpart";
            additionalIncludeDirectories += ";$(JFRAME_DIR)\\include\\core";
            compilerTool.AdditionalIncludeDirectories = additionalIncludeDirectories;

            //
            var preProcDefines = compilerTool.PreprocessorDefinitions;
            if (preProcDefines != "") {
                preProcDefines += ";";
            }
            if (bDebug) {
                //
            } else {
                preProcDefines += "QT_DLL";
                preProcDefines += ";QT_NO_DEBUG";
                preProcDefines += ";";
            }
            preProcDefines += GetPlatformDefine(config);
            preProcDefines += ";QT_CORE_LIB";
            preProcDefines += ";QT_GUI_LIB";
            preProcDefines += ";QT_WIDGETS_LIB";
            preProcDefines += ";QTWINMIGRATE_LIB";
            preProcDefines += ";JWT_LIB";
            preProcDefines += ";TINYXML_LIB";
            preProcDefines += ";JFRAME_FACADE_LIB";
            preProcDefines += ";JFRAME_FACTORY_LIB";
            preProcDefines += ";JFRAME_KERNEL_LIB";
            preProcDefines += ";QT_WIDGETS_LIB";
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
            additionalLibraryDirectories += "$(QTDIR)\\lib";
            additionalLibraryDirectories += ";$(JFRAME_DIR)\\lib";
            additionalLibraryDirectories += ";$(JFRAME_DIR)\\lib\\3rdpart";
            additionalLibraryDirectories += ";$(JFRAME_DIR)\\lib\\core";
            linkerTool.AdditionalLibraryDirectories = additionalLibraryDirectories;

            //
            var additionalDependencies = linkerTool.AdditionalDependencies;
            if (additionalDependencies != "") {
                additionalDependencies += "\r\n";
            }
            if (bDebug) {
                additionalDependencies += "qtmaind.lib";
                additionalDependencies += "\r\nQt5Cored.lib";
                additionalDependencies += "\r\nQt5Guid.lib";
                additionalDependencies += "\r\nQt5Widgetsd.lib";
            } else {
                additionalDependencies += "qtmain.lib";
                additionalDependencies += "\r\nQt5Core.lib";
                additionalDependencies += "\r\nQt5Gui.lib";
                additionalDependencies += "\r\nQt5Widgets.lib";
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
        if (sourceName == "component.h") {
            targetName = wizard.FindSymbol("HEADER_FILE_NAME");
        } else if (sourceName == "component.cpp") {
            targetName = wizard.FindSymbol("SOURCE_FILE_NAME");
        } else if (sourceName == "component.xml") {
            targetName = wizard.FindSymbol("COMPONENT_NAME") + ".xml";
        } else if (sourceName == "widget.h") {
            targetName = wizard.FindSymbol("UI_HEADER_FILE_NAME");
        } else if (sourceName == "widget.cpp") {
            targetName = wizard.FindSymbol("UI_SOURCE_FILE_NAME");
        } else if (sourceName == "stdafx.h") {
            targetName = "stdafx.h";
        } else if (sourceName == "stdafx.cpp") {
            targetName = "stdafx.cpp";
        } else if (sourceName == "targetver.h") {
            targetName = "targetver.h";
        } else if (sourceName == "dllmain.cpp") {
            targetName = "dllmain.cpp";
        } else if (sourceName == "Resource.h") {
            targetName = "Resource.h";
        } else if (sourceName == "component.rc") {
            targetName = wizard.FindSymbol("PROJECT_NAME") + ".rc";
        } else if (sourceName == "res/component.rc2") {
            targetName = "res/" + wizard.FindSymbol("PROJECT_NAME") + ".rc2";
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