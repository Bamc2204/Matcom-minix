# Hito 2 – Corrección de bug en pthread y comando tree

**Estudiante:** Bryan Alejandro Mora Céspedes  
**Grupo:** C212  
**Fecha:** 28/04/2026  

---

## 1. Bug en `pthread_mutex_trylock`

**Síntoma:** El programa de prueba se colgaba en la segunda llamada a `pthread_mutex_trylock`.

**Causa:** La función se llamaba a sí misma recursivamente sin condición de salida.

**Corrección:** Se reemplazó por una llamada a la función nativa `mthread_mutex_trylock`, que ya maneja el deadlock correctamente.

```c
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    if (PTHREAD_MUTEX_INITIALIZER == *mutex)
        mthread_mutex_init(mutex, NULL);
    return mthread_mutex_trylock(mutex);
}

## 2. Comando tree

**Funcionalidad:** Recorre directorios recursivamente, mostrando el contenido indentado 4 espacios por nivel. Evita seguir enlaces simbólicos (usa lstat y S_ISLNK) para impedir ciclos.

**Implementación:** Se escribió tree.c en /usr/src/usr.bin/tree, compilado con clang -o tree tree.c e instalado en /usr/bin.

**Pruebas realizadas:**

tree /usr | head -40 – estructura indentada de /usr sin errores.

tree /home – sin salida (directorio vacío) y sin errores.

tree . – lista correctamente los archivos del directorio actual.

Ajuste necesario: Se añadió #define PATH_MAX 1024 al código fuente para compatibilidad con MINIX 3.
