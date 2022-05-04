// command(?)

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
int  MAX = 100;

struct Luz {
    int color;
    int intensidad;

    Luz(){
        intensidad = 0;
    }

    bool toggle(){
        if (intensidad = 0)
            intensidad = 100;
        else
            intensidad = 0;
        return 1;
    }

    bool subir_intensidad() {
        if(intensidad < 100) {
            intensidad += 10;
        }
        else {
            cout <<"La luz ya esta a su maxima intensidad " << endl;
            return 0;
        }
        return 1;
    }
    
    bool bajar_intensidad(){
        if (intensidad > 0){
            intensidad -= 10;
        }
        else{
            cout <<"La luz ya esta a su minima intensidad " << endl;
            return 0;
        }
    }
};


struct Ventilador {
    int velocidad;
    
    Ventilador() {
        velocidad = 0;
    };
    
    bool subir_velocidad() {
        if(velocidad < 10) {
            velocidad += 1;
        }
        else {
            cout <<"El ventilador ya esta a su maxima velocidad " << endl;
            return 0;
        }
        return 1;
    }

    bool bajar_velocidad(){
        if (velocidad > 0){
            velocidad -= 1;
        }
        else{
            cout <<"El ventilador ya esta a su minima velocidad " << endl;
            return 0;
        }
        return 1;
    }
};

class Ambiente{
public:
    map<string, Luz> luces;
    Ventilador v;
    
    Ambiente(){
        v = Ventilador();
    };

    // void PrintState(){
        
    //     cout << "La velocidad del Fan es: " << v.velocidad << "%" << endl;
    //     cout << endl;
    // }

    bool insertar_luz(string color) {
        Luz l = Luz();
        luces.insert(pair<string,Luz>(color, l));
        return 1;
    }
};

template<typename objeto>
class Boton{
private:
    objeto objetivo;
    string descripcion;
    bool (*funcion)();
public:
    void mostrar_descripcion (){
        cout  << '\n' << descripcion;
    }
    Boton(objeto obj, std::string desc, bool(*func)()) : objeto{obj}, descripcion{desc}, funcion{func}{}
    
    bool accionar() {
        return objetivo.funcion();
    }
};

template<typename objeto>
class Control{
public:
    Ambiente target;
    vector<Boton*> botones;
    
    void mostrar_funciones() {
        cout  << "\n\tEscriba el # de la función que quiere utilizar.\n";
        for (auto& boton : botones){
            (boton)->mostrar_descripcion();
        }
    }
};

int main() {
    auto a = Ambiente();
    auto c = Control();
    
    a.insertar_luz("Yellow");
    cout << a.luces["Yellow"].intensidad << endl;

    auto luz = Luz();
    auto boton1 = new Boton("Luz Amarilla", luz, &luz.toggle);
    
    c.botones.push_back(new Boton("Luz Amarilla", &a::luces["Yellow"]::toggle() );
    
}



