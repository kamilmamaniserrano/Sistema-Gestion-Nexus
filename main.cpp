#include <iostream>
#include <string>
#include <windows.h>
#include <mysql.h> 

using namespace std;

struct Alumno {
    int id;
    string nombre;
    string apellido;
    string celular;
    double nota_final;
};

int main() {
    MYSQL* conn;
    conn = mysql_init(0);

    // Conexión a la base de datos
    conn = mysql_real_connect(conn, "localhost", "root", "koms/2007/09/29", "nexus_academy", 3306, NULL, 0);

    if (!conn) {
        cout << "[ERROR] Fallo la conexion a la base de datos." << endl;
        return 1; // Salir del programa si no hay conexión
    }

    int opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n========================================" << endl;
        cout << "   SISTEMA DE GESTION - NEXUS ACADEMY   " << endl;
        cout << "========================================" << endl;
        cout << "1. Agregar nuevo alumno" << endl;
        cout << "2. Mostrar todos los alumnos" << endl;
        cout << "3. Eliminar alumno" << endl;
        cout << "4. Actualizar datos" << endl;
        cout << "5. CUADRO DE HONOR (Top 5)" << endl;
        cout << "6. Alumnos en riesgo (Nota < 4)" << endl;
        cout << "7. Salir del sistema" << endl;
cout << "Elige una opcion: ";
        cin >> opcion; // <-- ¡ESTA ES LA LÍNEA MÁGICA QUE FALTAB

        switch (opcion) {
            case 1: {
                cout << "\n--- REGISTRAR NUEVO ALUMNO ---" << endl;
                int id; // <-- Nueva variable para el ID
                string nombre, apellido, celular;
                double nota;

                // 1. Pedimos los datos al usuario (incluyendo el ID)
                cout << "ID del alumno (ej. 1, 2, 3): ";
                cin >> id;
                cout << "Nombre: ";
                cin >> nombre;
                cout << "Apellido: ";
                cin >> apellido;
                cout << "Celular: ";
                cin >> celular;
                cout << "Nota Final: ";
                cin >> nota;

                // 2. Armamos la instrucción SQL (INSERT) incluyendo el ID
                // Nota: el id no lleva comillas simples porque es un número entero
                string query = "INSERT INTO alumnos (id, nombre, apellido, celular, nota_final) VALUES (" + 
                               to_string(id) + ", '" + nombre + "', '" + apellido + "', '" + celular + "', " + to_string(nota) + ")";

                // 3. Enviamos la instrucción a MySQL
                if (mysql_query(conn, query.c_str())) {
                    cout << "\n[ERROR] No se pudo guardar: " << mysql_error(conn) << endl;
                } else {
                    cout << "\n[EXITO] ¡Alumno registrado correctamente en la base de datos!" << endl;
                }
                break;
            }
            case 2: {
                cout << "\n--- LISTA DE ALUMNOS ---" << endl;
                // 1. Preparamos la instrucción SQL (SELECT) para traer todo
                string query = "SELECT id, nombre, apellido, celular, nota_final FROM alumnos";

                // 2. Enviamos la consulta
                if (mysql_query(conn, query.c_str())) {
                    cout << "[ERROR] No se pudo obtener la lista: " << mysql_error(conn) << endl;
                } else {
                    // 3. Guardamos el resultado que nos devuelve MySQL
                    MYSQL_RES* result = mysql_store_result(conn); 
                    
                    if (result) {
                        MYSQL_ROW row;
                        cout << "ID\t| Nombre\t| Apellido\t| Celular\t| Nota" << endl;
                        cout << "--------------------------------------------------------------" << endl;
                        
                        // 4. Recorremos fila por fila e imprimimos los datos
                        while ((row = mysql_fetch_row(result))) {
                            cout << row[0] << "\t| " 
                                 << row[1] << "\t\t| " 
                                 << row[2] << "\t\t| " 
                                 << row[3] << "\t| " 
                                 << row[4] << endl;
                        }
                        // 5. Liberamos la memoria (muy importante en C++)
                        mysql_free_result(result); 
                    }
                }
                break;
            }
            case 3: {
                cout << "\n--- ELIMINAR ALUMNO ---" << endl;
                int id_eliminar;
                
                // 1. Pedimos el ID del alumno a borrar
                cout << "Ingresa el ID del alumno que deseas eliminar: ";
                cin >> id_eliminar;

                // 2. Armamos la instrucción SQL (DELETE)
                // IMPORTANTE: El "WHERE id =" es lo que evita que borremos a todos
                string query = "DELETE FROM alumnos WHERE id = " + to_string(id_eliminar);

                // 3. Enviamos la instrucción a MySQL
                if (mysql_query(conn, query.c_str())) {
                    cout << "\n[ERROR] No se pudo eliminar: " << mysql_error(conn) << endl;
                } else {
                    // mysql_affected_rows nos dice cuántas filas se borraron realmente
                    if (mysql_affected_rows(conn) > 0) {
                        cout << "\n[EXITO] ¡Alumno eliminado correctamente de la base de datos!" << endl;
                    } else {
                        cout << "\n[AVISO] No se encontro ningun alumno con el ID " << id_eliminar << "." << endl;
                    }
                }
                break;
            }
            case 4: {
                cout << "\n--- ACTUALIZAR DATOS DE ALUMNO ---" << endl;
                int id_buscar;
                string nuevo_celular;
                double nueva_nota;

                // 1. Pedimos el ID del alumno que queremos modificar
                cout << "Ingresa el ID del alumno a actualizar: ";
                cin >> id_buscar;
                
                // 2. Pedimos los nuevos datos
                cout << "Ingresa el NUEVO numero de celular: ";
                cin >> nuevo_celular;
                cout << "Ingresa la NUEVA nota final: ";
                cin >> nueva_nota;

                // 3. Armamos la instrucción SQL (UPDATE)
                // Ojo: Le decimos a MySQL que actualice celular y nota_final DONDE (WHERE) el id coincida
                string query = "UPDATE alumnos SET celular = '" + nuevo_celular + "', nota_final = " + to_string(nueva_nota) + " WHERE id = " + to_string(id_buscar);

                // 4. Enviamos la instrucción a MySQL
                if (mysql_query(conn, query.c_str())) {
                    cout << "\n[ERROR] No se pudo actualizar: " << mysql_error(conn) << endl;
                } else {
                    // mysql_affected_rows nos dice cuántas filas se cambiaron.
                    // Si es 0, significa que el ID no existía.
                    if (mysql_affected_rows(conn) > 0) {
                        cout << "\n[EXITO] ¡Datos del alumno actualizados correctamente!" << endl;
                    } else {
                        cout << "\n[AVISO] No se encontro ningun alumno con el ID " << id_buscar << "." << endl;
                    }
                }
                break;
            }
            case 5: {
                cout << "\n***** CUADRO DE HONOR - TOP 5 NOTAS *****" << endl;
                // SQL: Ordenar por nota_final de forma Descendente (DESC) y Limitar a 5 resultados
                string query = "SELECT nombre, apellido, nota_final FROM alumnos ORDER BY nota_final DESC LIMIT 5";

                if (mysql_query(conn, query.c_str())) {
                    cout << "[ERROR] " << mysql_error(conn) << endl;
                } else {
                    MYSQL_RES* result = mysql_store_result(conn);
                    if (result) {
                        MYSQL_ROW row;
                        int puesto = 1;
                        while ((row = mysql_fetch_row(result))) {
                            cout << puesto << "° | " << row[0] << " " << row[1] << " - Nota: " << row[2] << endl;
                            puesto++;
                        }
                        mysql_free_result(result);
                    }
                }
                break;
            }
            case 6: {
                cout << "\n!!! ALUMNOS EN RIESGO DE REPROBAR (Nota < 4) !!!" << endl;
                // SQL: Filtrar usando WHERE para notas menores a 4
                string query = "SELECT nombre, apellido, nota_final FROM alumnos WHERE nota_final < 4";

                if (mysql_query(conn, query.c_str())) {
                    cout << "[ERROR] " << mysql_error(conn) << endl;
                } else {
                    MYSQL_RES* result = mysql_store_result(conn);
                    if (result) {
                        MYSQL_ROW row;
                        bool hayRiesgo = false;
                        while ((row = mysql_fetch_row(result))) {
                            cout << "- " << row[0] << " " << row[1] << " | Nota actual: " << row[2] << endl;
                            hayRiesgo = true;
                        }
                        if (!hayRiesgo) cout << "No hay alumnos en riesgo. ¡Buen trabajo!" << endl;
                        mysql_free_result(result);
                    }
                }
                break;
            }
            case 7:
                cout << "\nSaliendo del sistema... ¡Hasta pronto!" << endl;
                salir = true;
                break;
        }
    }

    // Cerramos la conexión al salir
    mysql_close(conn);
    return 0;
}