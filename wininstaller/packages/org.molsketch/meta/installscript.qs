 
function Component()
{
    console.log("Installing component page");

    installer.addWizardPageItem(component, "PostInstallActions", QInstaller.StartMenuSelection);
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        var iconId = 0;
        var programPath = installer.value("TargetDir") + "\\molsketch-qt5.exe";

        var startMenuPage = gui.pageById(QInstaller.StartMenuSelection);
        var createDesktopShortcut = gui.findChild(startMenuPage, "createDesktopShortcutCheckbox").checked;
        var associateMskFiles = gui.findChild(startMenuPage, "associateMskFilesCheckbox").checked;
        var associateMsmFiles = gui.findChild(startMenuPage, "associateMsmFilesCheckbox").checked;

        console.log("Create desktop link: " + createDesktopShortcut +
                    " Associate MSK files: " + associateMskFiles +
                    " Associate MSM files: " + associateMsmFiles);

        if (associateMskFiles)
            component.addOperation("RegisterFileType",
                                   "msk",
                                   "\"" + programPath + "\" \"%1\"",
                                   "Molsketch Drawing",
                                   "application/x-molsketch",
                                   programPath + "," + iconId,
                                   "ProgId=Molsketch");
        if (associateMsmFiles)
            component.addOperation("RegisterFileType",
                                   "msm",
                                   "\"" + programPath + "\" \"%1\"",
                                   "Molsketch Molecule",
                                   "application/x-molsketch-molecule",
                                   programPath + "," + iconId,
                                   "ProgId=Molsketch");
        if (createDesktopShortcut)
            component.addOperation("CreateShortcut",
                                   programPath,
                                   "@DesktopDir@/Molsketch.lnk",
                                   "workingDirectory=@TargetDir@",
                                   "iconPath=@TargetDir@\\molsketch.ico");
        component.addOperation("CreateShortcut",
                               programPath,
                               "@StartMenuDir@/Molsketch.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=@TargetDir@\\molsketch.ico");
        // TODO Add uninstall link
    }
}
