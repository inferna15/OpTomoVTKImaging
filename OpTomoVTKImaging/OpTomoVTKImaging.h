#pragma once

#ifdef _WIN32
	#ifdef MODEL_EXPORTS
		#define MODEL_API __declspec(dllexport)  // DLL dışa aktarım (export)
	#else
		#define MODEL_API __declspec(dllimport)  // DLL içeri aktarma (import)
	#endif
#else
	// Eğer sadece Windows için yapıyorsanız, diğer platformlara gerek yok
	#define MODEL_API
#endif

#include "Model.h"

extern "C" {
	MODEL_API Model* StartUp(const char* path);
	MODEL_API void* GetWindow(Model* model);
	MODEL_API int Work(Model* model);
	MODEL_API void CleanUp(Model* model);
	MODEL_API void SetWindowSize(Model* model, int height, int width);
}
