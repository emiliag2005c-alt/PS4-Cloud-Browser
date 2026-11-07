# Input Mapper — editor de perfiles (diseño de PKG)

## Objetivo
Una aplicación PS4 independiente, instalable como PKG, para crear y administrar perfiles de teclado y mouse. No se presenta como un plugin ni como un mapper funcional todavía.

## Perfiles
- Incluir un solo perfil inicial: **PC estándar — teclado y mouse**.
- Permitir crear tantos perfiles personalizados como el usuario quiera.
- Crear, duplicar, renombrar, importar, exportar, borrar y seleccionar perfiles.
- Permitir asignar un perfil a un juego; si no se puede identificar el título, permitir selección manual.
- No incluir una galería de plantillas por juego ni obligar a usar la predeterminada.

## Configuración predeterminada

| Acción | Entrada |
|---|---|
| Movimiento | WASD |
| Cámara | Mouse |
| Ataque ligero (R1) | Clic izquierdo |
| Ataque pesado (R2) | Clic derecho |
| Bloquear con escudo (L1) | Q |
| Apuntar / lanzar hacha (L2) | Ctrl o botón lateral del mouse |
| Esquivar (X / Cross) | Espacio |
| Interactuar | E |
| Correr (L3) | Shift |
| Fijar objetivo (R3) | Clic central |
| Menú | Esc |

L2 queda asignado únicamente a Ctrl en el perfil predeterminado. Si el usuario conecta otro mouse, podrá asignar manualmente sus botones adicionales desde el editor. No asumir que el mouse dispone de botones laterales.

## Editar perfil
- Tabla de acciones lógicas del mando y acciones semánticas del juego.
- Cada acción tiene botón **Asignar**: el usuario lo pulsa y luego presiona la tecla o botón físico que quiera capturar.
- Permitir borrar asignaciones, agregar entradas alternativas y detectar conflictos.
- Movimiento y cámara configurables por separado.
- Sensibilidad horizontal/vertical, inversión vertical y suavizado.
- Permitir ajustar la asociación de acciones como Interactuar al botón de mando correspondiente en cada juego, sin asumir que siempre es el mismo.

## Prueba de entradas
- Mostrar en pantalla qué tecla o botón está pulsado.
- Mostrar el estado lógico de cada acción.
- No enviar entradas a juegos desde esta pantalla.

## Guardado
- Guardar JSON en almacenamiento de usuario bajo `/data/PS4InputMapper/profiles/`.
- Mantener copia de seguridad antes de sobrescribir.
- Validar versión de formato y rangos antes de cargar.

## Principios técnicos
- Las acciones se mantienen activas mientras la entrada física siga presionada; al soltar, se liberan.
- L2 y R2 deben modelarse como gatillos independientes, con valor analógico 0–255 en la capa de salida futura.
- Los perfiles son datos, no código ejecutable.
- Nunca instalar automáticamente plugins, modificar kernel, parchear juegos ni cambiar configuración del sistema.

## Estado técnico
Este editor será una aplicación separada para no romper el navegador Cloud Browser. El core portable existente solo traduce estados normalizados; todavía no hay lector HID conectado a este editor ni una ruta verificada para inyectar controles en juegos. La interfaz de perfiles no implica que la compatibilidad universal ya esté resuelta.
