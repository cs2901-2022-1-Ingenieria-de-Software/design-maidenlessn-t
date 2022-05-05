// command(?)

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
int  MAX = 100;

// on and off states
enum State { off = 0, on = 1 }; 

///////////////// old implementation /////////////////////
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

//////////////////////////////////////////////////////////


///////////////// new implementation /////////////////////

//- COMMAND PATTERN -//

// Receivers:
// class Light (parent: Device)
// class Fan (parent: Device)
class Device{
private:
    State currentState = off;
public:
    State toggle(){
        if (currentState == off)
            currentState = on;
        else
            currentState = off;
        return currentState;
    }
    State getCurrentState(){
        return currentState;
    }
};

class Light : public Device{
private:
    string color;
public:
    Light(string _color): color{_color}{};
};

class Fan : public Device{
private:
    int maxSpeed;
    int minSpeed;
    int currentSpeed;

public:
    Fan(){ minSpeed = 0; maxSpeed = 100; currentSpeed = 0;};
    Fan(int _minSpeed, int _maxSpeed): minSpeed{_minSpeed}, maxSpeed{_maxSpeed}{};

    int getCurrentSpeed(){
        return currentSpeed;
    }

    bool increaseSpeed(int amount){
        if ((currentSpeed + amount) < maxSpeed)
            currentSpeed += amount;
        else
            currentSpeed = maxSpeed;

        if (getCurrentState() == off){
            toggle();
        }
        return 1;
    }

    bool decreaseSpeed(int amount){
        if ((currentSpeed - amount) > minSpeed)
            currentSpeed -= amount;
        else
            currentSpeed = minSpeed;

        if (currentSpeed == minSpeed && getCurrentState() == on){
            toggle();
        }
        return 1;
    }
};

// Command parent class
class Command{
public:
    virtual bool execute() const = 0;
};

// Command implementations
class toggleCommand : public Command{
protected:
    Device* d;
public:
    toggleCommand(Device* _d){ d = _d;};

    bool execute() const override{
        d->toggle();
        return true;
    }
};

class increaseCommand : public Command{
protected:
    Fan* d;
    int amount;
public:
    increaseCommand(Fan* _d, int _amount){ d = _d; amount = _amount;};
    
    bool execute() const override{
        d->increaseSpeed(amount);
        return true;
    }
};

class decreaseCommand : public Command{
protected:
    Fan* d;
    int amount;
public:
    decreaseCommand(Fan* _d, int _amount){ d = _d; amount = _amount;};
    
    bool execute() const override{
        d->decreaseSpeed(amount);
        return true;
    }
};

void executeCommand(Command *c) {
    c->execute();
}

// TODO: new main, new Button (Role of Invoker, kind of what executecommand does rn), New Ambient (role of Application/Client)

int main() {
    auto a = Ambiente();
    auto c = Control();
    
    a.insertar_luz("Yellow");
    cout << a.luces["Yellow"].intensidad << endl;

    auto luz = Luz();
    auto boton1 = new Boton("Luz Amarilla", luz, &luz.toggle);
    
    c.botones.push_back(new Boton("Luz Amarilla", &a::luces["Yellow"]::toggle() );
}





