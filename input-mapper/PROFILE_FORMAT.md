# Perfil personalizable: formato JSON v1

Los perfiles son archivos JSON editables desde el futuro PKG. Cada acción del mando se asigna a una tecla o botón físico. Las asignaciones de movimiento y cámara son independientes.

- `device`: `keyboard` o `mouse`.
- `code`: nombre estable de tecla o botón, no un código dependiente del layout del teclado.
- `action`: acción lógica del mando.
- Los gatillos L2/R2 son independientes y deben admitir presión analógica cuando exista una ruta de salida que lo soporte.
- El editor debe validar que no existan entradas duplicadas, salvo que el usuario permita explícitamente compartir una tecla.

El ejemplo inicial está en `profiles/gow-ragnarok.json`.
