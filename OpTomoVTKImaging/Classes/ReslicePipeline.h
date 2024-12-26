#pragma once

#include <vtkColorTransferFunction.h>
#include <vtkExtractVOI.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>

class ReslicePipeline {
private:
	vtkImageReslice* reslice;
	vtkImageMapToColors* colorMap;
	vtkImageMapper* mapper;
	vtkActor2D* actor;
	vtkRenderer* renderer;

public:
	ReslicePipeline();
	~ReslicePipeline();
	void Initialize(vtkExtractVOI* extractVOI, vtkColorTransferFunction* color, int direction, 
		double origin, int layer, double spacing, int window, int level);
	vtkRenderer* GetRenderer();
};