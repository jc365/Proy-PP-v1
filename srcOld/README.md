## Esta es una guia de iniciacion tipica ("Hola Mundo")
https://guides.github.com/activities/hello-world/


## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/jc365/jc365.github.io/edit/master/README.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Páginas de programación
https://aprendiendoarduino.wordpress.com/cursos/curso-arduino-avanzado-2017/
## Sentencias de control
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/jc365/jc365.github.io/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.




En la primera parte de esta bilogía dedicada al control de flujo pudimos ver las sentencias if, if…else yfor. En esta segunda parte aumentaremos la familia de los bucles, esas sentencias cuya función es repetir un bloque de código siempre que una condición se cumpla, con los bucles while y do…while. Además, veremos formas de alterar el comportamiento natural de los bucles con las sentencias break y continue.

while
Con while aumentamos la familia de los bucles para el control de flujo de un programa, ya que ejecuta de forma continua un bloque de código mientras la condición de cabecera se cumpla. Al igual que pasaba con el bucle for, la condición se evalúa con cada repetición del bloque de código hasta que se deje de cumplir, continuando entonces con el código que haya después del bucle while.

Estructura básica del bucle while
1
2
3
while (valorAComparar ?? valorReferencia) {
codigoAEjecutar();
}
Podemos realizar la misma impresión de los números enteros desde el 0 hasta el 9 por el puerto serie que hacíamos con el bucle for, esta vez haciendo uso de un bucle while:

Impresión de enteros desde 0 hasta 9 con while
1
2
3
4
5
6
7
// Bucle while para imprimir los enteros de 0 a 9
int i = 0;
while (i < 10) {// Evaluamos condición
Serial.println(i);// Imprimimos número
i++;// Modificamos valores tomados
// en cuenta en la condición.
}
do…while
Terminamos los bucles con una modificación del bucle while en el que la condición se evalúa al final del bloque de código entre llaves. Por tanto, el bule do…while no deja de ser un bucle while que se va a ejecutar al menos una vez.

Estructura básica de un bucle do...while
1
2
3
do {
codigoaEjecutar();
} while (valorAComparar ?? valorReferencia);
Para no variar, implementemos el ejemplo que estamos utilizando a lo largo del post con el bucle do…while, imprimiendo los enteros que van desde el 0 hasta el 9:

Imprimir enteros desde 0 hasta 9 con do...while
1
2
3
4
5
int i = 0;
do {
Serial.println(i);
i++;
} while (i < 10);
“Control” del control de flujo – Sentencias break y continue
Como ya comentamos en la primera parte dedicada al control de flujo de un sketch Arduino, hay situaciones en las cuales necesitamos de alguna herramienta extra, bien para evitar acorralar nuestro sketch en un bucle infinito, o bien para añadir algo de complejidad a nuestro sketch.

La sentencia break aborta cualquier instrucción de control de flujo vista hasta ahora. Cuando nuestro programa ejecuta, por ejemplo, un bucle for, y llegado a un punto necesita abandonarlo, sea por la razón que sea, incluiremos una sentencia break, y el programa continuará su ejecución justo después del bloque donde la hemos incluido.

Podemos verlo claro con un ejemplo rescatado de la primera parte. El código del que partíamos era tal que así:

Problema bucle infinito for
1
2
3
4
5
6
7
int i = 0;
for ( ; ; ) {
if (i < 10) {
Serial.println(i);
}
i++;
}
Nuestro código va a imprimir por el puerto serie lo deseado, pero el bucle seguirá ejecutándose de forma infinita hasta producirse un overflow, y volverá a empezar de cero. Para evitarlo, podemos colocar unasentencia break en el momento en el que la condición se deje de cumplir:

Solución bucle infinito for con un break
1
2
3
4
5
6
7
8
9
int i = 0;
for ( ; ; ) {
if (i < 10) {
Serial.println(i);
} else {
break;
}
i++;
}
La sentencia continue, por su parte, aborta la ejecución actual dentro de un bucle de control de flujo. A diferencia de break, el bucle no se abandona, sino que se seguirá ejecutando mientras la condición se cumpla.

Imaginemos que vamos a imprimir, como antes, los números enteros desde el 0 hasta el 9, pero solo queremos imprimir aquellos que son divisibles por 2. Podemos incluir, en este caso, una sentencia continue, siempre que el número correspondiente a una ejecución del bucle no sea divisible por 2:

Imprimir enteros divisible por 2 desde el 0 al 9
1
2
3
4
5
6
7
for (int i = 0; i < 10; i++) {
// Operador módulo, calcula el resto de dividir i/2
if (i % 2 != 0) {
continue;
}
Serial.println(i);
}
Como veis, el lenguaje nos ofrece multitud de herramientas para simplificar y variar la ejecución de parte de un sketch, de manera que podemos adaptar nuestro código a nuestras necesidades y dar un rodeo a los problemas que se nos plantean.

Esperamos que este artículo os sirva de ayuda a la hora de escribir vuestros programas.



// linea de prueba
