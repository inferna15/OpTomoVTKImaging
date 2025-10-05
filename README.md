# VTK Medical Imaging Core DLL

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![VTK](https://img.shields.io/badge/VTK-0D2A54?style=for-the-badge&logo=kitware&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Proje Hakkında

**VTK Medical Imaging Core**, C++ dili ve **VTK (The Visualization Toolkit)** kütüphanesi kullanılarak geliştirilmiş, yüksek performanslı bir dinamik bağlantı kütüphanesidir (DLL). Bu projenin temel amacı, DICOM gibi tıbbi görüntüleme verilerinin işlenmesi, analizi ve 3D/2D görselleştirilmesi gibi karmaşık görevleri üstlenerek, bu işlevselliği bir arayüz (GUI) uygulamasının kolayca kullanabileceği basit bir API olarak sunmaktır.

Bu kütüphane, karmaşık VTK mantığını ve C++ kodunu soyutlayarak, WPF (C#), Qt (C++) veya başka bir teknolojiyle geliştirilmiş ön yüz uygulamalarının ağır görselleştirme yükünü üstlenir.

## Özellikler

-   **DICOM Serisi Yükleme:** Bir klasördeki DICOM dosyalarını okuyup 3D hacimsel veri olarak işler.
-   **3D Hacimsel Görüntüleme (Volume Rendering):** Yüklenen verileri 3D olarak görselleştirir.
-   **2D Kesit Görüntüleme (Slicing):** Veri setinden Aksiyel, Sagital ve Koronal düzlemlerde 2D kesitler oluşturur.
-   **Pencereleme (Window/Level):** Görüntü kontrastını ve parlaklığını ayarlamak için fonksiyonlar sunar.
-   **Performans:** Karmaşık işlemler C++ üzerinde çalıştığı için yüksek performans sunar.
-   **Platform Bağımsızlığı:** CMake ile derlendiği için Windows, Linux ve macOS platformlarında kullanılabilir.

## Mimarî

Bu DLL, görselleştirme motoru ile son kullanıcı arayüzü arasında bir katman görevi görür. Bu sayede:
1.  **Sorumluluklar Ayrılır:** Arayüz geliştiricileri, karmaşık VTK pipeline'ları kurmak yerine sadece DLL'in sunduğu basit fonksiyonları çağırır.
2.  **Dil Bağımsızlığı:** `extern "C"` ile sunulan C-stili API sayesinde, C# (P/Invoke), Python (ctypes) gibi farklı dillerden kolayca kullanılabilir.
3.  **Performans:** Hesaplama yoğun işlemler, yönetilen dillere (managed languages) kıyasla daha performanslı olan C++'ta gerçekleştirilir.

## Bağımlılıklar

Projeyi derlemek için sisteminizde aşağıdaki araçların kurulu olması gerekmektedir:
-   **VTK (9.x veya üstü):** Derlenmiş ve kurulmuş olmalıdır.
-   **CMake (3.15 veya üstü):** Projeyi yapılandırmak için kullanılır.
-   **Bir C++ Derleyicisi:** (Örn: MSVC, GCC, Clang).

## Kaynaktan Derleme

1.  **Projeyi klonlayın:**
    ```bash
    git clone [https://github.com/kullanici-adiniz/vtk-medical-core-dll.git](https://github.com/kullanici-adiniz/vtk-medical-core-dll.git)
    cd vtk-medical-core-dll
    ```
2.  **CMake ile yapılandırın:**
    Proje klasöründe bir `build` dizini oluşturun ve içine girin.
    ```bash
    mkdir build
    cd build
    ```
    CMake'i çalıştırırken sisteminizdeki VTK kurulumunun yolunu belirtin.
    ```bash
    # VTK_DIR değişkenini kendi VTK kurulum yolunuzla değiştirin
    cmake -DVTK_DIR=/path/to/your/vtk/lib/cmake/vtk-9.x ..
    ```
3.  **Projeyi derleyin:**
    ```bash
    cmake --build . --config Release
    ```
    Derleme başarılı olduğunda, `MedicalImagingCore.dll` (veya `.so`/`.dylib`) dosyası `build/Release` klasöründe oluşturulacaktır.

## Entegrasyon ve API Kullanımı

Bu DLL'i bir uygulamaya entegre etmek için, DLL'i yükleyip dışa aktarılan (exported) fonksiyonlarını çağırmanız yeterlidir.
