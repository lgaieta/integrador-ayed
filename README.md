# Cómo clonar este repositorio y empezar a trabajar

Este proyecto contiene código C++ (por ejemplo, el archivo `integrador.cpp`). A continuación se explican los pasos para instalar Git, clonar el repositorio y compilar/ejecutar el programa.

---

## 1) Instalar Git

- Windows:
  1. Descarga el instalador oficial desde: https://git-scm.com/downloads
  2. Ejecuta el instalador y sigue las opciones predeterminadas (puedes instalar "Git Bash" para usar una terminal cómoda).
  3. Verifica la instalación abriendo una terminal (PowerShell o Git Bash) y ejecutando:
     
     ```
     git --version
     ```
  Si ves la versión de Git, la instalación fue exitosa.

- macOS:
  - Si tienes Homebrew: 
    
    ```
    brew install git
    ```
  - Alternativamente, instalar Xcode (incluye Git) o descargar Git desde la página oficial.

- Linux:
  - Usa el gestor de paquetes de tu distribución. Ejemplos:
    - Debian/Ubuntu:
      
      ```
      sudo apt-get update && sudo apt-get install git
      ```
    - Fedora:
      
      ```
      sudo dnf install git
      ```
    - Arch:
      
      ```
      sudo pacman -S git
      ```

---

## 2) Clonar el repositorio

1. Abre una terminal.
2. Navega hasta el directorio donde quieres alojar el proyecto.
3. Ejecuta el comando de clonación:
   
   ```
   git clone https://github.com/lgaieta/integrador-ayed
   ```
4. Entra a la carpeta del proyecto (el nombre puede variar según el repositorio):
   
   ```
   cd integrador-ayed
   ```

---

## 3) Empezar a trabajar: compilar y ejecutar

El proyecto incluye un archivo fuente C++ (`integrador.cpp`). Necesitas un compilador C++ (por ejemplo, g++ o MSVC).

- En Windows (con g++/MinGW-w64):
  1. Asegúrate de tener g++ instalado (por ejemplo, vía MSYS2/MinGW-w64).
  2. Compila:
     
     ```
     g++ -std=c++17 -O2 integrador.cpp -o integrador.exe
     ```
  3. Ejecuta:
     
     ```
     .\integrador.exe
     ```

- En Linux/macOS (con g++):
  1. Compila:
     
     ```
     g++ -std=c++17 -O2 integrador.cpp -o integrador
     ```
  2. Ejecuta:
     
     ```
     ./integrador
     ```

Opcional: Puedes usar un IDE como Visual Studio Code para abrir la carpeta del proyecto y configurar tareas de compilación. Asegúrate de tener instalado el compilador y la extensión C/C++ correspondiente.

---

## 4) Consejos

- Para actualizar tu copia local con los últimos cambios del repositorio remoto:
  
  ```
  git pull
  ```
- Para verificar el estado de tus archivos y cambios:
  
  ```
  git status
  ```