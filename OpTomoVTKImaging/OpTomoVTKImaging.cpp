#include "OpTomoVTKImaging.h"

extern "C" MODEL_API Model* StartUp(const char* path) {
	return new Model(path);
}

extern "C" MODEL_API void* GetWindow(Model* model) {
	return model->GetWindow();
}

extern "C" MODEL_API int Work(Model* model) {
	model->runMainPipeline();
	return 44; // Malatya dadkdaşkdfadka
}

extern "C" MODEL_API void CleanUp(Model* model) {
	delete model;
}

extern "C" MODEL_API void SetWindowSize(Model* model, int height, int width) {
	model->SetWindowSize(height, width);
}

int main() {
	Model* model = new Model("C:\\Users\\fatil\\OneDrive\\Belgeler\\Dicoms\\256slices_dcm");
	model->runMainPipeline();
	return 0;
}