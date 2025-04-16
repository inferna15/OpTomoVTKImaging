#include "Model.h"

Model::Model(const char* path) {
	dicomPath = path;

	dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
	colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	resliceAxial = vtkSmartPointer<vtkImageReslice>::New();
	colorAxial = vtkSmartPointer<vtkImageMapToColors>::New();
	mapperAxial = vtkSmartPointer<vtkImageMapper>::New();
	actorAxial = vtkSmartPointer<vtkActor2D>::New();
	rendererAxial = vtkSmartPointer<vtkRenderer>::New();
	linesAxial[0] = vtkSmartPointer<vtkLineSource>::New();
	linesAxial[1] = vtkSmartPointer<vtkLineSource>::New();

	resliceSagittal = vtkSmartPointer<vtkImageReslice>::New();
	colorSagittal = vtkSmartPointer<vtkImageMapToColors>::New();
	mapperSagittal = vtkSmartPointer<vtkImageMapper>::New();
	actorSagittal = vtkSmartPointer<vtkActor2D>::New();
	rendererSagittal = vtkSmartPointer<vtkRenderer>::New();
	linesSagittal[0] = vtkSmartPointer<vtkLineSource>::New();
	linesSagittal[1] = vtkSmartPointer<vtkLineSource>::New();

	resliceFrontal = vtkSmartPointer<vtkImageReslice>::New();
	colorFrontal = vtkSmartPointer<vtkImageMapToColors>::New();
	mapperFrontal = vtkSmartPointer<vtkImageMapper>::New();
	actorFrontal = vtkSmartPointer<vtkActor2D>::New();
	rendererFrontal = vtkSmartPointer<vtkRenderer>::New();
	linesFrontal[0] = vtkSmartPointer<vtkLineSource>::New();
	linesFrontal[1] = vtkSmartPointer<vtkLineSource>::New();

	mapperVolume = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
	actorVolume = vtkSmartPointer<vtkVolume>::New();
	rendererVolume = vtkSmartPointer<vtkRenderer>::New();
	for (int i = 0; i < 4; i++) {
		linesAxialVolume[i] = vtkSmartPointer<vtkLineSource>::New();
		linesSagittalVolume[i] = vtkSmartPointer<vtkLineSource>::New();
		linesFrontalVolume[i] = vtkSmartPointer<vtkLineSource>::New();
	}
	lineAxialAndSagittalVolume = vtkSmartPointer<vtkLineSource>::New();
	lineAxialAndFrontalVolume = vtkSmartPointer<vtkLineSource>::New();
	lineFrontalAndSagittalVolume = vtkSmartPointer<vtkLineSource>::New();

	callbackCommand = vtkSmartPointer<vtkCallbackCommand>::New();
	callbackCommand->SetCallback([](vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
		Model* self = static_cast<Model*>(clientData);
		self->setPanels();
	});

	callbackCommand->SetClientData(this);
	renderWindow->AddObserver(vtkCommand::WindowResizeEvent, callbackCommand);
}

void* Model::GetWindow() {
	return renderWindow->GetGenericWindowId();
}

void Model::SetWindowSize(int height, int width) {
	renderWindow->SetSize(width, height);
	renderWindowInteractor->SetSize(width, height);
}

vtkAlgorithmOutput* Model::readDicom() {
	dicomReader->SetDirectoryName(dicomPath);
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

void Model::setExtractVOI(int minX, int maxX, int minY, int maxY, int minZ, int maxZ) {
	extractVOI->SetVOI(minX, maxX, minY, maxY, minZ, maxZ);
}

void Model::runAxialSlicePipeline(vtkAlgorithmOutput* input) {
	// Main Axial Pipeline
	resliceAxial->SetInputConnection(input);
	resliceAxial->SetOutputDimensionality(2);
	resliceAxial->SetResliceAxesDirectionCosines(1, 0, 0, 0, 0, 1, 0, -1, 0);
	resliceAxial->SetResliceAxesOrigin(layer[0] * spacing[0], layer[1] * spacing[1], layer[2] * spacing[2]);
	resliceAxial->SetInterpolationModeToLinear();

	colorAxial->SetInputConnection(resliceAxial->GetOutputPort());
	colorAxial->SetLookupTable(colorFunction);
	colorAxial->Update();

	mapperAxial->SetInputConnection(colorAxial->GetOutputPort());
	mapperAxial->SetColorWindow(windowColorValue);
	mapperAxial->SetColorLevel(levelColorValue);

	actorAxial->SetMapper(mapperAxial);

	rendererAxial->AddActor(actorAxial);
	rendererAxial->SetBackground(0, 0, 0);
	rendererAxial->SetViewport(0, 0.5, 0.5, 1);

	// Çizgiler
	rendererAxial->AddActor(run2DLinePipeline(linesSagittal[0]->GetOutputPort()));
	rendererAxial->AddActor(run2DLinePipeline(linesFrontal[0]->GetOutputPort()));
	set2DAxialLines();

	renderWindow->AddRenderer(rendererAxial);
}

void Model::runSagittalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceSagittal->SetInputConnection(input);
	resliceSagittal->SetOutputDimensionality(2);
	resliceSagittal->SetResliceAxesDirectionCosines(0, 0, 1, 0, 1, 0, 1, 0, 0);
	resliceSagittal->SetResliceAxesOrigin(layer[0] * spacing[0], layer[1] * spacing[1], layer[2] * spacing[2]);
	resliceSagittal->SetInterpolationModeToLinear();

	colorSagittal->SetInputConnection(resliceSagittal->GetOutputPort());
	colorSagittal->SetLookupTable(colorFunction);
	colorSagittal->Update();

	mapperSagittal->SetInputConnection(colorSagittal->GetOutputPort());
	mapperSagittal->SetColorWindow(windowColorValue);
	mapperSagittal->SetColorLevel(levelColorValue);

	actorSagittal->SetMapper(mapperSagittal);

	rendererSagittal->AddActor(actorSagittal);
	rendererSagittal->SetBackground(0, 0, 0);
	rendererSagittal->SetViewport(0.5, 0.5, 1, 1);

	// Çizgiler
	rendererSagittal->AddActor(run2DLinePipeline(linesAxial[1]->GetOutputPort()));
	rendererSagittal->AddActor(run2DLinePipeline(linesFrontal[1]->GetOutputPort()));
	set2DSagittalLines();

	renderWindow->AddRenderer(rendererSagittal);
}

void Model::runFrontalSlicePipeline(vtkAlgorithmOutput* input) {
	resliceFrontal->SetInputConnection(input);
	resliceFrontal->SetOutputDimensionality(2);
	resliceFrontal->SetResliceAxesDirectionCosines(1, 0, 0, 0, 1, 0, 0, 0, 1);
	resliceFrontal->SetResliceAxesOrigin(layer[0] * spacing[0], layer[1] * spacing[1], layer[2] * spacing[2]);
	resliceFrontal->SetInterpolationModeToLinear();

	colorFrontal->SetInputConnection(resliceFrontal->GetOutputPort());
	colorFrontal->SetLookupTable(colorFunction);
	colorFrontal->Update();

	mapperFrontal->SetInputConnection(colorFrontal->GetOutputPort());
	mapperFrontal->SetColorWindow(windowColorValue);
	mapperFrontal->SetColorLevel(levelColorValue);

	actorFrontal->SetMapper(mapperFrontal);

	rendererFrontal->AddActor(actorFrontal);
	rendererFrontal->SetBackground(0, 0, 0);
	rendererFrontal->SetViewport(0, 0, 0.5, 0.5);

	// Çizgiler
	rendererFrontal->AddActor(run2DLinePipeline(linesAxial[0]->GetOutputPort()));
	rendererFrontal->AddActor(run2DLinePipeline(linesSagittal[1]->GetOutputPort()));
	set2DFrontalLines();

	renderWindow->AddRenderer(rendererFrontal);
}

void Model::runVolume3DPipeline(vtkAlgorithmOutput* input) {
	mapperVolume->SetInputConnection(input);
	mapperVolume->SetRequestedRenderModeToGPU();

	propertyVolume->SetColor(colorFunction);
	propertyVolume->SetScalarOpacity(opacityFunction);

	actorVolume->SetMapper(mapperVolume);
	actorVolume->SetProperty(propertyVolume);

	rendererVolume->AddActor(actorVolume);
	rendererVolume->SetBackground(0, 0, 0);
	rendererVolume->SetViewport(0.5, 0, 1, 0.5);

	// Çizgiler
	for (int i = 0; i < 4; i++) {
		rendererVolume->AddActor(run3DLinePipeline(linesAxialVolume[i]->GetOutputPort()));
		rendererVolume->AddActor(run3DLinePipeline(linesSagittalVolume[i]->GetOutputPort()));
		rendererVolume->AddActor(run3DLinePipeline(linesFrontalVolume[i]->GetOutputPort()));
	}
	rendererVolume->AddActor(run3DLinePipeline(lineAxialAndSagittalVolume->GetOutputPort()));
	rendererVolume->AddActor(run3DLinePipeline(lineAxialAndFrontalVolume->GetOutputPort()));
	rendererVolume->AddActor(run3DLinePipeline(lineFrontalAndSagittalVolume->GetOutputPort()));
	set3DAxialLines();
	set3DSagittalLines();
	set3DFrontalLines();
	set3DBothLines();

	renderWindow->AddRenderer(rendererVolume);
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

void Model::setSizeOfSlices() {
	height = renderWindow->GetSize()[1];
	width = renderWindow->GetSize()[0];

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
	if ((double)realSizeOfX * spacing[0] / (double)slicePanelWidth >= (double)realSizeOfZ * spacing[2] / (double)slicePanelHeight) {
		ratio[1] = (double)slicePanelWidth / (double)realSizeOfX;
	}
	else {
		ratio[1] = (double)slicePanelHeight / (double)realSizeOfZ;
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
	if ((double)realSizeOfZ * spacing[2] / (double)slicePanelWidth >= (double)realSizeOfY * spacing[1] / (double)slicePanelHeight) {
		ratio[0] = (double)slicePanelWidth / (double)realSizeOfZ;
	}
	else {
		ratio[0] = (double)slicePanelHeight / (double)realSizeOfY;
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
	if ((double)realSizeOfX * spacing[0] / (double)slicePanelWidth >= (double)realSizeOfY * spacing[1] / (double)slicePanelHeight) {
		ratio[2] = (double)slicePanelWidth / (double)realSizeOfX;
	}
	else {
		ratio[2] = (double)slicePanelHeight / (double)realSizeOfY;
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

void Model::set2DAxialLines() {
	// Frontal içinde olan yatay çizgi
	linesAxial[0]->SetPoint1(centerPointOfSliceOnX + (realSizeOfX / 2 + motion[0]) * ratio[2] * zoom[2], centerPointOfSliceOnY, 0);
	linesAxial[0]->SetPoint2(centerPointOfSliceOnX - (realSizeOfX / 2 - motion[0]) * ratio[2] * zoom[2], centerPointOfSliceOnY, 0);
	// Sagittal içinde olan yatay çizgi
	linesAxial[1]->SetPoint1(centerPointOfSliceOnX + (realSizeOfZ / 2 + motion[2]) * ratio[0] * zoom[0], centerPointOfSliceOnY, 0);
	linesAxial[1]->SetPoint2(centerPointOfSliceOnX - (realSizeOfZ / 2 - motion[2]) * ratio[0] * zoom[0], centerPointOfSliceOnY, 0);
}

void Model::set2DSagittalLines() {
	// Axial içinde olan dikey çizgi
	linesSagittal[0]->SetPoint1(centerPointOfSliceOnX, centerPointOfSliceOnY + (realSizeOfZ / 2 + motion[2]) * ratio[1] * zoom[1], 0);
	linesSagittal[0]->SetPoint2(centerPointOfSliceOnX, centerPointOfSliceOnY - (realSizeOfZ / 2 - motion[2]) * ratio[1] * zoom[1], 0);
	// Frontal içinde olan dikey çizgi
	linesSagittal[1]->SetPoint1(centerPointOfSliceOnX, centerPointOfSliceOnY + (realSizeOfY / 2 + motion[1]) * ratio[2] * zoom[2], 0);
	linesSagittal[1]->SetPoint2(centerPointOfSliceOnX, centerPointOfSliceOnY - (realSizeOfY / 2 - motion[1]) * ratio[2] * zoom[2], 0);
}

void Model::set2DFrontalLines() {
	// Axial içinde olan yatay çizgi
	linesFrontal[0]->SetPoint1(centerPointOfSliceOnX + (realSizeOfX / 2 + motion[0]) * ratio[1] * zoom[1], centerPointOfSliceOnY, 0);
	linesFrontal[0]->SetPoint2(centerPointOfSliceOnX - (realSizeOfX / 2 - motion[0]) * ratio[1] * zoom[1], centerPointOfSliceOnY, 0);
	// Sagittal içinde olan dikey çizgi
	linesFrontal[1]->SetPoint1(centerPointOfSliceOnX, centerPointOfSliceOnY + (realSizeOfY / 2 + motion[1]) * ratio[0] * zoom[0], 0);
	linesFrontal[1]->SetPoint2(centerPointOfSliceOnX, centerPointOfSliceOnY - (realSizeOfY / 2 - motion[1]) * ratio[0] * zoom[0], 0);
}

void Model::set3DAxialLines() {
	linesAxialVolume[0]->SetPoint1(extent[0] * spacing[0], layer[1] * spacing[1], extent[4] * spacing[2]);
	linesAxialVolume[0]->SetPoint2(extent[1] * spacing[0], layer[1] * spacing[1], extent[4] * spacing[2]);

	linesAxialVolume[1]->SetPoint1(extent[1] * spacing[0], layer[1] * spacing[1], extent[4] * spacing[2]);
	linesAxialVolume[1]->SetPoint2(extent[1] * spacing[0], layer[1] * spacing[1], extent[5] * spacing[2]);

	linesAxialVolume[2]->SetPoint1(extent[1] * spacing[0], layer[1] * spacing[1], extent[5] * spacing[2]);
	linesAxialVolume[2]->SetPoint2(extent[0] * spacing[0], layer[1] * spacing[1], extent[5] * spacing[2]);

	linesAxialVolume[3]->SetPoint1(extent[0] * spacing[0], layer[1] * spacing[1], extent[5] * spacing[2]);
	linesAxialVolume[3]->SetPoint2(extent[0] * spacing[0], layer[1] * spacing[1], extent[4] * spacing[2]);
}

void Model::set3DSagittalLines() {
	linesSagittalVolume[0]->SetPoint1(layer[0] * spacing[0], extent[2] * spacing[1], extent[4] * spacing[2]);
	linesSagittalVolume[0]->SetPoint2(layer[0] * spacing[0], extent[3] * spacing[1], extent[4] * spacing[2]);

	linesSagittalVolume[1]->SetPoint1(layer[0] * spacing[0], extent[3] * spacing[1], extent[4] * spacing[2]);
	linesSagittalVolume[1]->SetPoint2(layer[0] * spacing[0], extent[3] * spacing[1], extent[5] * spacing[2]);

	linesSagittalVolume[2]->SetPoint1(layer[0] * spacing[0], extent[3] * spacing[1], extent[5] * spacing[2]);
	linesSagittalVolume[2]->SetPoint2(layer[0] * spacing[0], extent[2] * spacing[1], extent[5] * spacing[2]);

	linesSagittalVolume[3]->SetPoint1(layer[0] * spacing[0], extent[2] * spacing[1], extent[5] * spacing[2]);
	linesSagittalVolume[3]->SetPoint2(layer[0] * spacing[0], extent[2] * spacing[1], extent[4] * spacing[2]);
}

void Model::set3DFrontalLines() {
	linesFrontalVolume[0]->SetPoint1(extent[0] * spacing[0], extent[2] * spacing[1], layer[2] * spacing[2]);
	linesFrontalVolume[0]->SetPoint2(extent[1] * spacing[0], extent[2] * spacing[1], layer[2] * spacing[2]);
	
	linesFrontalVolume[1]->SetPoint1(extent[1] * spacing[0], extent[2] * spacing[1], layer[2] * spacing[2]);
	linesFrontalVolume[1]->SetPoint2(extent[1] * spacing[0], extent[3] * spacing[1], layer[2] * spacing[2]);

	linesFrontalVolume[2]->SetPoint1(extent[1] * spacing[0], extent[3] * spacing[1], layer[2] * spacing[2]);
	linesFrontalVolume[2]->SetPoint2(extent[0] * spacing[0], extent[3] * spacing[1], layer[2] * spacing[2]);

	linesFrontalVolume[3]->SetPoint1(extent[0] * spacing[0], extent[3] * spacing[1], layer[2] * spacing[2]);
	linesFrontalVolume[3]->SetPoint2(extent[0] * spacing[0], extent[2] * spacing[1], layer[2] * spacing[2]);
	
}

void Model::set3DBothLines() {
	lineAxialAndSagittalVolume->SetPoint1(layer[0] * spacing[0], layer[1] * spacing[1], extent[4] * spacing[2]);
	lineAxialAndSagittalVolume->SetPoint2(layer[0] * spacing[0], layer[1] * spacing[1], extent[5] * spacing[2]);
	lineAxialAndFrontalVolume->SetPoint1(extent[0] * spacing[0], layer[1] * spacing[1], layer[2] * spacing[2]);
	lineAxialAndFrontalVolume->SetPoint2(extent[1] * spacing[0], layer[1] * spacing[1], layer[2] * spacing[2]);
	lineFrontalAndSagittalVolume->SetPoint1(layer[0] * spacing[0], extent[2] * spacing[1], layer[2] * spacing[2]);
	lineFrontalAndSagittalVolume->SetPoint2(layer[0] * spacing[0], extent[3] * spacing[1], layer[2] * spacing[2]);
}

void Model::setPanels() {
	setSizeOfSlices();
	set2DAxialLines();
	set2DSagittalLines();
	set2DFrontalLines();
	set3DAxialLines();
	set3DSagittalLines();
	set3DFrontalLines();
	set3DBothLines();
}

void Model::runMainPipeline() {
	vtkAlgorithmOutput* outputOfReader = readDicom();

	layer[0] = realSizeOfX / 2; layer[1] = realSizeOfY / 2; layer[2] = realSizeOfZ / 2;
	zoom[0] = 1; zoom[1] = 1; zoom[2] = 1;
	motion[0] = 0; motion[1] = 0; motion[2] = 0;
	isMax = false;

	setColorFunction(range[0], range[1]);
	setOpacityFunction(range[0], range[1]);

	extractVOI->SetInputConnection(outputOfReader);
	setExtractVOI(extent[0], extent[1], extent[2], extent[3], extent[4], extent[5]);

	runAxialSlicePipeline(extractVOI->GetOutputPort());
	runSagittalSlicePipeline(extractVOI->GetOutputPort());
	runFrontalSlicePipeline(extractVOI->GetOutputPort());
	runVolume3DPipeline(extractVOI->GetOutputPort());

	renderWindow->InitializeFromCurrentContext();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}