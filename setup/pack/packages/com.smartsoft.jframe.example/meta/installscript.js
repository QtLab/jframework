function Component() {

}

Component.prototype.createOperatioins = function() {
    // call default implementation to actually install application.exe!
    component.createOperatioins()

    //
    if (systemInfo.productType == "windows") {
        //
        component.addOperation(
            "CreateShortcut",
            "@TargetDir@/jframework/bin/application.exe",
            "@StartMenuDir@/application.lnk",
            "workingDirectory=@TargetDir@/jframework/bin/",
            "iconPath=@TargetDir@/jframework/bin/application.exe",
            "iconId=0")
    }
}

Component.prototype.createOperationsForArchive = function(archive) {
    //
    component.addOperation("Extract", archive, "@TargetDir@/jframework")
}