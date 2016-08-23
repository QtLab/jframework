function Component() {
    installer.installationFinished.connect(this, Component.prototype.installationFinished)
}

Component.prototype.createOperations = function() {
    // call default implementation to actually install application.exe!
    component.createOperations();

    //
    if (systemInfo.productType === "windows") {
        //
        component.addOperation(
            "GlobalConfig",
            "HKEY_CURRENT_USER\\Software\\@Publisher@\\@Name@\\@Version@",
            "InstallPath",
            "@TargetDir@\\jframe")
    }
}

Component.prototype.createOperationsForArchive = function(archive) {
    //
    component.addOperation("Extract", archive, "@TargetDir@/jframe/component")
}

Component.prototype.installationFinished = function() {
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            if (systemInfo.productType === "windows") {
                //
                console.log("!!!*****")
            } else {
                console.log("Not supported os!")
            }
        }
    } catch (e) {
        console.log(e)
    }
}

