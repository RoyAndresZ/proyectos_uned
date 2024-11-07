/*
Autor: Roy Andres Zarate Ferreto
Fecha: 2 de Octubre del 2024
Proyecto 1
Ref.:Libro de Programacion c++ pag.108 a 115
Sesion virtual 1- Tutor:Alexander Angeline Mora
Sesion virtual 1- Brayner Gerardo Salmer�n Castillo

*/
#include <iostream> // Para entrada y Salida
#include <iomanip>  // Para manejar los decimales
#include <string>   // Trabajar con String
#include <cstdlib>  // Para usar el comando system("cls") o system("clear")
#include <cctype>   // Para Funciones que manejan caracteres como toupper,isdigit
#include <thread>   // Para usar std::this_thread::sleep_for
#include <chrono>   // Para usar std::chrono::seconds

using namespace std; // Para evitar el uso del prefijo std::
const float IVA = 0.13;


void limpiarPantalla()  // Funcion para Limpiar Pantalla
 {
    #ifdef _WIN32
        system("cls");  // Para Windows
    #else
        system("clear"); // Para Linux o Mac
    #endif
}
// validar si una cadela es digito

bool esNumero (string cedula){    //validar si una cadena contiene solo numeros

    for (char c:cedula)
    {

        if (!isdigit (c)) return false;

    }
    return true;

}

int main(){

    system("title Proyecto 1 Roy Zarate");
    setlocale (LC_ALL, "Spanish");



    // Rubros fijos
    const int preciom3 = 150;
    const int hidrante = 182;
    const int recursosHidrico = 50;
    const int alcantarillado = 1200;
    double totalm3 = 0.0;

    // Variables para almacenar los datos del usuario
    int metrosCubicos;
    int totalapagar;
    string nombre, cedula, medidor;
    char excentoIVA = 'N';
    char regresarMenu;
    char volverMenu;
    bool datos_ingresados = false;
    double montoIVA = 0.0;
    char opcion;
    do {


        limpiarPantalla(); // Limpiar pantalla al inicio del ciclo

        // Mostrar Men�
        cout << "          Menu        \n" ;
        cout << " 1. Ingrese Datos       \n" ;
        cout << " 2. Recibo de Consumo   \n" ;
        cout << " 3. Salir del Programa  \n" ;
        cin  >> opcion;

        // Opcion 1 ingresar los datos

        switch(opcion) {
        case '1':
            {

            limpiarPantalla();
        while (true){
            cout << "Ingrese el n�mero de c�dula del due�o del servicio (9 d�gitos): ";
            cin >> cedula;

            if (cedula.length() != 9 || !esNumero(cedula)){ // verificar que cedula contenga numeros y sea 9 digitos

                limpiarPantalla();
                cout << "Valor de cedula incorrecto.\n";
            }else{
                break; // Rompe si los datos son incorrectos
                }
            }
            limpiarPantalla();
            cout << "C�dula           : " << cedula << "\n";


            cout << "Ingrese el nombre completo del due�o del servicio: ";
            cin.ignore();
            getline(cin, nombre);
            limpiarPantalla ();

            cout << "C�dula           : " << cedula << "\n";
            cout << "Nombre del due�o : " << nombre << "\n";

        while (true){
            cout << "Ingrese el n�mero del medidor (6 d�gitos): ";
            cin >> medidor;

            if (medidor.length() != 6 || !esNumero(medidor)) {
                cout << "Valor del medidor incorrecto.\n";
            }else{
                break;

            }
        }
            limpiarPantalla ();

            cout << "C�dula           : " << cedula << "\n";
            cout << "Nombre del due�o : " << nombre << "\n";
            cout << "Numero de Medidor: " << medidor << "\n";
        while (true){

            cout << "Ingrese el valor de metros c�bicos del medidor (valor num�rico): ";
            cin >> metrosCubicos;



            if (cin.fail()) {  // Si la entrada no es un n�mero v�lido
                cin.clear();    // Limpia el estado de error de 'cin'
                cin.ignore(1000, '\n');  // Ignora los caracteres incorrectos en el buffer
                cout << "Valor incorrecto, debe ser un n�mero.\n";
            } else if (metrosCubicos < 0 || metrosCubicos > 1000){
                 cout << "Valor de metros cubicos fuera de rango \n";
                }else{
                    break;  // Si la entrada es v�lida, salir del buclee
                }

          }
            limpiarPantalla ();

            cout << "C�dula           : " << cedula << "\n";
            cout << "Nombre del due�o : " << nombre << "\n";
            cout << "Numero de Medidor: " << medidor << "\n";
            cout << "Metros Cubicos    : " << metrosCubicos << "\n";

         do{
            cout << "�Est� exento del IVA (S/N)? ";
            cin >> excentoIVA;
            excentoIVA = toupper(excentoIVA);

                if(excentoIVA != 'S' && excentoIVA != 'N'){
                cout << "Entrada no v�lida. Por favor ingrese 's' para s� o 'n' para no..\n";
                cin >>excentoIVA;
                }
            }while (excentoIVA != 'S' && excentoIVA != 'N');



            limpiarPantalla ();

            cout << "C�dula           : " << cedula << "\n";
            cout << "Nombre del due�o : " << nombre << "\n";
            cout << "Numero de Medidor: " << medidor << "\n";
            cout << "Metros Cubicos    : " << metrosCubicos << "\n";
            cout << "Exento de IVA    : " << (excentoIVA == 'S' ? "S�" : "No") << "\n";

            datos_ingresados = true; // Marca que los datos fueron ingresados correctamente

            // Preguntar si desea volver a ingresar datos o salir al men�
            //char regresarMenu;
        do{
            cout << "�Desea regresar al men� principal? (s/n): ";
            cin >> regresarMenu;
            regresarMenu = toupper(regresarMenu);

                if (regresarMenu != 'S' && regresarMenu != 'N') {
            cout << "Entrada no v�lida. Por favor ingrese 's' para s� o 'n' para no..\n";
            cin >>regresarMenu;
                }
    }while (regresarMenu != 'S' && regresarMenu != 'N');

            if (regresarMenu == 'S') {
            cout << "Volviendo al men�.\n";

            }else if (regresarMenu == 'N'){
            cout << "Permanecer en datos. \n";
            // Espera 3 segundos antes de continuar
            this_thread::sleep_for(chrono::seconds(10));
            break;
             }
        }


        break;
        case '2':
            {


            limpiarPantalla(); // Limpiar pantalla antes de mostrar la factura

            if (!datos_ingresados) {
                cout << "Debe ingresar primero la informacion.\n";
                system ("pause");
                break;
            }
            if (metrosCubicos <= 10){
                totalm3 = metrosCubicos * preciom3;
            }else if(metrosCubicos <= 50){
                totalm3 =(10 * preciom3)+((metrosCubicos - 10)*preciom3*1.10);
            }else if (metrosCubicos <=100){
                totalm3 =(10 * preciom3)+(40 * preciom3 * 1.10)+((metrosCubicos - 50)* preciom3 * 1.20);
            }else {
                totalm3 =(10 * preciom3)+(40 * preciom3 * 1.10)+(50 * preciom3 * 1.20)+((metrosCubicos -100)*preciom3*1.30);
            }

            totalapagar = totalm3 + hidrante + recursosHidrico +alcantarillado;

            // Calcular el IVA si no est� exento
            if (excentoIVA == 'N') {
                montoIVA = totalapagar * 0.13;
            } else {
                montoIVA = 0; // No se cobra IVA si est� exento
            }

            // Sumar el IVA al total a pagar si aplica
            totalapagar += montoIVA;


            // Aqu� se puede mostrar el recibo de consumo usando los datos ingresados.
            cout << "          Recibo de Consumo:\n";
            cout << "  N�mero de c�dula                 : " << cedula << "\n";
            cout << "  Nombre de due�o del servicio     : " << nombre << "\n";
            cout << "  N�mero del medidor               : " << medidor << "\n";
            cout << "  Metros c�bicos consumidos        : " << metrosCubicos << "\n";
            cout << "  Exento de IVA                    : " << (excentoIVA == 'S' ? "S�" : "No") << "\n";
            cout << "          Recibo:\n";
            cout << "---------------------------------------------------\n";
            cout << "  Consumo de Agua                  : �" << totalm3 << "\n";
            cout << "  Hidrantes                        : �" << hidrante << "\n";
            cout << "  Protecci�n Recurso H�drico       : �" << recursosHidrico << "\n";
            cout << "  Alcantarillado                   : �" << alcantarillado << "\n";
            cout << "  IVA                              : �" << montoIVA << "\n";
            cout << "         TOTAL a Pagar en colones  : �" << totalapagar << "\n";



          do{
            cout << "�Desea volver al Men�? (s/n): ";
            cin >> volverMenu;
            volverMenu = toupper(volverMenu);

                if (volverMenu != 'S' && volverMenu != 'N') {
            cout << "Entrada no v�lida. Por favor ingrese 's' para s� o 'n' para no..\n";
            cin >> volverMenu;
                    }
        }while (volverMenu != 'S' && volverMenu != 'N');

            if (volverMenu == 'S') {
            cout << "Volviendo al men�.\n";

            }else if (volverMenu == 'N'){
                cout << "Permanecer en factura. \n";
                 // Espera 3 segundos antes de continuar
                this_thread::sleep_for(chrono::seconds(10));
                break;
            }

        }
        break;
        case '3':
            limpiarPantalla();
            cout << "Saliendo del programa...\n";
            system("pause");
            break;
    default:
            cout << "Opci�n inv�lida, vuelva a intentarlo.\n";
            system("pause");
        }

    } while (opcion != '3'); // El programa se repite hasta que el usuario elija la opci�n 3 (salir)

    return 0;
}
