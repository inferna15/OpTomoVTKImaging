#pragma once

#include "VolumePipeline.h"
#include "ReslicePipeline.h"

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>


class Model {
private:
	vtkRenderWindow* window;
	vtkRenderWindowInteractor* inter;
	VolumePipeline* volumePipeline;
	ReslicePipeline* axialPipeline;
	ReslicePipeline* sagittalPipeline;
	ReslicePipeline* frontalPipeline;
	vtkDICOMImageReader* reader;
	vtkExtractVOI* extractVOI;


public:
	Model();
	~Model();
	void Initialize();
	void* GetWindow();
	void SetWindowSize(int height, int width);
};