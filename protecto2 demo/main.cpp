/*
Autor: Roy Andres Zarate Ferreto
Fecha: 8 de Noviembree del 2024
Proyecto 2
Ref.:Libro de Programacion c++ pag.240 a 243, 273 a 276
Sesion virtual 2- Tutor:Alexander Angeline Mora
Sesion virtual 2- Brayner Gerardo Salmer�n Castillo

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <vector>
#include <algorithm>
using namespace std;

// Definiciones de clases
class Pieza {
public:
    double costo;
    int cantidad;
    string codigo;
    string nombre;
    string empresa;
    string factura;
    string cedula;
};

class Pedido {
public:
    int cantidad;
    string codigoPedido;
    string codigoPieza;
    string cedulaEmpleado;
    string fechaSolicitud;
    string estado;

    Pedido() : cantidad(0) {}
    Pedido(string codigoPed, string codigoP, int cant, string cedulaEmp, string fecha, string est)
        : codigoPedido(codigoPed), codigoPieza(codigoP), cantidad(cant), cedulaEmpleado(cedulaEmp), fechaSolicitud(fecha), estado(est) {}
};

// Declaraci�n de funciones
void limpiarPantalla();
void reportePedidos();
void reporteInventario();
void ingresoPedido();
void consultaPedido();
void consultarPieza();
void modificarPieza();


bool validarNumeroPedido(const string & pedido);
bool validarNumeroPieza(const string & pieza);

vector<Pieza> inventario;
vector<Pedido> pedidos;

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void guardarInventario() {
    ofstream file("INVENTARIO.TXT");
    for (const auto &pieza : inventario) {
        file << pieza.codigo << "," << pieza.nombre << "," << pieza.costo << ","
             << pieza.cantidad << "," << pieza.empresa << "," << pieza.factura
             << "," << pieza.cedula << endl;
    }
}
// Funci�n para guardar los pedidos en un archivo de texto
void guardarPedidos() {
    ofstream file("PEDIDOS.TXT");
    for (const auto &pedido : pedidos) {
        file << pedido.codigoPedido << "," << pedido.codigoPieza << ","
             << pedido.cantidad << "," << pedido.cedulaEmpleado << ","
             << pedido.fechaSolicitud << "," << pedido.estado << endl;
    }
}

// Funci�n para consultar una pieza en el inventario
void consultarPieza() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo de la pieza: ";
    cin >> codigoBusqueda;

    // Buscar la pieza en el inventario
    for (const auto &pieza : inventario) {
        if (pieza.codigo == codigoBusqueda) {
            cout << "Pieza encontrada:\n"
                 << "C�digo: " << pieza.codigo << "\nNombre: " << pieza.nombre
                 << "\nCosto: " << pieza.costo << "\nCantidad: " << pieza.cantidad
                 << "\nEmpresa: " << pieza.empresa << "\nFactura: " << pieza.factura
                 << "\nC�dula de ingreso: " << pieza.cedula << endl;
            return;
        }
    }
    cout << "Pieza no encontrada." << endl;
}

void cargarInventario() {
    ifstream file("INVENTARIO.TXT");
    if (!file.is_open()) return;

    string linea;
    while (getline(file, linea)) {
        Pieza pieza;
        stringstream ss(linea);
        getline(ss, pieza.codigo, ',');
        getline(ss, pieza.nombre, ',');
        ss >> pieza.costo;
        ss.ignore(1);
        ss >> pieza.cantidad;
        ss.ignore(1);
        getline(ss, pieza.empresa, ',');
        getline(ss, pieza.factura, ',');
        getline(ss, pieza.cedula, ',');

        inventario.push_back(pieza);
    }
    file.close();
}
// Funci�n para cargar los pedidos desde un archivo de texto
void cargarPedidos() {
    ifstream file("PEDIDOS.TXT");
    if (!file.is_open()) return;

    string linea;
    while (getline(file, linea)) {
        Pedido pedido;
        stringstream ss(linea);
        getline(ss, pedido.codigoPedido, ',');
        getline(ss, pedido.codigoPieza, ',');
        ss >> pedido.cantidad;
        ss.ignore(1);
        getline(ss, pedido.cedulaEmpleado, ',');
        getline(ss, pedido.fechaSolicitud, ',');
        getline(ss, pedido.estado, ',');

        pedidos.push_back(pedido);// Agrega el pedido al vector de pedidos
    }
    file.close();
}

// Funci�n para consultar un pedido espec�fico
void consultaPedido() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo del pedido: ";
    cin >> codigoBusqueda;
     // Buscar el pedido en el vector
    for (const auto &pedido : pedidos) {
        if (pedido.codigoPedido == codigoBusqueda) {
            cout << "Pedido encontrado:\n"
                 << "C�digo de pedido: " << pedido.codigoPedido
                 << "\nC�digo de pieza: " << pedido.codigoPieza
                 << "\nCantidad: " << pedido.cantidad
                 << "\nC�dula del empleado: " << pedido.cedulaEmpleado
                 << "\nFecha de solicitud: " << pedido.fechaSolicitud
                 << "\nEstado: " << pedido.estado << endl;
            return;
        }
    }
    cout << "Pedido no encontrado." << endl;
}

void leerPedidosDesdeArchivo(const string& archivo) {
    ifstream file(archivo);

    if (!file.is_open()) {  // Comprobamos si el archivo se abri� correctamente
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    Pedido pedido;
    string line;
    while (getline(file, line)) {  // Leemos l�nea por l�nea
        stringstream ss(line);
        ss >> pedido.codigoPedido
           >> pedido.codigoPieza
           >> pedido.cantidad
           >> pedido.cedulaEmpleado
           >> pedido.fechaSolicitud
           >> pedido.estado;

        pedidos.push_back(pedido);  // Agregamos el pedido al vector
    }

    file.close();
}
// Funci�n para mostrar el reporte de pedidos
void reportePedidos() {
    cout << "\n--- Reporte de Pedidos ---" << endl;

    if (pedidos.empty()) {
        cout << "No hay pedidos registrados." << endl;
        return;
    }

    // Recorremos el arreglo de pedidos e imprimimos cada uno
    for (const auto &pedido : pedidos) {
        cout << "  C�digo Pedido: " << pedido.codigoPedido
             << "| C�digo Pieza: " << pedido.codigoPieza
             << "| Cantidad: " << pedido.cantidad
             << "| C�dula quien solicita pieza(s): " << pedido.cedulaEmpleado
             << "| Fecha Solicitud: " << pedido.fechaSolicitud
             << "| Estado: " << pedido.estado << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    }
}

// Funci�n para mostrar el reporte de inventario
void reporteInventario() {
    cout << "\n--- Reporte de Inventario ---" << endl;
    for (const auto &pieza : inventario) {
        cout << "C�digo Pieza: " << pieza.codigo << ", Nombre Pieza: " << pieza.nombre << ", Costo: " << pieza.costo
             << ", Cantidad: " << pieza.cantidad << ", Nombre de la Empresa: " << pieza.empresa
             << ", Factura: " << pieza.factura << ", C�dula quien ingresa pieza: " << pieza.cedula << endl;
    }
}

// Funci�n para eliminar una pieza del inventario
void eliminarPieza() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo de la pieza a eliminar: ";
    cin >> codigoBusqueda;

    ifstream archivo("INVENTARIO.TXT");
    ofstream temp("TEMP.TXT");

    bool encontrada = false;
    string linea;

    // Funci�n para eliminar una pieza del inventario
    if (!archivo.is_open()) {
        cout << "Error al abrir INVENTARIO.TXT para lectura." << endl;
        return;
    }
    if (!temp.is_open()) {
        cout << "Error al abrir TEMP.TXT para escritura." << endl;
        return;
    }

    // Funci�n para eliminar una pieza del inventario
    while (getline(archivo, linea)) {

        istringstream ss(linea);
        string codigoPieza;
        getline(ss, codigoPieza, ',');

        if (codigoPieza == codigoBusqueda) {
            char confirmacion;
            cout << "Pieza con c�digo " << codigoBusqueda << " encontrada. �Esta Seguro de eliminar pieza? (s/n): ";
            cin >> confirmacion;
            confirmacion = toupper(confirmacion);

            if (confirmacion == 'S') {
                cout << "Pieza con c�digo " << codigoBusqueda << " eliminada." << endl;
                encontrada = true;
            } else {
                cout << "Operaci�n cancelada. La pieza no se elimin�." << endl;
                encontrada = true;
                temp << linea << endl; // Guardar l�neas que no coincidan con el c�digo buscado
            }
        }else{
            temp << linea << endl;
        }
      }
        if (!encontrada) {
            cout << "Pieza con c�digo " << codigoBusqueda << " no encontrada." << endl;
        }

    // Cerrar archivos y actualizar el archivo de inventario
    archivo.close();
    temp.close();


    remove("INVENTARIO.TXT");
    rename("TEMP.TXT", "INVENTARIO.TXT");
}


// Funci�n para validar que el costo y cantidad sean num�ricos
double ingresarNumeroPositivo(const string &mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor <= 0) {
            cout << "Entrada inv�lida. Ingrese un n�mero positivo." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return valor;
}

// Funci�n para modificar una pieza en el inventario
void modificarPieza() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo de la pieza a modificar: ";
    cin >> codigoBusqueda;

    ifstream inventario("INVENTARIO.TXT"); // Archivo original
    ofstream temp("TEMP.TXT"); // Archivo temporal para escribir los datos modificados
    string linea;
    bool encontrada = false;

    // Verificar si se pudo abrir el archivo de inventario
    if (!inventario.is_open() || !temp.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Leer cada l�nea del archivo original
    while (getline(inventario, linea)) {
        stringstream ss(linea);
        Pieza pieza;

        // Leer todos los campos de la pieza desde el archivo
        getline(ss, pieza.codigo, ',');
        getline(ss, pieza.nombre, ',');
        ss >> pieza.costo;
        ss.ignore(1); // Ignorar la coma
        ss >> pieza.cantidad;
        ss.ignore(1); // Ignorar la coma
        getline(ss, pieza.empresa, ',');
        getline(ss, pieza.factura, ',');
        getline(ss, pieza.cedula, ',');

        // Si la pieza encontrada coincide con el c�digo buscado, actualizar solo los campos deseados
        if (pieza.codigo == codigoBusqueda) {
            encontrada = true;
            cout << "Pieza encontrada: " << linea << endl;
            cout << "Solamente se puede modificar Costo y Cantidad." << endl;

            // Solicitar nuevo costo y cantidad al usuario
            pieza.costo = ingresarNumeroPositivo("Ingrese nuevo costo de la pieza: ");
            pieza.cantidad = (int)ingresarNumeroPositivo("Ingrese nueva cantidad de piezas: ");
            cout << "Pieza modificada correctamente." << endl;
        }

        // Escribir todos los datos de la pieza al archivo temporal
        temp << pieza.codigo << "," << pieza.nombre << "," << pieza.costo << ","
             << pieza.cantidad << "," << pieza.empresa << "," << pieza.factura
             << "," << pieza.cedula << endl;
             temp.flush();
    }

    // Informar si la pieza no se encontr� en el inventario
    if (!encontrada) {
        cout << "Pieza no encontrada." << endl;
    }

    // Cerrar archivos y renombrar el archivo temporal para reemplazar el original
    inventario.close();
    temp.close();
    remove("INVENTARIO.TXT");
    rename("TEMP.TXT", "INVENTARIO.TXT");
}


// Funci�n para validar el n�mero de pedido
bool validarNumeroPedido(const string &pedido) {
    if (pedido.length() != 10 || pedido.substr(0, 2) != "PE") return false;
    for (size_t i = 2; i < pedido.length(); ++i) {
        if (!isdigit(pedido[i])) return false;
    }
    return true;
}
// Funci�n para validar el n�mero de pieza
bool validarNumeroPieza(const string &pieza) {
    if (pieza.length() != 9 || pieza[0] != 'P') return false;
    for (size_t i = 1; i < pieza.length(); ++i) {
        if (!isdigit(pieza[i])) return false;
    }
    return true;
}
int obtenerUltimaPieza();
int obtenerUltimaPieza(const string& nombreArchivo);

string generarCodigoPieza() {
    int ultimaPieza = obtenerUltimaPieza("INVENTARIO.TXT"); // Usar el archivo como argumento
    ultimaPieza++; // Incrementar el n�mero para el siguiente c�digo
    return "P" + to_string(ultimaPieza).insert(0, 8 - to_string(ultimaPieza).length(), '0');
}

int obtenerUltimaPieza() {
    return obtenerUltimaPieza("INVENTARIO.TXT"); // Llamada a la versi�n con el archivo por defecto
}


int obtenerUltimaPieza(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    int ultimaPieza = 0;

    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {

            string pieza = linea.substr(0, 9);
            if (validarNumeroPieza(pieza)) {

                int numero = stoi(pieza.substr(1));
                if (numero > ultimaPieza) {
                    ultimaPieza = numero;
                }
            }
        }
        archivo.close();
    }
    return ultimaPieza;
}



int obtenerUltimoPedido();
int obtenerUltimoPedido(const string& nombreArchivo);

string generarCodigoPedido() {
    int ultimoPedido = obtenerUltimoPedido("PEDIDOS.TXT"); // Usar el archivo como argumento
    ultimoPedido++; // Incrementar el n�mero para el siguiente c�digo
    return "PE" + to_string(ultimoPedido).insert(0, 8 - to_string(ultimoPedido).length(), '0');
}



int obtenerUltimoPedido(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    int ultimoPedido = 0;

    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {

            string pedido = linea.substr(0, 10);
            if (validarNumeroPedido(pedido)) {

                int numero = stoi(pedido.substr(2));
                if (numero > ultimoPedido) {
                    ultimoPedido = numero;
                }
            }
        }
        archivo.close();
    }
    return ultimoPedido;
}


int main() {
    int ultimaPieza = obtenerUltimaPieza("INVENTARIO.TXT") + 1;
    int ultimoPedido = obtenerUltimoPedido("PEDIDOS.TXT") + 1;
    int opcionPrincipal;
    char opcion,volver;
    char opcionsubMenu;
    char opcionsubMenupedido;
    string codigoPedidoCancelar;
    string codigoPedidoEntrega;

    Pieza pieza;
    Pedido pedido;

    cargarInventario();
    cargarPedidos();


    system ("title proyecto 2");
    setlocale(LC_ALL, "spanish");

     do {
        cout << "     Menu Principal     " << endl;
        cout << " 1. Inventario " << endl;
        cout << " 2. Pedido "  << endl;
        cout << " 3. Reporte de Inventario" << endl;
        cout << " 4. Reporte de Pedidos" << endl;
        cout << " 5. Salir" << endl;
        cout << endl;
        cout << "seleccione una opcion" << endl;
        cin >> opcionPrincipal;



        switch(opcionPrincipal){
            case 1:{ //Menu Inventario
                limpiarPantalla();
                do{

                    cout << "            INVENTARIO         "   << endl;
                    cout << "a. Ingresar pieza al inventario"   << endl;
                    cout << "b. Consultar pieza del inventario" << endl;
                    cout << "c. Modificar pieza del inventario" << endl;
                    cout << "d. Eliminar pieza del inventario"  << endl;
                    cout << "     Seleccione una opci�n:     "  << endl;
                    cin >> opcionsubMenu;

                    switch(opcionsubMenu){
                    case 'a':{
                            limpiarPantalla();
                        do{


                            cout << "Ingresando pieza al inventario..." << endl;
                            do {
                                cout << "�Desea ingresar el c�digo de pieza manualmente? (s/n): ";
                                cin >> opcion;
                                opcion = toupper(opcion);


                                if (opcion == 'S') {
                                    cout << "C�digo de pieza (ej. P00000001): ";
                                    cin >> pieza.codigo;
                                    while (!validarNumeroPieza(pieza.codigo)) {
                                        cout << "N�mero de pedido inv�lido. Intente nuevamente: ";
                                        cin >> pieza.codigo;
                                    }
                                } else if (opcion == 'N') {
                                    pieza.codigo = generarCodigoPieza();
                                    cout << "C�digo de pieza generado autom�ticamente: " << pieza.codigo << endl;
                                } else {
                                    cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                }
                            } while (opcion != 'S' && opcion != 'N');


                            cout << "Nombre de la pieza: ";
                                cin >> pieza.nombre;

                            do{
                                cout << "Costo de la pieza por unidad: ";
                                cin >> pieza.costo;

                                if(cin.fail()  || pieza.costo <= 0){
                                    cout << "Error: Ingrese un numero entero para costo de pieza.\n";
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    }
                            }while (pieza.costo <= 0);

                            do{
                                cout << "Cantidad de piezas: ";
                                cin >> pieza.cantidad;

                                if (cin.fail() || pieza.cantidad <= 0) {
                                cout << "Error: Ingrese un n�mero entero positivo para la cantidad de piezas.\n";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                            } while (pieza.cantidad <= 0);


                            cout << "Nombre de la Empresa que provee las piezas: ";
                            cin >> pieza.empresa;
                            cout << "Factura de la empresa que provee las piezas: ";
                            cin >> pieza.factura;

                            do{
                                cout << "N�mero de c�dula de quien ingresa las piezas al inventario: ";
                                cin >> pieza.cedula;
                                if (pieza.cedula.length() != 9 || !isdigit(pieza.cedula[0])) {
                                    cout << "C�dula incorrecta, debe ser num�rica y de 9 d�gitos.\n";
                                }

                            }while (pieza.cedula.length() != 9 || !isdigit(pieza.cedula[0]));

                            inventario.push_back(pieza);
                            guardarInventario();
                            cout << "Pieza ingresada correctamente." << endl;

                            cout << "�Desea volver al men� Inventario? (s/n): ";
                            cin >> volver;
                            volver = toupper(volver);
                            if(volver == 'S'){
                                break;
                            }else if (volver == 'N'){

                            }else{
                                cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                            }
                           }while(volver!= 's') ;
                           limpiarPantalla();
                           break;
                        }

                    case 'b':{
                            limpiarPantalla();
                            do{

                            cout << "Consultando pieza del inventario..." << endl;
                            consultarPieza();

                            cout << "�Desea volver al men� Inventario? (s/n): ";
                            cin >> volver;
                            volver = toupper(volver);
                            if(volver == 'S'){
                                break;
                            }else if (volver == 'N'){

                            }else{
                                cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                            }
                       }while(volver!= 's') ;
                        limpiarPantalla();
                        break;
                    }
                    case 'c':{
                        do{
                            limpiarPantalla();
                            cout << "Modificando pieza del inventario..." << endl;
                            modificarPieza();

                            cout << "�Desea volver al men� Inventario? (s/n): ";
                            cin >> volver;
                            volver = toupper(volver);
                            if(volver == 'S'){
                                break;
                            }else if (volver == 'N'){

                            }else{
                                cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                            }
                       }while(volver!= 's') ;
                       limpiarPantalla();
                       break;
                    }



                    case 'd':{
                        do{
                            limpiarPantalla();

                            cout << "Eliminando pieza del inventario..." << endl;
                            eliminarPieza();

                            cout << "�Desea volver al men� Inventario? (s/n): ";
                            cin >> volver;
                            volver = toupper(volver);
                            if(volver == 'S'){
                                break;
                            }else if (volver == 'N'){

                            }else{
                                cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                            }
                       }while(volver!= 's') ;
                       limpiarPantalla();
                       break;
                    }


                    default:
                        limpiarPantalla();
                        cout << "Opci�n inv�lida, intente de nuevo." << endl;
                        break;

                     }
                }while (opcionsubMenu != 'd');
                break;
            }


            case 2:{ //menu pedido
                limpiarPantalla();
                do{
                    cout << "   PEDIDO  " << endl;
                    cout << "a. Ingreso de pedido" << endl;
                    cout << "b. Consulta de pedido" << endl;
                    cout << "c. Cancelar pedido" << endl;
                    cout << "d. Registrar entrega de pedido" << endl;
                    cout << "Seleccione una opci�n: ";
                    cin >> opcionsubMenupedido;
                    switch (opcionsubMenupedido){
                        case 'a':{
                            cout << "a. Ingreso de pedido" << endl;
                            do {
                                cout << "�Desea ingresar el c�digo de pedido manualmente? (s/n): ";
                                cin >> opcion;
                                opcion = toupper(opcion);

                                    if (opcion == 'S') {
                                        cout << "C�digo de pedido (ej. PE00000001): ";
                                        cin >> pedido.codigoPedido;
                                        while (!validarNumeroPedido(pedido.codigoPedido)) {
                                            cout << "N�mero de pedido inv�lido. Intente nuevamente: ";
                                            cin >> pedido.codigoPedido;
                                        }
                                    } else if (opcion == 'N') {
                                        pedido.codigoPedido = generarCodigoPedido();
                                        cout << "C�digo de pedido generado autom�ticamente: " << pedido.codigoPedido << endl;
                                    } else {
                                        cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                    }
                                } while (opcion != 'S' && opcion != 'N');

                                cout << "C�digo de pieza: ";
                                cin >> pedido.codigoPieza;
                                cout << "Cantidad de piezas a solicitar: ";
                                cin >> pedido.cantidad;

                                do{cout << "N�mero de identificaci�n del empleado: ";
                                    cin >> pedido.cedulaEmpleado;
                                    if (pedido.cedulaEmpleado.length() != 9 || !isdigit(pedido.cedulaEmpleado[0])) {
                                            cout << "C�dula incorrecta, debe ser num�rica y de 9 d�gitos.\n";
                                    }
                                }while (pedido.cedulaEmpleado.length() != 9 || !isdigit(pedido.cedulaEmpleado[0]));


                                cout << "Fecha solicitud, en formato(DD/MM/YYYY): ";
                                cin >> pedido.fechaSolicitud;
                                cout << "Estado del pedido: ";
                                pedido.estado = "En Proceso";
                                 cout << pedido.estado <<endl;

                                pedidos.push_back(pedido);
                                    guardarPedidos();
                                    cout << "Pedido ingresado correctamente." << endl;
                                do{
                                cout << "�Desea volver al men� Pedidos? (s/n): ";
                                cin >> volver;
                                volver = toupper(volver);
                                if(volver == 'S'){
                                    break;
                                }else if (volver == 'N'){

                                }else{
                                    cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                }
                               }while(volver!= 's') ;
                               limpiarPantalla();
                        }   break;


                        case 'b':{
                            do{
                                limpiarPantalla();
                                cout << "b. Consulta de pedido" << endl;
                                consultaPedido();

                                cout << "�Desea volver al men� Pedidos? (s/n): ";
                                cin >> volver;
                                volver = toupper(volver);
                                if(volver == 'S'){
                                    break;
                                }else if (volver == 'N'){

                                }else{
                                    cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                }
                           }while(volver!= 's') ;
                           limpiarPantalla();
                           break;
                        }


                        case 'c':{
                            do{
                                limpiarPantalla();
                                cout << "Ingrese el c�digo de pedido para cancelar: ";
                                cin >> codigoPedidoCancelar;

                                bool encontrado = false;
                                for (auto& pedido : pedidos) {
                                    if (pedido.codigoPedido == codigoPedidoCancelar) {
                                        pedido.estado = "Cancelado";
                                        cout << "Pedido " << codigoPedidoCancelar << " ha sido cancelado." << endl;
                                        encontrado = true;
                                        guardarPedidos();
                                        break;
                                        }
                                    }
                                    if (!encontrado) {
                                        cout << "Pedido no encontrado." << endl;
                                    }


                                cout << "�Desea volver al men� Pedidos? (s/n): ";
                                cin >> volver;
                                volver = toupper(volver);
                                if(volver == 'S'){
                                    break;
                                }else if (volver == 'N'){

                                }else{
                                    cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                }
                           }while(volver!= 's') ;
                           limpiarPantalla();
                           break;
                        }


                        case 'd':{
                            do{
                                limpiarPantalla();
                                cout << "d. Registrar entrega de pedido" << endl;
                                cout << "Ingrese el c�digo de pedido para registrar entrega: ";
                                cin >> codigoPedidoEntrega;

                                bool encontrado = false;
                                for (auto& pedido : pedidos) {
                                    if (pedido.codigoPedido == codigoPedidoEntrega) {
                                        pedido.estado = "Entregado";
                                        cout << "Pedido " << codigoPedidoEntrega << " ha sido marcado como entregado." << endl;
                                        encontrado = true;
                                        guardarPedidos();
                                        break;
                                    }
                                }
                                if (!encontrado) {
                                    cout << "Pedido no encontrado." << endl;
                                }


                                cout << "�Desea volver al men� Pedidos? (s/n): ";
                                cin >> volver;
                                volver = toupper(volver);
                                if(volver == 'S'){
                                    break;
                                }else if (volver == 'N'){

                                }else{
                                    cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                                }
                           }while(volver!= 's') ;
                           limpiarPantalla();
                           break;
                        }

                        default:
                            limpiarPantalla();
                            cout << "Opci�n inv�lida, intente de nuevo." << endl;
                            cout << endl << endl;
                            break;

                         }

                        }while (opcionsubMenupedido != 'e');
                }
            case 3:{
                do{
                    limpiarPantalla();
                    cout << "Reporte de Inventario" << endl;
                    reporteInventario();

                    cout << "�Desea volver al men� Principal? (s/n): ";
                    cin >> volver;
                    volver = toupper(volver);
                    if(volver == 'S'){
                        break;
                    }else if (volver == 'N'){

                    }else{
                        cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                    }
                }while(volver!= 's') ;
                limpiarPantalla();
                break;

            }
            case 4:{
                do{
                    limpiarPantalla();
                    cout << "Reporte de Pedido" << endl;
                    reportePedidos();
                    cout << "�Desea volver al men� Principal? (s/n): ";
                    cin >> volver;
                    volver = toupper(volver);
                    if(volver == 'S'){
                        break;
                    }else if (volver == 'N'){

                    }else{
                        cout << "Opci�n no v�lida. Por favor, ingrese 's' para s� o 'n' para no." << endl;
                    }
                }while(volver!= 's') ;
                limpiarPantalla();
                break;

            }
            case 5:
                limpiarPantalla();
                cout << "Salir" ;
                break;

            default:
                cout << "Opci�n inv�lida, intente de nuevo." << endl;
                break;
                system("pause");


            }
        }while (opcionPrincipal != 5);


     return 0;
}
