function GetNameFromFile(strFile) {
    var nPos = strFile.lastIndexOf(".");
    return strFile.substr(0, nPos);
}

function OnFinish(selProj, selObj) {
    try {
        // vs
        var projectPath = wizard.FindSymbol("PROJECT_PATH");
        var projectName = wizard.FindSymbol("PROJECT_NAME");
        var solutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
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
        // component_class
        var componentClassName = wizard.FindSymbol("COMPONENT_CLASS_NAME");
        var headerFileName = wizard.FindSymbol("HEADER_FILE_NAME");
        var sourceFileName = wizard.FindSymbol("SOURCE_FILE_NAME");
        var precompiled = true; //wizard.FindSymbol("PRECOMPILED");
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
        var index = 0;
        try {
            index = 0; //["QWidget", "QDialog", "QMainWindow"].indexOf("QMainWindow");
        } catch (e) {
            var msg = "name: " + e.name + "\n" +
                "message: " + e.message + "\n" +
                "description: " + e.description;
            wizard.ReportError("??? => \n" + msg);
        }
        wizard.AddSymbol("BASECLASS_INSTANCEOF_QOBJECT", (index >= 0));

        wizard.AddSymbol("UI_BASE_CLASS_NAME_IS_EMPTY", uiBaseClassName === "");
        wizard.AddSymbol("UI_BASE_CLASS_IS_QOBJECT", uiBaseClassName === "QObject");
        wizard.AddSymbol("UI_BASE_CLASS_INERITS_QWIDGET", uiBaseClassName === "QWidget" || uiBaseClassName === "QDialog" || uiBaseClassName === "QMainWindow");

        //////////

        try {
            selProj = CreateProject(projectName, projectPath);
            selProj.Object.Keyword = "Qt4VS";
        } catch (e) {
            var msg = "name: " + e.name + "\n" +
                "message: " + e.message + "\n" +
                "description: " + e.description;
            wizard.ReportError("xxx => \n" + msg);
        }

        AddCommonConfig(selProj, projectName);

        //
        SetupFilters(selProj);

        SetResDlgFont();

        AddFilesToProjectWithInfFile(selProj, projectName);

        SetCommonPchSettings(selProj);

        try {
            selProj.Object.Save();
        } catch (e) {
            var msg = "name: " + e.name + "\n" +
                "message: " + e.message + "\n" +
                "description: " + e.description;
            wizard.ReportError("yyy => \n" + msg);
        }

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