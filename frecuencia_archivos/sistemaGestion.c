/*
 * SISTEMA DE GESTIÓN DE ESTUDIANTES
 * Incluye: CRUD, búsquedas, estadísticas, persistencia en archivos, etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ESTUDIANTES 1000
#define MAX_NOMBRE 100
#define MAX_CARRERA 50
#define MAX_MATERIAS 10
#define ARCHIVO_DATOS "estudiantes.dat"
#define ARCHIVO_BACKUP "backup_estudiantes.dat"
#define ARCHIVO_LOG "sistema.log"

/* ========== ESTRUCTURAS DE DATOS ========== */
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

typedef struct {
    char nombre[50];
    float calificacion;
    int creditos;
} Materia;

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    char apellido[MAX_NOMBRE];
    int edad;
    char carrera[MAX_CARRERA];
    float promedio;
    Fecha fecha_ingreso;
    Materia materias[MAX_MATERIAS];
    int num_materias;
    int activo;  // 1 = activo, 0 = inactivo
} Estudiante;

typedef struct {
    Estudiante* estudiantes[MAX_ESTUDIANTES];
    int contador;
    int siguiente_id;
} Sistema;

/* ========== PROTOTIPOS DE FUNCIONES ========== */
// Funciones principales del sistema
void inicializar_sistema(Sistema* sistema);
void liberar_sistema(Sistema* sistema);
void mostrar_menu_principal();
void ejecutar_opcion(Sistema* sistema, int opcion);

// CRUD de estudiantes
void agregar_estudiante(Sistema* sistema);
void mostrar_estudiante(const Estudiante* e);
void listar_estudiantes(const Sistema* sistema);
void buscar_estudiante(const Sistema* sistema);
void modificar_estudiante(Sistema* sistema);
void eliminar_estudiante(Sistema* sistema);
void reactivar_estudiante(Sistema* sistema);

// Funciones de búsqueda
void buscar_por_nombre(const Sistema* sistema);
void buscar_por_carrera(const Sistema* sistema);
void buscar_por_rango_promedio(const Sistema* sistema);
void buscar_por_edad(const Sistema* sistema);

// Funciones de estadísticas
void mostrar_estadisticas(const Sistema* sistema);
void calcular_estadisticas(const Sistema* sistema, float* promedio_general, 
                          float* max_promedio, float* min_promedio,
                          int* total_activos, int* total_inactivos);
void top_estudiantes(const Sistema* sistema, int n);
void estudiantes_por_carrera(const Sistema* sistema);
void grafico_distribucion_edades(const Sistema* sistema);

// Funciones de materias
void gestionar_materias_estudiante(Sistema* sistema, int id_estudiante);
void agregar_materia(Estudiante* estudiante);
void mostrar_materias(const Estudiante* estudiante);
void calcular_promedio_estudiante(Estudiante* estudiante);

// Funciones de archivos
void guardar_datos(const Sistema* sistema);
void cargar_datos(Sistema* sistema);
void crear_backup(const Sistema* sistema);
void restaurar_backup(Sistema* sistema);
void exportar_csv(const Sistema* sistema);
void importar_csv(Sistema* sistema);
void registrar_log(const char* accion);

// Funciones de utilidad
int validar_entero(const char* mensaje, int min, int max);
float validar_float(const char* mensaje, float min, float max);
void validar_cadena(const char* mensaje, char* cadena, int max_len);
void limpiar_buffer();
void pausar();
void mostrar_encabezado(const char* titulo);
Fecha obtener_fecha_actual();
int comparar_fechas(Fecha f1, Fecha f2);
int calcular_edad_estudiante(Fecha fecha_nacimiento);
void ordenar_estudiantes(Estudiante* estudiantes[], int n, int criterio);

// Funciones de validación
int validar_email(const char* email);
int validar_matricula(const char* matricula);
int validar_telefono(const char* telefono);

// Funciones adicionales
void generar_reporte_academico(const Sistema* sistema);
void enviar_notificacion(const char* mensaje);
void simular_proceso_carga();
void mostrar_animacion_carga();
void generar_codigo_barras(int id);

/* ========== VARIABLES GLOBALES ========== */
int total_operaciones = 0;
time_t hora_inicio;

/* ========== IMPLEMENTACIÓN DE FUNCIONES ========== */

void inicializar_sistema(Sistema* sistema) {
    sistema->contador = 0;
    sistema->siguiente_id = 1000;
    hora_inicio = time(NULL);
    
    for(int i = 0; i < MAX_ESTUDIANTES; i++) {
        sistema->estudiantes[i] = NULL;
    }
    
    printf("Sistema inicializado correctamente.\n");
    registrar_log("Sistema inicializado");
}

void liberar_sistema(Sistema* sistema) {
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i] != NULL) {
            free(sistema->estudiantes[i]);
        }
    }
    printf("Memoria liberada correctamente.\n");
}

void mostrar_menu_principal() {
    system("clear || cls");
    printf("========================================\n");
    printf("    SISTEMA DE GESTION DE ESTUDIANTES   \n");
    printf("========================================\n");
    printf("1. Agregar nuevo estudiante\n");
    printf("2. Listar todos los estudiantes\n");
    printf("3. Buscar estudiante\n");
    printf("4. Modificar estudiante\n");
    printf("5. Eliminar estudiante (logico)\n");
    printf("6. Reactivar estudiante\n");
    printf("7. Gestionar materias de estudiante\n");
    printf("8. Mostrar estadisticas\n");
    printf("9. Top 10 estudiantes\n");
    printf("10. Exportar a CSV\n");
    printf("11. Importar desde CSV\n");
    printf("12. Crear backup\n");
    printf("13. Restaurar backup\n");
    printf("14. Generar reporte academico\n");
    printf("15. Mostrar informacion del sistema\n");
    printf("0. Salir\n");
    printf("========================================\n");
    printf("Total de operaciones realizadas: %d\n", total_operaciones);
    printf("Seleccione una opcion: ");
}

void ejecutar_opcion(Sistema* sistema, int opcion) {
    total_operaciones++;
    
    switch(opcion) {
        case 1:
            agregar_estudiante(sistema);
            break;
        case 2:
            listar_estudiantes(sistema);
            break;
        case 3:
            buscar_estudiante(sistema);
            break;
        case 4:
            modificar_estudiante(sistema);
            break;
        case 5:
            eliminar_estudiante(sistema);
            break;
        case 6:
            reactivar_estudiante(sistema);
            break;
        case 7:
            {
                int id = validar_entero("ID del estudiante: ", 1, 999999);
                gestionar_materias_estudiante(sistema, id);
            }
            break;
        case 8:
            mostrar_estadisticas(sistema);
            break;
        case 9:
            top_estudiantes(sistema, 10);
            break;
        case 10:
            exportar_csv(sistema);
            break;
        case 11:
            importar_csv(sistema);
            break;
        case 12:
            crear_backup(sistema);
            break;
        case 13:
            restaurar_backup(sistema);
            break;
        case 14:
            generar_reporte_academico(sistema);
            break;
        case 15:
            {
                time_t ahora = time(NULL);
                double segundos = difftime(ahora, hora_inicio);
                printf("\n=== INFORMACION DEL SISTEMA ===\n");
                printf("Estudiantes registrados: %d\n", sistema->contador);
                printf("Siguiente ID disponible: %d\n", sistema->siguiente_id);
                printf("Tiempo de ejecucion: %.0f segundos\n", segundos);
                printf("Operaciones realizadas: %d\n", total_operaciones);
                printf("Memoria utilizada: %lu bytes\n", 
                       sistema->contador * sizeof(Estudiante));
            }
            break;
        case 0:
            printf("Guardando datos...\n");
            guardar_datos(sistema);
            printf("Saliendo del sistema. ¡Hasta pronto!\n");
            registrar_log("Sistema finalizado");
            exit(0);
        default:
            printf("Opcion no valida. Intente nuevamente.\n");
    }
    pausar();
}

void agregar_estudiante(Sistema* sistema) {
    mostrar_encabezado("AGREGAR NUEVO ESTUDIANTE");
    
    if(sistema->contador >= MAX_ESTUDIANTES) {
        printf("Error: Capacidad maxima alcanzada.\n");
        return;
    }
    
    Estudiante* nuevo = (Estudiante*)malloc(sizeof(Estudiante));
    if(!nuevo) {
        printf("Error: No hay memoria disponible.\n");
        return;
    }
    
    // Asignar ID automático
    nuevo->id = sistema->siguiente_id++;
    
    printf("Registrando estudiante ID: %d\n", nuevo->id);
    printf("----------------------------------------\n");
    
    // Capturar datos
    validar_cadena("Nombre: ", nuevo->nombre, MAX_NOMBRE);
    validar_cadena("Apellido: ", nuevo->apellido, MAX_NOMBRE);
    
    nuevo->edad = validar_entero("Edad (17-80): ", 17, 80);
    
    validar_cadena("Carrera: ", nuevo->carrera, MAX_CARRERA);
    
    nuevo->promedio = validar_float("Promedio general (0.0-10.0): ", 0.0, 10.0);
    
    // Fecha de ingreso (actual)
    nuevo->fecha_ingreso = obtener_fecha_actual();
    
    // Inicializar materias
    nuevo->num_materias = 0;
    for(int i = 0; i < MAX_MATERIAS; i++) {
        strcpy(nuevo->materias[i].nombre, "");
        nuevo->materias[i].calificacion = 0.0;
        nuevo->materias[i].creditos = 0;
    }
    
    nuevo->activo = 1;
    
    // Agregar al sistema
    sistema->estudiantes[sistema->contador] = nuevo;
    sistema->contador++;
    
    printf("\n¡Estudiante agregado exitosamente!\n");
    printf("ID asignado: %d\n", nuevo->id);
    
    // Preguntar si desea agregar materias
    char respuesta;
    printf("\n¿Desea agregar materias ahora? (s/n): ");
    scanf(" %c", &respuesta);
    if(tolower(respuesta) == 's') {
        agregar_materia(nuevo);
        calcular_promedio_estudiante(nuevo);
    }
    
    registrar_log("Estudiante agregado");
}

void mostrar_estudiante(const Estudiante* e) {
    if(!e) return;
    
    printf("\n════════════════════════════════════════\n");
    printf("ID: %d\n", e->id);
    printf("Nombre completo: %s %s\n", e->nombre, e->apellido);
    printf("Edad: %d años\n", e->edad);
    printf("Carrera: %s\n", e->carrera);
    printf("Promedio general: %.2f\n", e->promedio);
    printf("Fecha de ingreso: %02d/%02d/%04d\n", 
           e->fecha_ingreso.dia, 
           e->fecha_ingreso.mes, 
           e->fecha_ingreso.anio);
    printf("Estado: %s\n", e->activo ? "ACTIVO" : "INACTIVO");
    printf("Materias inscritas: %d\n", e->num_materias);
    
    if(e->num_materias > 0) {
        printf("\n--- Materias ---\n");
        for(int i = 0; i < e->num_materias; i++) {
            printf("%d. %s - %.2f (%d creditos)\n", 
                   i+1, 
                   e->materias[i].nombre,
                   e->materias[i].calificacion,
                   e->materias[i].creditos);
        }
    }
    printf("════════════════════════════════════════\n");
}

void listar_estudiantes(const Sistema* sistema) {
    mostrar_encabezado("LISTADO DE ESTUDIANTES");
    
    if(sistema->contador == 0) {
        printf("No hay estudiantes registrados.\n");
        return;
    }
    
    int activos = 0;
    printf("Total de estudiantes: %d\n\n", sistema->contador);
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            mostrar_estudiante(sistema->estudiantes[i]);
            activos++;
        }
    }
    
    printf("\nEstudiantes activos: %d\n", activos);
    printf("Estudiantes inactivos: %d\n", sistema->contador - activos);
}

void buscar_estudiante(const Sistema* sistema) {
    mostrar_encabezado("BUSCAR ESTUDIANTE");
    
    printf("Metodos de busqueda:\n");
    printf("1. Por ID\n");
    printf("2. Por nombre\n");
    printf("3. Por carrera\n");
    printf("4. Por rango de promedio\n");
    printf("5. Por edad\n");
    printf("6. Volver al menu principal\n");
    
    int opcion = validar_entero("\nSeleccione metodo: ", 1, 6);
    
    switch(opcion) {
        case 1:
            {
                int id = validar_entero("ID del estudiante: ", 1, 999999);
                int encontrado = 0;
                for(int i = 0; i < sistema->contador; i++) {
                    if(sistema->estudiantes[i]->id == id && 
                       sistema->estudiantes[i]->activo) {
                        mostrar_estudiante(sistema->estudiantes[i]);
                        encontrado = 1;
                        break;
                    }
                }
                if(!encontrado) {
                    printf("No se encontro estudiante con ID %d\n", id);
                }
            }
            break;
        case 2:
            buscar_por_nombre(sistema);
            break;
        case 3:
            buscar_por_carrera(sistema);
            break;
        case 4:
            buscar_por_rango_promedio(sistema);
            break;
        case 5:
            buscar_por_edad(sistema);
            break;
        case 6:
            return;
    }
}

void buscar_por_nombre(const Sistema* sistema) {
    char nombre_buscar[MAX_NOMBRE];
    printf("Ingrese nombre o apellido a buscar: ");
    limpiar_buffer();
    fgets(nombre_buscar, MAX_NOMBRE, stdin);
    nombre_buscar[strcspn(nombre_buscar, "\n")] = 0;
    
    printf("\nResultados de la busqueda:\n");
    int encontrados = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            if(strstr(sistema->estudiantes[i]->nombre, nombre_buscar) != NULL ||
               strstr(sistema->estudiantes[i]->apellido, nombre_buscar) != NULL) {
                mostrar_estudiante(sistema->estudiantes[i]);
                encontrados++;
            }
        }
    }
    
    if(encontrados == 0) {
        printf("No se encontraron estudiantes con ese nombre.\n");
    } else {
        printf("Total encontrados: %d\n", encontrados);
    }
}

void buscar_por_carrera(const Sistema* sistema) {
    char carrera_buscar[MAX_CARRERA];
    printf("Ingrese carrera a buscar: ");
    limpiar_buffer();
    fgets(carrera_buscar, MAX_CARRERA, stdin);
    carrera_buscar[strcspn(carrera_buscar, "\n")] = 0;
    
    printf("\nEstudiantes de %s:\n", carrera_buscar);
    int encontrados = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo &&
           strcmp(sistema->estudiantes[i]->carrera, carrera_buscar) == 0) {
            mostrar_estudiante(sistema->estudiantes[i]);
            encontrados++;
        }
    }
    
    if(encontrados == 0) {
        printf("No hay estudiantes en esa carrera.\n");
    } else {
        printf("Total en carrera: %d\n", encontrados);
    }
}

void buscar_por_rango_promedio(const Sistema* sistema) {
    float min = validar_float("Promedio minimo: ", 0.0, 10.0);
    float max = validar_float("Promedio maximo: ", min, 10.0);
    
    printf("\nEstudiantes con promedio entre %.2f y %.2f:\n", min, max);
    int encontrados = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            float prom = sistema->estudiantes[i]->promedio;
            if(prom >= min && prom <= max) {
                mostrar_estudiante(sistema->estudiantes[i]);
                encontrados++;
            }
        }
    }
    
    if(encontrados == 0) {
        printf("No hay estudiantes en ese rango de promedio.\n");
    } else {
        printf("Total encontrados: %d\n", encontrados);
    }
}

void buscar_por_edad(const Sistema* sistema) {
    int min = validar_entero("Edad minima: ", 17, 80);
    int max = validar_entero("Edad maxima: ", min, 80);
    
    printf("\nEstudiantes con edad entre %d y %d años:\n", min, max);
    int encontrados = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            int edad = sistema->estudiantes[i]->edad;
            if(edad >= min && edad <= max) {
                mostrar_estudiante(sistema->estudiantes[i]);
                encontrados++;
            }
        }
    }
    
    if(encontrados == 0) {
        printf("No hay estudiantes en ese rango de edad.\n");
    } else {
        printf("Total encontrados: %d\n", encontrados);
    }
}

void modificar_estudiante(Sistema* sistema) {
    mostrar_encabezado("MODIFICAR ESTUDIANTE");
    
    int id = validar_entero("ID del estudiante a modificar: ", 1, 999999);
    int encontrado = -1;
    
    // Buscar estudiante
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->id == id) {
            encontrado = i;
            break;
        }
    }
    
    if(encontrado == -1) {
        printf("No se encontro estudiante con ID %d\n", id);
        return;
    }
    
    Estudiante* e = sistema->estudiantes[encontrado];
    
    printf("\nEstudiante encontrado:\n");
    mostrar_estudiante(e);
    
    printf("\n¿Que desea modificar?\n");
    printf("1. Nombre\n");
    printf("2. Apellido\n");
    printf("3. Edad\n");
    printf("4. Carrera\n");
    printf("5. Promedio general\n");
    printf("6. Todo lo anterior\n");
    printf("0. Cancelar\n");
    
    int opcion = validar_entero("Seleccione opcion: ", 0, 6);
    
    if(opcion == 0) {
        printf("Modificacion cancelada.\n");
        return;
    }
    
    switch(opcion) {
        case 1:
            validar_cadena("Nuevo nombre: ", e->nombre, MAX_NOMBRE);
            break;
        case 2:
            validar_cadena("Nuevo apellido: ", e->apellido, MAX_NOMBRE);
            break;
        case 3:
            e->edad = validar_entero("Nueva edad (17-80): ", 17, 80);
            break;
        case 4:
            validar_cadena("Nueva carrera: ", e->carrera, MAX_CARRERA);
            break;
        case 5:
            e->promedio = validar_float("Nuevo promedio (0.0-10.0): ", 0.0, 10.0);
            break;
        case 6:
            validar_cadena("Nuevo nombre: ", e->nombre, MAX_NOMBRE);
            validar_cadena("Nuevo apellido: ", e->apellido, MAX_NOMBRE);
            e->edad = validar_entero("Nueva edad (17-80): ", 17, 80);
            validar_cadena("Nueva carrera: ", e->carrera, MAX_CARRERA);
            e->promedio = validar_float("Nuevo promedio (0.0-10.0): ", 0.0, 10.0);
            break;
    }
    
    printf("\n¡Estudiante modificado exitosamente!\n");
    mostrar_estudiante(e);
    
    registrar_log("Estudiante modificado");
}

void eliminar_estudiante(Sistema* sistema) {
    mostrar_encabezado("ELIMINAR ESTUDIANTE (LOGICO)");
    
    int id = validar_entero("ID del estudiante a eliminar: ", 1, 999999);
    int encontrado = -1;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->id == id) {
            encontrado = i;
            break;
        }
    }
    
    if(encontrado == -1) {
        printf("No se encontro estudiante con ID %d\n", id);
        return;
    }
    
    if(!sistema->estudiantes[encontrado]->activo) {
        printf("El estudiante ya esta inactivo.\n");
        return;
    }
    
    printf("\nEstudiante a eliminar:\n");
    mostrar_estudiante(sistema->estudiantes[encontrado]);
    
    char confirmacion;
    printf("\n¿Esta seguro de eliminar este estudiante? (s/n): ");
    scanf(" %c", &confirmacion);
    
    if(tolower(confirmacion) == 's') {
        sistema->estudiantes[encontrado]->activo = 0;
        printf("Estudiante marcado como INACTIVO.\n");
        registrar_log("Estudiante eliminado (logico)");
    } else {
        printf("Operacion cancelada.\n");
    }
}

void reactivar_estudiante(Sistema* sistema) {
    mostrar_encabezado("REACTIVAR ESTUDIANTE");
    
    int id = validar_entero("ID del estudiante a reactivar: ", 1, 999999);
    int encontrado = -1;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->id == id) {
            encontrado = i;
            break;
        }
    }
    
    if(encontrado == -1) {
        printf("No se encontro estudiante con ID %d\n", id);
        return;
    }
    
    if(sistema->estudiantes[encontrado]->activo) {
        printf("El estudiante ya esta activo.\n");
        return;
    }
    
    printf("\nEstudiante a reactivar:\n");
    mostrar_estudiante(sistema->estudiantes[encontrado]);
    
    char confirmacion;
    printf("\n¿Reactivar este estudiante? (s/n): ");
    scanf(" %c", &confirmacion);
    
    if(tolower(confirmacion) == 's') {
        sistema->estudiantes[encontrado]->activo = 1;
        printf("Estudiante reactivado exitosamente.\n");
        registrar_log("Estudiante reactivado");
    } else {
        printf("Operacion cancelada.\n");
    }
}

void gestionar_materias_estudiante(Sistema* sistema, int id_estudiante) {
    mostrar_encabezado("GESTION DE MATERIAS");
    
    int encontrado = -1;
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->id == id_estudiante) {
            encontrado = i;
            break;
        }
    }
    
    if(encontrado == -1) {
        printf("No se encontro estudiante con ID %d\n", id_estudiante);
        return;
    }
    
    Estudiante* e = sistema->estudiantes[encontrado];
    
    while(1) {
        printf("\n--- Materias de %s %s ---\n", e->nombre, e->apellido);
        printf("Materias actuales: %d/%d\n", e->num_materias, MAX_MATERIAS);
        printf("\nOpciones:\n");
        printf("1. Ver materias\n");
        printf("2. Agregar materia\n");
        printf("3. Eliminar materia\n");
        printf("4. Calcular promedio actual\n");
        printf("0. Volver\n");
        
        int opcion = validar_entero("Seleccione opcion: ", 0, 4);
        
        switch(opcion) {
            case 1:
                mostrar_materias(e);
                break;
            case 2:
                if(e->num_materias >= MAX_MATERIAS) {
                    printf("Error: Maximo de materias alcanzado.\n");
                } else {
                    agregar_materia(e);
                }
                break;
            case 3:
                // Implementar eliminación de materia
                printf("Funcionalidad en desarrollo...\n");
                break;
            case 4:
                calcular_promedio_estudiante(e);
                printf("Promedio actualizado: %.2f\n", e->promedio);
                break;
            case 0:
                return;
        }
        pausar();
    }
}

void agregar_materia(Estudiante* estudiante) {
    if(estudiante->num_materias >= MAX_MATERIAS) {
        printf("No se pueden agregar mas materias.\n");
        return;
    }
    
    int indice = estudiante->num_materias;
    
    printf("\nAgregando materia %d:\n", indice + 1);
    
    printf("Nombre de la materia: ");
    limpiar_buffer();
    fgets(estudiante->materias[indice].nombre, 50, stdin);
    estudiante->materias[indice].nombre[strcspn(
        estudiante->materias[indice].nombre, "\n")] = 0;
    
    estudiante->materias[indice].calificacion = 
        validar_float("Calificacion (0.0-10.0): ", 0.0, 10.0);
    
    estudiante->materias[indice].creditos = 
        validar_entero("Creditos (1-10): ", 1, 10);
    
    estudiante->num_materias++;
    
    printf("Materia agregada exitosamente.\n");
    registrar_log("Materia agregada");
}

void mostrar_materias(const Estudiante* estudiante) {
    if(estudiante->num_materias == 0) {
        printf("El estudiante no tiene materias registradas.\n");
        return;
    }
    
    printf("\n=== MATERIAS DE %s %s ===\n", 
           estudiante->nombre, estudiante->apellido);
    printf("Total: %d materias\n\n", estudiante->num_materias);
    
    float suma_calificaciones = 0;
    int suma_creditos = 0;
    
    for(int i = 0; i < estudiante->num_materias; i++) {
        printf("%d. %s\n", i+1, estudiante->materias[i].nombre);
        printf("   Calificacion: %.2f\n", estudiante->materias[i].calificacion);
        printf("   Creditos: %d\n", estudiante->materias[i].creditos);
        printf("   --------------------\n");
        
        suma_calificaciones += estudiante->materias[i].calificacion * 
                              estudiante->materias[i].creditos;
        suma_creditos += estudiante->materias[i].creditos;
    }
    
    if(suma_creditos > 0) {
        float promedio_ponderado = suma_calificaciones / suma_creditos;
        printf("\nPromedio ponderado: %.2f\n", promedio_ponderado);
        printf("Total de creditos: %d\n", suma_creditos);
    }
}

void calcular_promedio_estudiante(Estudiante* estudiante) {
    if(estudiante->num_materias == 0) {
        printf("No hay materias para calcular promedio.\n");
        return;
    }
    
    float suma = 0;
    for(int i = 0; i < estudiante->num_materias; i++) {
        suma += estudiante->materias[i].calificacion;
    }
    
    estudiante->promedio = suma / estudiante->num_materias;
}

void mostrar_estadisticas(const Sistema* sistema) {
    mostrar_encabezado("ESTADISTICAS DEL SISTEMA");
    
    if(sistema->contador == 0) {
        printf("No hay datos para mostrar estadisticas.\n");
        return;
    }
    
    float promedio_general, max_promedio, min_promedio;
    int total_activos, total_inactivos;
    
    calcular_estadisticas(sistema, &promedio_general, &max_promedio, 
                         &min_promedio, &total_activos, &total_inactivos);
    
    printf("\n=== ESTADISTICAS GENERALES ===\n");
    printf("Total de estudiantes: %d\n", sistema->contador);
    printf("Estudiantes activos: %d (%.1f%%)\n", 
           total_activos, 
           (float)total_activos/sistema->contador*100);
    printf("Estudiantes inactivos: %d (%.1f%%)\n", 
           total_inactivos,
           (float)total_inactivos/sistema->contador*100);
    printf("\n=== PROMEDIOS ===\n");
    printf("Promedio general del sistema: %.2f\n", promedio_general);
    printf("Promedio mas alto: %.2f\n", max_promedio);
    printf("Promedio mas bajo: %.2f\n", min_promedio);
    printf("Rango de promedios: %.2f\n", max_promedio - min_promedio);
    
    // Mostrar distribución por edades
    grafico_distribucion_edades(sistema);
    
    // Mostrar distribución por carrera
    estudiantes_por_carrera(sistema);
    
    pausar();
}

void calcular_estadisticas(const Sistema* sistema, float* promedio_general, 
                          float* max_promedio, float* min_promedio,
                          int* total_activos, int* total_inactivos) {
    *total_activos = 0;
    *total_inactivos = 0;
    
    if(sistema->contador == 0) {
        *promedio_general = 0;
        *max_promedio = 0;
        *min_promedio = 0;
        return;
    }
    
    float suma_promedios = 0;
    *max_promedio = 0;
    *min_promedio = 10;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            (*total_activos)++;
            float prom = sistema->estudiantes[i]->promedio;
            suma_promedios += prom;
            
            if(prom > *max_promedio) *max_promedio = prom;
            if(prom < *min_promedio) *min_promedio = prom;
        } else {
            (*total_inactivos)++;
        }
    }
    
    *promedio_general = (*total_activos > 0) ? 
                       suma_promedios / *total_activos : 0;
}

void top_estudiantes(const Sistema* sistema, int n) {
    mostrar_encabezado("TOP ESTUDIANTES");
    
    if(sistema->contador == 0) {
        printf("No hay estudiantes registrados.\n");
        return;
    }
    
    // Crear copia de punteros a estudiantes activos
    Estudiante* top[MAX_ESTUDIANTES];
    int contador_activos = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            top[contador_activos++] = sistema->estudiantes[i];
        }
    }
    
    if(contador_activos == 0) {
        printf("No hay estudiantes activos.\n");
        return;
    }
    
    // Ordenar por promedio (burbuja simple)
    for(int i = 0; i < contador_activos - 1; i++) {
        for(int j = 0; j < contador_activos - i - 1; j++) {
            if(top[j]->promedio < top[j+1]->promedio) {
                Estudiante* temp = top[j];
                top[j] = top[j+1];
                top[j+1] = temp;
            }
        }
    }
    
    // Mostrar top n (o menos si hay menos activos)
    int mostrar = (n < contador_activos) ? n : contador_activos;
    
    printf("=== TOP %d ESTUDIANTES ===\n", mostrar);
    printf("┌─────┬────────────┬──────────────────────┬───────────┬──────────┐\n");
    printf("│ Pos │     ID     │       Nombre        │  Carrera  │ Promedio │\n");
    printf("├─────┼────────────┼──────────────────────┼───────────┼──────────┤\n");
    
    for(int i = 0; i < mostrar; i++) {
        printf("│ %3d │ %10d │ %-20s │ %-9s │  %6.2f  │\n",
               i+1,
               top[i]->id,
               top[i]->apellido,  // Mostrar solo apellido para ahorrar espacio
               top[i]->carrera,
               top[i]->promedio);
    }
    printf("└─────┴────────────┴──────────────────────┴───────────┴──────────┘\n");
}

void estudiantes_por_carrera(const Sistema* sistema) {
    printf("\n=== DISTRIBUCION POR CARRERA ===\n");
    
    // Contar estudiantes por carrera
    char carreras[MAX_ESTUDIANTES][MAX_CARRERA];
    int conteo[MAX_ESTUDIANTES] = {0};
    int num_carreras = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            int encontrada = -1;
            
            // Buscar si la carrera ya está en el arreglo
            for(int j = 0; j < num_carreras; j++) {
                if(strcmp(carreras[j], sistema->estudiantes[i]->carrera) == 0) {
                    encontrada = j;
                    break;
                }
            }
            
            if(encontrada == -1) {
                // Nueva carrera
                strcpy(carreras[num_carreras], sistema->estudiantes[i]->carrera);
                conteo[num_carreras] = 1;
                num_carreras++;
            } else {
                // Carrera existente
                conteo[encontrada]++;
            }
        }
    }
    
    // Mostrar resultados
    for(int i = 0; i < num_carreras; i++) {
        printf("%-30s: %3d estudiantes", carreras[i], conteo[i]);
        
        // Mostrar barra simple
        int barras = (conteo[i] * 50) / sistema->contador;
        printf(" [");
        for(int j = 0; j < barras; j++) {
            printf("█");
        }
        printf("]\n");
    }
}

void grafico_distribucion_edades(const Sistema* sistema) {
    printf("\n=== DISTRIBUCION POR EDAD ===\n");
    
    int edades[10] = {0}; // 0: 17-20, 1: 21-24, etc.
    const char* rangos[] = {
        "17-20", "21-24", "25-28", "29-32", "33-36",
        "37-40", "41-44", "45-48", "49-52", "53+"
    };
    
    // Contar por rangos de edad
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            int edad = sistema->estudiantes[i]->edad;
            int indice = (edad - 17) / 4;
            if(indice > 9) indice = 9;
            edades[indice]++;
        }
    }
    
    // Encontrar máximo para escalar
    int max = 0;
    for(int i = 0; i < 10; i++) {
        if(edades[i] > max) max = edades[i];
    }
    
    // Mostrar gráfico
    for(int i = 0; i < 10; i++) {
        printf("%-6s: %3d ", rangos[i], edades[i]);
        
        if(max > 0) {
            int longitud = (edades[i] * 50) / max;
            for(int j = 0; j < longitud; j++) {
                printf("█");
            }
        }
        printf("\n");
    }
}

void guardar_datos(const Sistema* sistema) {
    mostrar_animacion_carga();
    
    FILE* archivo = fopen(ARCHIVO_DATOS, "wb");
    if(!archivo) {
        printf("Error al abrir archivo para escritura.\n");
        return;
    }
    
    // Guardar metadatos
    fwrite(&sistema->contador, sizeof(int), 1, archivo);
    fwrite(&sistema->siguiente_id, sizeof(int), 1, archivo);
    
    // Guardar cada estudiante
    for(int i = 0; i < sistema->contador; i++) {
        fwrite(sistema->estudiantes[i], sizeof(Estudiante), 1, archivo);
    }
    
    fclose(archivo);
    printf("Datos guardados en '%s' (%d estudiantes)\n", 
           ARCHIVO_DATOS, sistema->contador);
    
    registrar_log("Datos guardados");
}

void cargar_datos(Sistema* sistema) {
    FILE* archivo = fopen(ARCHIVO_DATOS, "rb");
    if(!archivo) {
        printf("No se encontro archivo de datos. Iniciando con sistema vacio.\n");
        return;
    }
    
    // Cargar metadatos
    fread(&sistema->contador, sizeof(int), 1, archivo);
    fread(&sistema->siguiente_id, sizeof(int), 1, archivo);
    
    // Cargar cada estudiante
    for(int i = 0; i < sistema->contador; i++) {
        sistema->estudiantes[i] = (Estudiante*)malloc(sizeof(Estudiante));
        if(!sistema->estudiantes[i]) {
            printf("Error de memoria al cargar estudiante %d\n", i);
            fclose(archivo);
            return;
        }
        fread(sistema->estudiantes[i], sizeof(Estudiante), 1, archivo);
    }
    
    fclose(archivo);
    printf("Datos cargados desde '%s' (%d estudiantes)\n", 
           ARCHIVO_DATOS, sistema->contador);
    
    registrar_log("Datos cargados");
}

void crear_backup(const Sistema* sistema) {
    mostrar_encabezado("CREAR BACKUP");
    
    time_t ahora = time(NULL);
    struct tm* tiempo = localtime(&ahora);
    char nombre_backup[100];
    
    sprintf(nombre_backup, "backup_%04d%02d%02d_%02d%02d%02d.dat",
            tiempo->tm_year + 1900, tiempo->tm_mon + 1, tiempo->tm_mday,
            tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec);
    
    FILE* origen = fopen(ARCHIVO_DATOS, "rb");
    FILE* destino = fopen(nombre_backup, "wb");
    
    if(!origen || !destino) {
        printf("Error al crear backup.\n");
        if(origen) fclose(origen);
        if(destino) fclose(destino);
        return;
    }
    
    // Copiar byte por byte
    char buffer[1024];
    size_t bytes;
    
    while((bytes = fread(buffer, 1, sizeof(buffer), origen)) > 0) {
        fwrite(buffer, 1, bytes, destino);
    }
    
    fclose(origen);
    fclose(destino);
    
    printf("Backup creado: %s\n", nombre_backup);
    registrar_log("Backup creado");
}

void restaurar_backup(Sistema* sistema) {
    mostrar_encabezado("RESTAURAR BACKUP");
    
    printf("Archivos de backup disponibles:\n");
    system("ls backup_*.dat 2>/dev/null || dir backup_*.dat 2>/dev/null || echo 'No se encontraron backups'");
    
    char nombre_archivo[100];
    printf("\nNombre del archivo a restaurar: ");
    scanf("%s", nombre_archivo);
    
    // Liberar memoria actual
    for(int i = 0; i < sistema->contador; i++) {
        free(sistema->estudiantes[i]);
        sistema->estudiantes[i] = NULL;
    }
    
    // Restaurar desde backup
    FILE* archivo = fopen(nombre_archivo, "rb");
    if(!archivo) {
        printf("Error al abrir archivo de backup.\n");
        inicializar_sistema(sistema);
        return;
    }
    
    fread(&sistema->contador, sizeof(int), 1, archivo);
    fread(&sistema->siguiente_id, sizeof(int), 1, archivo);
    
    for(int i = 0; i < sistema->contador; i++) {
        sistema->estudiantes[i] = (Estudiante*)malloc(sizeof(Estudiante));
        fread(sistema->estudiantes[i], sizeof(Estudiante), 1, archivo);
    }
    
    fclose(archivo);
    
    printf("Backup restaurado desde '%s'\n", nombre_archivo);
    printf("Estudiantes cargados: %d\n", sistema->contador);
    
    registrar_log("Backup restaurado");
}

void exportar_csv(const Sistema* sistema) {
    mostrar_encabezado("EXPORTAR A CSV");
    
    FILE* archivo = fopen("estudiantes_exportados.csv", "w");
    if(!archivo) {
        printf("Error al crear archivo CSV.\n");
        return;
    }
    
    // Encabezados
    fprintf(archivo, "ID,Nombre,Apellido,Edad,Carrera,Promedio,Estado,Materias\n");
    
    // Datos
    for(int i = 0; i < sistema->contador; i++) {
        Estudiante* e = sistema->estudiantes[i];
        fprintf(archivo, "%d,%s,%s,%d,%s,%.2f,%s,%d\n",
                e->id, e->nombre, e->apellido, e->edad,
                e->carrera, e->promedio,
                e->activo ? "Activo" : "Inactivo",
                e->num_materias);
    }
    
    fclose(archivo);
    printf("Datos exportados a 'estudiantes_exportados.csv'\n");
    registrar_log("Datos exportados a CSV");
}

void importar_csv(Sistema* sistema) {
    printf("Funcionalidad en desarrollo...\n");
    // Implementar lectura de CSV
}

void registrar_log(const char* accion) {
    FILE* log = fopen(ARCHIVO_LOG, "a");
    if(!log) return;
    
    time_t ahora = time(NULL);
    char* fecha = ctime(&ahora);
    fecha[strlen(fecha)-1] = '\0';  // Eliminar salto de línea
    
    fprintf(log, "[%s] %s\n", fecha, accion);
    fclose(log);
}

int validar_entero(const char* mensaje, int min, int max) {
    int valor;
    while(1) {
        printf("%s", mensaje);
        if(scanf("%d", &valor) == 1 && valor >= min && valor <= max) {
            limpiar_buffer();
            return valor;
        }
        printf("Error: Ingrese un numero entre %d y %d\n", min, max);
        limpiar_buffer();
    }
}

float validar_float(const char* mensaje, float min, float max) {
    float valor;
    while(1) {
        printf("%s", mensaje);
        if(scanf("%f", &valor) == 1 && valor >= min && valor <= max) {
            limpiar_buffer();
            return valor;
        }
        printf("Error: Ingrese un numero entre %.1f y %.1f\n", min, max);
        limpiar_buffer();
    }
}

void validar_cadena(const char* mensaje, char* cadena, int max_len) {
    printf("%s", mensaje);
    limpiar_buffer();
    fgets(cadena, max_len, stdin);
    cadena[strcspn(cadena, "\n")] = 0;
    
    // Validar que no esté vacía
    while(strlen(cadena) == 0) {
        printf("Error: Este campo no puede estar vacio. Ingrese nuevamente: ");
        fgets(cadena, max_len, stdin);
        cadena[strcspn(cadena, "\n")] = 0;
    }
}

void limpiar_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nPresione Enter para continuar...");
    limpiar_buffer();
}

void mostrar_encabezado(const char* titulo) {
    system("clear || cls");
    printf("\n════════════════════════════════════════\n");
    printf("         %s\n", titulo);
    printf("════════════════════════════════════════\n\n");
}

Fecha obtener_fecha_actual() {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    
    Fecha fecha;
    fecha.dia = tm_info->tm_mday;
    fecha.mes = tm_info->tm_mon + 1;
    fecha.anio = tm_info->tm_year + 1900;
    
    return fecha;
}

int comparar_fechas(Fecha f1, Fecha f2) {
    if(f1.anio != f2.anio)
        return f1.anio - f2.anio;
    if(f1.mes != f2.mes)
        return f1.mes - f2.mes;
    return f1.dia - f2.dia;
}

void generar_reporte_academico(const Sistema* sistema) {
    mostrar_encabezado("REPORTE ACADEMICO");
    
    if(sistema->contador == 0) {
        printf("No hay datos para generar reporte.\n");
        return;
    }
    
    FILE* reporte = fopen("reporte_academico.txt", "w");
    if(!reporte) {
        printf("Error al crear archivo de reporte.\n");
        return;
    }
    
    time_t ahora = time(NULL);
    fprintf(reporte, "REPORTE ACADEMICO - GENERADO: %s", ctime(&ahora));
    fprintf(reporte, "========================================\n\n");
    
    // Estadísticas generales
    float promedio_general, max_promedio, min_promedio;
    int total_activos, total_inactivos;
    
    calcular_estadisticas(sistema, &promedio_general, &max_promedio, 
                         &min_promedio, &total_activos, &total_inactivos);
    
    fprintf(reporte, "ESTADISTICAS GENERALES:\n");
    fprintf(reporte, "Estudiantes totales: %d\n", sistema->contador);
    fprintf(reporte, "Estudiantes activos: %d\n", total_activos);
    fprintf(reporte, "Estudiantes inactivos: %d\n", total_inactivos);
    fprintf(reporte, "Promedio general: %.2f\n", promedio_general);
    fprintf(reporte, "Promedio maximo: %.2f\n", max_promedio);
    fprintf(reporte, "Promedio minimo: %.2f\n", min_promedio);
    fprintf(reporte, "\n");
    
    // Lista de estudiantes con mejores promedios
    fprintf(reporte, "TOP 10 ESTUDIANTES:\n");
    fprintf(reporte, "--------------------------------------------------\n");
    
    // Crear lista ordenada
    Estudiante* ordenados[MAX_ESTUDIANTES];
    int contador_activos = 0;
    
    for(int i = 0; i < sistema->contador; i++) {
        if(sistema->estudiantes[i]->activo) {
            ordenados[contador_activos++] = sistema->estudiantes[i];
        }
    }
    
    // Ordenar
    for(int i = 0; i < contador_activos - 1; i++) {
        for(int j = 0; j < contador_activos - i - 1; j++) {
            if(ordenados[j]->promedio < ordenados[j+1]->promedio) {
                Estudiante* temp = ordenados[j];
                ordenados[j] = ordenados[j+1];
                ordenados[j+1] = temp;
            }
        }
    }
    
    // Escribir top 10
    int mostrar = (contador_activos < 10) ? contador_activos : 10;
    for(int i = 0; i < mostrar; i++) {
        fprintf(reporte, "%2d. %-20s %-20s %-15s %.2f\n",
                i+1,
                ordenados[i]->nombre,
                ordenados[i]->apellido,
                ordenados[i]->carrera,
                ordenados[i]->promedio);
    }
    
    fclose(reporte);
    printf("Reporte generado en 'reporte_academico.txt'\n");
    registrar_log("Reporte academico generado");
}

void simular_proceso_carga() {
    printf("Procesando");
    for(int i = 0; i < 10; i++) {
        printf(".");
        fflush(stdout);
        usleep(100000);  // 100ms
    }
    printf("\n");
}

void mostrar_animacion_carga() {
    const char* frames[] = {"|", "/", "-", "\\"};
    printf("Guardando datos ");
    for(int i = 0; i < 20; i++) {
        printf("\b%c", frames[i % 4]);
        fflush(stdout);
        usleep(50000);
    }
    printf("\b \n");
}

void generar_codigo_barras(int id) {
    // Implementación simple de código de barras
    printf("Código de barras para ID %d: |", id);
    for(int i = 0; i < 10; i++) {
        printf("%c", (id >> i) & 1 ? '█' : ' ');
    }
    printf("|\n");
}

int validar_email(const char* email) {
    // Validación simple de email
    int tiene_arroba = 0;
    int tiene_punto = 0;
    
    for(int i = 0; email[i] != '\0'; i++) {
        if(email[i] == '@') tiene_arroba = 1;
        if(email[i] == '.' && tiene_arroba) tiene_punto = 1;
    }
    
    return tiene_arroba && tiene_punto;
}

int validar_matricula(const char* matricula) {
    // Validar que sean solo dígitos y tenga longitud específica
    if(strlen(matricula) != 9) return 0;
    
    for(int i = 0; matricula[i] != '\0'; i++) {
        if(!isdigit(matricula[i])) return 0;
    }
    
    return 1;
}

int validar_telefono(const char* telefono) {
    // Validar formato de teléfono
    int digitos = 0;
    
    for(int i = 0; telefono[i] != '\0'; i++) {
        if(isdigit(telefono[i])) digitos++;
    }
    
    return digitos >= 10;  // Al menos 10 dígitos
}

/* ========== FUNCIÓN PRINCIPAL ========== */
int main() {
    Sistema sistema;
    
    printf("Iniciando Sistema de Gestion de Estudiantes...\n");
    simular_proceso_carga();
    
    // Inicializar sistema
    inicializar_sistema(&sistema);
    
    // Cargar datos existentes
    cargar_datos(&sistema);
    
    // Menú principal
    while(1) {
        mostrar_menu_principal();
        int opcion = validar_entero("", 0, 15);
        ejecutar_opcion(&sistema, opcion);
    }
    
    // Esta línea nunca se alcanza por el exit(0) en la opción 0
    liberar_sistema(&sistema);
    return 0;
}
