#include "VolumePipeline.h"

VolumePipeline::VolumePipeline() {
	mapper = vtkSmartVolumeMapper::New();
	volume = vtkVolume::New();
	renderer = vtkRenderer::New();
}

VolumePipeline::~VolumePipeline() {
	volume->Delete();
	renderer->Delete();
	mapper->Delete();
}

void VolumePipeline::Initialize(vtkExtractVOI* extractVOI, vtkColorTransferFunction* color, vtkPiecewiseFunction* opacity) {
	mapper->SetInputConnection(extractVOI->GetOutputPort());
	volume->SetMapper(mapper);
	volume->GetProperty()->SetColor(color);
	volume->GetProperty()->SetScalarOpacity(opacity);
	renderer->AddActor(volume);
	renderer->SetViewport(0.5, 0, 1, 0.5);
}

vtkRenderer* VolumePipeline::GetRenderer() {
	return renderer;
}