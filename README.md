# PS4 Cloud Browser

Aplicación homebrew experimental para PS4, con objetivo de prueba en una CUH-1215A con GoldHEN.

## Versión actual: 0.5.3 — renderizado optimizado

- **0.2:** se confirmó que la aplicación iniciaba y permanecía abierta en la PS4.
- **0.3:** la primera interfaz gráfica quedó en pantalla negra en la consola.
- **0.4:** se confirmó VideoOut y la visualización de la interfaz.
- **0.5 / 0.5.1:** las pruebas experimentales de entrada provocaron un cierre inesperado después de mostrar la interfaz.
- **0.5.2:** se aislaron las API de entrada; la interfaz permaneció abierta sin inicializar teclado ni mouse.
- **0.5.3:** se optimizó el dibujo de rectángulos para reducir el coste de la rasterización por CPU. Las API de teclado y mouse siguen desactivadas para conservar la prueba de estabilidad.

No hay cursor ni lectura de periféricos en esta compilación. No incluye motor web ni compatibilidad con GeForce NOW.

## Compilación

Cada push a `main`, o una ejecución manual desde GitHub Actions, inicia **Build PS4 Cloud Browser**. El flujo descarga OpenOrbis PS4 Toolchain, compila la aplicación, crea el PKG, lo valida y publica los artefactos.

El artefacto esperado se llama `PS4-Cloud-Browser-0.5.3`.

## Próximos pasos

- [x] Crear la estructura inicial del proyecto.
- [x] Confirmar arranque en PS4.
- [x] Confirmar salida gráfica.
- [x] Aislar la estabilidad desactivando las API de entrada.
- [x] Optimizar el renderizado de rectángulos.
- [ ] Medir el rendimiento en la consola.
- [ ] Reintroducir teclado con ABI comprobada y pruebas aisladas.
- [ ] Investigar una API de mouse con ABI comprobada.
- [ ] Investigar un motor web viable.
- [ ] Evaluar compatibilidad real con GeForce NOW.

## Limitaciones

Que el PKG compile no garantiza que arranque o sea estable en hardware. Los navegadores modernos y los servicios de juego en la nube pueden requerir APIs, códecs, decodificación de vídeo y DRM que una aplicación homebrew quizá no pueda ofrecer. La compatibilidad con GeForce NOW no está garantizada.
