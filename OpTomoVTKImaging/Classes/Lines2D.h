#pragma once

#include <vtkLineSource.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkProperty2D.h>

class Lines2D {
private:
	vtkLineSource* lines[6];
	vtkPolyDataMapper2D* mapper[6];
	vtkActor2D* actor[6];
	double linesPosition[6][2][2];

public:
	Lines2D();
	~Lines2D();
	void Initialize(vtkRenderer* axial, vtkRenderer* sagittal, vtkRenderer* frontal);
	void SetAxial();
	void SetSagittal();
	void SetFrontal();
	void SetAxialPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height);
	void SetSagittalPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height);
	void SetFrontalPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height);
};