 
function Component()
{
    console.log("Installing component page");
    installer.addWizardPageItem(component, "CreateDesktopLink", QInstaller.ReadyForInstallation, 1);
    if (!installer.isUpdater()) installer.addWizardPageItem(component, "AssociateFileSuffixes", QInstaller.ReadyForInstallation, 0);
}

function isCheckboxPresentAndChecked (checkboxId) {
    var page = gui.pageById(QInstaller.ReadyForInstallation);
    if (!page) return false;
    var checkbox = gui.findChild(page, checkboxId);
    return checkbox && checkbox.checked;
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        var iconId = 0;
        var programPath = installer.value("TargetDir") + "\\molsketch.exe";
        var maintenancePath = installer.value("TargetDir") + "\\maintenancetool.exe";

        var createDesktopShortcut = isCheckboxPresentAndChecked("createDesktopShortcutCheckbox");
        var associateMskFiles = isCheckboxPresentAndChecked("associateMskFilesCheckbox");
        var associateMsmFiles = isCheckboxPresentAndChecked("associateMsmFilesCheckbox");

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
                                   "@TargetDir@\\molsketch-molecule.ico",
                                   "ProgId=Molsketch-Molecule");
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
        component.addOperation("CreateShortcut",
                               maintenancePath,
                               "@StartMenuDir@/Molsketch installer.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=@TargetDir@\\molsketch.ico");
    }
}
