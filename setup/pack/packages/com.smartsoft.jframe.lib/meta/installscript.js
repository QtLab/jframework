function Component() {
    installer.installationFinished.connect(this, Component.prototype.installationFinished)
}

Component.prototype.createOperations = function() {
    // call default implementation to actually install application!
    component.createOperations()

    try {
        //
        if (systemInfo.productType === "windows") {
            //
            component.addOperation(
                "EnvironmentVariable",
                "JFRAME_DIR",
                "@TargetDir@\\jframework")
        } else if (systemInfo.productType === "neokylin") {
            /*
            component.addOperation(
                "GlobalConfig",
                "/etc/environment",
                "JFRAME_DIR",
                "@TargetDir@/jframework"
            );
            */
            component.addOperation(
                "LineReplace",
                "/etc/environment",
                "JFRAME_DIR=",
                "JFRAME_DIR=@TargetDir@/jframework"
            );
            //
            //component.addOperation("Execute", "/bin/bash", "source /etc/environment");
        }
    } catch (e) {
        console.log(e)
    }
}

Component.prototype.createOperationsForArchive = function(archive) {
    component.addOperation("Extract", archive, "@TargetDir@")
}

Component.prototype.installationFinished = function() {
    try {
        if (installer.isInstalled() && installer.status == QInstaller.Success) {
            if (systemInfo.productType === "windows") {
                //
                console.log("***** !!!!!!")
            } else if (systemInfo.productType === "neokylin") {
                //
                console.log("===== !!!!!!")
            }
        }
    } catch (e) {
        console.log(e)
    }
}
