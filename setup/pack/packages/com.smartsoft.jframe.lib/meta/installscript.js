function Component() {

}

Component.prototype.createOperations = function() {
    // call default implementation to actually install application.exe!
    component.createOperations()

    //
    if (systemInfo.productType === "windows") {
        //
        component.addOperation(
            "EnvironmentVariable",
            "JFRAME_DIR",
            "@TargetDir@\\jframework")
    }
}

Component.prototype.createOperationsForArchive = function(archive) {
    component.addOperation("Extract", archive, "@TargetDir@")
}