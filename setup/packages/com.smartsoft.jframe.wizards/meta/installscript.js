function Component() {

}

Component.prototype.createOperations = function() {
    // call default implementation to actually install application.exe!
    component.createOperations();

    //
    if (systemInfo.productType === "windows") {
        //
    }
}

Component.prototype.createOperationsForArchive = function(archive) {
    //
    if (systemInfo.productType === "windows") {
        //
    } else if (systemInfo.productType === "unix") {
        component.addOperation("Extract", archive, "@TargetDir@/wizards")
    }
}