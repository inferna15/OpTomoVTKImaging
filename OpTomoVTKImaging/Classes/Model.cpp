#include "Model.h"

Model::Model() {
	//pipeline = new VolumePipeline();
	window = vtkRenderWindow::New();
	inter = vtkRenderWindowInteractor::New();
}

Model::~Model() {
	inter->Delete();
	window->Delete();
	delete pipeline;
}

void Model::Initialize() {
	window->AddRenderer(pipeline->GetRenderer());
	window->SetInteractor(inter);

	window->SetBorders(false);
	window->Initialize();
}

void* Model::GetWindow() {
	window->InitializeFromCurrentContext();
	return window->GetGenericWindowId();
}

void Model::SetWindowSize(int height, int width) {
	window->SetSize(width, height);
}