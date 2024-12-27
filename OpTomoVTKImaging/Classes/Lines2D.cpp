#include "Lines2D.h"

Lines2D::Lines2D() {
	for (int i = 0; i < 6; i++) {
		lines[i] = vtkLineSource::New();
		mapper[i] = vtkPolyDataMapper2D::New();
		actor[i] = vtkActor2D::New();
	}
}

Lines2D::~Lines2D() {
	for (int i = 0; i < 6; i++) {
		actor[i]->Delete();
		mapper[i]->Delete();
		lines[i]->Delete();
	}
}

void Lines2D::Initialize(vtkRenderer* axial, vtkRenderer* sagittal, vtkRenderer* frontal) {
	for (int i = 0; i < 6; i++) {
		mapper[i]->SetInputConnection(lines[i]->GetOutputPort());
		actor[i]->SetMapper(mapper[i]);
		actor[i]->GetProperty()->SetLineWidth(1);

		if (i / 2 == 0)
			actor[i]->GetProperty()->SetColor(1, 0, 0);
		else if (i / 2 == 1)
			actor[i]->GetProperty()->SetColor(0, 1, 0);
		else
			actor[i]->GetProperty()->SetColor(0, 0, 1);

		if (i == 3 || i == 5)
			sagittal->AddActor(actor[i]);
		else if (i == 0 || i == 4)
			axial->AddActor(actor[i]);
		else
			frontal->AddActor(actor[i]);
	}
}

void Lines2D::SetAxial() {
	lines[2]->SetPoint1(linesPosition[2][0][0], linesPosition[2][0][1], 0);
	lines[2]->SetPoint1(linesPosition[2][1][0], linesPosition[2][1][1], 0);
	lines[3]->SetPoint1(linesPosition[3][0][0], linesPosition[3][0][1], 0);
	lines[3]->SetPoint1(linesPosition[3][1][0], linesPosition[3][1][1], 0);
}

void Lines2D::SetSagittal() {
	lines[0]->SetPoint1(linesPosition[0][0][0], linesPosition[0][0][1], 0);
	lines[0]->SetPoint1(linesPosition[0][1][0], linesPosition[0][1][1], 0);
	lines[1]->SetPoint1(linesPosition[1][0][0], linesPosition[1][0][1], 0);
	lines[1]->SetPoint1(linesPosition[1][1][0], linesPosition[1][1][1], 0);
}

void Lines2D::SetFrontal() {
	lines[4]->SetPoint1(linesPosition[4][0][0], linesPosition[4][0][1], 0);
	lines[4]->SetPoint1(linesPosition[4][1][0], linesPosition[4][1][1], 0);
	lines[5]->SetPoint1(linesPosition[5][0][0], linesPosition[5][0][1], 0);
	lines[5]->SetPoint1(linesPosition[5][1][0], linesPosition[5][1][1], 0);
}

void Lines2D::SetAxialPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height) {
	linesPosition[2][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	// height ve width ile orta nokta belirlenecek ve ona göre yeni bir algoritma hazýrlanacak.
}

void Lines2D::SetSagittalPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height) {
	linesPosition[2][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	// height ve width ile orta nokta belirlenecek ve ona göre yeni bir algoritma hazýrlanacak.
}

void Lines2D::SetFrontalPositions(int layer, int* motion, double* ratio, int* offset, double* zoom, double width, double height) {
	linesPosition[2][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[2][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][0][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][0] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	linesPosition[3][1][1] = (offset[2] + motion[0] * ratio[2]) * zoom[2] - ((width / 2) * (zoom[2] - 1.0));
	// height ve width ile orta nokta belirlenecek ve ona göre yeni bir algoritma hazýrlanacak.
}