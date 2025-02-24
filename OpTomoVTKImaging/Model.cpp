#include "Model.h"

vtkAlgorithmOutput* Model::readDicom(const char* path) {
	dicomReader->SetDirectoryName(path);
	dicomReader->Update();

	dicomReader->GetDataExtent(extent);
	dicomReader->GetDataSpacing(spacing);

	double doubleRange[2];
	dicomReader->GetOutput()->GetScalarRange(doubleRange);
	range[0] = (int)doubleRange[0];
	range[1] = (int)doubleRange[1];
	windowColorValue = range[1] - range[0];
	levelColorValue = windowColorValue / 2;

	return dicomReader->GetOutputPort();
}

void Model::setColorFunction(int min, int max) {
	colorFunction->RemoveAllPoints();
	colorFunction->AddRGBPoint(min, 0, 0, 0);
	colorFunction->AddRGBPoint(max, 1, 1, 1);
}

void Model::setOpacityFunction(int min, int max) {
	opacityFunction->RemoveAllPoints();
	opacityFunction->AddPoint(range[0], 0);
	opacityFunction->AddPoint(range[1], 1);
}

void Model::setLayerToDefault() {
	layer[0] = (extent[1] - extent[0]) / 2;
	layer[1] = (extent[3] - extent[2]) / 2;
	layer[2] = (extent[5] - extent[4]) / 2;
}

void Model::setExtractVOI(int minX, int maxX, int minY, int maxY, int minZ, int maxZ) {
	extractVOI->SetVOI(minX, maxX, minY, maxY, minZ, maxZ);
}

void Model::runAxialSlicePipeline(vtkAlgorithmOutput* input) {
	resliceAxial = vtkSmartPointer<vtkImageReslice>::New();
	resliceAxial->SetInputConnection(input);
	resliceAxial->SetOutputDimensionality(2);
	resliceAxial->SetResliceAxesDirectionCosines(1, 0, 0, 0, 1, 0, 0, 0, 1);
	resliceAxial->SetResliceAxesOrigin(0, layer[1] * spacing[1], 0);
	resliceAxial->SetInterpolationModeToCubic();

	colorAxial = vtkSmartPointer<vtkImageMapToColors>::New();
	colorAxial->SetInputConnection(resliceAxial->GetOutputPort());
	colorAxial->SetLookupTable(colorFunction);
	colorAxial->Update();

	mapperAxial = vtkSmartPointer<vtkImageMapper>::New();
	mapperAxial->SetInputConnection(colorAxial->GetOutputPort());
	mapperAxial->SetColorWindow(windowColorValue);
	mapperAxial->SetColorLevel(levelColorValue);

	actorAxial = vtkSmartPointer<vtkActor2D>::New();
	actorAxial->SetMapper(mapperAxial);

	rendererAxial = vtkSmartPointer<vtkRenderer>::New();
	rendererAxial->AddActor(actorAxial);
	rendererAxial->SetBackground(0, 0, 0);
	rendererAxial->SetViewport(0, 0.5, 0.5, 1);

	renderWindow->AddRenderer(rendererAxial);
}

void Model::runSagittalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceSagittal = vtkSmartPointer<vtkImageReslice>::New();
	resliceSagittal->SetInputConnection(input);
	resliceSagittal->SetOutputDimensionality(2);
	resliceSagittal->SetResliceAxesDirectionCosines(0, 0, -1, 0, 1, 0, 1, 0, 0);
	resliceSagittal->SetResliceAxesOrigin(layer[0] * spacing[0], 0, 0);
	resliceSagittal->SetInterpolationModeToCubic();

	colorSagittal = vtkSmartPointer<vtkImageMapToColors>::New();
	colorSagittal->SetInputConnection(resliceSagittal->GetOutputPort());
	colorSagittal->SetLookupTable(colorFunction);
	colorSagittal->Update();

	mapperSagittal = vtkSmartPointer<vtkImageMapper>::New();
	mapperSagittal->SetInputConnection(colorSagittal->GetOutputPort());
	mapperSagittal->SetColorWindow(windowColorValue);
	mapperSagittal->SetColorLevel(levelColorValue);

	actorSagittal = vtkSmartPointer<vtkActor2D>::New();
	actorSagittal->SetMapper(mapperSagittal);

	rendererSagittal = vtkSmartPointer<vtkRenderer>::New();
	rendererSagittal->AddActor(actorSagittal);
	rendererSagittal->SetBackground(0, 0, 0);
	rendererSagittal->SetViewport(0.5, 0.5, 1, 1);

	renderWindow->AddRenderer(rendererSagittal);
}

void Model::runFrontalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceFrontal = vtkSmartPointer<vtkImageReslice>::New();
	resliceFrontal->SetInputConnection(input);
	resliceFrontal->SetOutputDimensionality(2);
	resliceFrontal->SetResliceAxesDirectionCosines(1, 0, 0, 0, 0, -1, 0, 1, 0);
	resliceFrontal->SetResliceAxesOrigin(0, 0, layer[2] * spacing[2]);
	resliceFrontal->SetInterpolationModeToCubic();

	colorFrontal = vtkSmartPointer<vtkImageMapToColors>::New();
	colorFrontal->SetInputConnection(resliceFrontal->GetOutputPort());
	colorFrontal->SetLookupTable(colorFunction);
	colorFrontal->Update();

	mapperFrontal = vtkSmartPointer<vtkImageMapper>::New();
	mapperFrontal->SetInputConnection(colorFrontal->GetOutputPort());
	mapperFrontal->SetColorWindow(windowColorValue);
	mapperFrontal->SetColorLevel(levelColorValue);

	actorFrontal = vtkSmartPointer<vtkActor2D>::New();
	actorFrontal->SetMapper(mapperFrontal);

	rendererFrontal = vtkSmartPointer<vtkRenderer>::New();
	rendererFrontal->AddActor(actorFrontal);
	rendererFrontal->SetBackground(0, 0, 0);
	rendererFrontal->SetViewport(0, 0, 0.5, 0.5);

	renderWindow->AddRenderer(rendererFrontal);
}

void Model::runVolume3DPipeline(vtkAlgorithmOutput* input) {
	mapperVolume = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	mapperVolume->SetInputConnection(input);
	mapperVolume->SetRequestedRenderModeToGPU();

	propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
	propertyVolume->SetColor(colorFunction);
	propertyVolume->SetScalarOpacity(opacityFunction);

	actorVolume = vtkSmartPointer<vtkVolume>::New();
	actorVolume->SetMapper(mapperVolume);
	actorVolume->SetProperty(propertyVolume);

	rendererVolume = vtkSmartPointer<vtkRenderer>::New();
	rendererVolume->AddActor(actorVolume);
	rendererVolume->SetBackground(0, 0, 0);
	rendererVolume->SetViewport(0.5, 0, 1, 0.5);

	renderWindow->AddRenderer(rendererVolume);
}

void Model::runMainPipeline(const char* path) {
	dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkAlgorithmOutput* outputOfReader = readDicom(path);

	colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	setColorFunction(range[0], range[1]);

	opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	setOpacityFunction(range[0], range[1]);

	extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputConnection(outputOfReader);
	setExtractVOI(extent[0], extent[1], extent[2], extent[3], extent[4], extent[5]);

	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	runAxialSlicePipeline(extractVOI->GetOutputPort());
	runSagittalSlicePipeline(extractVOI->GetOutputPort());
	runFrontalSlicePipeline(extractVOI->GetOutputPort());
	runVolume3DPipeline(extractVOI->GetOutputPort());
}