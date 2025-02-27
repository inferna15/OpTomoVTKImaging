#include "Model.h"

Model::Model(const char* path) {
	dicomPath = path;
}

void* Model::GetWindow() {
	renderWindow->InitializeFromCurrentContext();
	return renderWindow->GetGenericWindowId();
}

void Model::SetWindowSize(int height, int width) {
	renderWindow->SetSize(width, height);
	this->width = width;
	this->height = height;
}

vtkAlgorithmOutput* Model::readDicom(const char* path) {
	dicomReader->SetDirectoryName(path);
	dicomReader->Update();

	dicomReader->GetDataExtent(extent);
	dicomReader->GetDataSpacing(spacing);

	realSizeOfX = extent[1] - extent[0];
	realSizeOfY = extent[3] - extent[2];
	realSizeOfZ = extent[5] - extent[4];

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
	// Main Axial Pipeline
	resliceAxial = vtkSmartPointer<vtkImageReslice>::New();
	resliceAxial->SetInputConnection(input);
	resliceAxial->SetOutputDimensionality(2);
	resliceAxial->SetResliceAxesDirectionCosines(1, 0, 0, 0, 0, 1, 0, 1, 0);
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

	// Çizgiler
	linesSagittal[0] = vtkSmartPointer<vtkLineSource>::New();
	linesFrontal[0] = vtkSmartPointer<vtkLineSource>::New();
	rendererAxial->AddActor(run2DLinePipeline(linesSagittal[0]->GetOutputPort()));
	rendererAxial->AddActor(run2DLinePipeline(linesFrontal[0]->GetOutputPort()));

	renderWindow->AddRenderer(rendererAxial);
}

void Model::runSagittalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceSagittal = vtkSmartPointer<vtkImageReslice>::New();
	resliceSagittal->SetInputConnection(input);
	resliceSagittal->SetOutputDimensionality(2);
	resliceSagittal->SetResliceAxesDirectionCosines(0, 0, 1, 0, 1, 0, 1, 0, 0);
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

	// Çizgiler
	linesAxial[1] = vtkSmartPointer<vtkLineSource>::New();
	linesFrontal[1] = vtkSmartPointer<vtkLineSource>::New();
	rendererSagittal->AddActor(run2DLinePipeline(linesAxial[1]->GetOutputPort()));
	rendererSagittal->AddActor(run2DLinePipeline(linesFrontal[1]->GetOutputPort()));

	renderWindow->AddRenderer(rendererSagittal);
}

void Model::runFrontalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceFrontal = vtkSmartPointer<vtkImageReslice>::New();
	resliceFrontal->SetInputConnection(input);
	resliceFrontal->SetOutputDimensionality(2);
	resliceFrontal->SetResliceAxesDirectionCosines(1, 0, 0, 0, 1, 0, 0, 0, 1);
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

	// Çizgiler
	linesAxial[0] = vtkSmartPointer<vtkLineSource>::New();
	linesSagittal[1] = vtkSmartPointer<vtkLineSource>::New();
	rendererSagittal->AddActor(run2DLinePipeline(linesAxial[0]->GetOutputPort()));
	rendererSagittal->AddActor(run2DLinePipeline(linesSagittal[1]->GetOutputPort()));

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

	// Çizgiler
	for (int i = 0; i < 4; i++) {
		linesAxialVolume[i] = vtkSmartPointer<vtkLineSource>::New();
		linesSagittalVolume[i] = vtkSmartPointer<vtkLineSource>::New();
		linesFrontalVolume[i] = vtkSmartPointer<vtkLineSource>::New();
		rendererVolume->AddActor(run3DLinePipeline(linesAxialVolume[i]->GetOutputPort()));
		rendererVolume->AddActor(run3DLinePipeline(linesSagittalVolume[i]->GetOutputPort()));
		rendererVolume->AddActor(run3DLinePipeline(linesFrontalVolume[i]->GetOutputPort()));
	}
	lineAxialAndSagittalVolume = vtkSmartPointer<vtkLineSource>::New();
	lineAxialAndFrontalVolume = vtkSmartPointer<vtkLineSource>::New();
	lineFrontalAndSagittalVolume = vtkSmartPointer<vtkLineSource>::New();
	rendererVolume->AddActor(run3DLinePipeline(lineAxialAndSagittalVolume->GetOutputPort()));
	rendererVolume->AddActor(run3DLinePipeline(lineAxialAndFrontalVolume->GetOutputPort()));
	rendererVolume->AddActor(run3DLinePipeline(lineFrontalAndSagittalVolume->GetOutputPort()));

	renderWindow->AddRenderer(rendererVolume);
}

void Model::setSizeOfSlices() {
	if (isMax) {
		slicePanelHeight = height;
		slicePanelWidth = width;
	}
	else {
		slicePanelHeight = height / 2;
		slicePanelWidth = width / 2;
	}
	centerPointOfSliceOnX = slicePanelWidth / 2;
	centerPointOfSliceOnY = slicePanelHeight / 2;

	// Axial
	if (realSizeOfX / slicePanelWidth >= realSizeOfZ / slicePanelHeight) {
		ratio[1] = slicePanelWidth / realSizeOfX;
	}
	else {
		ratio[1] = slicePanelHeight / realSizeOfZ;
	}
	resliceAxial->SetOutputExtent(
		centerPointOfSliceOnX + (ratio[1] * zoom[1] * motion[0]) - (zoom[1] * centerPointOfSliceOnX), 
		centerPointOfSliceOnX + (ratio[1] * zoom[1] * motion[0]) + (zoom[1] * centerPointOfSliceOnX), 
		centerPointOfSliceOnY + (ratio[1] * zoom[1] * motion[2]) - (zoom[1] * centerPointOfSliceOnY),
		centerPointOfSliceOnY + (ratio[1] * zoom[1] * motion[2]) + (zoom[1] * centerPointOfSliceOnY),
		0, 0
	);
	resliceAxial->SetOutputSpacing(
		spacing[0] / (ratio[1] * zoom[1]),
		spacing[1] / (ratio[1] * zoom[1]),
		spacing[2] / (ratio[1] * zoom[1])
	);

	// Sagittal
	if (realSizeOfZ / slicePanelWidth >= realSizeOfY / slicePanelHeight) {
		ratio[0] = slicePanelWidth / realSizeOfZ;
	}
	else {
		ratio[0] = slicePanelHeight / realSizeOfY;
	}
	resliceSagittal->SetOutputExtent(
		centerPointOfSliceOnX + (ratio[0] * zoom[0] * motion[2]) - (zoom[0] * centerPointOfSliceOnX),
		centerPointOfSliceOnX + (ratio[0] * zoom[0] * motion[2]) + (zoom[0] * centerPointOfSliceOnX),
		centerPointOfSliceOnY + (ratio[0] * zoom[0] * motion[1]) - (zoom[0] * centerPointOfSliceOnY),
		centerPointOfSliceOnY + (ratio[0] * zoom[0] * motion[1]) + (zoom[0] * centerPointOfSliceOnY),
		0, 0
	);
	resliceSagittal->SetOutputSpacing(
		spacing[0] / (ratio[0] * zoom[0]),
		spacing[1] / (ratio[0] * zoom[0]),
		spacing[2] / (ratio[0] * zoom[0])
	);

	// Frontal
	if (realSizeOfX / slicePanelWidth >= realSizeOfY / slicePanelHeight) {
		ratio[2] = slicePanelWidth / realSizeOfX;
	}
	else {
		ratio[2] = slicePanelHeight / realSizeOfY;
	}
	resliceFrontal->SetOutputExtent(
		centerPointOfSliceOnX + (ratio[2] * zoom[2] * motion[0]) - (zoom[2] * centerPointOfSliceOnX),
		centerPointOfSliceOnX + (ratio[2] * zoom[2] * motion[0]) + (zoom[2] * centerPointOfSliceOnX),
		centerPointOfSliceOnY + (ratio[2] * zoom[2] * motion[1]) - (zoom[2] * centerPointOfSliceOnY),
		centerPointOfSliceOnY + (ratio[2] * zoom[2] * motion[1]) + (zoom[2] * centerPointOfSliceOnY),
		0, 0
	);
	resliceFrontal->SetOutputSpacing(
		spacing[0] / (ratio[2] * zoom[2]),
		spacing[1] / (ratio[2] * zoom[2]),
		spacing[2] / (ratio[2] * zoom[2])
	);
}

vtkSmartPointer<vtkActor2D> Model::run2DLinePipeline(vtkAlgorithmOutput* input) {
	vtkSmartPointer<vtkPolyDataMapper2D> lineMapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
	vtkSmartPointer<vtkActor2D> lineActor = vtkSmartPointer<vtkActor2D>::New();
	lineMapper->SetInputConnection(input);
	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetLineWidth(1);
	lineActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
	return lineActor;
}

vtkSmartPointer<vtkActor> Model::run3DLinePipeline(vtkAlgorithmOutput* input) {
	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineMapper->SetInputConnection(input);
	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetLineWidth(1);
	lineActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
	return lineActor;
}

void Model::runMainPipeline() {
	dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkAlgorithmOutput* outputOfReader = readDicom(dicomPath);

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