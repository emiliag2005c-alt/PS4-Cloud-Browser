# Perfil personalizable: formato JSON v1

Los perfiles son archivos JSON editables desde el futuro PKG. El editor incluye un único perfil predeterminado de estilo PC y permite crear perfiles personalizados ilimitados.

## Entradas

- `device`: `keyboard` o `mouse`.
- `code`: nombre estable de tecla o botón, independiente del layout cuando sea posible.
- `alternates`: lista opcional de entradas alternativas que activan la misma acción. Solo se usa cuando el usuario configura explícitamente entradas adicionales; no se presupone que el dispositivo tenga botones que no existen.
- `action`: acción lógica. Puede ser un botón de mando (R1, R2, L1, L2, CROSS, L3, R3, OPTIONS) o una acción semántica del juego como `INTERACT`. La asociación final de acciones semánticas con botones del mando debe poder ajustarse por juego.
- Movimiento y cámara se configuran independientemente de los botones.
- L2/R2 son gatillos independientes y deben admitir presión analógica cuando exista una ruta de salida que lo soporte.
- El editor debe advertir de conflictos entre entradas, pero permitir compartir una entrada si el usuario lo confirma.

## Perfil incluido

`profiles/pc-default.json` contiene la distribución inicial de teclado y mouse. Es un punto de partida editable, no una plantilla obligatoria ni una configuración universal para todos los juegos.
