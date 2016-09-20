// 
function UnderScoreCaseToCamelCase(name) {
    //
    if ((typeof name) !== "string") {
        return "";
    }
    //
    var arr = name.split('_');
    for (var i = 0; i < arr.length; ++i) {
        arr[i] = arr[i].charAt(0).toUpperCase() + arr[i].substring(1);
    }
    return arr.join('');
}

//
function CreateTabArray() {
    return [
        Default, //
        modules, //
        component_class, //
        component_ui, //
        summary //
    ];
}

/**
 * Common.js is a script file which contains the helper functions
 * used by the wizards.  Include this line if you want to
 * call these functions from your html scripts.
 */
(function AddJSCommonScripts() {
    document.scripts("INCLUDE_SCRIPT").src =
        window.external.FindSymbol("SCRIPT_COMMON_PATH") + "/Script.js";
    document.scripts("INCLUDE_COMMON").src =
        window.external.FindSymbol("SCRIPT_COMMON_PATH") + "/Common.js";
})();

//
function AddQtModules() {
    //
    var strModules = "";

    // Essentials
    if (window.external.FindSymbol('THREED_MODULE'))
        strModules += "3D, ";
    if (window.external.FindSymbol('CORE_MODULE'))
        strModules += "Core, ";
    if (window.external.FindSymbol('GUI_MODULE'))
        strModules += "GUI, ";
    if (window.external.FindSymbol('LOCATION_MODULE'))
        strModules += "Location, ";
    if (window.external.FindSymbol('MULTIMEDIA_MODULE'))
        strModules += "Multimedia, ";
    if (window.external.FindSymbol('MULTIMEDIAWIDGETS_MODULE'))
        strModules += "Multimedia Widgets, ";
    if (window.external.FindSymbol('NETWORK_MODULE'))
        strModules += "Network, ";
    if (window.external.FindSymbol('QML_MODULE'))
        strModules += "Qml, ";
    if (window.external.FindSymbol('QUICK_MODULE'))
        strModules += "Quick, ";
    if (window.external.FindSymbol('SQL_MODULE'))
        strModules += "SQL, ";
    if (window.external.FindSymbol('TEST_MODULE'))
        strModules += "Test, ";
    if (window.external.FindSymbol('WEBKIT_MODULE'))
        strModules += "WebKit, ";

    // Add-ons
    // Active Qt better split to server and container
    if (window.external.FindSymbol('AQSERVER_MODULE'))
        strModules += "ActiveQt Server, ";
    if (window.external.FindSymbol('AQCONTAINER_MODULE'))
        strModules += "ActiveQt Container, ";

    if (window.external.FindSymbol('BLUETOOTH_MODULE'))
        strModules += "Bluetooth, ";
    if (window.external.FindSymbol('CONCURRENT_MODULE'))
        strModules += "Concurrent, ";
    if (window.external.FindSymbol('DECLARATIVE_MODULE'))
        strModules += "Declarative, ";
    if (window.external.FindSymbol('ENGINIO_MODULE'))
        strModules += "Enginio, ";
    if (window.external.FindSymbol('HELP_MODULE'))
        strModules += "Help, ";
    if (window.external.FindSymbol('NFC_MODULE'))
        strModules += "NFC, ";
    if (window.external.FindSymbol('OPENGL_MODULE'))
        strModules += "OpenGL, ";
    if (window.external.FindSymbol('POSITIONING_MODULE'))
        strModules += "Positioning, ";
    if (window.external.FindSymbol('PRINTSUPPORT_MODULE'))
        strModules += "Print Support, ";
    if (window.external.FindSymbol('QUICKWIDGETS_MODULE'))
        strModules += "Quick Widgets, ";
    if (window.external.FindSymbol('SCRIPT_MODULE'))
        strModules += "Script, ";
    if (window.external.FindSymbol('SCRIPTTOOLS_MODULE'))
        strModules += "Script Tools, ";
    if (window.external.FindSymbol('SENSORS_MODULE'))
        strModules += "Sensors, ";
    if (window.external.FindSymbol('SERIALPORT_MODULE'))
        strModules += "Serial Port, ";
    if (window.external.FindSymbol('SVG_MODULE'))
        strModules += "SVG, ";
    if (window.external.FindSymbol('UITOOLS_MODULE'))
        strModules += "UI Tools, ";
    if (window.external.FindSymbol('WEBCHANNEL_MODULE'))
        strModules += "Web Channel, ";
    if (window.external.FindSymbol('WEBKITWIDGETS_MODULE'))
        strModules += "Webkit Widgets, ";
    if (window.external.FindSymbol('WEBSOCKETS_MODULE'))
        strModules += "Web Sockets, ";
    if (window.external.FindSymbol('WIDGETS_MODULE'))
        strModules += "Widgets, ";
    if (window.external.FindSymbol('WINDOWSEXTRAS_MODULE'))
        strModules += "Windows Extras, ";
    if (window.external.FindSymbol('XML_MODULE'))
        strModules += "XML, ";
    if (window.external.FindSymbol('XMLPATTERNS_MODULE'))
        strModules += "XML Patterns, ";

    if (strModules.length < 1)
        strModules = "None selected";
    else
        strModules = strModules.slice(0, strModules.length - 2);

    QTMODULES.innerText = strModules;
}

//
function SetPageComponentUiVisible() {
    //
    var spans = document.all.tags("span");
    var pageComponentUi = spans.item('component_ui');
    //
    var linkComponentUi = document.getElementById('LINK_COMPONENT_UI');
    var includeIJComponentUi = window.external.FindSymbol('INCLUDE_IJCOMPONENTUI');
    if (includeIJComponentUi) {
        linkComponentUi.style.visibility = "visible";
        linkComponentUi.style.display = "";
        pageComponentUi.disabled = false;
    } else {
        linkComponentUi.style.visibility = "hidden";
        linkComponentUi.style.display = "none";
        pageComponentUi.disabled = true;
    }
}

//
function GetFriendlyProjectName() {
    var regexp = /\s/g;
    var projectName = window.external.FindSymbol('PROJECT_NAME').replace(regexp, "");
    var friendlyProjectName = CreateSafeName(projectName);
    return friendlyProjectName;
}