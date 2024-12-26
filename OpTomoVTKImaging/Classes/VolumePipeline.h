#pragma once

#include <vtkExtractVOI.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkRenderer.h>

class VolumePipeline {
private:
	vtkSmartVolumeMapper* mapper;
	vtkVolume* volume;
	vtkRenderer* renderer;

public:
	VolumePipeline();
	~VolumePipeline();
	void Initialize(vtkExtractVOI* extractVOI, vtkColorTransferFunction* color, vtkPiecewiseFunction* opacity);
	vtkRenderer* GetRenderer();
};