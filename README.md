# Agente Conversacional

Práctica de la asignatura de **Inteligencia Artificial** del Doble Grado en Ingeniería Informática y Matemáticas de la Universidad de Granada.

## Descripción

Este programa consiste en un bot diseñado para simular conversaciones reales enfocadas en la gestión de un negocio. Se estructura en cuatro niveles, los cuales se detallan a continuación:

0. El nivel inicial es introductorio, y consta de cinco preguntas tipo entrevista a las que el bot responde con respuestas predefinidas. Su propósito es familiarizar al usuario con el manejo del bot.

1. El primer nivel es bastante básico, pero ya permite realizar reservas a distintas horas del día. Además, comienza a reconocer respuestas y se adapta a las preferencias del usuario en situaciones con baja incertidumbre.

2. En el segundo nivel, el bot comienza a comprender expresiones básicas como "mañana por la tarde" o "la próxima semana", acercándose al lenguage cotidiano. Permite además modificar reservas previamente realizadas.

3. El último nivel es el más avanzado. En este nivel, si solicitas una cita en un horario ocupado, el bot sugiere horarios cercanos, adaptandose mejor al comportamiento del usuario. También puede mostrar algunos sentimientos de ira o frustración si el cliente no toma una decisión, o incluso presentar la disponibilidad de los días restantes de la semana.

Para obtener un conocimiento más detallado del código, este se encuentra en ``bots/mybot/aiml``.

## Ejecución

Para ejecutar el bot en **Linux**,  es necesario otorgar permisos de ejecución al archivo `run.sh` y, a continuación, proceder con su ejecución de la siguiente manera: 

````
chmod +x run.sh
./run.sh
````

Tras esto, podrá iniciarse la primera interacción con el bot. Para consultar ejemplos de conversaciones, se puede acceder al documento ``Preguntas.pdf``.
