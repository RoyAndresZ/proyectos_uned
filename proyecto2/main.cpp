#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <cctype>
#include <limits>

using namespace std;

// Definiciones de clases
class Pieza {
public:
    string codigo;
    string nombre;
    double costo;
    int cantidad;
    string empresa;
    string factura;
    string cedula;
};

class Pedido {
public:
    string codigoPedido;
    string codigoPieza;
    int cantidad;
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


void consultarPieza() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo de la pieza: ";
    cin >> codigoBusqueda;

    ifstream inventario("INVENTARIO.TXT");
    if (!inventario.is_open()) {
        cout << "Error al abrir INVENTARIO.TXT" << endl;
        return;
    }

    string linea;
    bool encontrada = false;

    while (getline(inventario, linea)) {
        Pieza pieza;
        size_t pos = 0;

        // Obtener el c�digo de la pieza
        pos = linea.find(",");
        pieza.codigo = linea.substr(0, pos);
        linea.erase(0, pos + 1);

        // Verificar si el c�digo coincide con el c�digo buscado
        if (pieza.codigo == codigoBusqueda) {
            // Obtener el nombre de la pieza
            pos = linea.find(",");
            pieza.nombre = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            // Obtener el costo
            pos = linea.find(",");
            pieza.costo = stod(linea.substr(0, pos));
            linea.erase(0, pos + 1);

            // Obtener la cantidad
            pos = linea.find(",");
            pieza.cantidad = stoi(linea.substr(0, pos));
            linea.erase(0, pos + 1);

            // Obtener la empresa proveedora
            pos = linea.find(",");
            pieza.empresa = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            // Obtener el n�mero de factura
            pos = linea.find(",");
            pieza.factura = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            // Obtener la c�dula de quien ingres� la pieza
            pieza.cedula = linea;

            // Mostrar la informaci�n de la pieza encontrada
            cout << "Pieza encontrada:" << endl;
            cout << "C�digo: " << pieza.codigo << endl;
            cout << "Nombre: " << pieza.nombre << endl;
            cout << "Costo: " << pieza.costo << endl;
            cout << "Cantidad: " << pieza.cantidad << endl;
            cout << "Empresa: " << pieza.empresa << endl;
            cout << "Factura: " << pieza.factura << endl;
            cout << "C�dula de ingreso: " << pieza.cedula << endl;

            encontrada = true;
            break;
        }
    }

    if (!encontrada) {
        cout << "Pieza no encontrada." << endl;
    }

    inventario.close();
}



void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Funci�n para consultar un pedido de manera m�s eficiente usando stringstream
void consultaPedido() {
    string codigoBusqueda;
    cout << "Ingrese el c�digo del pedido: ";
    cin >> codigoBusqueda;

    ifstream pedidos("PEDIDOS.TXT");
    string linea;
    bool encontrado = false;

    while (getline(pedidos, linea)) {
        stringstream ss(linea);
        Pedido pedido;
        getline(ss, pedido.codigoPedido, ',');
        getline(ss, pedido.codigoPieza, ',');
        ss >> pedido.cantidad;
        ss.ignore(1);
        getline(ss, pedido.cedulaEmpleado, ',');
        getline(ss, pedido.fechaSolicitud, ',');

        if (pedido.codigoPedido == codigoBusqueda) {
            encontrado = true;
            cout << "Pedido encontrado:\n"
                 << "C�digo de pedido: " << pedido.codigoPedido << "\n"
                 << "C�digo de pieza: " << pedido.codigoPieza << "\n"
                 << "Cantidad: " << pedido.cantidad << "\n"
                 << "C�dula del empleado: " << pedido.cedulaEmpleado << "\n"
                 << "Fecha de solicitud: " << pedido.fechaSolicitud << endl;
            break;
        }
    }

    if (!encontrado) {
        cout << "Pedido no encontrado." << endl;
    }
    pedidos.close();
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

    ifstream inventario("INVENTARIO.TXT");
    ofstream temp("TEMP.TXT");
    string linea;
    bool encontrada = false;

    while (getline(inventario, linea)) {
        stringstream ss(linea);
        Pieza pieza;
        getline(ss, pieza.codigo, ',');
        if (pieza.codigo == codigoBusqueda) {
            encontrada = true;
            cout << "Pieza encontrada: " << linea << endl;

            cout << "Ingrese nuevo nombre de la pieza: ";
            cin >> pieza.nombre;
            pieza.costo = ingresarNumeroPositivo("Ingrese nuevo costo de la pieza: ");
            pieza.cantidad = (int) ingresarNumeroPositivo("Ingrese nueva cantidad de piezas: ");
            cout << "Ingrese nuevo nombre de la empresa proveedora: ";
            cin >> pieza.empresa;
            cout << "Ingrese nuevo n�mero de factura: ";
            cin >> pieza.factura;
            cout << "Ingrese nuevo n�mero de c�dula de quien ingresa: ";
            cin >> pieza.cedula;

            temp << pieza.codigo << "," << pieza.nombre << "," << pieza.costo << ","
                 << pieza.cantidad << "," << pieza.empresa << ","
                 << pieza.factura << "," << pieza.cedula << endl;
            cout << "Pieza modificada correctamente." << endl;
        } else {
            temp << linea << endl;
        }
    }

    if (!encontrada) {
        cout << "Pieza no encontrada." << endl;
    }

    inventario.close();
    temp.close();
    remove("INVENTARIO.TXT");
    rename("TEMP.TXT", "INVENTARIO.TXT");
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

void reporteInventario() {
    ifstream inventario("INVENTARIO.TXT");
    if (!inventario.is_open()) {
        cout << "Error al abrir INVENTARIO.TXT" << endl;
        return;
    }
    cout << "\n--- Reporte de Inventario ---" << endl;
    string linea;
    while (getline(inventario, linea)) {
        cout << linea << endl;
    }
    inventario.close();
}

void reportePedidos() {
    ifstream pedidos("PEDIDOS.TXT");
    if (!pedidos.is_open()) {
        cout << "Error al abrir PEDIDOS.TXT" << endl;
        return;
    }
    cout << "\n--- Reporte de Pedidos ---" << endl;
    string linea;
    while (getline(pedidos, linea)) {
        cout << linea << endl;
    }
    pedidos.close();
}







int main() {
      char opcion;
     int opcionPrincipal;
     char opcionsubMenu;
     char opcionsubMenupedido;

     //char seguir =
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
                    cout << "  INVENTARIO  " << endl;
                    cout << "a. Ingresar pieza al inventario" << endl;
                    cout << "b. Consultar pieza del inventario" << endl;
                    cout << "c. Modificar pieza del inventario" << endl;
                    cout << "d. Eliminar pieza del inventario" << endl;
                    cout << "Seleccione una opci�n: ";
                    cin >> opcion;
                    switch(opcion){
                    limpiarPantalla();

                    case 'a':{

                        string numeroPieza;
                        int costoPieza;
                        int cantidaPiezas;
                        int cedulaIngreso;
                        int facturaEmpresa;
                        string NombrePieza;
                        string nombreEmpresa;
                            cout << "Ingresando pieza al inventario..." << endl;
                            cout << "C�digo de pieza (ej. P00000001): ";
                            cin >> numeroPieza;
                                while (!validarNumeroPieza(numeroPieza)) {
                                cout << "N�mero de pedido inv�lido. Intente nuevamente: ";
                                cin >> numeroPieza;
                            }

                            cout << "Nombre de la pieza: ";
                            cin >> NombrePieza;
                            cout << "Costo de la pieza por unidad: ";
                            cin >> costoPieza;
                            cout << "Cantidad de piezas: ";
                            cin >> cantidaPiezas;
                            cout << "Nombre de la Empresa que provee las piezas: ";
                            cin >> nombreEmpresa;
                            cout << "Factura de la empresa que provee las piezas: ";
                            cin >> facturaEmpresa;
                            cout << "N�mero de c�dula de quien ingresa las piezas al inventario: ";
                            cin >> cedulaIngreso;


                            ofstream Inventario("INVENTARIO.TXT", ios::app);
                            if (Inventario.is_open()) {
                                Inventario << numeroPieza << "," << NombrePieza << "," << costoPieza << "," << cantidaPiezas << "," << nombreEmpresa << "," << facturaEmpresa << "," << cedulaIngreso << "," << endl;
                                cout << "Pedido ingresado correctamente." << endl;
                            } else {
                                cout << "Error al abrir INVENTARIO.TXT" << endl;
                            }
                            Inventario.close();
                            break;
                        }


                    case 'b':
                        cout << "Consultando pieza del inventario..." << endl;
                            consultarPieza();
                        break;

                    case 'c':
                        cout << "Modificando pieza del inventario..." << endl;
                            modificarPieza();

                        break;

                    case 'd':
                        cout << "Eliminando pieza del inventario..." << endl;

                        break;

                    case 'e':
                        cout << "Saliendo del programa...\n";
                        break;


                    default:
                        cout << "Opci�n inv�lida, intente de nuevo." << endl;
                        system ("pause");
                        limpiarPantalla();
                     }     break;
                    }while (opcion != 'e');
                    break;



            case 2:
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
                            string numeroPedido;
                            int codigoPieza;
                            int cantidad;
                            int cedulaEmpleado;
                            string fechaSolicitud;

                                cout << "C�digo de pedido (ej. PE00000001): ";
                                cin >> numeroPedido;
                                    while (!validarNumeroPedido(numeroPedido)) {
                                        cout << "N�mero de pedido inv�lido. Intente nuevamente: ";
                                        cin >> numeroPedido;
                                        }

                                cout << "C�digo de pieza: ";
                                cin >> codigoPieza;
                                cout << "Cantidad: ";
                                cin >> cantidad;
                                cout << "C�dula del empleado: ";
                                cin >> cedulaEmpleado;
                                cout << "Fecha (DD/MM/YYYY): ";
                                cin >> fechaSolicitud;


                                ofstream pedidos("PEDIDOS.TXT", ios::app);
                                if (pedidos.is_open()) {
                                    pedidos << numeroPedido << "," << codigoPieza << "," << cantidad << "," << cedulaEmpleado << "," << fechaSolicitud << ","  << endl;
                                    cout << "Pedido ingresado correctamente." << endl;
                                } else {
                                    cout << "Error al abrir PEDIDOS.TXT" << endl;
                                }
                                pedidos.close();
                            break;
                        }

                    case 'b':
                        cout << "b. Consulta de pedido" << endl;
                        consultaPedido();
                        break;

                    case 'c':
                        cout << "c. Cancelar pedido" << endl;


                        break;

                    case 'd':
                        cout << "d. Registrar entrega de pedido" << endl;

                        break;

                    default:
                            cout << "Opci�n inv�lida, intente de nuevo." << endl;
                            system ("pause");
                            limpiarPantalla();
                            }
                }while (opcionsubMenupedido != 'e');
                break;



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
                cout << "Opci�n inv�lida, intente de nuevo." << endl;
                break;
                limpiarPantalla();
            }
        }while (opcionPrincipal != 5);


     return 0;
}
