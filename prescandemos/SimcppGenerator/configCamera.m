file = prescan.experiment.getDefaultFilename;
prescan.experiment.convertPexToDataModels;

xp = prescan.api.experiment.loadExperimentFromFile(file);
cameras = prescan.api.camera.getCameraSensors(xp);

for i=1:length(cameras)
    cameras(i).imageFormat = 'BGRU8';
end

xp.saveToFile(file);