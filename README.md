# csv_cparse
Parse CSV with C



El objetivo de la prueba es crear un programa en C que lea datos de un fichero y devuelva
los datos de temperatura y precipitación de la ciudad seleccionada para los próximos días,
cumpliendo los siguientes requisitos:

1. Los parámetros de entrada serán la fecha, el nombre de la ciudad y la unidad
(grados Celsius o Fahrenheit).

2. Devolver la información solicitada en formato JSON.

El problema debe resolverse de la forma más eficiente posible. Se valorará la claridad y la
documentación del código, explicando todo el proceso, así como el resultado técnico.

Debe entregarse al menos el código desarrollado y ejemplos de uso. Se agradecerá la
implementación de una interfaz simple para visualizar los datos.

# Planteamiento

- asumiendo que habran miliones de entradas en el csv
- como almacenar los datos?
- como gestionar la busqueda?
- optimizar memoria? ==
- optimizar tiempo busqueda?
- optimizar tiempo inserción?

# Demo

```bash
git clone https://github.com/2XL/csv_cparse
cd csv_cparse
```



```bash
$ make retry

CLEAN
...
COMPILE
...
argv[0] ./main
Testing code
```

- Result found

```txt
------
Options:
Sample:
        yyyy/mm/dd
        location
        C
Query:
        2018/06/02
        Sevilla
        C
Quit:
        exit

...:
Choose location [Madrid]:
Madrid
Choose date [2018/06/06]:
2018/06/06
Choose unit [Celsius] or [Fahrenheit]:
Celsius
Found!

{
        "Fecha": 2018/06/06,
        "Ciudad": Madrid,
        "Temperatura Máxima (C)": 22.450001,
        "Temperatura Mínima (C)": 11.950000,
        "Precipitación": 0.200000,
        "Nubosidad": 500
}
```

- Result found

```
---
Options:
Sample:
        yyyy/mm/dd
        location
        C
Query:
        2018/06/02
        Sevilla
        C
Quit:
        exit

...:
Choose location [Madrid]:
Valencia
Choose date [2018/06/06]:
2018/05/13
Choose unit [Celsius] or [Fahrenheit]:
Fahrenheit
Found!

{
        "Fecha": 2018/05/13,
        "Ciudad": Valencia,
        "Temperatura Máxima (F)": 57.047000,
        "Temperatura Mínima (F)": 46.157000,
        "Precipitación": 1.600000,
        "Nubosidad": 600
}

```

- Result not Found

```
---
Options:
Sample:
        yyyy/mm/dd
        location
        C
Query:
        2018/06/02
        Sevilla
        C
Quit:
        exit

...:
Choose location [Madrid]:
exit


-----------------------------------------------
Options:
Query:
        yyyy/mm/dd
        location
        unit
Sample:
        2018/06/02
        Sevilla
        Celsius
Quit:
        exit

...:
Choose location [Madrid]:
Celsius
Choose date [2018/06/06]:
2018/06/06
Choose unit [Celsius] or [Fahrenheit]:
Fahrenheit
No result found!!!!!!!!!!!!!!

```

- exit

```
---
Options:
Query:
        yyyy/mm/dd
        location
        unit
Sample:
        2018/06/02
        Sevilla
        Celsius
Quit:
        exit

...:
Choose location [Madrid]:
exit
...

```

input: (ciudad, FECHA, UNIDAD_MAG)
output: (list of temperatura, precipitacion), json format

# Fases Implementation y Problemas

0. Declarar estructura de datos
1. Leer los datos
2. Normalizar los entradas de csv
- TODO: handle normalization
    parse and remove trailing space,
    2018/05/12;Gijón;9,88;4,2;1,2;80
    2018/05/29; Gijón;11,48;7,08;0,6;90
- TODO: handle undefined between delimiter
    2018/05/28;Valencia;19,195;12,375;0,8;90
    2018/05/29;Valencia;15,785;9,735;0,4;20
    2018/05/30;Valencia;15,015;6,655;;10
    2018/05/31;Valencia;15,015;6,655;;0
- TODO: handle float with coma instead of dot
    2018/05/11;Madrid;11,55;6,25;0;10
3. Inicializar arbol binario
- TODO: height balance tree...
    ordered csv files, lookup cost O(N) ...
- TODO: check tree distribution && rebalance tree
4. Bucle interactivo
5. Liberar memoria


# Repaso C


Predefined Pointer-Related Types:

```c
size_t :    created to provide a safe type for sizes
    // represents the maximum size any object can be in C
    // sizeof -> return size_t
    // malloc(size_t) -> ptr
    // printf formatter -> %zu

ptrdiff_t:  created to handle pointer arithmetic
intptr_t:   used for storing pointer address
```

-  Avoid casting pointer to integer. Prevent case 64 bit pointer & integer only 4 bytes, will result in data lost

Pointer Operators

```c
*                           Declare a pointer
*       Dereference         Dereference a pointer
->      Point to            Access fields of a structure referenced by a pointer
+       Addition            Increment pointer @
-       Substraction        Decrement pointer @
==      Equal               Compare two pointers            -- > comparators return 0 for false and 1 for true.
!=      inEqual             Compare two pointers
>       Greater than
>=      Greater or equal
<       Less than
<=      Less or equal
(int)   Cast                To change the type of pointer
```


Data type sizes

```
Type    Size in Bytes
byte    1
char    1
short   2
int     4
long    8
float   4
double  8
```

Dynamic Memory Allocation Functions

- malloc    : Allocates memory from the heap

    ```c
    void* malloc(size_t);
    ```

- realloc   : Reallocates memory to a larger or smaller amount based on a previously allocated block of memory

    ```c
    void *realloc(void *ptr, size_t size);
    ```

- calloc    : Allocates and zeros out memory from the heap :: when need to zero mem, however exec may take longer

    ```c
    void *calloc(size_t numElements, size_t elementSize); // set all content to binary zeros

    int *pi = calloc(5,sizeof(int));

    or

    int *pi = malloc(5 * sizeof(int));
    memset(pi, 0, 5* sizeof(int));
    ```

- free      : Returns a block of memory to the heap

    ```c
    void free(void *ptr);
    ```



# String Fundamentals

- A string is a sequence of characters terminated with the ASCII NUL character as (\0).

- String literal pool: area of memory assigned to a literal pool, which holds the character sequences making up a string.
when a literal is used more than one there is normally only a copy of the string in the string literal pool this will reduce the amount of space needed for the application.
Since a literal is normally considered to be immutable, it does not hurt to have a single copy it. how ever its not good practice to assume there will be a single copy of it.

- Comparing Strings

```
int strcmp(const char *s1, const char *s2);
negative:   s1 precedes s2 lexicographically
zero:       if the two strings are equal
positive:   if s1 follows s2 lexicographically

char command[16];
printf("Enter a Command: ");
scanf("%s", command);
if (strcmp(command, "Quit") == 0) {
printf("The command was Quit");
} else {
printf("The command was not Quit");
}
```

- Copy Strings

```c
char* strcpy(char *s1, const char *s2);

/*
Basic approach:
1. Read in the string using a large array of char
2. Use malloc to allocate just the right amount of memory
3. Use strcpy to copy the string into the dynamically allocated memory
*/
```

- Concat String

```c
char *strcat(char *s1, const char *s2);

Concat s2 at the end of s1
s1 + s2
s1 must have enough allocated space to fit appended len(s2)

it returns s1 @ddres


```


# Mistakes

A common error involving the dereference operator is demonstrated
below:
```c
int *pi;
*pi = (int*) malloc(sizeof(int));
```
The problem is with the lefthand side of the assignment operation. We
are dereferencing the pointer. This will assign the address returned by
malloc to the address stored in pi . If this is the first time an assignment
is made to the pointer, then the address contained in the pointer is
probably invalid. The correct approach is shown below:
```c
pi = (int*) malloc(sizeof(int));
```

NULL    is used as a special pointer ((void*) 0).
NUL     is a char and is defined as '\0'



- malloc
    1. Always remember to +1 for the NUL terminator
    2. Don't use `sizeof` operator
    3. Use `strlen` function to determine the length of an existing string.



# Good practice

- assign NULL to freed pointer

- check allocation
```c
int *pi = (int*) malloc(sizeof(int));
if(pi != NULL) {
    // Pointer should be good
} else {
    // Bad pointer
}
```

-- manage memory allocation/deallocation at the same level. (function

# Fun

```c
char *chunk;
while (1) {
    chunk = (char*) malloc(1000000);
    printf("Allocating\n");
}
```

# Unit Testing with C

TODO


# Books

- Understanding and Using C Pointers || Reese 2013-05-18

# References

[c review](https://www.programmingsimplified.com/c-program-examples)

[c parsing csv](https://stackoverflow.com/questions/12911299/read-csv-file-in-c)

[c unit test](https://libcheck.github.io/check/doc/check_html/check_3.html)

[c hastable](http://www.sparknotes.com/cs/searching/hashtables/section3/)

[c hashmap](https://github.com/petewarden/c_hashmap)

[c balanced bst](https://www.geeksforgeeks.org/convert-normal-bst-balanced-bst/)