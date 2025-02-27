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
#include <vtkAutoInit.h>
#include <vtkPolyDataMapper2D.h>;
#include <vtkProperty2D.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#pragma region VTK_MODULE_INIT
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
#pragma endregion

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
	vtkSmartPointer<vtkLineSource> linesAxial[2]; // Eski versiyona g�re 2. ve 3. �izgiler

	// Sagittal
	vtkSmartPointer<vtkImageReslice> resliceSagittal;
	vtkSmartPointer<vtkImageMapToColors> colorSagittal;
	vtkSmartPointer<vtkImageMapper> mapperSagittal;
	vtkSmartPointer<vtkActor2D> actorSagittal;
	vtkSmartPointer<vtkRenderer> rendererSagittal;
	vtkSmartPointer<vtkLineSource> linesSagittal[2]; // Eski versiyona g�re 0. ve 1. �izgiler

	// Frontal
	vtkSmartPointer<vtkImageReslice> resliceFrontal;
	vtkSmartPointer<vtkImageMapToColors> colorFrontal;
	vtkSmartPointer<vtkImageMapper> mapperFrontal;
	vtkSmartPointer<vtkActor2D> actorFrontal;
	vtkSmartPointer<vtkRenderer> rendererFrontal;
	vtkSmartPointer<vtkLineSource> linesFrontal[2]; // Eski versiyona g�re 4. ve 5. �izgiler

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
	const char* dicomPath;
	int extent[6];
	double spacing[3];
	int layer[3];
	double windowColorValue, levelColorValue;
	double ratio[3];
	int motion[3];
	double zoom[3];
	int width, height;
	bool isMax;
	int range[2];
	int realSizeOfX, realSizeOfY, realSizeOfZ;
	int slicePanelHeight, slicePanelWidth;
	int centerPointOfSliceOnX, centerPointOfSliceOnY;

#pragma endregion

public:
	Model(const char* path);
	void* GetWindow();
	void SetWindowSize(int height, int width);

	/// <summary>
	/// Dicom g�r�nt�leri okur.
	/// Extent, Spacing, Range, WindowColor ve LevelColor de�erlerini al�r.
	/// </summary>
	/// <param name="path">Dicom g�r�nt�lerin bulundu�u klas�r�n yolu.</param>
	/// <returns>vtkDICOMImageReader->GetOutputPort()</returns>
	vtkAlgorithmOutput* readDicom(const char* path);

	/// <summary>
	/// ColorFunction i�in nokta de�erlerini de�i�tirir.
	/// </summary>
	/// <param name="min">Alt s�n�r de�eri</param>
	/// <param name="max">�st s�n�r de�eri</param>
	void setColorFunction(int min, int max);

	/// <summary>
	/// OpacityFunction i�in nokta de�erlerini de�i�tirir.
	/// </summary>
	/// <param name="min">Alt s�n�r de�eri</param>
	/// <param name="max">�st s�n�r de�eri</param>
	void setOpacityFunction(int min, int max);

	/// <summary>
	/// Layer de�erlerini extent de�erlerine g�re ba�lang�� de�erine ayarlar.
	/// </summary>
	void setLayerToDefault();

	/// <summary>
	/// ExtractVOI s�n�r de�erlerini ayarlar.
	/// </summary>
	/// <param name="minX">Sagittal k�sm�n minimum s�n�r�</param>
	/// <param name="maxX">Sagittal k�sm�n maksimum s�n�r�</param>
	/// <param name="minY">Axial k�sm�n minimum s�n�r�</param>
	/// <param name="maxY">Axial k�sm�n maksimum s�n�r�</param>
	/// <param name="minZ">Frontal k�sm�n minimum s�n�r�</param>
	/// <param name="maxZ">Frontal k�sm�n maksimum s�n�r�</param>
	void setExtractVOI(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);

	void runAxialSlicePipeline(vtkAlgorithmOutput* input);

	void runSagittalSlicePipeline(vtkAlgorithmOutput* input);

	void runFrontalSlicePipeline(vtkAlgorithmOutput* input);

	void runVolume3DPipeline(vtkAlgorithmOutput* input);

	void setSizeOfSlices();

	vtkSmartPointer<vtkActor2D> run2DLinePipeline(vtkAlgorithmOutput* input);

	vtkSmartPointer<vtkActor> run3DLinePipeline(vtkAlgorithmOutput* input);

	/// <summary>
	/// Ana pipeline hatt�n� �al��t�r�r.
	/// </summary>
	/// <param name="path">Dicom g�r�nt�lerin bulundu�u klas�r�n yolu.</param>
	void runMainPipeline();
};