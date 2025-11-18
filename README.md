# 🩸 **UmbraCrypt**  
### Un cifrador de archivos por capas con sobrescritura segura

---

## 📌 Descripción general

**UmbraCrypt** es una herramienta de línea de comandos escrita en **C++** diseñada para cifrar y descifrar archivos **de cualquier formato**, usando un sistema de **XOR extendido** y un esquema de **capas de cifrado**. El programa sobrescribe completamente el archivo original durante el cifrado y conserva el archivo encriptado sin alteraciones durante el descifrado, garantizando que **nunca quede rastro del contenido previo**.

Incluye un sistema propio de metadatos para detectar archivos cifrados y contabilizar cuántas capas se han aplicado, además de animaciones visuales y un menú interactivo estilizado.

---

## ✨ Características principales

### 🔐 **Encriptación por capas**
- Permite aplicar múltiples capas de encriptación.
- Cada capa se contabiliza en el encabezado del archivo.
- UmbraCrypt detecta automáticamente si el archivo ya está encriptado.

### 🧵 **Desencriptado seguro (solo en memoria)**
- El archivo cifrado **no se modifica nunca**.
- El contenido desencriptado se muestra únicamente por consola.
- Permite intentar múltiples capas de desencriptado.

### 💥 **Sobrescritura del archivo original**
- Al encriptar, el archivo original es sobrescrito completamente.
- No quedan restos recuperables del contenido previo.

### 💾 **Copia de seguridad opcional**
- Antes de encriptar, el usuario puede generar un backup del archivo.

### 🔍 **Detección de capas vía encabezado**
- UmbraCrypt añade un encabezado del tipo:
  ```
  MEJINENC|layers=N
  ```
- Fácil identificación de archivos procesados por el programa.

### 🎨 **Interfaz estilizada en consola**
- Banners, títulos y separadores visuales.
- Barras de progreso animadas.
- Limpieza automática de pantalla.

---

## 📁 Estructura del proyecto

```
UmbraCrypt/
│
├── src/
│   ├── main.cpp
│   ├── menu.cpp
│   ├── visuals.cpp
│   ├── crypto.cpp
│   ├── fileutils.cpp
│   └── metadata.cpp
│
├── include/
│   ├── menu.hpp
│   ├── visuals.hpp
│   ├── crypto.hpp
│   ├── fileutils.hpp
│   └── metadata.hpp
│
├── UmbraCrypt (o UmbraCrypt.exe)
└── Makefile
```

---

## 🧠 Funcionamiento interno

### ✔ Algoritmo de cifrado
UmbraCrypt utiliza un cifrado **XOR expandido**:
- La clave introducida por el usuario se expande a un vector de bytes.
- El contenido del archivo se procesa byte a byte con `buffer[i] ^ key[i % keySize]`.
- El proceso es reversible con la misma clave.

### ✔ Encabezado del archivo
UmbraCrypt agrega un encabezado en texto plano al inicio del archivo:
```
MEJINENC|layers=N
```
Esto permite reconocer archivos procesados y el número de capas aplicadas.

---

## 🖥 Uso del programa

### ▶ Ejecutar el programa
En Linux:
```
make
./UmbraCrypt
```
En Windows:
```
make
UmbraCrypt.exe
```

### ▶ Menú principal
El programa presenta un menú interactivo:
```
1. Encriptar archivo
2. Desencriptar archivo (mostrar por consola)
3. Ver información de archivo
4. Salir
```

### ✔ Encriptar un archivo
- Introduces ruta.
- Introduces clave (mínimo 8 caracteres) y la confirmas.
- Opcional: crear copia de seguridad.
- El archivo original es sobrescrito con la versión cifrada.

### ✔ Desencriptar un archivo
- Introduces ruta.
- Introduces clave.
- El contenido descifrado se muestra en consola.
- Se puede seguir descifrando más capas.

---

## 🔧 Compilación manual
Sin Makefile:

En Linux:
```
g++ -std=c++17 src/*.cpp -Iinclude -o UmbraCrypt
```

En Windows (MinGW):
```
g++ -std=c++17 src/*.cpp -Iinclude -o UmbraCrypt.exe
```

---

## 📝 Licencia
UmbraCrypt es un proyecto de ejemplo educativo. Modifiable a gusto del usuario.
