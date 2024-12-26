#include "OpTomoVTKImaging.h"

extern "C" MODEL_API Model* StartUp(const char* path) {
	return new Model();
}

extern "C" MODEL_API void* GetWindow(Model* model) {
	return model->GetWindow();
}

extern "C" MODEL_API void Work(Model* model) {
	model->Initialize();
}

extern "C" MODEL_API void CleanUp(Model* model) {
	delete model;
}

extern "C" MODEL_API void SetWindowSize(Model* model, int height, int width) {
	model->SetWindowSize(height, width);
}