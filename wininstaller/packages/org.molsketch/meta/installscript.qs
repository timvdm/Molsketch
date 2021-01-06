 
function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        var iconId = 0;
        var programPath = installer.value("TargetDir") + "\\molsketch-qt5.exe";
        component.addOperation("RegisterFileType",
                               "msk",
                               programPath + " '%1'",
                               "Molsketch Drawing",
                               "application/x-molsketch",
                               programPath + "," + iconId,
                               "ProgId=Molsketch");
        component.addOperation("RegisterFileType",
                               "msm",
                               programPath + " '%1'",
                               "Molsketch Molecule",
                               "application/x-molsketch-molecule",
                               programPath + "," + iconId,
                               "ProgId=Molsketch");
    }
}
