#pragma once

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkExtractVOI.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapper.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkLineSource.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkImageData.h>
#include <vtkAlgorithmOutput.h>

class Model {

#pragma region VTK Classes Definition
private:
	// General
	vtkSmartPointer<vtkDICOMImageReader> dicomReader;
	vtkSmartPointer<vtkExtractVOI> extractVOI;
	vtkSmartPointer<vtkColorTransferFunction> colorFunction;
	vtkSmartPointer<vtkPiecewiseFunction> opacityFunction;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

	// Axial
	vtkSmartPointer<vtkImageReslice> resliceAxial;
	vtkSmartPointer<vtkImageMapToColors> colorAxial;
	vtkSmartPointer<vtkImageMapper> mapperAxial;
	vtkSmartPointer<vtkActor2D> actorAxial;
	vtkSmartPointer<vtkRenderer> rendererAxial;
	vtkSmartPointer<vtkLineSource> linesAxial[2]; // Eski versiyona göre 0. ve 4. çizgiler

	// Sagittal
	vtkSmartPointer<vtkImageReslice> resliceSagittal;
	vtkSmartPointer<vtkImageMapToColors> colorSagittal;
	vtkSmartPointer<vtkImageMapper> mapperSagittal;
	vtkSmartPointer<vtkActor2D> actorSagittal;
	vtkSmartPointer<vtkRenderer> rendererSagittal;
	vtkSmartPointer<vtkLineSource> linesSagittal[2]; // Eski versiyona göre 3. ve 5. çizgiler

	// Frontal
	vtkSmartPointer<vtkImageReslice> resliceFrontal;
	vtkSmartPointer<vtkImageMapToColors> colorFrontal;
	vtkSmartPointer<vtkImageMapper> mapperFrontal;
	vtkSmartPointer<vtkActor2D> actorFrontal;
	vtkSmartPointer<vtkRenderer> rendererFrontal;
	vtkSmartPointer<vtkLineSource> linesFrontal[2]; // Eski versiyona göre 1. ve 2. çizgiler

	// Volume
	vtkSmartPointer<vtkSmartVolumeMapper> mapperVolume;
	vtkSmartPointer<vtkVolumeProperty> propertyVolume;
	vtkSmartPointer<vtkVolume> actorVolume;
	vtkSmartPointer<vtkRenderer> rendererVolume;
	vtkSmartPointer<vtkLineSource> linesAxialVolume[4];
	vtkSmartPointer<vtkLineSource> linesSagittalVolume[4];
	vtkSmartPointer<vtkLineSource> linesFrontalVolume[4];
	vtkSmartPointer<vtkLineSource> lineAxialAndSagittalVolume;
	vtkSmartPointer<vtkLineSource> lineAxialAndFrontalVolume;
	vtkSmartPointer<vtkLineSource> lineFrontalAndSagittalVolume;
#pragma endregion
#pragma region Other Fields Definition
private:
	int extent[6];
	double spacing[3];
	int layer[3];
	double windowColorValue;
	double levelColorValue;
	double ratio[3];
	int motion[3];
	double zoom[3];
	int width;
	int height;
	bool isMax;
	int range[2];

#pragma endregion

public:
	/// <summary>
	/// Dicom görüntüleri okur.
	/// Extent, Spacing, Range, WindowColor ve LevelColor deðerlerini alýr.
	/// </summary>
	/// <param name="path">Dicom görüntülerin bulunduðu klasörün yolu.</param>
	/// <returns>vtkDICOMImageReader->GetOutputPort()</returns>
	vtkAlgorithmOutput* readDicom(const char* path);

	/// <summary>
	/// ColorFunction için nokta deðerlerini deðiþtirir.
	/// </summary>
	/// <param name="min">Alt sýnýr deðeri</param>
	/// <param name="max">Üst sýnýr deðeri</param>
	void setColorFunction(int min, int max);

	/// <summary>
	/// OpacityFunction için nokta deðerlerini deðiþtirir.
	/// </summary>
	/// <param name="min">Alt sýnýr deðeri</param>
	/// <param name="max">Üst sýnýr deðeri</param>
	void setOpacityFunction(int min, int max);

	/// <summary>
	/// Layer deðerlerini extent deðerlerine göre baþlangýç deðerine ayarlar.
	/// </summary>
	void setLayerToDefault();

	/// <summary>
	/// ExtractVOI sýnýr deðerlerini ayarlar.
	/// </summary>
	/// <param name="minX">Sagittal kýsmýn minimum sýnýrý</param>
	/// <param name="maxX">Sagittal kýsmýn maksimum sýnýrý</param>
	/// <param name="minY">Axial kýsmýn minimum sýnýrý</param>
	/// <param name="maxY">Axial kýsmýn maksimum sýnýrý</param>
	/// <param name="minZ">Frontal kýsmýn minimum sýnýrý</param>
	/// <param name="maxZ">Frontal kýsmýn maksimum sýnýrý</param>
	void setExtractVOI(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);

	void runAxialSlicePipeline(vtkAlgorithmOutput* input);

	void runSagittalSlicePipeline(vtkAlgorithmOutput* input);

	void runFrontalSlicePipeline(vtkAlgorithmOutput* input);

	void runVolume3DPipeline(vtkAlgorithmOutput* input);

	/// <summary>
	/// Ana pipeline hattýný çalýþtýrýr.
	/// </summary>
	/// <param name="path">Dicom görüntülerin bulunduðu klasörün yolu.</param>
	void runMainPipeline(const char* path);
};