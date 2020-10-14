# Biblioteca_DB
## Integrantes
- Marlon Mejia
- Luis Cordero
- Víctor Ascuña

## Introduccion
El objetivo de este proyecto es experimentar y comparar dos tecnicas de indexación para bases de datos. Para ello se emplearán archivos .csv con gran cantidad de datos. Las técnicas a comparar en este experimento son: ISAM y el Sequential File, aunque ya se conoce que el ISAM es una de las mejores, queremos ver la magnitud de la mejora comparado con el Sequential File. Una de las razones por las que escogimos estas dos tecnicas es que ya estaban avanzadas gracias a los laboratorios. Además ambos son eficientes en diferentes aspectos: el ISAM en cuanto a tiempo de ejecución de consultas, y el Sequential File para espacio de almacenamiento en disco.

## Tecnicas a usar
### Sequential File
Para el Sequential File empleamos 2 archivos, uno representa toda la data con ordenada y el otro que es un auxiliar con nueva data que tambien forma parte de la base de datos. Para dicho ordenamiento se requiere de una llave que es designada por un "id" que viene en la data en este caso. 

### ISAM
Para el ISAM se empleó el Dense Index File, también se requirió de 2 archivos, uno para la data sin orden u organización alguna, y el otro como indice que guarda la posición de los registros con su key.


##Resultados experimentales

###Sequential File

|        | P1 | P2 | P3 | P4 | P5 | P6 | P7 | P8 | P9 | P10 |
|--------|----|----|----|----|----|----|----|----|----|-----|
| Fill   |690ms|788ms|687ms|843ms|702ms|687ms|790ms|692ms|713ms|706ms|
| Insert |881ms|831ms|798ms|749ms|749ms|782ms|797ms|826ms|863ms|773ms|
| Search |749ms|689ms|691ms|668ms|701ms|670ms|671ms|700ms|695ms|664ms|
| Delete |825ms|814ms|840ms|864ms|834ms|806ms|845ms|861ms|828ms|811ms|

###Dense Index File

|        | P1 | P2 | P3 | P4 | P5 | P6 | P7 | P8 | P9 | P10 |
|--------|----|----|----|----|----|----|----|----|----|-----|
| Fill   |797ms|808ms|719ms|696ms|716ms|671ms|706ms|689ms|732ms|676ms|
| Insert |644ms|680ms|671ms|680ms|611ms|683ms|690ms|684ms|670ms|647ms|
| Search |748ms|765ms|821ms|729ms|768ms|822ms|764ms|750ms|742ms|783ms|
| Delete |812ms|873ms|811ms|823ms|748ms|835ms|778ms|803ms|759ms|794ms|

Promedio:
|        | Sequential File | ISAM |
|--------|-----------------|------|
| Fill   |     730ms            |  721ms    |
| Insert |     804ms            |  666ms    |
| Search |     690ms            |  769ms    |
| Delete |     839ms            |  804ms    |  

## Data


666ms
