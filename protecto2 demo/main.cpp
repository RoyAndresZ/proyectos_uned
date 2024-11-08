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
        : cantidad(cant), codigoPedido(codigoPed), codigoPieza(codigoP), cedulaEmpleado(cedulaEmp), fechaSolicitud(fecha), estado(est) {}
};

// Declaración de funciones
void limpiarPantalla();
void ingresarPieza();
void reportePedidos();
void reporteInventario();
void ingresoPedido();
void consultaPedido();
void consultarPieza();
void modificarPieza();
void guardarInventario();
void volverMenuInventario();

string generarCodigoPieza();

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

bool deseaReintentar() {
    char volver;
    cout << "¿Desea intentar de nuevo?" << endl;
    cout << "Presione (s) para Sí o cualquier otra tecla para volver al menú: ";
    cin >> volver;
    volver = toupper(volver);
    return volver == 'S';
}

// Función para validar que el costo y cantidad sean numéricos
double ingresarNumeroPositivo(const string &mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor <= 0) {
            cout << "Entrada inválida. Ingrese un número positivo." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return valor;
}

void ingresarPieza() {
    char opcion;
    Pieza pieza;
    limpiarPantalla();
    cout << "Ingresando pieza al inventario..." << endl;
    do {
        cout << "¿Desea ingresar el código de pieza manualmente? (s/n): ";
        cin >> opcion;
        opcion = toupper(opcion);

        if (opcion == 'S') {
            cout << "Código de pieza (ej. P00000001): ";
            cin >> pieza.codigo;
            while (!validarNumeroPieza(pieza.codigo)) {
                cout << "Número de pedido inválido. Intente nuevamente: ";
                cin >> pieza.codigo;
            }
        } else if (opcion == 'N') {
            pieza.codigo = generarCodigoPieza();
            cout << "Código de pieza generado automáticamente: " << pieza.codigo << endl;
        } else {
            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
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
        cout << "Error: Ingrese un número entero positivo para la cantidad de piezas.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (pieza.cantidad <= 0);

    cout << "Nombre de la Empresa que provee las piezas: ";
    cin >> pieza.empresa;
    cout << "Factura de la empresa que provee las piezas: ";
    cin >> pieza.factura;

    do{
        cout << "Número de cédula de quien ingresa las piezas al inventario: ";
        cin >> pieza.cedula;
        if (pieza.cedula.length() != 9 || !isdigit(pieza.cedula[0])) {
            cout << "Cédula incorrecta, debe ser numérica y de 9 dígitos.\n";
        }
    } while (pieza.cedula.length() != 9 || !isdigit(pieza.cedula[0]));

    inventario.push_back(pieza);
    guardarInventario();
    cout << "Pieza ingresada correctamente." << endl << endl;
    system("pause");
}

void consultarPieza() {
    string codigoBusqueda;
    char volver;

    limpiarPantalla();
    cout << "Consultando pieza del inventario..." << endl;
    cout << "Ingrese el código de la pieza a consultar (ej. P00000001): ";
    cin >> codigoBusqueda;

    if (!validarNumeroPieza(codigoBusqueda)) {
        cout << "Número de pieza inválido." << endl;
        if(deseaReintentar()){
            consultarPieza();
        }else {
            return;
        }
    }

    for (const auto &pieza : inventario) {
        if (pieza.codigo == codigoBusqueda) {
            cout << "Pieza encontrada:\n"
                 << "Código: " << pieza.codigo << "\nNombre: " << pieza.nombre
                 << "\nCosto: " << pieza.costo << "\nCantidad: " << pieza.cantidad
                 << "\nEmpresa: " << pieza.empresa << "\nFactura: " << pieza.factura
                 << "\nCédula de ingreso: " << pieza.cedula << endl;
            system("pause");
            return;
        }
    }
    cout << "Pieza no encontrada." << endl;
    if(deseaReintentar()){
        consultarPieza();
    }else {
        return;
    }
}

// Función para modificar una pieza en el inventario
void modificarPieza() {
    string codigoBusqueda;
    limpiarPantalla();
    cout << "Modificando pieza del inventario..." << endl;
    cout << "Ingrese el código de la pieza a modificar: ";
    cin >> codigoBusqueda;

    ifstream inventario("INVENTARIO.TXT");
    ofstream temp("TEMP.TXT");
    string linea;
    bool encontrada = false;

    while (getline(inventario, linea)) {
        stringstream ss(linea);
        Pieza pieza;
        getline(ss, pieza.codigo, ',');
        getline(ss, pieza.nombre, ',');
        ss >> pieza.costo;
        ss.ignore(1, ',');
        ss >> pieza.cantidad;
        getline(ss, pieza.empresa, ',');
        getline(ss, pieza.factura, ',');
        getline(ss, pieza.cedula);

        if (pieza.codigo == codigoBusqueda) {
            encontrada = true;
            cout << linea << endl;
            cout << "Pieza encontrada:\n"
                 << "Código: " << pieza.codigo << "\nNombre: " << pieza.nombre
                 << "\nCosto: " << pieza.costo << "\nCantidad: " << pieza.cantidad
                 << "\nEmpresa: " << pieza.empresa << "\nFactura: " << pieza.factura
                 << "\nCédula de ingreso: " << pieza.cedula << endl << endl;

            cout << "Solamente se puede modificar Costo y Cantidad. "<< endl;

            pieza.costo = ingresarNumeroPositivo("Ingrese nuevo costo de la pieza: ");
            pieza.cantidad = (int) ingresarNumeroPositivo("Ingrese nueva cantidad de piezas: ");


            temp << pieza.codigo << "," << pieza.nombre << "," << pieza.costo << ","
                 << pieza.cantidad << "," << pieza.empresa << ","
                 << pieza.factura << "," << pieza.cedula << endl;
            cout << "Pieza modificada correctamente." << endl;
            system("pause");
        } else {
            temp << linea << endl;
        }
    }

    if (!encontrada) {
        cout << "Pieza no encontrada." << endl;
        if(deseaReintentar()){
            modificarPieza();
        }else {
            return;
        }
    }
    inventario.close();
    temp.close();
    remove("INVENTARIO.TXT");
    rename("TEMP.TXT", "INVENTARIO.TXT");
}

void guardarInventario() {
    ofstream file("INVENTARIO.TXT");
    for (const auto &pieza : inventario) {
        file << pieza.codigo << "," << pieza.nombre << "," << pieza.costo << ","
             << pieza.cantidad << "," << pieza.empresa << "," << pieza.factura
             << "," << pieza.cedula << endl;
    }
}

void guardarPedidos() {
    ofstream file("PEDIDOS.TXT");
    for (const auto &pedido : pedidos) {
        file << pedido.codigoPedido << "," << pedido.codigoPieza << ","
             << pedido.cantidad << "," << pedido.cedulaEmpleado << ","
             << pedido.fechaSolicitud << "," << pedido.estado << endl;
    }
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

        pedidos.push_back(pedido);
    }
    file.close();
}


void consultaPedido() {
    string codigoBusqueda;
    cout << "Ingrese el código del pedido: ";
    cin >> codigoBusqueda;

    for (const auto &pedido : pedidos) {
        if (pedido.codigoPedido == codigoBusqueda) {
            cout << "Pedido encontrado:\n"
                 << "Código de pedido: " << pedido.codigoPedido
                 << "\nCódigo de pieza: " << pedido.codigoPieza
                 << "\nCantidad: " << pedido.cantidad
                 << "\nCédula del empleado: " << pedido.cedulaEmpleado
                 << "\nFecha de solicitud: " << pedido.fechaSolicitud
                 << "\nEstado: " << pedido.estado << endl;
            return;
        }
    }
    cout << "Pedido no encontrado." << endl;
}

void reporteInventario() {
    cout << "\n--- Reporte de Inventario ---" << endl;
    for (const auto &pieza : inventario) {
        cout << "Código: " << pieza.codigo << ", Nombre: " << pieza.nombre << ", Costo: " << pieza.costo
             << ", Cantidad: " << pieza.cantidad << ", Empresa: " << pieza.empresa
             << ", Factura: " << pieza.factura << ", Cédula: " << pieza.cedula << endl;
    }
}

void reportePedidos() {
    cout << "\n--- Reporte de Pedidos ---" << endl;
    for (const auto &pedido : pedidos) {
        cout << "Código Pedido: " << pedido.codigoPedido << ", Código Pieza: " << pedido.codigoPieza
             << ", Cantidad: " << pedido.cantidad << ", Cedula Empleado: " << pedido.cedulaEmpleado
             << ", Fecha Solicitud: " << pedido.fechaSolicitud << ", Estado: " << pedido.estado << endl;
    }
}

void eliminarPieza() {
    string codigoBusqueda;
    cout << "Ingrese el código de la pieza a eliminar: ";
    cin >> codigoBusqueda;

    ifstream archivo("INVENTARIO.TXT");
    ofstream temp("TEMP.TXT");

    bool encontrada = false;
    string linea;


    if (!archivo.is_open()) {
        cout << "Error al abrir INVENTARIO.TXT para lectura." << endl;
        return;
    }
    if (!temp.is_open()) {
        cout << "Error al abrir TEMP.TXT para escritura." << endl;
        return;
    }


    while (getline(archivo, linea)) {

        istringstream ss(linea);
        string codigoPieza;
        getline(ss, codigoPieza, ',');

        if (codigoPieza == codigoBusqueda) {

            cout << "Pieza con código " << codigoBusqueda << " eliminada." << endl;
            encontrada = true;
        } else {

            temp << linea << endl;
        }
    }

    if (!encontrada) {
        cout << "Pieza con código " << codigoBusqueda << " no encontrada." << endl;
    }


    archivo.close();
    temp.close();


    remove("INVENTARIO.TXT");
    rename("TEMP.TXT", "INVENTARIO.TXT");
}

void volverMenuInventario(){
    char volver;
    do{
        cout << "¿Desea volver al menú Inventario? (s/n): ";
        cin >> volver;
        volver = toupper(volver);
        if(volver == 'S'){
            break;
        }else if (volver == 'N'){

        }else{
        cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
        }
    } while(volver!= 's') ;
}



bool validarNumeroPedido(const string &pedido) {
    if (pedido.length() != 10 || pedido.substr(0, 2) != "PE") return false;
    for (size_t i = 2; i < pedido.length(); ++i) {
        if (!isdigit(pedido[i])) return false;
    }
    return true;
}

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
    ultimaPieza++; // Incrementar el número para el siguiente código
    return "P" + to_string(ultimaPieza).insert(0, 8 - to_string(ultimaPieza).length(), '0');
}

int obtenerUltimaPieza() {
    return obtenerUltimaPieza("INVENTARIO.TXT"); // Llamada a la versión con el archivo por defecto
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
    ultimoPedido++; // Incrementar el número para el siguiente código
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

            limpiarPantalla();

        switch(opcionPrincipal){

            case 1:
                do{
                    cout << "            INVENTARIO         "   << endl;
                    cout << "a. Ingresar pieza al inventario"   << endl;
                    cout << "b. Consultar pieza del inventario" << endl;
                    cout << "c. Modificar pieza del inventario" << endl;
                    cout << "d. Eliminar pieza del inventario"  << endl;
                    cout << "     Seleccione una opción:     "  << endl;
                    cin >> opcionsubMenu;

                    switch(opcionsubMenu){

                    case 'a':
                        ingresarPieza();
                        limpiarPantalla();
                        break;

                    case 'b':
                        consultarPieza();
                        limpiarPantalla();
                        break;

                    case 'c':
                        modificarPieza();
                        limpiarPantalla();
                        break;
                    case 'd':
                        do{
                        cout << "Eliminando pieza del inventario..." << endl;
                        eliminarPieza();

                        cout << "¿Desea volver al menú Inventario? (s/n): ";
                        cin >> volver;
                        volver = toupper(volver);
                        if(volver == 'S'){
                            break;
                        }else if (volver == 'N'){

                        }else{
                            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                        }
                       }while(volver!= 's') ;
                       break;
                            limpiarPantalla();


                    default:
                        cout << "Opción inválida, intente de nuevo." << endl;
                        system("pause");
                        limpiarPantalla();
                        break;

                     }

                    }while (opcionsubMenu != 'e');



            case 2:
                do{
                    cout << "   PEDIDO  " << endl;
                    cout << "a. Ingreso de pedido" << endl;
                    cout << "b. Consulta de pedido" << endl;
                    cout << "c. Cancelar pedido" << endl;
                    cout << "d. Registrar entrega de pedido" << endl;
                    cout << "Seleccione una opción: ";
                    cin >> opcionsubMenupedido;
                    limpiarPantalla();
                switch (opcionsubMenupedido){

                        case 'a':{
                            Pedido pedido;
                            cout << "a. Ingreso de pedido" << endl;


                                    do {
                                        cout << "¿Desea ingresar el código de pedido manualmente? (s/n): ";
                                        cin >> opcion;
                                        opcion = toupper(opcion);

                                        if (opcion == 'S') {
                                            cout << "Código de pedido (ej. PE00000001): ";
                                            cin >> pedido.codigoPedido;
                                            while (!validarNumeroPedido(pedido.codigoPedido)) {
                                                cout << "Número de pedido inválido. Intente nuevamente: ";
                                                cin >> pedido.codigoPedido;
                                            }
                                        } else if (opcion == 'N') {
                                            pedido.codigoPedido = generarCodigoPedido();
                                            cout << "Código de pedido generado automáticamente: " << pedido.codigoPedido << endl;
                                        } else {
                                            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                                        }
                                        } while (opcion != 'S' && opcion != 'N');


                                cout << "Código de pieza: ";
                                cin >> pedido.codigoPieza;
                                cout << "Cantidad de piezas a solicitar: ";
                                cin >> pedido.cantidad;

                                do{cout << "Número de identificación del empleado: ";
                                    cin >> pedido.cedulaEmpleado;
                                    if (pedido.cedulaEmpleado.length() != 9 || !isdigit(pedido.cedulaEmpleado[0])) {
                                    }   cout << "Cédula incorrecta, debe ser numérica y de 9 dígitos.\n";
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
                                cout << "¿Desea volver al menú Pedidos? (s/n): ";
                                cin >> volver;
                                volver = toupper(volver);
                                if(volver == 'S'){
                                    break;
                                }else if (volver == 'N'){

                                }else{
                                    cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                                }
                               }while(volver!= 's') ;
                            }   break;
                        limpiarPantalla();

                    case 'b':
                        do{
                        cout << "b. Consulta de pedido" << endl;
                        consultaPedido();
                        cout << "¿Desea volver al menú Pedidos? (s/n): ";
                        cin >> volver;
                        volver = toupper(volver);
                        if(volver == 'S'){
                            break;
                        }else if (volver == 'N'){

                        }else{
                            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                        }
                       }while(volver!= 's') ;
                       break;



                    case 'c':
                        do{
                            string codigoPedidoCancelar;
                            cout << "c. Cancelar pedido" << endl;
                            cout << "Ingrese el código de pedido para cancelar: ";
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


                        cout << "¿Desea volver al menú Pedidos? (s/n): ";
                        cin >> volver;
                        volver = toupper(volver);
                        if(volver == 'S'){
                            break;
                        }else if (volver == 'N'){

                        }else{
                            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                        }
                       }while(volver!= 's') ;
                       break;


                        break;

                    case 'd':
                        do{
                        string codigoPedidoEntrega;
                        cout << "d. Registrar entrega de pedido" << endl;
                        cout << "Ingrese el código de pedido para registrar entrega: ";
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


                        cout << "¿Desea volver al menú Pedidos? (s/n): ";
                        cin >> volver;
                        volver = toupper(volver);
                        if(volver == 'S'){
                            break;
                        }else if (volver == 'N'){

                        }else{
                            cout << "Opción no válida. Por favor, ingrese 's' para sí o 'n' para no." << endl;
                        }
                       }while(volver!= 's') ;
                       break;


                    default:
                        limpiarPantalla();
                        cout << "Opción inválida, intente de nuevo." << endl;
                        cout << endl << endl;
                        break;

                     }

                    }while (opcionsubMenupedido != 'e');

            case 3:
                cout << "Reporte de Inventario" << endl;
                reporteInventario();
                break;
                 limpiarPantalla();

            case 4:
                cout << "Reporte de Pedido" << endl;
                reportePedidos();
                break;
                 limpiarPantalla();

            case 5:
                cout << "Salir" ;

                break;
                 limpiarPantalla();
        default:
                cout << "Opción inválida, intente de nuevo." << endl;
                cout << endl;
                break;
                limpiarPantalla();
            }
        }while (opcionPrincipal != 5);


     return 0;
}
