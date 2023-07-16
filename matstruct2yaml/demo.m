addpath('C:\Users\yiyan5ez\Desktop\PrescanSimcppGenerator\matstruct2yaml');
savepath;
file = prescan.experiment.getDefaultFilename;
xp = prescan.api.experiment.loadExperimentFromFile(file);
xp_struct = xp.getAsMatlabStruct;
yaml.dumpFile("pcs.yaml", xp_struct.cameramodel);