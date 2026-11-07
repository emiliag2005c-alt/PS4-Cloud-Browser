# PS4 Cloud Browser

Aplicación homebrew experimental para PS4, probada como objetivo en una CUH-1215A con GoldHEN.

## Versión actual: 0.5.2 — prueba de estabilidad gráfica

- **0.2:** se confirmó que la aplicación iniciaba y permanecía abierta en la PS4.
- **0.3:** la primera interfaz gráfica quedó en pantalla negra en la consola.
- **0.4:** se confirmó VideoOut y la visualización de la interfaz.
- **0.5 / 0.5.1:** se incorporaron pruebas experimentales de entrada; la consola llegó a mostrar la interfaz y después se produjo un cierre inesperado.
- **0.5.2:** se desactivaron por completo la inicialización y la lectura de teclado y mouse. Esta compilación solo prueba si el renderizado gráfico puede permanecer estable.

Esta versión no incluye motor web ni compatibilidad con GeForce NOW.

## Compilación

Cada push a `main`, o una ejecución manual desde GitHub Actions, inicia **Build PS4 Cloud Browser**. El flujo descarga OpenOrbis PS4 Toolchain, compila la aplicación, crea el PKG, lo valida y publica los artefactos.

El artefacto esperado se llama `PS4-Cloud-Browser-0.5.2`.

## Próximos pasos

- [x] Crear la estructura inicial del proyecto.
- [x] Confirmar arranque en PS4.
- [x] Confirmar salida gráfica.
- [x] Aislar el renderizado desactivando las API de entrada para diagnosticar el cierre.
- [ ] Verificar estabilidad gráfica en la consola.
- [ ] Investigar e implementar teclado con ABI comprobada.
- [ ] Investigar una API de mouse con ABI comprobada.
- [ ] Investigar un motor web viable.
- [ ] Evaluar compatibilidad real con GeForce NOW.

## Limitaciones

Que el PKG compile no garantiza que arranque o sea estable en hardware. Los navegadores modernos y los servicios de juego en la nube pueden requerir APIs, códecs, decodificación de vídeo y DRM que una aplicación homebrew quizá no pueda ofrecer. La compatibilidad con GeForce NOW no está garantizada.
