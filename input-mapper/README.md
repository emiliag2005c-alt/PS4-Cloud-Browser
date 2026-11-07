# PS4 Input Mapper

Proyecto separado del PS4 Cloud Browser para crear y administrar perfiles de teclado y mouse.

## Objetivo de la primera versión

- Aplicación PS4 independiente en formato PKG.
- Perfil inicial editable: **PC estándar — teclado y mouse**.
- Crear, duplicar, renombrar, seleccionar, importar, exportar y borrar perfiles.
- Editor de asignaciones con captura de tecla/botón y detección de conflictos.
- Ajustes independientes de movimiento y cámara.
- Prueba visual de entradas y guardado persistente.

## Perfil inicial

| Acción | Entrada |
|---|---|
| Movimiento | WASD |
| Cámara | Mouse |
| Ataque ligero (R1) | Clic izquierdo |
| Ataque pesado (R2) | Clic derecho |
| Bloquear con escudo (L1) | Q |
| Apuntar / lanzar hacha (L2) | Ctrl |
| Esquivar (Cross) | Espacio |
| Interactuar | E |
| Correr (L3) | Shift |
| Fijar objetivo (R3) | Clic central |
| Menú | Esc |

No se presupone que el mouse tenga botones laterales. El editor permitirá asignarlos manualmente si el dispositivo los ofrece.

## Estado técnico honesto

El repositorio contiene el formato JSON, el perfil inicial y un core portable de traducción con pruebas de escritorio. **Todavía no contiene un editor PS4 instalable ni un sistema verificado de inyección de mando en juegos.** La aplicación de configuración y el runtime de entrada son componentes separados.

La compatibilidad universal con juegos que solo aceptan mando depende de encontrar y validar una ruta segura de entrada en PS4/GoldHEN. No se afirmará que existe hasta probarla en hardware real.

## Pruebas locales

```sh
cc -std=c11 -Wall -Wextra -Werror -Iinput-mapper/include \
  input-mapper/src/mapper.c input-mapper/tests/test_mapper.c -o mapper_test
./mapper_test
python3 input-mapper/tests/test_profile.py
```

## Diseño del editor

Consulta [PROFILE_EDITOR.md](PROFILE_EDITOR.md) y [PROFILE_FORMAT.md](PROFILE_FORMAT.md).
