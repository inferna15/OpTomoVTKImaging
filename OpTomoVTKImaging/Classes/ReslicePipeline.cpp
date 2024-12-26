#include "ReslicePipeline.h"

ReslicePipeline::ReslicePipeline() {
	reslice = vtkImageReslice::New();
	colorMap = vtkImageMapToColors::New();
	mapper = vtkImageMapper::New();
	actor = vtkActor2D::New();
	renderer = vtkRenderer::New();
}

ReslicePipeline::~ReslicePipeline() {
	renderer->Delete();
	actor->Delete();
	mapper->Delete();
	colorMap->Delete();
	reslice->Delete();
}

void ReslicePipeline::Initialize(vtkExtractVOI* extractVOI, vtkColorTransferFunction* color, int direction, 
								double origin, int layer, double spacing, int window, int level) {
	reslice->SetInputConnection(extractVOI->GetOutputPort());
	reslice->SetOutputDimensionality(2);

	if (direction == 0) {
		reslice->SetResliceAxesDirectionCosines(0, 0, 1, 1, 0, 0, 0, 1, 0);
		reslice->SetResliceAxesOrigin(origin + layer * spacing, 0, 0);
	} 
	else if (direction == 1) {
		reslice->SetResliceAxesDirectionCosines(1, 0, 0, 0, 0, 1, 0, 1, 0);
		reslice->SetResliceAxesOrigin(0, origin + layer * spacing, 0);
	}
	else if (direction == 2) {
		reslice->SetResliceAxesDirectionCosines(1, 0, 0, 0, 1, 0, 0, 0, 1);
		reslice->SetResliceAxesOrigin(0, 0, origin + layer * spacing);
	}
	else {
		std::cout << "Error: Reslice invalid direction." << "\n";
		return;
	}

	colorMap->SetInputConnection(reslice->GetOutputPort());
	colorMap->SetLookupTable(color);
	colorMap->Update();

	mapper->SetInputConnection(colorMap->GetOutputPort());
	mapper->SetColorWindow(window);
	mapper->SetColorLevel(level);

	actor->SetMapper(mapper);

	renderer->AddActor(actor);
	renderer->SetBackground(0, 0, 0);

	if (direction == 0) {
		renderer->SetViewport(0.5, 0.5, 1, 1);
	}
	else if (direction == 1) {
		renderer->SetViewport(0, 0.5, 0.5, 1);
	}
	else if (direction == 2) {
		renderer->SetViewport(0, 0, 0.5, 0.5);
	}
	else {
		std::cout << "Error: Reslice invalid direction." << "\n";
		return;
	}
}

vtkRenderer* ReslicePipeline::GetRenderer() {
	return renderer;
}