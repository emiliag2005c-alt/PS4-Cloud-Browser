# Input Mapper — editor de perfiles (diseño de PKG)

## Objetivo
Una aplicación PS4 independiente, instalable como PKG, para crear y administrar perfiles de teclado y mouse. No se presenta como un plugin ni como un mapper funcional todavía.

## Pantallas

1. **Perfiles**
   - Lista de perfiles guardados.
   - Crear, duplicar, renombrar, importar, exportar y borrar.
   - Seleccionar perfil activo.
   - Perfil predeterminado por juego; selección manual si no se puede detectar el título.

2. **Editar perfil**
   - Nombre del juego y, opcionalmente, Title ID.
   - Tabla de acciones del mando: Cross, Circle, Square, Triangle, L1, R1, L2, R2, L3, R3, Options y Share.
   - Cada acción tiene botón "Asignar": el usuario pulsa una tecla o botón del mouse para capturarla.
   - Permitir borrar una asignación y detectar conflictos (dos acciones con la misma entrada).
   - WASD / movimiento y mouse / cámara configurables por separado.
   - Sensibilidad horizontal/vertical, inversión vertical y suavizado.

3. **Prueba de entradas**
   - Mostrar en pantalla qué tecla o botón está pulsado.
   - Mostrar el estado lógico de cada acción.
   - No enviar entradas a juegos desde esta pantalla.

4. **Guardar**
   - Guardar JSON en almacenamiento de usuario bajo `/data/PS4InputMapper/profiles/`.
   - Mantener copia de seguridad antes de sobrescribir.
   - Validar versión de formato y rangos antes de cargar.

## Principios de interacción
- Las acciones se mantienen activas mientras la entrada física siga presionada; al soltar, se liberan.
- L2 y R2 deben modelarse como gatillos independientes, con valor analógico 0–255 en la capa de salida futura.
- Los perfiles son datos, no código ejecutable.
- Nunca instalar automáticamente plugins, modificar kernel, parchear juegos ni cambiar configuración del sistema.

## Estado técnico
El repositorio actual ya tiene una app PKG de diagnóstico/navegador con OpenOrbis, pero este editor será una aplicación separada para no romper el navegador. El core portable existente solo traduce estados normalizados; todavía no hay lector HID conectado a este editor ni una ruta verificada para inyectar controles en juegos.
