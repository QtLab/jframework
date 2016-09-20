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
function UpdateDefaultSymbols() {
    //
    var projectPath = window.external.FindSymbol("PROJECT_PATH");
    var projectName = window.external.FindSymbol("PROJECT_NAME");
    var solutionPath = projectPath.substr(0, projectPath.length - projectName.length);
    projectPath = solutionPath + "\\source\\framework\\" + projectName;
    window.external.AddSymbol("SOLUTION_PATH", solutionPath);
    window.external.AddSymbol("PROJECT_PATH", projectPath);
}

//
function CreateTabArray() {
    return [
        Default, //
        modules, //
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
function GetFriendlyProjectName() {
    var regexp = /\s/g;
    var projectName = window.external.FindSymbol('PROJECT_NAME').replace(regexp, "");
    var friendlyProjectName = CreateSafeName(projectName);
    return friendlyProjectName;
}