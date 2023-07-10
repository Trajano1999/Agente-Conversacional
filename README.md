# Agente Conversacional

Práctica de la asignatura de **Inteligencia Artificial**.

## Descripción

Este programa consiste en un bot que simula conersaciones reales de distintos temas, es decir, un agente conversacional. Está dividido en 4 niveles que se explican a continuación.

0. Este nivel es el inicial ya que consta solamente de 5 preguntas de tipo entrevista con las que el bot responderá con una respuesta estipulada. Sirve para aprender a manejarse con el bot.

1. El primer nivel es bastante básico, pero ya permite reservar citas en distintas horas del día, comienza a distinguir respuestas y se adapta a los gustos del usuario en bajos niveles de incertidumbre.

2. El segundo nivel ya comienza a entender expresiones básicas como "mañana por la tarde" o "la próxima semana", que se parecen algo más al lenguage que usamos diariamente. Permite además modificar reservas.

3. Finalmente este nivel es el más avanzado. En este nivel, si solicitas una cita en una hora ocupada te ofrece horas cercanas por si te interesan, adaptandose más al comportamiento del usuario. También puede mostrar algunos sentimientos de ira o enfado si el cliente no se decide o incluso mostrar los dias disponibles del resto de la semana.

Espero que se disfrute de su uso. Para conocer más a fondo el código o buscar más preguntas, el código se encuentra en `bots/mybot/aiml`.

## Ejecución

Para ejecutar este bot en **Linux**, en primer lugar debemos dar permisos de ejecución al archivo `run.sh`, y posteriormente ejecutarlo. Para ello escribiremos : 

`chmod +x run.sh`

`./run.sh`

Tras esto, ya podremos entablar la primera conversación. Para buscar ejemplos de conversaciones podemos acceder al documento `Preguntas.pdf`.